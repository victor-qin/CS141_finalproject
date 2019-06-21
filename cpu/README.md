Code for a MIPS processor and memory with support for the following 
instructions: add, sub, and, or, slt, addi, lw, sw, beq, j.

In the `asm` directory you will find files for providing the
instructions to the processor. Use the `assemble.sh` script to
assemble a MIPS assembly file (assumed to be called `test.asm`)
into a hex machine code file called `imem.memh`. You may also
pre-populate the data memory using the `dmem.memh` file if you want.

You may run the processor by importing the files into Xilinx ISE
or you may use Icarus Verilog (compile with `iverilog *.v`, then
`./a.out` to run). The current testbench will print all the registers
as well as memory address 84 (the current test succeeds if 7 is placed
at that location). Feel free to change this code.

Note that the current assembler does not support the `mul` instruction.
You will have to extend it to support it. The `mul` instruction is
a newer MIPS feature and hence is not mentioned in the textbook. Please
refer to the MIPS32 instruction set reference we have provided on
Canvas. The `mul` instruction is special because it is an R-type except
for the fact that it has a non-zero op-code.

`mul` instruction fields:
* op:    011100
* funct: 000010

Please extend the assembler and processor to support the `mul`
instruction, and then write MIPS assembly to perform matrix
multiplication with your processor. You may use the Verilog `*`
operator.
