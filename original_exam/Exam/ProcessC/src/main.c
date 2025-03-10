

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

void handle_signal(int sig, int* bool) { bool = 1; }

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <folder_path> <...>\n", argv[0]);
    return 1;
  }
  printf("Process C Initialized\n");
  struct mq_attr attr;

  // Initialize the queue attributes
  attr.mq_flags = 0;
  attr.mq_maxmsg = 5;
  attr.mq_msgsize = sizeof(char) * 30;
  attr.mq_curmsgs = 0;

  char* QUEUE_NAME;
  char* SHM_NAME;
  QUEUE_NAME = argv[1];
  SHM_NAME = argv[2];
  float BLOCK_SIZE = sizeof(float) * 25;

  mqd_t queue;
  char message[1024];
  int bytes_read;
  char* received_message;

  int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr =
      mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  // int bool = 0;
  signal(SIGUSR1, handle_signal);

  while (1) {
    if (bool == 1) {
      for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%f\n", shm_ptr[i]);
      }
      bool = 0;
    }
  }
}