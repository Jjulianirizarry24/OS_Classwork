#include "functions.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Main Function
 *
 * NOTE: Need to pass file path as argument
 */
int main(int argc, char *argv[]) { mainProcess(argv[1]); }
