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

// Shared variables
int mousex = 0, mousey = 0;
int mid_row;
int mid_col;
int timer = 3;
int goalX, goalY;
int rows, cols;

char *quadrant;
char *mouse_location;

// Mutexes for shared variables
pthread_mutex_t mouse_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t goal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t display_mutex = PTHREAD_MUTEX_INITIALIZER;

void *input();

/**
 * @brief Main process function
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

  pthread_mutex_lock(&mouse_mutex);
  mid_row = rows / 2;
  mid_col = cols / 2;
  mousex = mid_col;
  mousey = mid_row;
  pthread_mutex_unlock(&mouse_mutex);

  pthread_mutex_lock(&goal_mutex);
  goalX = rand() % cols;
  goalY = rand() % rows;
  pthread_mutex_unlock(&goal_mutex);

  sleep(3);

  // Main loop
  while (1) {
    clear();

    // Get terminal size | y, x
    getmaxyx(stdscr, rows, cols);

    pthread_mutex_lock(&mouse_mutex);
    mid_row = rows / 2;
    mid_col = cols / 2;
    pthread_mutex_unlock(&mouse_mutex);

    // Draw Dividers
    for (int i = 0; i < rows; i++) {
      mvprintw(i, mid_col, "*");
    }
    for (int j = 0; j < cols; j++) {
      mvprintw(mid_row, j, "*");
    }

    pthread_mutex_lock(&display_mutex);
    mvprintw(mid_row, mid_col - 5, " Time: %d ", timer);
    pthread_mutex_unlock(&display_mutex);

    if (timer == 0) {
      pthread_mutex_lock(&display_mutex);
      mvprintw(mid_row, mid_col - 5, " You lose! ");
      pthread_mutex_unlock(&display_mutex);
      clear();
      endwin();
      exit(0);
    } else {
      timer -= 1;
      sleep(1);
    }

    pthread_mutex_lock(&goal_mutex);
    mvprintw(goalY, goalX, "# Click Here #");
    pthread_mutex_unlock(&goal_mutex);

    // Display mouse info with mutex protection
    pthread_mutex_lock(&mouse_mutex);
    mvprintw(1, 1, "Current Mouse X Val: %d", mousex);
    mvprintw(2, 1, "Current Mouse Y Val: %d", mousey);

    // Determine quadrants
    if (goalY < mid_row && goalX < mid_col) {
      quadrant = "Q1 (Top-Left)";
    } else if (goalY < mid_row && goalX >= mid_col) {
      quadrant = "Q2 (Top-Right)";
    } else if (goalY >= mid_row && goalX < mid_col) {
      quadrant = "Q3 (Bottom-Left)";
    } else {
      quadrant = "Q4 (Bottom-Right)";
    }

    if (mousey < mid_row && mousex < mid_col) {
      mouse_location = "Q1 (Top-Left)";
    } else if (mousey < mid_row && mousex >= mid_col) {
      mouse_location = "Q2 (Top-Right)";
    } else if (mousey >= mid_row && mousex < mid_col) {
      mouse_location = "Q3 (Bottom-Left)";
    } else {
      mouse_location = "Q4 (Bottom-Right)";
    }

    mvprintw(3, 1, "Target Quadrant: %s", quadrant);
    mvprintw(4, 1, "Current User Quadrant: %s", mouse_location);
    mvprintw(mousey, mousex, ">.<");
    pthread_mutex_unlock(&mouse_mutex);

    refresh();
    usleep(10000);
  }

  endwin();
}

void *input() {
  const char *device = "/dev/input/event5";
  int fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("Error opening device");
    return NULL;
  }

  struct input_event ev;

  while (1) {
    if (read(fd, &ev, sizeof(struct input_event)) ==
        sizeof(struct input_event)) {
      pthread_mutex_lock(&mouse_mutex);

      if (ev.type == EV_REL && ev.code == ABS_X) {
        int temp = mousex + ev.value;
        if (temp >= 0 && temp <= cols) {
          mousex = temp;
        }
      }
      if (ev.type == EV_REL && ev.code == ABS_Y) {
        int temp = mousey + ev.value;
        if (temp >= 0 && temp <= rows) {
          mousey = temp;
        }
      }

      // Check for click and correct quadrant
      if (ev.type == EV_KEY && ev.value == 0) {
        pthread_mutex_lock(&goal_mutex);
        if (strcmp(quadrant, mouse_location) == 0) {
          pthread_mutex_lock(&display_mutex);
          mvprintw(mid_row, mid_col - 5, " You win! ");
          pthread_mutex_unlock(&display_mutex);

          sleep(1);
          timer = 3;
          goalX = rand() % cols;
          goalY = rand() % rows;
        }
        pthread_mutex_unlock(&goal_mutex);
      }

      pthread_mutex_unlock(&mouse_mutex);
    }
  }

  close(fd);
  return NULL;
}