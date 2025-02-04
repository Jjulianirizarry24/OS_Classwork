#include <stdio.h>

#include "functions.h"

int main() {
  char *months[] = {"January",   "February", "March",    "April",
                    "May",       "June",     "July",     "August",
                    "September", "October",  "November", "December"};

  printf("\nStart of Program \n");

  for (int i = 0; i < 3; i++) {
    int inputSeconds = 0;
    printf("Hello! Please enter amount of seconds: ");
    scanf("%d", &inputSeconds);

    printf("\n");

    int day = 1;
    int month = 0;
    int year = 1970;

    int *d = &day;
    int *m = &month;
    int *y = &year;

    fill_date(inputSeconds, d, m, y);

    // Print the result in the required format
    printf("\nYour Date is => | %s %d, %d | \n", months[*m], *d, *y);
  }
  printf("\nEnd of Program \n");
  return 0;
}
