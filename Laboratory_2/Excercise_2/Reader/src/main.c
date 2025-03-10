

#include <fcntl.h>
#include <features.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

#define SHM_NAME "/shm_block"
#define SHARED_MEM_SIZE 1000

/**
 * @brief Signal handler for SIGUSR1.
 *
 * This function is invoked when the SIGUSR1 signal is received.
 * It opens the shared memory object, maps it into the process's address space,
 * and then either initializes the first element to 1 if the memory is empty or
 * adds a new value at the first available index by doubling the previous value.
 * Finally, it unmaps and closes the shared memory.
 *
 * @param sig The signal number (expected to be SIGUSR1).
 **/
void handle_signal(int sig) {
  printf("\nSignal 1 processed\n\n");

  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  float *shared_data = mmap(0, sizeof(float) * SHARED_MEM_SIZE,
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shared_data[0] == 0) {
    shared_data[0] = 1;
    printf("\nAdded value %f at index 0\n", shared_data[0]);
  } else {
    for (int i = 1; i < SHARED_MEM_SIZE; i++) {
      if (shared_data[i] == 0.0f) {
        shared_data[i] = shared_data[i - 1] * 2;

        printf("\nAdded value %f at index %d\n", shared_data[i], i);
        break;
      }
    }

    printf("\n All done \n");
  }
  munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
  close(fd);
}

/**
 * @brief Signal handler for SIGUSR2.
 *
 * This function is invoked when the SIGUSR2 signal is received.
 * It opens and maps the shared memory, then searches from the end
 * for the last non-zero value and sets it to zero (effectively deleting it).
 * After processing, it unmaps and closes the shared memory.
 *
 * @param sig The signal number (expected to be SIGUSR2).
 */
void handle_signal_2(int sig) {
  printf("\nSignal 2 processed\n\n");
  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  float *shared_data = mmap(NULL, sizeof(float) * SHARED_MEM_SIZE,
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (int i = SHARED_MEM_SIZE - 1; i >= 0; i--) {
    if (shared_data[i] != 0) {
      printf("deleted float %f at index %d \n", shared_data[i], i);
      shared_data[i] = 0;

      break;
    }
  }
  munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
  close(fd);
}

/**
 * @brief Main function.
 *R1, SIGUSR2, and SIGALRM are registered. The
 * program then enters an infinite loop, waiting for signals.
 *
 */
void handle_alarm(int sig) {
  printf("\nSignal 3 processed\n\n");
  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  float *shared_data = mmap(NULL, sizeof(float) * SHARED_MEM_SIZE,
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (int i = 0; i < SHARED_MEM_SIZE; i++) {
    if (shared_data[i] != 0) {
      shared_data[i] /= 2;
    }
  }
  printf("Divided all numbers :)\n");

  munmap(shared_data, sizeof(float) * SHARED_MEM_SIZE);
  close(fd);
}

int main() {
  int fd = shm_open(SHM_NAME, O_RDWR, 0666);
  float *shared_data = mmap(NULL, sizeof(float) * SHARED_MEM_SIZE,
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for (int i = 0; i < SHARED_MEM_SIZE; i++) {
    shared_data[i] = 0;
  }
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal_2);
  signal(SIGALRM, handle_alarm);

  for (;;);
}