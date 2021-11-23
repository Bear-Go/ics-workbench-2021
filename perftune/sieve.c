#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  int cnt = 0;
  for (int i = 2; i <= n; ++i) {
    if (!is_prime[i]) primes[cnt++] = i;
    for (int j = 0; j < cnt && primes[j]*i <= n; ++j) {
      int temp = primes[j] * i;
      if (temp <= n) {
        is_prime[temp] = 1;
        if (i%primes[j]==0) break;
      }
    }
  }
  return primes;
}
