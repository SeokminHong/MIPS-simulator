;; End (endianness)

addi    $s0, $0, 100
addi    $t0, $0, 0
addi    $t1, $0, 7
L1:
sb      $t0, 0($s0)
beq     $t0, $t1, L2
addi    $t0, $t0, 1
addi    $s0, $s0, 1
j       L1
L2:
lw      $t2, 100($0)
lw      $t3, 104($0)
end:
b       end

;; \x64\x00\x10\x20\x00\x00\x08\x20\x07\x00\x09\x20\x00\x00\x08\xa2\x05\x00\x09\x11\x00\x00\x00\x00\x01\x00\x08\x21\x01\x00\x10\x22\x03\x00\x00\x08\x00\x00\x00\x00\x64\x00\x0a\x8c\x68\x00\x0b\x8c\xff\xff\x00\x10\x00\x00\x00\x00