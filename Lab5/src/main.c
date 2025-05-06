#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Program launcher
 *
 *
 * @details The main function serves as the entry point for the application.
 * It calls the mainProcess() function which handles all game initialization
 * and execution.
 *
 * @note This function is intentionally minimal to keep all game logic
 * contained within the mainProcess() function for better modularity.
 *
 * @see mainProcess() for the actual game implementation
 */
int main() { mainProcess(); }
