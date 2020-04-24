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

;; \xde\xab\x11\x3c\xfe\x9a\x24\x36\xde\xab\x25\x36\xaa\xaa\x26\x36\x24\x40\x85\x00\x27\x40\x08\x01\x27\x48\xa6\x00\x25\x50\x09\x01\xff\xff\x4a\x31\xff\xff\x00\x10\x00\x00\x00\x00