#include "functions.h"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int x;
  int y;
} BallPosition;
int dx, dy;
int break_loop = 0; // 0 manual, 1 auto
int counter_val;
int row, col;

#define row 15
#define col 25
#define ENTER_NCURSES 10

int matrix[row][col];

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

  // int matrix[col][row];

  // Initialize the matrix
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      // Set border values to 1
      if (i == 0 || i == row - 1 || j == 0 || j == col - 1) {
        matrix[i][j] = 1;
      } else {
        matrix[i][j] = 0; // Inner values can be 0 or any default value
      }
    }
  }

  pthread_t id1, id2;

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  BallPosition ball_pos;
  counter_val = 0;

  ball_pos.x = 0;
  ball_pos.y = 0;

  int key_stroke;
  while (1) {
    key_stroke = getch();
    if (key_stroke == ENTER_NCURSES) {
      break_loop = 1;
      pthread_create(&id1, NULL, changeSpeed, (void *)&ball_pos);
      pthread_create(&id2, NULL, visualizeBall, (void *)&ball_pos);
      break;
    }
  }

  dx = dy = 1;
  while (1) {
    // int r = row;
    // int c = col;
    int r;
    int c;
    getmaxyx(stdscr, r, c); /* Obtiene el numbero de filas y columnas */
    usleep(100000);
    if (break_loop == 1) {
      ball_pos.x += dx;
      ball_pos.y += dy;
      counter_val++;
    } else {
      continue;
    }

    if (ball_pos.x >= c - 1) {
      dx = -1;
    }
    if (ball_pos.y >= r - 1) {
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
 * @brief ...
 *
 * ...
 *
 */
void *visualizeBall(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;

  wait(1);
  while (1) {

    clear();

    if (break_loop == 0) {
      mvprintw(1, 0, "Manual");
    } else {
      mvprintw(1, 0, "Auto");
    }

    mvprintw(0, 0, "%d", counter_val);
    mvprintw(ball_pos->y, ball_pos->x, "o");

    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        int temp = matrix[i][j];
        switch (temp) {

        case 0:
          mvprintw(i + 2, j, " ");
          break;
        case 1:
          mvprintw(i + 2, j, "*");
          break;
        }
      }
    }

    refresh();

    usleep(100000);
  }
  getch();
  endwin();
}

/**
 * @brief ...
 *
 * ...
 *
 */
void *changeSpeed(void *param) {
  int key_stroke;
  BallPosition *ball;
  ball = (BallPosition *)param;

  while (1) {
    key_stroke = getch();
    if (break_loop == 1) {
      switch (key_stroke) {
      case KEY_BACKSPACE:
        break_loop = 0;
        break;
      }
    }

    // Operate Manually
    if (break_loop == 0) {

      switch (key_stroke) {

      // Quit Manual Mode
      case ' ':
        break_loop = 1;
        break;

      // TODO: Set boundaries
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
