;; DH3
lw	$t1, 0($t0)
lw	$t2, 4($t0)
lw	$t4, 8($t0)
add	$t3, $t1, $t2
sw	$t3, 12($t0)
add	$t5, $t1, $t4
sw	$t5, 16($t0)
end:
b       end

;; \x8d\x09\x00\x00\x8d\x0a\x00\x04\x8d\x0c\x00\x08\x01\x2a\x58\x20\xad\x0b\x00\x0c\x01\x2c\x68\x20\xad\x0d\x00\x10\x10\x00\xff\xff\x00\x00\x00\x00