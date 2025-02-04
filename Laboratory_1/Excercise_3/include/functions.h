#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
  int day;
  int month;
  int year;
} Date;

void is_leap_year(int year, int months[]);
Date fill_date(long second);

#endif