#include <stdio.h>

#include "excercise1.h"

int main() {
  int init_number, end_number;

  // Ask the user for the lower and upper limits
  printf("Enter initial number value: ");
  scanf("%d", &init_number);

  printf("Enter last number value: ");
  scanf("%d", &end_number);

  int result = cosine_series(init_number, end_number);
  printf("\n Answer is: ");
  if (result == 1) {
    printf("Positive\n");
  } else {
    printf("Negative\n");
  }

  return 0;
}
