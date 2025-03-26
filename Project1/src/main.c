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
 * The entry point for the program. It expects a file path as a command-line
 * argument, which is passed to the `mainProcess` function for further
 * processing.
 *
 * @note The file path must be passed as the first command-line argument.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 *
 */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    // Print error message and exit if not enough arguments
    fprintf(stderr, "Error: Not enough arguments. Please follow this format: "
                    "./project1 <filepath>\n");
    return 1; // Exit with an error code
  }
  mainProcess(argv[1]);
}
