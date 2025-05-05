#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/input.h>
#include <math.h>
#include <ncurses.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

int mousex = 0, mousey = 0;
void *input();

/**
 * @brief Main process function
 *
 * ...
 */
void mainProcess() {
  srand((unsigned int)time(NULL));

  pthread_t input_thread;
  if (pthread_create(&input_thread, NULL, input, NULL) != 0) {
    perror("Failed to create input thread");
    exit(EXIT_FAILURE);
  }

  int rows, cols;
  int timer = 3;

  // Initialize ncurses
  initscr();
  noecho();
  curs_set(FALSE);
  getmaxyx(stdscr, rows, cols);
  int goalX, goalY;

  goalX = rand() % cols;
  goalY = rand() % rows;

  // Print terminal logic
  while (1) {
    clear();

    // Get terminal size | y, x
    getmaxyx(stdscr, rows, cols);

    int mid_row = rows / 2;
    int mid_col = cols / 2;

    // Quadrant boundaries
    // Q1: top-left
    int q1_start_y = 0, q1_end_y = mid_row;
    int q1_start_x = 0, q1_end_x = mid_col;

    // Q2: top-right
    int q2_start_y = 0, q2_end_y = mid_row;
    int q2_start_x = mid_col, q2_end_x = cols;

    // Q3: bottom-left
    int q3_start_y = mid_row, q3_end_y = rows;
    int q3_start_x = 0, q3_end_x = mid_col;

    // Q4: bottom-right
    int q4_start_y = mid_row, q4_end_y = rows;
    int q4_start_x = mid_col, q4_end_x = cols;

    for (int i = 0; i < rows; i++) {
      mvprintw(i, mid_col, "*");
    }

    // Draw horizontal divider
    for (int j = 0; j < cols; j++) {
      mvprintw(mid_row, j, "*");
    }

    mvprintw(mid_row, mid_col - 5, " Time: %d ", timer);

    if (timer == 0) {
      mvprintw(mid_row, mid_col - 5, " You lose! ");
      sleep(1);
      timer = 3;

      // reset goal loc
      goalX = rand() % cols;
      goalY = rand() % rows;
    } else {
      timer -= 1;
      sleep(1);
    }
    mvprintw(goalY, goalX, "# Click Here #");
    // Refresh to show changes
    mvprintw(1, 1, "Current Mouse X Val: %d", mousex);
    mvprintw(2, 1, "Current Mouse Y Val: %d", mousey);
    refresh();
    usleep(10000);
  }

  // End ncurses mode
  endwin();
}

void *input() {
  const char *device = "/dev/input/event15"; // event2
  int fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("Error opening device");
    return 1;
  }

  struct input_event ev;

  printf("Listening for keyboard events. Press Ctrl+C to quit.\n");
  while (1) {
    // Read an event
    if (read(fd, &ev, sizeof(struct input_event)) ==
        sizeof(struct input_event)) {
      if (ev.type == EV_ABS && ev.code == ABS_X) { // Key press event
        mousex = ev.value;
      }
      if (ev.type == EV_ABS && ev.code == ABS_Y) { // Key press event
        mousey = ev.value;
      }

      // if (ev.type == EV_KEY && ev.value == 0) { // Key release event
      //   printf("Key %d released.\n", ev.code);
      // }
    }
  }

  close(fd);
}