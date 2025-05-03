#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE 4096

int main()
{
  float x = 19;

  printf("my pid: %d\n", getpid());

  int *first_block = mmap(NULL, 3 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  int *second_block = mmap(0x36000, 3 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  printf("first block: %lx\n", first_block);
  printf("second block: %lx\n", second_block);
  printf("x addr: %lx\n", &x);
  
  // // Free the allocated memory
  // if (munmap(first_block, 3 * PAGE_SIZE) == -1) {
  //   perror("munmap failed");
  //   return 1;
  // }

  // printf("first block erased\n");

  sleep(1000);

  return 0;
}

