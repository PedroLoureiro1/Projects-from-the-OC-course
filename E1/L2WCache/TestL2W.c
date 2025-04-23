#include "L2WCache.h"

int main() {

  // set seed for random number generator
  srand(0);

  int clock1;


  printf("\nTest accesses\n");

  for (int j = 0; j < 8; j++) {
    int address = 0x00000000;
    if (j % 2 == 0){
      address = 0x00008000;
    }
    write(address, (unsigned char *)(&address));
    clock1 = getTime();
    printf("Write; Address %d; Value %d; Time %d\n", address, address, clock1);
  }

  
  return 0;
}
