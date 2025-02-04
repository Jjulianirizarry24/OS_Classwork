#include "functions.h"

#include <math.h>
#include <stdio.h>

/**
 * @brief Determines if a year is a leap year and updates the days in February.
 *
 * This function checks if the given year is a leap year. If it is, it updates
 * the `days_in_month` array to set February to 29 days. Otherwise, it sets
 * February to 28 days.
 *
 * @param year Pointer to the year to check.
 * @param days_in_month Array representing the days in each month of year.
 */
void is_leap_year(int year, int days_in_month[]) {
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    days_in_month[1] = 29;
  } else {
    days_in_month[1] = 28;
  }
}

/**
 * @brief Fills the day, month, and year based on the given time in seconds.
 *
 * This function takes a time in seconds since January 1, 1970 and calculates
 * the corresponding day, month, and year that has passed. The function takes
 * into account leap years while performing the calculations.
 *
 * This is similar to the previous excercise, but we return a Struct instead of
 * modifying pointers.
 *
 *
 * @param second User input representing time passed since initial date.
 *
 * @return Date Struct representing new Date after seconds have passed.
 */
Date fill_date(long second) {
  // Constant values
  const int secondsPerYear = 365 * 24 * 60 * 60;
  const int secondsPerDay = 24 * 60 * 60;

  int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int day = 1;
  int month = 0;
  int year = 1970;

  // Add Years
  while (second >= secondsPerYear) {
    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      second -= 366 * 24 * 60 * 60;
    } else {
      second -= 365 * 24 * 60 * 60;
    }
    year++;
  }

  // leap Year Check
  is_leap_year(year, days_in_month);

  // Add months
  printf("\n days_in_month = %d \n", days_in_month[month]);

  while (second >= days_in_month[month] * secondsPerDay) {
    second -= days_in_month[month] * secondsPerDay;
    month++;
  }

  // Add to day
  day = second / secondsPerDay + 1;

  Date date = {day, month, year};

  return date;
}
