;; SU (signed vs unsigned)

lui     $s0, 0xffff
ori     $s0, $s0, 0xffff
ori     $s1, $0, 0x0001
slt     $t0, $s0, $s1
sltu    $t1, $s0, $s1
end:
b       end

;; \x3c\x10\xff\xff\x36\x10\xff\xff\x34\x11\x00\x01\x02\x11\x40\x2a\x02\x11\x48\x2b\x10\x00\xff\xff\x00\x00\x00\x00