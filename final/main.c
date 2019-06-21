
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUMBER_OF_TESTS 5
#define NUMBER_OF_TILES 6

float * generate_rand_input(int rows, int cols) {
  // ============================================
  // Allocating the matrices based on dimensions rows x cols
  // ============================================
  float * mat = malloc( rows * cols * sizeof(float));

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      mat[i * cols + j] = rand() % 16;
    }
  }

  return mat;
}


float * generate_output(int rows, int cols) {
  // ============================================
  // Allocating the output matrix based on dimensions rows x cols
  // with 0's as initial value
  // ============================================
  float * mat = malloc( rows * cols * sizeof(float));

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      mat[i * cols + j] = 0.0;
    }
  }

  return mat;
}


void print_mat(float * mat, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%f ", mat[i * cols + j]);
    }
    printf("\n");
  }
}



// ============================================
// Comparing two matrices of the same size for correctness
// ============================================
bool compare_matrices(float * mat1, float * mat2, int rows, int cols) {
  bool equal = true;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int index = i * cols + j;
      bool check = (mat1[index] == mat2[index]);
      equal = equal & check;
    }
  }
  return equal;
}


// ============================================
// Implementation of naive matrix multiplication
// a = b * c
// a: matrix of dimension out_rows x out_cols
// b: matrix of dimension out_rows x in_cols
// c: matrix of dimension in_cols x out_cols
// ============================================
void naivemm(float * a, float * b, float * c, int out_rows, int out_cols, int in_cols) {
  for (int i = 0; i < out_rows; i++) {
    for (int j = 0; j < out_cols; j++) {
      for (int k = 0; k < in_cols; k++) {
        int a_index = i * out_cols + j;
        int b_index = i * in_cols + k;
        int c_index = k * out_cols + j;

        a[a_index] = a[a_index] + (b[b_index] * c[c_index]);
      }
    }
  }
}

//tiles = how many rows or cols to split by ex. tiles = 8 means 64 tiles
void tilemm(float * a, float * b, float * c, int out_rows, int out_cols, int in_cols, int tiles){
  //Find tiling
  int out_row_len = (int) (out_rows/tiles);
  int out_col_len = (int) (out_cols/tiles);
  int in_col_len = (int) (in_cols/tiles);

  //do one tile first
  for(int tilecol = 0; tilecol < tiles*tiles; tilecol++){ //keep c matrix still, move b along rows
    int c_col = tilecol % tiles;
    int c_row = (int) (tilecol/tiles);
    for(int tilerow = 0; tilerow < tiles; tilerow++){

      //Naive multiplication on small tiled matrix
      for (int i = 0; i < out_row_len; i++) {
        for (int j = 0; j < out_col_len; j++) {
          int a_index = tilerow * out_col_len * out_cols + i * out_cols + (c_col * in_col_len + j);
          for (int k = 0; k < in_col_len; k++) {

            int b_index = tilerow * in_col_len * in_cols + i * in_cols + (c_row * in_col_len + k);
            int c_index = c_row * out_col_len * out_cols + k * out_cols + (c_col * out_col_len + j);

            a[a_index] = a[a_index] + (b[b_index] * c[c_index]);
          }
        }
      }
    }
  }
}


int main() {

  int MATRIX_ROWS[NUMBER_OF_TESTS] = {8, 32, 256, 512, 1024};
  int MATRIX_COLS[NUMBER_OF_TESTS] = {8, 32, 256, 512, 1024};// for a square matrix
  clock_t start_clock, end_clock;
  double cpu_time;
  int number_of_iterations[5] = {10000, 1000, 100, 100, 10};
  int TILE[NUMBER_OF_TILES] = {2, 4, 8, 16, 32, 64};

  // Run through each matrix size
  for(int test = 0; test < NUMBER_OF_TESTS; test++){

    // ============================================
    // Running and profiling
    // ============================================
    //Running naive - including generation and freeing to ensure we're multiplying/adding same sized matrices
    start_clock = clock();
    for(int iter = 0; iter < number_of_iterations[test]; iter++){
      float * mat_a_naive = generate_output(MATRIX_ROWS[test], MATRIX_ROWS[test]);
      float * mat_b = generate_rand_input(MATRIX_ROWS[test], MATRIX_COLS[test]);
      float * mat_c = generate_rand_input(MATRIX_ROWS[test], MATRIX_COLS[test]);

      //Execute naive
      naivemm(mat_a_naive, mat_b, mat_c, MATRIX_ROWS[test], MATRIX_COLS[test], MATRIX_COLS[test]);

      free(mat_a_naive);
      free(mat_b);
      free(mat_c);
    }
    end_clock = clock();

    // Naive completed - Compute execution time.
    printf("=========================================\n");
    printf("Naive implementation: Rows - %d \t Iterations - %d \t \n", MATRIX_ROWS[test], number_of_iterations[test]);
    cpu_time = ( (double) (end_clock - start_clock) ) / CLOCKS_PER_SEC;
    printf("Total execution time: \t\t %lf seconds \n", cpu_time);
    cpu_time = cpu_time / (double) number_of_iterations[test]; // time per iteration
    printf("Per iteration execution time: \t %lf seconds \n", cpu_time);
    printf("=========================================\n");

    //Running tiled versions
    for(int tile = 0; tile < NUMBER_OF_TILES; tile++){
      if((TILE[tile] >= 8 && MATRIX_ROWS[test] == 8)
          || (TILE[tile] >= 32 && MATRIX_ROWS[test] == 32)){
        continue;
      }

      start_clock = clock();
      for(int iter = 0; iter < number_of_iterations[test]; iter++){
        float * mat_a_tile = generate_output(MATRIX_ROWS[test], MATRIX_ROWS[test]);
        float * mat_b = generate_rand_input(MATRIX_ROWS[test], MATRIX_COLS[test]);
        float * mat_c = generate_rand_input(MATRIX_ROWS[test], MATRIX_COLS[test]);

        //Execute tiled
        tilemm(mat_a_tile, mat_b, mat_c, MATRIX_ROWS[test], MATRIX_COLS[test], MATRIX_COLS[test], TILE[tile]);

        free(mat_a_tile);
        free(mat_b);
        free(mat_c);
      }
      end_clock = clock();

      // Naive completed - Compute execution time.
      printf("=========================================\n");
      printf("Tile implementation: Rows - %d \t Iterations - %d \t Tiles - %d\n", MATRIX_ROWS[test], number_of_iterations[test], TILE[tile]);
      cpu_time = ( (double) (end_clock - start_clock) ) / CLOCKS_PER_SEC;
      printf("Total execution time: \t\t %lf seconds \n", cpu_time);
      cpu_time = cpu_time / (double) number_of_iterations[test]; // time per iteration
      printf("Per iteration execution time: \t %lf seconds \n", cpu_time);
      printf("=========================================\n");
    }

  }
  return 0;
}
