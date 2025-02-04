#include <stdio.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int main() {
  srand(time(NULL));
  printf("Start of Program :) \n\n");

  printf("Enter text: ");
  char input[80];
  fgets(input, 80, stdin);
  char copy[80];

  printf("%s", input);
  strcpy(copy, input);

  printf("\n");
  char *base = input;
  char *mod = copy;

  upper_rand(base, mod);
  print_arrays(base, mod);

  printf("\n End of Program. Goodbye! \n");

  return 0;
}
