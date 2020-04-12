# MIPS Simulator by Seokmin Hong

## Build

    $ cmake CMakeLists.txt
    $ make

## Run

    $ ./mips-sim {Program_binary} {Number_of_cycles} [Initial_register_state]

Example:

    $ ./mips-sim examples/p1/p1e1.txt 1 init.txt