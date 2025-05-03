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

/**
 * @brief Main process function
 *
 * ...
 */
void mainProcess() {

  int rows, cols;
  int timer = 3;

  // Initialize ncurses
  initscr();
  noecho();
  curs_set(FALSE);
  // keypad(window, TRUE);

  // Print terminal logic
  while (1) {
    clear();
    // Get terminal size
    getmaxyx(stdscr, rows, cols);

    // Calculate midpoints
    int mid_row = rows / 2;
    int mid_col = cols / 2;

    // Draw vertical divider
    for (int i = 0; i < rows; i++) {
      mvprintw(i, mid_col, "*");
    }

    // Draw horizontal divider
    for (int j = 0; j < cols; j++) {
      mvprintw(mid_row, j, "*");
    }

    mvprintw(mid_row, mid_col-5, " Time: %d ", timer);

    if (timer == 0) {
      mvprintw(mid_row, mid_col-5, " You lose! ");
      sleep(1);
      timer = 3;
    } else {
      timer -= 1;
      sleep(1);
    }



    
    // Refresh to show changes
    refresh();
    usleep(10000);
  }

  // End ncurses mode
  endwin();
}

void quit(void) { exit(0); }

void input() {
  // // Replace with your keyboard's device file
  // const char *device = "/dev/input/event6"; // event2
  // int fd = open(device, O_RDONLY);
  // if (fd == -1) {
  //   perror("Error opening device");
  //   exit(1);
  // }

  // struct input_event ev;

  // printf("Listening for mouse events. Press Ctrl+C to quit.\n");
  // while (1) {
  //   int mousex, mousey;
  //   // Read an event
  //   if (read(fd, &ev, sizeof(struct input_event)) ==
  //       sizeof(struct input_event)) {
  //     if (ev.type == EV_ABS && ev.code == ABS_X) { // Key press event
  //       mousex = ev.value;
  //       printf("%d, %d\n", mousex, mousey);
  //     }
  //     if (ev.type == EV_ABS && ev.code == ABS_Y) { // Key press event
  //       mousey = ev.value;
  //       printf("%d, %d\n", mousex, mousey);
  //     }

  //     printf("%d, %d\n", mousex, mousey);

  //     if (ev.type == EV_KEY && ev.value == 0) { // Key release event
  //       printf("Key %d released.\n", ev.code);
  //       // previous_key = -1;
  //       // current_key = -1;
  //     }
  //     // printf("%d, %d\n", mousex, mousey);
  //   }
  // }

  // close(fd);
}