;; Logical (no branch, no memory I/O)
; f(a,b,c) = ~(a&b)|~(b|c)&0xffff

lui $s1, 0xabde
ori $a0, $s1, 0x9afe
ori $a1, $s1, 0xabde
ori $a2, $s1, 0xaaaa
and $t0, $a0, $a1
nor $t0, $t0, $t0
nor $t1, $a1, $a2
or $t2, $t0, $t1
andi $t2, $t2, 0xffff
end:
b       end

;; \x3c\x11\xab\xde\x36\x24\x9a\xfe\x36\x25\xab\xde\x36\x26\xaa\xaa\x00\x85\x40\x24\x01\x08\x40\x27\x00\xa6\x48\x27\x01\x09\x50\x25\x31\x4a\xff\xff\x10\x00\xff\xff\x00\x00\x00\x00