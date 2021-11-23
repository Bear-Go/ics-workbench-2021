#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool not_prime[N/2];
static int  primes[N];

// 线性筛 + 去偶数
int *sieve(int n) {
  primes[0] = 2;
  int cnt = 1;
  for (int i = 0, num = 3; num < n; num = ((++i) << 1) + 3) {
    if (!not_prime[i]) primes[cnt++] = num;
    for (int j = 1; j < cnt && num * primes[j] < n; ++j) {
      int t = num * primes[j];
      if (t < n) {
        not_prime[(t - 3) >> 1] = 1;
        if (num % primes[j] == 0) break;
      }
    }
  }
  return primes;
}
