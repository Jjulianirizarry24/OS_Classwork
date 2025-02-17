#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/shm_block"
#define SHARED_MEM_SIZE 1000

/**
 * @brief Main entry point of the program.
 *
 * This function sets up a shared memory block for inter-process communication,
 * forks a child process to execute an external reader executable, and displays
 * a user menu in the parent process. Based on the user's menu selection, it
 * sends signals to the child process to trigger various operations:
 *
 * - SIGUSR1: Add a number.
 * - SIGUSR2: Remove a number.
 * - SIGALRM: Divide all numbers by two.
 *
 * Option 0 quits program.
 * Option 4 prints the current numbers stored in shared memory.
 *
 */
int main() {
  int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, sizeof(float) * 1000);
  float* shm_ptr = mmap(NULL, sizeof(float) * 1000, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);

  pid_t pid = fork();
  if (pid == 0) {
    execlp("../../Reader/build/reader", "reader", (char*)NULL);
  }

  else {
    int loop_val = 1;
    while (loop_val) {
      sleep(1);
      printf("  --------- \n");
      printf(" | M e n u |\n");
      printf("  --------- \n");
      printf("__________________________________\n");
      printf("0. Exit program                   |\n");
      printf("1. Add number [SIGUSR1]           |\n");
      printf("2. Remove number [SIGUSR2]        |\n");
      printf("3. Divide numbers by two [SIGALRM]|\n");
      printf("4. Print current numbers          |\n");
      printf("Enter your selection here: ");

      int input;
      scanf("%d", &input);

      printf("\n\n --------------------------- \n\n");
      switch (input) {
        case 0:
          loop_val = 0;
          break;
        case 1:
          kill(pid, SIGUSR1);
          break;
        case 2:
          kill(pid, SIGUSR2);
          break;
        case 3:
          kill(pid, SIGALRM);
          break;
        case 4:

          for (int i = 0; i < SHARED_MEM_SIZE; i++) {
            if (shm_ptr[i] == 0) {
              continue;
            }
            printf("\n [ %.2f ]  \n", shm_ptr[i]);
          }
          printf("\n");
          break;
        default:
          printf("Invalid option!\n");
      }
    }
  }

  munmap(shm_ptr, sizeof(float) * SHARED_MEM_SIZE);
  close(fd);
  shm_unlink(SHM_NAME);

  return 0;
}