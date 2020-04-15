# MIPS Simulator by Seokmin Hong

## Build

    $ cmake CMakeLists.txt
    $ make

## Run

    $ ./MIPS_Simulator {Program_binary} {Number_of_cycles} [Initial_register_state]

Example:

    $ ./MIPS_Simulator examples/p1/p1e1.txt 10 init.txt

## Description

Phase 1 for the project of Computer Architecture class. It handles branch and jump instructions. It regards that branch is always taken and `$ra` is only set by `jal` instruction.