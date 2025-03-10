#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

char* shm;
char* queue;
int bool = 1;

handle_exit(int sig) {
  // bool = 0;
  printf("\n exit program A\n");
  shm_unlink(shm);
  mq_unlink(queue);
  exit(0);
}

// Process A
int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <folder_path> <...> <...>\n", argv[0]);
    return 1;
  }

  printf("Start of Program A\n");

  // ----------------- Initialize Variables -----------------
  char* QUEUE_NAME;
  char* SHM_NAME;
  float BLOCK_SIZE;

  SHM_NAME = argv[1];
  QUEUE_NAME = argv[2];

  shm = SHM_NAME;
  queue = QUEUE_NAME;

  BLOCK_SIZE = sizeof(float) * 25;

  mqd_t queue;

  struct mq_attr attr;

  // Initialize the queue attributes
  attr.mq_flags = 0;
  attr.mq_maxmsg = 5;
  attr.mq_msgsize = sizeof(char) * 30;
  attr.mq_curmsgs = 0;

  shm_unlink(SHM_NAME);
  mq_unlink(QUEUE_NAME);

  queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
  if (queue == (mqd_t)-1) {
    perror("mq_open failed in Process A");
    exit(1);
  }

  // Initialize shared memory block
  int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr =
      mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  pid_t pid;
  //----------------- Call Process B and C-----------------
  for (int i = 0; i < 2; i++) {
    // Process B
    pid = fork();
    if (pid == 0 && i == 0) {
      execlp("../../ProcessB/build/processB", "processB", SHM_NAME, QUEUE_NAME,
             (char*)NULL);
      exit(0);
    }
    // Process C
    if (pid == 0 && i == 1) {
      execlp("../../ProcessC/build/processC", "processC", SHM_NAME,
             (char*)NULL);
      exit(0);
    }
    if (pid == 0) {
      exit(0);
    }
  }

  struct sigaction sa;
  sa.sa_handler = handle_exit;
  sigaction(SIGINT, &sa, NULL);

  while (bool) {
    sleep(1);
    printf("  --------- \n");
    printf(" | M e n u |\n");
    printf("  --------- \n");
    printf("__________________________________\n");
    printf("0. Exit program                   |\n");
    printf("1. Square                         |\n");
    printf("2. Divide                         |\n");
    printf("3. Plot                           |\n");
    printf("Enter your selection here: ");

    int input;
    scanf("%d", &input);

    printf("\n\n --------------------------- \n\n");

    if (input == 0) {
      break;
    }

    // Square
    if (input == 1) {
      mq_send(queue, "square", sizeof(char) * 30, 0);
    }

    // Divide
    else if (input == 2) {
      mq_send(queue, "half", sizeof(char) * 30, 0);
    }

    // Plot
    else if (input == 3) {
      kill(pid, SIGUSR1);
    }

    else {
      printf("\n Invalid option. Try Again. \n");
    }
  }

  // printf("\n End Process A \n");
  // munmap(shm_ptr, BLOCK_SIZE);
  // shm_unlink(SHM_NAME);
  // mq_unlink(QUEUE_NAME);

  return 0;
}