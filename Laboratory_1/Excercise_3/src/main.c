#include <stdio.h>

#include "functions.h"

int main() {
  char* months[] = {"January",   "February", "March",    "April",
                    "May",       "June",     "July",     "August",
                    "September", "October",  "November", "December"};

  for (int i = 0; i < 3; i++) {
    int inputSeconds = 0;
    printf("Hello! Please enter amount of seconds: ");
    scanf("%d", &inputSeconds);

    printf("\n");

    Date date = fill_date(inputSeconds);

    // Print the result in the required format
    printf("\n | %s %d, %d | \n", months[date.month], date.day, date.year);
  }
  return 0;
}
