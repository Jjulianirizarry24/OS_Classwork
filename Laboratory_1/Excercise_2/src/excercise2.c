#include <math.h>
#include <stdio.h>

#include "functions.h"

/*

Pointers are a very powerful tool in C language. They provide flexibility but
must be used carefully to avoid accessing forbidden memory locations. One of the
applications of pointers is to “return” more than one value in a C function.
This can be done by sending the directions of many variables to the function and
then filling out the variables using their directions.  Write a C program that
contains the function:

     void fill_date(long second, int* day, int* month, int* year);

The function must calculate the exact date (day/month/year) using the number of
seconds (first parameter) that have passed since 00:00 of January 1st, 1970.

Write a program that asks the user three numbers of seconds and prints the exact
dates accordingly. i.e., if the user types 345,667seconds, the program must
print: January 5, 1970 Upload a compressed file with the project's folder,
including the "src" and "include" directories and the cmakelists.txt file. The
program will not be evaluated if it does not compile correctly with the
cmakelists.txt file.

*/

void fill_date(long second, int* day, int* month, int* year) {

  
}
