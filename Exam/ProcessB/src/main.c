

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

handle_exit(int sig) {
  printf("\nexit program B\n");
  exit(0);
}
int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <folder_path> <...> <...>\n", argv[0]);
    return 1;
  }
  printf("Process B Initialized\n");

  struct mq_attr attr;

  // Initialize the queue attributes
  attr.mq_flags = 0;
  attr.mq_maxmsg = 5;
  attr.mq_msgsize = sizeof(char) * 30;
  attr.mq_curmsgs = 0;

  char* QUEUE_NAME;
  char* SHM_NAME;
  SHM_NAME = argv[1];
  QUEUE_NAME = argv[2];
  float BLOCK_SIZE = sizeof(float) * 25;

  mqd_t queue;
  char message[1024];
  int bytes_read;
  char* received_message;

  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr =
      mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (int i = 0; i < 25; i++) {
    shm_ptr[i] = 2.0;
  }

  struct sigaction sa;
  sa.sa_handler = handle_exit;
  
  while (1) {
    sigaction(SIGINT, &sa, NULL);
    queue = mq_open(QUEUE_NAME, O_RDONLY);
    if (queue == -1) {
      continue;
    }
    bytes_read = mq_receive(queue, message, sizeof(char) * 30, NULL);

    // received_message = message;
    if (bytes_read == -1) {
      perror("mq_receive failed");
      exit(1);
    }
    printf("\n\n Received Message: %s \n\n", message);

    if (strcmp(message, "square") == 0) {
      for (int i = 0; i < 25; i++) {
        shm_ptr[i] *= shm_ptr[i];
      }
    }

    if (strcmp(message, "half") == 0) {
      for (int i = 0; i < 25; i++) {
        shm_ptr[i] /= 2;
      }
    }

    mq_close(queue);
  }
}