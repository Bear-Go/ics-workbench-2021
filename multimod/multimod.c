#include <stdint.h>

static inline uint64_t mod(uint64_t a, uint64_t m) {
    uint64_t b = m;
    while (b <= a >> 1) b <<= 1;
    for (; a >= m; b >>= 1) 
    	if (a >= b) a -= b;
    return a;
}

static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t max = 0xffffffffffffffff;
    if (a + b < a || a + b < b)
        return addmod(mod(a + b, m), mod(max, m) + 1, m);
    else
        return mod(a + b, m);
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t ans = 0;
    for (; a ; a >>= 1) {
        if (a & 1) ans = addmod(ans, b, m);
        b = addmod(b, b, m);
    }
    return ans;
}
