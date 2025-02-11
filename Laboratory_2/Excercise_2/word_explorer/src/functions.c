#include "functions.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

/**
 * @brief Checks if a given file contains a specific word.
 *
 * This function opens a file and reads its contents to determine if
 * the given word is present. It reads the file line by line using
 * a buffer to ensure proper searching.
 *
 * @param filepath Path to the file.
 * @param word The word to search for.
 * @return 1 if the word is found in the file, 0 otherwise.
 * @note This function prints an error message if the file cannot be opened.
 */
int ContainsWord(const char *filepath, const char *word) {
  int fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    perror("Error opening file");
    return 0;
  }

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytes_read] = '\0';
    if (strstr(buffer, word) != NULL) {
      close(fd);
      return 1;
    }
  }

  close(fd);
  return 0;
}

/**
 * @brief Recursively explores a directory and prints files containing the word.
 *
 * This function iterates over all files in the specified directory and
 * checks each file for the presence of the given word. It ignores
 * special entries like "." and ".." and handles file status retrieval errors.
 *
 * @param dirpath Path to the directory.
 * @param word The word to search for in files.
 * @note This function prints an error message if the directory cannot be
 * opened.
 */
void ExploreDirectory(const char *dirpath, const char *word) {
  DIR *dir = opendir(dirpath);
  if (!dir) {
    perror("Error opening directory");
    return;
  }

  struct dirent *entry;
  char filepath[1024];

  while ((entry = readdir(dir)) != NULL) {
    // Ignore "." and ".." entries
    if (entry->d_name[0] == '.') continue;

    snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

    struct stat path_stat;
    stat(filepath, &path_stat);

    if (S_ISREG(path_stat.st_mode)) {
      if (ContainsWord(filepath, word)) {
        printf("Found in: %s\n", filepath);
      }
    }
  }

  closedir(dir);
}
