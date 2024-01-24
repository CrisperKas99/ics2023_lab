#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

//check if sum1+sum2 overflows
bool overflow(uint64_t sum1, uint64_t sum2, uint64_t sum){
	if(sum < sum1 || sum < sum2 )
		return true;
	return false;
}

//return x%m
// uint64_t mymod(uint64_t x, uint64_t m){
//   while (x >= m){
//     x -= m;
//   }
//   return x;
// }

uint64_t mymod(uint64_t x, uint64_t m){
  for(int i = 63; i >= 0; i--){
    uint64_t test = ~ ((1ULL << (64 - i)) - 1);
    //printf("No%d.test=%ld\n",i,test);
    if (x >= m << i && (test & m ) == false){
      x -= m << i;
    }
  }
  //assert(0);
  if(x >= m){
    x -= m;
  }
  return x;
}

// uint64_t myquickmod(uint64_t x, uint64_t m){
//   uint64_t result = 0;
//   while (x > 0){
//     result = (result + (x & 1)) % m;
//     x >>= 1;
//   }
//   return result;
// }

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result = 0;

  //降低a和b的规模
  a = mymod (a, m);
  b = mymod (b, m);

  for (int i = 0;i < 64; i++){
    if (b & (1ULL << i)) {
      if (overflow (result, a, result+a) == false){
        result += a;
        mymod(result,m);
      }
      else { //overflows
        uint64_t x = result;
        uint64_t y = a;
        while (overflow (x, y, x+y) == true){
          x = mymod (x+y, m);
          y = mymod (~m+1, m);
        }
        result = mymod (x+y, m);
      }
    }

    
    //a right shift 1 digit
    if (overflow (a,a,a+a) == false){
      a <<= 1;
      a = mymod (a,m);
    }
    else {
      uint64_t z = a;
      uint64_t t = a ;
      while (overflow(z,t,z+t) == true){
        //printf("here\n");
        z = mymod (z+t, m);
        //printf("z\n");
        t = mymod (~m+1, m);
        //printf("t\n");
     }
    //printf("here2\n");
    a = mymod (z+t,m);
    }

  }
   result = mymod(result, m);
   return result;
}


