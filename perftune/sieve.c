#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


#define N 10000000

static bool is_prime[N] = {};
static int  primes[N];


int *sieve(int n) {
  assert(n+1<N);
  for (int i = 0; i <= n; i++){
    is_prime[i] = true;
  }
  is_prime[2] = false;

for (int i = 3; i <= n; i+=2) {
  if(is_prime[i]){
    if(i*i > n){
      break;
    }
    for(int j=i*i; j<=n; j += 2*i){
      is_prime[j]=false;
    }
  }
}

  int *p = primes;
  *p++ = 2;
  for (int i = 3; i <= n; i += 2)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
}
