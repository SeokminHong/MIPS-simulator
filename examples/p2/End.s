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

;; \x20\x10\x00\x64\x20\x08\x00\x00\x20\x09\x00\x07\xa2\x08\x00\x00\x11\x09\x00\x05\x00\x00\x00\x00\x21\x08\x00\x01\x22\x10\x00\x01\x08\x00\x00\x03\x00\x00\x00\x00\x8c\x0a\x00\x64\x8c\x0b\x00\x68\x10\x00\xff\xff\x00\x00\x00\x00