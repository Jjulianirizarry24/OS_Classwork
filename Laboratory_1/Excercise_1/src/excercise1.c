#include "excercise1.h"

#include <math.h>
#include <stdio.h>
#define PI 3.141592654

/**
 * @brief Calculates the sum of cosines for a series of angles.
 *
 * This function computes the cosine of each integer value from `init_number` to
 * `end_number` (inclusive), and adds the results to a running sum. It prints
 * each cosine value as it processes the numbers. Finally, it returns 1 if the
 * sum is non-negative, or -1 if the sum is negative.
 *
 * @param init_number The starting number of the series.
 * @param end_number The ending number of the series.
 *
 * @return 1 if sum is positive, otherwise -1.
 *
 */
int cosine_series(int init_number, int end_number) {
  double curr_sum = 0;

  for (int i = init_number; i <= end_number; i++) {
    double temp = cos((i * PI) / 180);

    curr_sum += temp;

    printf("\n cos of number %d: %f \n", i, temp);
  }

  return (curr_sum >= 0) ? 1 : -1;
}
