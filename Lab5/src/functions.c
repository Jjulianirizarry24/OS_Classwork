#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/input.h>
#include <math.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

int mousex = 0, mousey = 0;
int mid_row;
int mid_col;
int timer = 3;
int goalX, goalY;

char *quadrant;
char *mouse_location;

int rows, cols;
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

  // Initialize ncurses
  initscr();
  noecho();
  curs_set(FALSE);
  getmaxyx(stdscr, rows, cols);

  mid_row = rows / 2;
  mid_col = cols / 2;
  mousex = mid_col;
  mousey = mid_row;
  sleep(3);

  goalX = rand() % cols;
  goalY = rand() % rows;

  // Print terminal logic
  while (1) {
    clear();

    // Get terminal size | y, x
    getmaxyx(stdscr, rows, cols);

    mid_row = rows / 2;
    mid_col = cols / 2;

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

    // Draw Dividers

    for (int i = 0; i < rows; i++) {
      mvprintw(i, mid_col, "*");
    }
    for (int j = 0; j < cols; j++) {
      mvprintw(mid_row, j, "*");
    }

    mvprintw(mid_row, mid_col - 5, " Time: %d ", timer);

    if (timer == 0) {
      mvprintw(mid_row, mid_col - 5, " You lose! ");
      clear();
      endwin();
      exit(0);
    } else {
      timer -= 1;
      sleep(1);
    }
    mvprintw(goalY, goalX, "# Click Here #");
    // Refresh to show changes
    mvprintw(1, 1, "Current Mouse X Val: %d", mousex);
    mvprintw(2, 1, "Current Mouse Y Val: %d", mousey);

    // Determine the quadrant of the goal coordinates
    if (goalY < mid_row && goalX < mid_col) {
      quadrant = "Q1 (Top-Left)";
    } else if (goalY < mid_row && goalX >= mid_col) {
      quadrant = "Q2 (Top-Right)";
    } else if (goalY >= mid_row && goalX < mid_col) {
      quadrant = "Q3 (Bottom-Left)";
    } else {
      quadrant = "Q4 (Bottom-Right)";
    }

    // Determine the quadrant of the goal coordinates
    if (mousey < mid_row && mousex < mid_col) {
      mouse_location = "Q1 (Top-Left)";
    } else if (mousey < mid_row && mousex >= mid_col) {
      mouse_location = "Q2 (Top-Right)";
    } else if (mousey >= mid_row && mousex < mid_col) {
      mouse_location = "Q3 (Bottom-Left)";
    } else {
      mouse_location = "Q4 (Bottom-Right)";
    }

    // Display the quadrant information
    mvprintw(3, 1, "Target Quadrant: %s", quadrant);

    // Display the quadrant information
    mvprintw(4, 1, "current user Quadrant: %s", mouse_location);

    mvprintw(mousey, mousex, ">.<");

    refresh();
    usleep(10000);
  }

  // End ncurses mode
  endwin();
}

void *input() {
  const char *device = "/dev/input/event5"; // event2
  int fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("Error opening device");
    return 1;
  }

  struct input_event ev;

  while (1) {
    // Read an event
    if (read(fd, &ev, sizeof(struct input_event)) ==
        sizeof(struct input_event)) {
      if (ev.type == EV_REL && ev.code == ABS_X) {
        int temp = mousex + ev.value;
        if (temp < 0 || temp > cols)
          continue;
        mousex = temp;
      }
      if (ev.type == EV_REL && ev.code == ABS_Y) {
        int temp = mousey + ev.value;
        if (temp < 0 || temp > rows)
          continue;
        mousey = temp;
      }

      if (ev.type == EV_KEY && ev.value == 0 &&
          strcmp(quadrant, mouse_location) == 0) { // Key release event
        mvprintw(mid_row, mid_col - 5, " You win! ");

        sleep(1);
        timer = 3;

        // reset goal loc
        goalX = rand() % cols;
        goalY = rand() % rows;
      }
    }
  }

  close(fd);
}