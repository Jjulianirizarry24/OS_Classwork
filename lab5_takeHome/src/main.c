#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to decode a float value
float decode(float original_value) { return -(original_value / 5.0f); }

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <binary_file> <rows> <columns>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *binary_file = argv[1];
  int rows = atoi(argv[2]);
  int cols = atoi(argv[3]);

  FILE *input = fopen(binary_file, "rb");
  if (!input) {
    perror("Error opening binary file");
    return EXIT_FAILURE;
  }

  // Allocate memory for the image data
  float *data = (float *)malloc(rows * cols * sizeof(float));
  if (!data) {
    fprintf(stderr, "Memory allocation failed.\n");
    fclose(input);
    return EXIT_FAILURE;
  }

  // Read binary data
  size_t read_count = fread(data, sizeof(float), rows * cols, input);
  if (read_count != (size_t)(rows * cols)) {
    fprintf(stderr, "Error: Expected %d values, but read %zu\n", rows * cols,
            read_count);
    free(data);
    fclose(input);
    return EXIT_FAILURE;
  }

  fclose(input);

  // Open CSV file for output
  FILE *output = fopen("../src/output.csv", "w");
  if (!output) {
    perror("Error opening output.csv for writing");
    free(data);
    return EXIT_FAILURE;
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

  printf("Decoded image written to output.csv\n");

  free(data);
  fclose(output);

  return EXIT_SUCCESS;
}
