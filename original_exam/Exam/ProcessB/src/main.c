

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

// #define SHM_NAME "/shm_block"
// #define SHARED_MEM_SIZE 1000

// void handle_signal(int sig) {
//   printf("\nSignal 1 processed\n\n");

//   int fd = shm_open(SHM_NAME, O_RDWR, 0666);
//   float *shared_data = mmap(0, sizeof(float) * SHARED_MEM_SIZE,
//                             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//   if (shared_data[0] == 0) {
//     shared_data[0] = 1;
//     printf("\nAdded value %f at index 0\n", shared_data[0]);
//   } else {
//     for (int i = 1; i < SHARED_MEM_SIZE; i++) {
//       if (shared_data[i] == 0.0f) {
//         shared_data[i] = shared_data[i - 1] * 2;

//         printf("\nAdded value %f at index %d\n", shared_data[i], i);
//         break;
//       }
//     }

//     printf("\n All done \n");
//   }
//   munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
//   close(fd);
// }

// /**
//  * @brief Signal handler for SIGUSR2.
//  *
//  * This function is invoked when the SIGUSR2 signal is received.
//  * It opens and maps the shared memory, then searches from the end
//  * for the last non-zero value and sets it to zero (effectively deleting it).
//  * After processing, it unmaps and closes the shared memory.
//  *
//  * @param sig The signal number (expected to be SIGUSR2).
//  */
// void handle_signal_2(int sig) {
//   printf("\nSignal 2 processed\n\n");
//   int fd = shm_open(SHM_NAME, O_RDWR, 0666);
//   float *shared_data = mmap(NULL, sizeof(float) * SHARED_MEM_SIZE,
//                             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

//   for (int i = SHARED_MEM_SIZE - 1; i >= 0; i--) {
//     if (shared_data[i] != 0) {
//       printf("deleted float %f at index %d \n", shared_data[i], i);
//       shared_data[i] = 0;

//       break;
//     }
//   }
//   munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
//   close(fd);
// }

// /**
//  * @brief Main function.
//  *
//  * This is the entry point of the program. It opens the shared memory object
//  * and maps it into the process's address space, initializing all elements to
//  * zero. Signal handlers for SIGUSR1, SIGUSR2, and SIGALRM are registered.
//  The
//  * program then enters an infinite loop, waiting for signals.
//  *
//  */
// void handle_alarm(int sig) {
//   printf("\nSignal 3 processed\n\n");
//   int fd = shm_open(SHM_NAME, O_RDWR, 0666);
//   float *shared_data = mmap(NULL, sizeof(float) * SHARED_MEM_SIZE,
//                             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

//   for (int i = 0; i < SHARED_MEM_SIZE; i++) {
//     if (shared_data[i] != 0) {
//       shared_data[i] /= 2;
//     }
//   }
//   printf("Divided all numbers :)\n");

//   munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
//   close(fd);
// }

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

  for (int i = 0; i < BLOCK_SIZE; i++) {
    shm_ptr[i] = 2.0;
  }

  // while (1) {

  queue = mq_open(QUEUE_NAME, O_RDONLY, 0644, &attr);
  bytes_read = mq_receive(queue, message, 8192, NULL);

  // received_message = message;

  printf("\n\n %s \n\n", message);

  // mq_close(queue);

  if (strcmp(message, "square") == 0) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      shm_ptr[i] = shm_ptr[i] * shm_ptr[i];
    }
    printf("\n received square message: %s\n", message);
  }
  if (strcmp(message, "half") == 0) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      shm_ptr[i] = shm_ptr[i] / 2;
    }
    printf("\n received half message: %s\n", message);
  }
  // }
  // for (;;);
}