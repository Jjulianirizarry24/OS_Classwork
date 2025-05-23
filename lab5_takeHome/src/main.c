#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Main function that handles input arguments, reads binary data, decodes
 * it, and writes to CSV.
 *
 * Usage:
 * @code
 * ./program <binary_file> <rows> <columns>
 * @endcode
 *
 * @param argc Argument count. Must be 4.
 * @param argv Argument vector: binary file path, number of rows, and number of
 * columns.
 * @return int Exit status (0 for success, 1 for error).
 */
int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <binary_file> <rows> <columns>\n", argv[0]);
    exit(1);
  }

  const char *binary_file = argv[1];
  int rows = atoi(argv[2]);
  int cols = atoi(argv[3]);

  FILE *input = fopen(binary_file, "rb");
  if (!input) {
    perror("Error opening binary file");
    exit(1);
  }

  // Allocate memory for the image data
  float *data = (float *)malloc(rows * cols * sizeof(float));
  if (!data) {
    fprintf(stderr, "Memory allocation failed.\n");
    fclose(input);
    exit(1);
  }

  // Read binary data
  size_t read_count = fread(data, sizeof(float), rows * cols, input);
  if (read_count != (size_t)(rows * cols)) {
    fprintf(stderr, "Error: Expected %d values, but read %zu\n", rows * cols,
            read_count);
    fclose(input);
    exit(1);
  }

  fclose(input);

  // Open CSV file for output
  FILE *output = fopen("output.csv", "w");
  if (!output) {
    perror("Error opening output.csv for writing");
    exit(1);
  }

  // Write to output file using specified decode instructions
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      float decoded = -(data[i * cols + j] / 5.0f);
      fprintf(output, "%.2f", decoded);
      if (j < cols - 1) {
        fprintf(output, ", ");
      }
    }
    fprintf(output, "\n");
  }

  printf("Finished program. Output csv was able to be created.\n");
  fclose(output);
}
