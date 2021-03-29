# MIPS Simulator by Seokmin Hong

The final project for computer architecture course.

## Build

    $ make

## Run

    $ ./simulator {Program_binary} {Number_of_cycles} {Mode} [Initial_register_state]

- Mode 0

    Supports pipelining without data forwarding or hazard detection. Internal forwarding of the register is supported.

- Mode 1

    Supports data forwarding and hazard detection in addition to Mode 0.


**Example**:

    $ ./simulator examples/p2/Swap-code.txt 50 0

It would run a test named `Swap` with mode 0, during 50 cycles. All registers are initialized as zero.

## Test

`make-test.cpp` supports the automatic testing. It generates the register file and binary codes from the metadata file: `{Test}-setting.txt`.

1. Test all examples.

        $ make test-all
    
    This commands executes all of tests on `./examples/p2/`.

2. Test a specific example.

        $ ./test.sh {Test_name}
    
    **Example**:

        $ ./test.sh Swap
