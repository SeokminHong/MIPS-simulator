;; DH6

lw      $2, 20($1)
and     $4, $2, $5
or      $8, $2, $6
add     $9, $4, $2
slt     $1, $6, $7
end:
b       end

;; \x14\x00\x22\x8c\x24\x20\x45\x00\x25\x40\x46\x00\x20\x48\x82\x00\x2a\x08\xc7\x00\xff\xff\x00\x10\x00\x00\x00\x00