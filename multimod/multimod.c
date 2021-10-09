#include <stdint.h>

static inline int is_overflow(uint64_t a, uint64_t b) {
    int c = 0;
    for (int i = 0; i < 64; ++ i, a >>= 1, b >>= 1){
        c = ((a & 1) & (b & 1)) | ((a & 1) & (c & 1)) | ((c & 1) & (b & 1));
    }
    return c;
}

static inline void transfer(uint64_t *a, uint64_t *b) {
    uint64_t t = 0xffffffffffffffff - *a;
    *b = *b - t;
    *a = 0xffffffffffffffff;
    return;
}

// static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
//     //wait to be solved
//     int flag = 1;
//     if (is_overflow(a, b)) {
//         transfer(&a, &b);
//     }
//     else {
//         a = a + b;
//         flag = 0;
//     }

//     while (a >= m) {
//         a -= m;
//         if (flag) {
//             if (is_overflow(a, b)) {
//                 transfer(&a, &b);
//             }
//             else {
//                 a = a + b;
//                 flag = 0;
//             }
//         }
//     }  
//     return a;
// }

static inline uint64_t addmod(uint64_t a, uint64_t b, uint64_t m) {
    if (is_overflow(a, b)) {
        transfer(&a, &b);
    }
    while (a >= m) {
        a -= m;
    }
    while (b >= m){
        b -= m;
    }
    a += b;
    while (a >= m) {
        a -= m;
    }
    return a;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t ans = 0;
    for (; a != 0; a >>= 1) {
        if (a & 1) {
            ans = addmod(ans, b, m);
        }
        b = addmod(b, b, m);
    }
    return ans;
}
