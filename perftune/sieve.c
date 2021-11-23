#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool not_prime[N/2];
static int  primes[N];

int *sieve(int n) {
  primes[0] = 2;
  int cnt = 1;
  for (int i = 0, num = i * 2 + 3; num < n; num = ++i * 2 + 3) {
    if (!not_prime[i]) primes[cnt++] = num;
    for (int j = 1; j < cnt && num * primes[j] < n; ++j) {
      not_prime[(num * primes[j] - 3) / 2] = 1;
      if (num % primes[j] == 0) break;
    }
  }
  return primes;
}
