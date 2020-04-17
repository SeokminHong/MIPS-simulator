# MIPS Simulator by Seokmin Hong

## Build

    $ make

## Run

    $ ./simulator {Program_binary} {Number_of_cycles} [Initial_register_state]

Example:

    $ ./simulator examples/p1/p1e1.txt 10 init.txt

## Test

1. Test all examples.

        $ make test

2. Test a specific example.

        $ make test1

## Description

Phase 1 for the project of Computer Architecture class. It handles branch and jump instructions. It regards that branch is always taken and `$ra` is only set by `jal` instruction.

### Test 1

#### Command

    $ ./simulator examples/p1/p1e1.txt 5

#### Output

    Cycle 1
    PC: 0000
    Instruction: 00854024

    Cycle 2
    PC: 0004
    Instruction: 01084027

    Cycle 3
    PC: 0008
    Instruction: 00a64827

    Cycle 4
    PC: 000C
    Instruction: 01095025

    Cycle 5
    PC: 0010
    Instruction: 314a0014

Threre is no branch and jump instructions so it just prints instructions by the sequence.

### Test 2

#### Command

    $ ./simulator examples/p1/p1e2.txt 10

#### Output

    Cycle 1
    PC: 0000
    Instruction: 20820000   # addi

    Cycle 2
    PC: 0004
    Instruction: 00a4802a   # slt

    Cycle 3
    PC: 0008
    Instruction: 12000001   # beq $s0, $zero, L1

    Cycle 4
    PC: 0010
    Instruction: 20840001   # L1: addi

    Cycle 5
    PC: 0014
    Instruction: 00441020   # add

    Cycle 6
    PC: 0018
    Instruction: 1485fffd   # bne $a0, $a1, L1

    Cycle 7
    PC: 0010
    Instruction: 20840001   # L1: addi

    Cycle 8
    PC: 0014
    Instruction: 00441020   # add

    Cycle 9
    PC: 0018
    Instruction: 1485fffd   # bne $a0, $a1, L1

    Cycle 10
    PC: 0010
    Instruction: 20840001   # L1: addi

It takes branch to L1 when the `beq` and `bne` instructions are executed.

### Test 3

#### Command

    $ ./simulator examples/p1/p1e3.txt 15

#### Output

    Cycle 1
    PC: 0000
    Instruction: 08000008   # j main

    Cycle 2
    PC: 0020
    Instruction: 0c000001   # main: jal swap

    Cycle 3
    PC: 0004
    Instruction: 00054880   # swap: sll

    Cycle 4
    PC: 0008
    Instruction: 00894820   # add

    Cycle 5
    PC: 000C
    Instruction: 8d280000   # lw

    Cycle 6
    PC: 0010
    Instruction: 852a0004   # lh

    Cycle 7
    PC: 0014
    Instruction: a52a0000   # sh

    Cycle 8
    PC: 0018
    Instruction: ad280004   # sw

    Cycle 9
    PC: 001C
    Instruction: 03e00008   # jr $ra

    Cycle 10
    PC: 0024
    Instruction: 20a50001   # addi

    Cycle 11
    PC: 0028
    Instruction: 00a6802a   # slt

    Cycle 12
    PC: 002C
    Instruction: 1200fffc   # beq $s0, $zero, main

    Cycle 13
    PC: 0020
    Instruction: 0c000001   # main: jal swap

    Cycle 14
    PC: 0004
    Instruction: 00054880   # swap: sll

    Cycle 15
    PC: 0008
    Instruction: 00894820   # add

The simulator implements `jr` and `jal` command.

### Test 4

#### Command

    $ ./simulator examples/p1/p1e4.txt 30

#### Output

    Cycle 1
    PC: 0000
    Instruction: 0c000004   # loop: jal fib

    Cycle 2
    PC: 0010
    Instruction: 23bdfffc   # fib: addi $sp, $sp, -4

    Cycle 3
    PC: 0014
    Instruction: afbf0000   # sw $ra, 0($sp)

    Cycle 4
    PC: 0018
    Instruction: 20a5ffff   # addi $a1, $a1, -1

    Cycle 5
    PC: 001C
    Instruction: 0c000011   # jal read

    Cycle 6
    PC: 0044
    Instruction: 00054080   # read: sll

    Cycle 7
    PC: 0048
    Instruction: 00884820   # add

    Cycle 8
    PC: 004C
    Instruction: 8d300000   # lw

    Cycle 9
    PC: 0050
    Instruction: 03e00008   # jr $ra

    Cycle 10
    PC: 0020
    Instruction: 22110000   # addi $s1, $s0, 0

    Cycle 11
    PC: 0024
    Instruction: 20a5ffff   # addi $a1, $a1, -1

    Cycle 12
    PC: 0028
    Instruction: 0c000011   # jal read

    Cycle 13
    PC: 0044
    Instruction: 00054080   # read: sll

    Cycle 14
    PC: 0048
    Instruction: 00884820   # add

    Cycle 15
    PC: 004C
    Instruction: 8d300000   # lw

    Cycle 16
    PC: 0050
    Instruction: 03e00008   # jr $ra

    Cycle 17
    PC: 002C
    Instruction: 02308820   # add $s1, $s1, $s0

    Cycle 18
    PC: 0030
    Instruction: 20a50002   # addi $a1, $a1, 2

    Cycle 19
    PC: 0034
    Instruction: 0c000015   # jal write

    Cycle 20
    PC: 0054
    Instruction: 00054080   # write: sll

    Cycle 21
    PC: 0058
    Instruction: 00884820   # add

    Cycle 22
    PC: 005C
    Instruction: ad310000   # sw

    Cycle 23
    PC: 0060
    Instruction: 03e00008   # jr $ra

    Cycle 24
    PC: 0038
    Instruction: 8fbf0000   # lw $ra, 0($sp)

    Cycle 25
    PC: 003C
    Instruction: 23bd0004   # addi $sp, $sp, 4

    Cycle 26
    PC: 0040
    Instruction: 03e00008   # jr $ra

    Cycle 27
    PC: 0004
    Instruction: 20a50001   # addi $a1, $a1, 1

    Cycle 28
    PC: 0008
    Instruction: 14a6fffd   # bne $a1, $a2, loop

    Cycle 29
    PC: 0000
    Instruction: 0c000004   # jal fib

    Cycle 30
    PC: 0010
    Instruction: 23bdfffc   # fib: addi

`fib` function calls `read` and `write` functions very well and it returns to `loop` label when the `jr $ra` instruction executed, cycle 26.