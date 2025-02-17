#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functions.h"

/**
 * @brief Entry point of the program.
 *
 * This program starts by printing a startup message and then prompts the user
 * to enter a string value. It then forks a child process. The child process
 * uses `execlp()` to replace its image with the `word_explorer` executable,
 * passing along a directory path and the user-provided string as arguments.
 * Meanwhile, the parent process waits for the child process to complete before
 * printing a termination message.
 *
 */
int main() {
  printf("Start of Program\n");

  char* input;
  printf("Enter your string value: ");
  scanf("%s", input);

  pid_t pid = fork();

  if (pid == 0) {
    execlp("../../word_explorer/build/word_explorer", "word_explorer",
           "../../word_explorer/build/test_files", input, (char*)NULL);

  } else {
    printf("Waiting for child process\n");
    wait(NULL);
    printf("End of program!\n");
  }

  return 0;
}
