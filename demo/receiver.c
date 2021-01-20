#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <util.h>
#include <l3.h>

// Controls the total number of samples for
// measuring channel capacity.
#define SAMPLES 1000


int main(int ac, char **av) {
  delayloop(3000000000U);

  // Fill L3 info, allocate buffer with huge pages,
  // create cache map, and allocate monitored set info.
  // Returns NULL when mmap or ptemap & probemap fails.
  l3pp_t l3 = l3_prepare(NULL);

  if (l3 == NULL) {
      printf("Initialization failed!\n");
      return 1;
  }

  int nsets = l3_getSets(l3);

  uint16_t *res = calloc(SAMPLES, sizeof(uint16_t));
  for (int i = 0; i < SAMPLES; i+= 4096/sizeof(uint16_t))
    res[i] = 1;
  
  for (int i = 0; i < nsets; i++) {
    l3_unmonitorall(l3);
    l3_monitor(l3, i);

    l3_repeatedprobecount(l3, SAMPLES, res, 0);

    for (int j = 0; j < SAMPLES; j++) {
      printf("%4d ", (int16_t)res[j]);
    }
    putchar('\n');
  }

  free(res);
  l3_release(l3);
  return 0;
}

