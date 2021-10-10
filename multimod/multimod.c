#include <stdint.h>
#include <assert.h>

static inline int is_overflow(uint64_t a, uint64_t b) {
    int c = 0;
    for (int i = 0; i < 64; ++ i, a >>= 1, b >>= 1){
        c = ((a & 1) & (b & 1)) | ((a & 1) & (c & 1)) | ((c & 1) & (b & 1));
    }
    assert(c == 0 || c == 1);
    return c;
}

static inline void transfer(uint64_t *a, uint64_t *b) {
    uint64_t t = 0xffffffffffffffff - *a;
    *b = *b - t;
    *a = 0xffffffffffffffff;
    assert(*a);
    assert(*b);
    return;
}

static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
    if (is_overflow(a, b)) {
        transfer(&a, &b);
    }
    if (a >= m) {
        a -= m;
    }
    if (b >= m) {
        b -= m;
    }
    assert(a>=0);
    assert(b>=0);
    a += b;
    if (a >= m) {
        a -= m;
    }
    assert(a>=0);
    return a;
}

uint64_t mod(uint64_t x, uint64_t m) {
    if (x < m) {
        return x;
    }
    
    uint64_t k, c, quotient = 0, remainder;
    while (x > m) {
        for (k = 0, c = m; x >= c; c <<= 1, ++ k) {
            if (x - c < m) {
                quotient += 1 << k;
                remainder = x - c;
                break;
            }
        }
        if (x - c < m) {
            break;
        }
        quotient += 1 << (k -1);
        x -= c >> 1;
    }
    return remainder;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t ans = 0;
    for (b = mod(b, m); a ; a >>= 1) {
        if (a & 1) {
            ans = addmod(ans, b, m);
        }
        b = addmod(b, b, m);
    }
    assert(ans>=0);
    return ans;
}

// uint64_t mod(uint64_t x, uint64_t m) {
//     assert(x >= 0);
//     while (x >= m) {
//         x -= m;
//     }
//     assert(x >= 0);
//     return x;
// }

// uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
//     uint64_t res = 0;
//     b = mod(b, m);
//     while (a) {
//         if (a & 1) {
//             res += b;
//             if (res >= m) {
//                 res -= m;
//             }
//         }
//         b += b;
//         if (b >= m) {
//             b -= m;
//         }
//         a >>= 1;
//     }
//     return res;
// }
