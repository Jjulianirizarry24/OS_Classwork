#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Decodes a float value using a specific transformation.
 *
 * The transformation used is: decoded = -(original / 5.0)
 *
 * @param original_value The original float value to decode.
 * @return The decoded float value.
 */
float decode(float original_value) { return -(original_value / 5.0f); }

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
    free(data);
    fclose(input);
    exit(1);
  }

  fclose(input);

  // Open CSV file for output
  FILE *output = fopen("../src/output.csv", "w");
  if (!output) {
    perror("Error opening output.csv for writing");
    free(data);
    exit(1);
  }

  // Process and write the decoded data
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      float decoded = decode(data[i * cols + j]);
      fprintf(output, "%.2f", decoded);
      if (j < cols - 1) {
        fprintf(output, ", ");
      }
    }
    fprintf(output, "\n");
  }

  printf("Finished program. Output csv was able to be created.\n");

  free(data);
  fclose(output);
}
