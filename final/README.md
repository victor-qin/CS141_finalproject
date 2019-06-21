# Victor Qin, CS 141 Final Project, Spring 2019
## Tiled Matrix Multiplication

The main.c file is the main executable - for every matrix size it runs the naive algorithm and prints it to the console first. It then proceeds through different tilings (2, 4, 8, 16, 32, 64) and runs the tilings that are possible for the matrix size.

The data analysis and graphing is done in the Excel file in this folder, with the raw data recorded in results1.txt and results2.txt.

main_orginal.c is a single run version of main.c The data it records is much less consistent than main.c - for example, when running 1024 sized matrices the tiled implementations are comparable in speed to the naive implementation.
