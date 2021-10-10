#include <stdint.h>
#include <assert.h>

// static inline int is_overflow(uint64_t a, uint64_t b) {
//     int c = 0;
//     for (int i = 0; i < 64; ++ i, a >>= 1, b >>= 1){
//         c = ((a & 1) & (b & 1)) | ((a & 1) & (c & 1)) | ((c & 1) & (b & 1));
//     }
//     assert(c == 0 || c == 1);
//     return c;
// }

// static inline void transfer(uint64_t *a, uint64_t *b) {
//     uint64_t t = 0xffffffffffffffff - *a;
//     *b = *b - t;
//     *a = 0xffffffffffffffff;
//     assert(*a);
//     assert(*b);
//     return;
// }

// static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
//     if (is_overflow(a, b)) {
//         transfer(&a, &b);
//     }
//     while (a >= m && a > 0) {
//         a -= m;
//     }
//     while (b >= m && b > 0){
//         b -= m;
//     }
//     assert(a>=0);
//     assert(b>=0);
//     a += b;
//     while (a >= m && a > 0) {
//         a -= m;
//     }
//     assert(a>=0);
//     return a;
// }

// uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
//     uint64_t ans = 0;
//     for (; a >  0; a >>= 1) {
//         if (a & 1) {
//             ans = addmod(ans, b, m);
//         }
//         b = addmod(b, b, m);
//     }
//     assert(ans>=0);
//     return ans;
// }

uint64_t mod(uint64_t x, uint64_t m) {
    assert(x >= 0);
    while (x >= m) {
        x -= m;
    }
    assert(x >= 0);
    return x;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t res = 0;
    b = mod(b, m);
    while (a) {
        if (a & 1) {
            res += b;
            res = mod(res, m);
        }
        b += b;
        b = mod(b, m);
        a >>= 1;
    }
    return res;
}
