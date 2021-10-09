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
    while (a >= m && a > 0) {
        a -= m;
    }
    while (b >= m && b > 0){
        b -= m;
    }
    assert(a>=0);
    assert(b>=0);
    a += b;
    while (a >= m && a > 0) {
        a -= m;
    }
    assert(a>=0);
    return a;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t ans = 0;
    for (; a >  0; a >>= 1) {
        if (a & 1) {
            ans = addmod(ans, b, m);
        }
        b = addmod(b, b, m);
    }
    assert(ans);
    return ans;
}
