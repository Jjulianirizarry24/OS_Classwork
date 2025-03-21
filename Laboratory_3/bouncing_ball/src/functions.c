#include "functions.h"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int dx, dy;
int break_loop = 0; // 0 manual, 1 auto
int counter_val;
int row, col;
#define ENTER_NCURSES 10

/**
 * @brief Bouncing Ball program using the ncurses Library and threads.
 *
 * Program that represents a bouncing ball using the
 * ncurses library. The main idea of this excercise is to learn how to
 * utilize threads to execute different functionalities in the program
 *
 * id1 => Controls keystrokes when in Manual Mode
 * id2 => Renders bouncing ball on the terminal
 *
 */
void mainProcess() {
  // Initialize variables
  pthread_t id1, id2;

  initscr();
  keypad(stdscr, TRUE);
  noecho();

  curs_set(0);

  BallPosition ball_pos;
  counter_val = 0;

  ball_pos.x = 10;
  ball_pos.y = 20;

  int key_stroke;
  while (1) {
    key_stroke = getch();
    // switch (key_stroke) {
    if (key_stroke == ENTER_NCURSES) {
      // case KEY_ENTER:
      break_loop = 1;

      pthread_create(&id1, NULL, Plotting, (void *)&ball_pos);
      pthread_create(&id2, NULL, readKeyboard, (void *)&ball_pos);
      break;
      // }
    }
  }

  dx = dy = 1;
  while (1) {
    getmaxyx(stdscr, row, col); /* Obtiene el numbero de filas y columnas */
    usleep(100000);
    if (break_loop == 1) {
      ball_pos.x += dx;
      ball_pos.y += dy;
      counter_val++;
    } else {
      continue;
    }

    // Change ball direction
    if (ball_pos.x >= col - 1) {
      dx = -1;
    }
    if (ball_pos.y >= row - 1) {
      dy = -1;
    }
    if (ball_pos.x <= 1) {
      dx = 1;
    }
    if (ball_pos.y <= 1) {
      dy = 1;
    }
  }
  return 0;
}

/**
 * @brief Function to visualize the ball on the terminal.
 *
 * Continuously updates the terminal display to show the ball's position
 * and the current mode (manual or automatic).
 *
 * @param param Pointer to the BallPosition structure.
 */
void *Plotting(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;

  while (1) {

    clear();

    if (break_loop == 0) {
      mvprintw(1, 0, "Manual");
    } else {
      mvprintw(1, 0, "Auto");
    }

    mvprintw(0, 0, "%d", counter_val);
    mvprintw(ball_pos->y, ball_pos->x, "o");
    refresh();

    usleep(100000);
  }
  getch();
  endwin();
}

/**
 * @brief Function to handle keyboard input in manual mode.
 *
 * Listens for keyboard input to move the ball manually when in manual mode.
 * Also allows switching between manual and automatic modes.
 *
 * Press Space to change between modes.
 *
 * @param param Pointer to the BallPosition structure.
 */
void *readKeyboard(void *param) {
  int key_stroke;
  keypad(stdscr, TRUE);

  BallPosition *ball;
  ball = (BallPosition *)param;
  noecho();

  while (1) {
    // get keyboard input
    key_stroke = getch();
    if (break_loop == 1) {

      switch (key_stroke) {
      case ' ':
        break_loop = 0;
        break;
      }
    }

    key_stroke = getch();

    // Operate Manually
    if (break_loop == 0) {
      wait(1);
      switch (key_stroke) {

      // Quit Manual Mode
      case ' ':
        break_loop = 1;
        break;
      case KEY_LEFT:
        ball->x += -1;
        counter_val++;
        break;
      case KEY_UP:
        ball->y += -1;
        counter_val++;
        break;
      case KEY_DOWN:
        ball->y += 1;
        counter_val++;
        break;
      case KEY_RIGHT:
        counter_val++;
        ball->x += 1;
        break;
      }

      // restrict ball from getting out of bounds when using arrow keys
      if (ball->x >= col) {
        ball->x -= 1;
        continue;
      }
      if (ball->y >= row) {
        ball->y -= 1;
        continue;
      }
      if (ball->x <= 0) {
        ball->x += 1;
        continue;
      }
      if (ball->y <= 0) {
        ball->y += 1;
        continue;
      }
    }
  }
}
