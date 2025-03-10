#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {

  int pid;
  sem_t *sema;
  sem_init(sema, 0, 0);

  

  sem_wait(sema);

  printf("done\n");

  return 0;
}