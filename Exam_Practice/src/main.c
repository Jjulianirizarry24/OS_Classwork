#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  int pid;
  sem_t sema;

  sem_open(sema, 0);

  printf("Starting Program\n");
  wait(1);

  if (pid == 0) {
    execlp("./process_2/build/practice", "practice", NULL);
    exit(0);
  }
  sem_wait(sema);

  printf("done\n");

  return 0;
}