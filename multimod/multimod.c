#include <stdint.h>

static inline int carry(uint64_t a, uint64_t b) {
    int c = 0;
    for (int i = 0; i < 64; ++ i, a >>= 1, b >>= 1){
        c = ((a & 1) & (b & 1)) | ((a & 1) & (c & 1)) | ((c & 1) & (b & 1));
    }
    return c;
}

static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
    //wait to be solved
    /*
    if (1) {
        uint64_t t = 0xffffffffffffffff - a;
        b = b - t;
        a = 0xffffffffffffffff;
    }
    else {
        a = a + b;
    }
    bool flag = false;
    */
    return (a + b) % m;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t old = (a * b) % m,ans = 0;
    for (; a != 0; a >>= 1) {
        if (a & 1) {
            ans = addmod(ans, b, m);
        }
        b = addmod(b, b, m);
    }
    return ans;
}
