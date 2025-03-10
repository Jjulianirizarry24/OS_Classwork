#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

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
