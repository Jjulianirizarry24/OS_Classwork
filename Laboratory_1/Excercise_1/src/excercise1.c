#include "excercise1.h"

#include <math.h>
#include <stdio.h>

/*

Write a function with the following prototype:
int cosine_series(int init_number, int end_number);

The function must compute the cosine of all integer numbers between init_number
and end_number (both included). It must return 1 if the sum is positive or zero.
In case the sum is negative, it must return -1. Write a main program that asks
the user for the lower and upper limits of the series (i.e., 4 and 23) and then
use the function to calculate if the cosine_series calculation is positive or
negative.

Upload a compressed file with the project's folder, including the "src" and
"include" directories and the cmakelists.txt file. The program will not be
evaluated if it does not compile correctly with the cmakelists.txt file.

*/

int cosine_series(int init_number, int end_number) {
  double curr = 0;

  for (int i = init_number; i <= end_number; i++) {
    curr += cos(init_number);

    if (curr < 0) {
      return -1;
    }
  }

  return 1;
}
