#include "functions.h"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;
int dx, dy;
int break_loop = 0; // 0 manual, 1 auto
int counter_val;
int difficulty;
int collision_amount;
int done;
int finish;
int close_threads;
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
 * Program contains three difficulty levels: Easy Medium and Hard
 *
 * Use mutex to maintain data integrity for ball position
 *
 *
 */
void mainProcess() {
  close_threads = 0;
  finish = 0;
  pthread_t id1, id2, id3, id4;
  pthread_mutex_init(&mutex, NULL);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  printf("Easy: 1\n");
  printf("Medium: 2\n");
  printf("Hard: 3\n");
  printf("Select difficulty: ");
  scanf("%d", &difficulty);

  // 25, 50, and 75% fills
  if (difficulty == 1) {
    collision_amount = 84;
  } else if (difficulty == 2) {
    collision_amount = 168;
  } else if (difficulty == 3) {
    collision_amount = 252;
  } else {
    printf("Invalid input. Ending program...\n");
    refresh();
    sleep(2); // Use sleep instead of wait
    exit(1);
  }

  printf("\n Succesfully entered difficulty %d\n", difficulty);

  clear();

  sleep(1);

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  BallPosition ball_pos;
  counter_val = 0;

  ball_pos.x = 2;
  ball_pos.y = 2;

  // Create threads
  break_loop = 1;
  pthread_create(&id1, NULL, movementController, (void *)&ball_pos);
  pthread_create(&id2, NULL, Plotting, (void *)&ball_pos);
  pthread_create(&id3, NULL, collisionChecker, (void *)&ball_pos);
  pthread_create(&id4, NULL, updateBall, (void *)&ball_pos);

  // exit while loop when no more '#'
  while (finish == 0) {
    continue;
  }

  // Wait till all threads close to end program
  close_threads = 1;
  pthread_join(id1, NULL);
  pthread_join(id2, NULL);
  pthread_join(id3, NULL);
  pthread_join(id4, NULL);

  clear();
  endwin();
  printf("\n End program\n");
}

/**
 * @brief Function to plot the game grid and ball position.
 *
 * Initializes the game grid, places collision points, and continuously updates
 * the display with the ball's position and collision points.
 *
 * @param param Pointer to the BallPosition structure.
 */
void *Plotting(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;
  dx = dy = 1;

  // Initialize Matrix with empty space, boundaries.
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

  // Fill the matrix's 0s with random 2s
  int count = 0;
  while (count < collision_amount) {
    int rand_x = rand() % (col - 2) + 1;
    int rand_y = rand() % (row - 2) + 1;

    // If the position is empty (0), place a 2 there and increment the count
    if (matrix[rand_y][rand_x] == 0) {
      matrix[rand_y][rand_x] = 2;
      count++;
    }
  }

  // Add ball
  matrix[ball_pos->x][ball_pos->y] = 3;

  // Plotting Logic
  sleep(1);
  while (close_threads != 1) {

    clear();
    int r;                  // y
    int c;                  // x
    getmaxyx(stdscr, r, c); /* Obtiene el numbero de filas y columnas */

    // Print header
    if (break_loop == 0) {
      mvprintw(0, c - 6, "Manual");
    } else {
      mvprintw(0, c - 6, "Auto");
    }
    mvprintw(0, 0, "%d", counter_val);

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
        case 2:
          mvprintw(i + 2, j, "#");
          break;
        case 3:
          mvprintw(i + 2, j, "o");
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
 * @brief Function to handle manual ball movement.
 *
 * Listens for keyboard input to move the ball manually when in manual mode.
 *
 * For debugging purposes
 *
 * @param param Pointer to the BallPosition structure.
 */
void *movementController(void *param) {
  int key_stroke;
  BallPosition *ball;
  ball = (BallPosition *)param;

  while (close_threads != 1) {
    // get keyboard input
    key_stroke = getch();
    if (close_threads == 1) {
      continue;
    }
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

      pthread_mutex_lock(&mutex);

      switch (key_stroke) {

      // Quit Manual Mode
      case ' ':
        break_loop = 1;
        break;

        // keys controll ball placement
      case KEY_LEFT:

        matrix[ball->x][ball->y] = 0;
        ball->y += -1;
        matrix[ball->x][ball->y] = 3;
        counter_val++;

        break;
      case KEY_UP:
        matrix[ball->x][ball->y] = 0;
        ball->x += -1;
        matrix[ball->x][ball->y] = 3;
        counter_val++;
        break;
      case KEY_DOWN:
        matrix[ball->x][ball->y] = 0;
        ball->x += 1;
        matrix[ball->x][ball->y] = 3;
        counter_val++;
        break;
      case KEY_RIGHT:
        matrix[ball->x][ball->y] = 0;
        ball->y += 1;
        matrix[ball->x][ball->y] = 3;
        counter_val++;
        break;
      }
      pthread_mutex_unlock(&mutex);
    }
  }
}

/**
 * @brief Function to check for collisions and update ball direction.
 *
 * Checks if all collision points are cleared and updates the ball's direction
 * when it hits the boundaries of the grid.
 *
 * @param param Pointer to the BallPosition structure.
 */
void *collisionChecker(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;
  int done;
  while (close_threads != 1) {
    done = 1;
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        if (matrix[i][j] == 2) {
          done = 0;
        }
      }
    }

    if (done == 1) {
      finish = 1;
    }

    // Check if the next position is an obstacle
    int next_x = ball_pos->x + dx;
    int next_y = ball_pos->y + dy;
    if (matrix[next_x][next_y] == 2) {
      matrix[next_x][next_y] = 0;
      dx = -dx;
      dy = -dy;
    }

    // Change Ball Direction upon hitting wall
    if (ball_pos->x >= row - 2) {
      dx = -1;
    }
    if (ball_pos->x <= 1) {
      dx = 1;
    }
    if (ball_pos->y >= col - 2) {
      dy = -1;
    }
    if (ball_pos->y <= 2) {
      dy = 1;
    }
  }
}

/**
 * @brief Function to update the ball's position automatically.
 *
 * Updates the ball's position in automatic mode and ensures thread-safe access
 * to shared resources using a mutex.
 *
 * @param param Pointer to the BallPosition structure.
 */
void *updateBall(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;

  while (close_threads != 1) {
    usleep(100000);
    if (break_loop == 1) {
      pthread_mutex_lock(&mutex);
      // anything ball touches turns to 0
      matrix[ball_pos->x][ball_pos->y] = 0;
      ball_pos->x += dx;
      ball_pos->y += dy;
      matrix[ball_pos->x][ball_pos->y] = 3;
      counter_val++;
      pthread_mutex_unlock(&mutex);
    } else {
      continue;
    }
  }
}