;; DH2

addi    $t0, $zero, 5
sw      $s0, 20($t1)
lw      $s0, 20($t1)
sub     $t2, $s0, $t3
end:
b       end
