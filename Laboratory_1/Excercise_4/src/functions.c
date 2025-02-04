#include "functions.h"

#include <math.h>
#include <stdio.h>

/*

Write a function that receives two pointers for two arrays of chars:

void upper_rand(char* base, char *mod);

The function asks the user to fill in the first array (base array). Then, for
every letter, it decides randomly whether to change it to upper case and stores
it in the second array.

Finally, the program must print both arrays inside another function:

void print_arrays(char* base, char *mod);

example: if the user types: “debi tirar mas fotos” the program may print
something like “DeBI TiRAR MaS FOToS”

Hint: Search how to use the rand() function on the web to generate random
numbers. To avoid always having the same results using the rand() function, use
the line srand(time(NULL)) once at the beginning of the program.

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Randomly converts characters in a string to uppercase.
 *
 * This function iterates through the characters in the `base` string. For each
 * character, it randomly decides whether to convert the character to uppercase
 * and stores the result in the `mod` string. If the character is not converted
 * to uppercase, it is copied as is.
 *
 * @param base Pointer to the original string of characters that will be
 * processed.
 * @param mod Pointer to the string that will store the modified characters.
 *
 */
void upper_rand(char* base, char* mod) {
  // Loop through the base array and decide randomly whether to change each
  // character
  for (int i = 0; base[i] != NULL; i++) {
    if (rand() % 3 == 0) {  // Randomly decide whether to convert to uppercase
      mod[i] = toupper(base[i]);
      continue;
    }
    mod[i] = base[i];
  }
}

/**
 * @brief Prints the original and modified strings.
 *
 * This function prints the original `base` string and the modified `mod`
 * string.
 *
 * @param base Pointer to the original string of characters.
 * @param mod Pointer to the modified string of characters.
 *
 */
void print_arrays(char* base, char* mod) {
  // Print both arrays
  printf("Before Modification: %s\n", base);
  printf("After Modification %s\n", mod);
}