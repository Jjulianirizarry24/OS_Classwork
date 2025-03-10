

#include <fcntl.h>
#include <features.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

int bool = 0;

handle_exit(int sig) {
  printf("\nexit program C\n");
  exit(0);
}

void handle_signal(int sig) {
  bool = 1;
  // printf("\n");
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <folder_path> <...>\n", argv[0]);
    return 1;
  }
  printf("Process C Initialized\n");

  char* SHM_NAME;
  SHM_NAME = argv[1];
  float BLOCK_SIZE = sizeof(float) * 25;

  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr =
      mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  struct sigaction sa;
  sa.sa_handler = handle_signal;

  struct sigaction sa2;
  sa2.sa_handler = handle_exit;
  sigaction(SIGINT, &sa2, NULL);

  while (1) {
    sigaction(SIGUSR1, &sa, NULL);

    if (bool == 1) {
      for (int i = 0; i < 25; i++) {
        printf("%f\n", shm_ptr[i]);
      }
      bool = 0;
    }
  }

  // printf("end program\n");
}