#include "functions.h"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  int x;
  int y;
} BallPosition;
int dx, dy;
int break_loop = 0; // 0 manual, 1 auto
int counter_val;
int row, col;
int difficulty;
int collision_amount;

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

  pthread_t id1, id2, id3, id4;
  printf("Select difficulty: ");
  scanf("%d", &difficulty);

  // 25, 50, and 75% fills
  if (difficulty == 0) {
    collision_amount = 93;
  } else if (difficulty == 1) {
    collision_amount = 187;
  } else if (difficulty == 2) {
    collision_amount = 281;
  } else {
    printf("Invalid input. Ending program...\n");
    refresh();
    sleep(2); // Use sleep instead of wait
    endwin();
    exit(1);
  }

  printf("\n Succesfully entered difficulty %d\n", difficulty);
  clear();

  sleep(1);

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  // printw("select difficulty: ");
  // scanw("%d", &difficulty);

  // // 25, 50 and 75% fills
  // if (difficulty == 0) {
  //   collision_amount = 93;
  // } else if (difficulty == 1) {
  //   collision_amount = 187;
  // } else if (difficulty == 2) {
  //   collision_amount = 281;
  // }

  // else {
  //   printw("invalid. Ending Progran...\n");
  //   wait(2);
  //   exit(1);
  // }

  BallPosition ball_pos;
  counter_val = 0;

  ball_pos.x = 2;
  ball_pos.y = 2;

  int key_stroke;

  // Initialize threads
  while (1) {
    key_stroke = getch();
    if (key_stroke == ENTER_NCURSES) {
      break_loop = 1;
      pthread_create(&id1, NULL, movementController, (void *)&ball_pos);
      pthread_create(&id2, NULL, Plotting, (void *)&ball_pos);
      pthread_create(&id3, NULL, collisionChecker, (void *)&ball_pos);
      pthread_create(&id4, NULL, updateBall, (void *)&ball_pos);
      break;
    }
  }

  while (1) {
  }

  return 0;
}

/**
 * @brief ...
 *
 * ...
 *
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

  // Fill the matrix's 0s with exactly 10 random 2s
  int count = 0;
  while (count < collision_amount) {
    // Generate random row and column indices (exclude borders)
    int rand_x = rand() % (col - 2) + 1; // Random column between 1 and col-2
    int rand_y = rand() % (row - 2) + 1; // Random row between 1 and row-2

    // If the position is empty (0), place a 2 there and increment the count
    if (matrix[rand_y][rand_x] == 0) {
      matrix[rand_y][rand_x] = 2;
      count++;
    }
  }

  // Add ball
  matrix[ball_pos->x][ball_pos->y] = 3;

  // Plotting Logic
  wait(1);
  while (1) {

    clear();
    int r;                  // y
    int c;                  // x
    getmaxyx(stdscr, r, c); /* Obtiene el numbero de filas y columnas */

    if (break_loop == 0) {
      mvprintw(0, c - 6, "Manual");
    } else {
      mvprintw(0, c - 6, "Auto");
    }

    mvprintw(0, 0, "%d", counter_val);
    // mvprintw(ball_pos->y, ball_pos->x, "o");

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
 * @brief ...
 *
 * ...
 *
 */
void *movementController(void *param) {
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
    }
  }
}

void *collisionChecker(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;

  while (1) {
    // Ball hits the right wall (x = 24)
    if (ball_pos->x >= row - 2) {
      dx = -1;
    }

    // Ball hits the left wall (x = 0)
    if (ball_pos->x <= 1) {
      dx = 1;
    }

    // Ball hits the bottom wall (y = 14)
    if (ball_pos->y >= col - 2) {
      dy = -1;
    }

    // Ball hits the top wall (y = 0)
    if (ball_pos->y <= 2) {
      dy = 1;
    }
  }
}

void *updateBall(void *param) {
  BallPosition *ball_pos;
  ball_pos = (BallPosition *)param;

  while (1) {
    int r;
    int c;
    getmaxyx(stdscr, r, c); /* Obtiene el numbero de filas y columnas */
    usleep(100000);
    if (break_loop == 1) {

      // anything ball touches turns to 0
      matrix[ball_pos->x][ball_pos->y] = 0;
      ball_pos->x += dx;
      ball_pos->y += dy;
      matrix[ball_pos->x][ball_pos->y] = 3;

      counter_val++;
    } else {
      continue;
    }
  }
}

// #include "functions.h"
// #include <ncurses.h>
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <unistd.h>

// typedef struct {
//   int x;
//   int y;
// } BallPosition;

// int dx, dy;
// int break_loop = 0; // 0 manual, 1 auto
// int counter_val;
// int row, col;
// int difficulty;
// int collision_amount;

// #define row 15
// #define col 25
// #define ENTER_NCURSES 10

// int matrix[row][col];
// pthread_mutex_t ball_mutex; // Mutex for synchronizing ball movement

// /**
//  * @brief Bouncing Ball program using the ncurses Library and threads.
//  *
//  * Program that represents a bouncing ball using the
//  * ncurses library. The main idea of this exercise is to learn how to
//  * utilize threads to execute different functionalities in the program
//  *
//  * id1 => Controls keystrokes when in Manual Mode
//  * id2 => Renders bouncing ball on the terminal
//  *
//  */
// void mainProcess() {

//   pthread_t id1, id2, id3, id4;

//   printf("Select difficulty: ");
//   scanf("%d", &difficulty);

//   // 25, 50, and 75% fills
//   if (difficulty == 0) {
//     collision_amount = 93;
//   } else if (difficulty == 1) {
//     collision_amount = 187;
//   } else if (difficulty == 2) {
//     collision_amount = 281;
//   } else {
//     printf("Invalid input. Ending program...\n");
//     refresh();
//     sleep(2); // Use sleep instead of wait
//     endwin();
//     exit(1);
//   }

//   // printf("\n Succesfully entered difficulty %d\n", difficulty);

//   initscr();
//   keypad(stdscr, TRUE);
//   noecho();
//   curs_set(0);

//   BallPosition ball_pos;
//   counter_val = 0;

//   ball_pos.x = 2;
//   ball_pos.y = 2;

//   // Initialize mutex for synchronizing ball position
//   pthread_mutex_init(&ball_mutex, NULL);

//   int key_stroke;

//   // Initialize threads
//   while (1) {
//     key_stroke = getch();
//     if (key_stroke == ENTER_NCURSES) {
//       break_loop = 1;
//       pthread_create(&id1, NULL, movementController, (void *)&ball_pos);
//       pthread_create(&id2, NULL, Plotting, (void *)&ball_pos);
//       pthread_create(&id3, NULL, collisionChecker, (void *)&ball_pos);
//       pthread_create(&id4, NULL, updateBall, (void *)&ball_pos);
//       break;
//     }
//   }

//   while (1) {
//     // Loop to keep the program running until manually stopped
//   }

//   endwin();
//   return 0;
// }

// /**
//  * @brief Plots the ball and matrix on the screen.
//  *
//  * The ball is represented by "o" and the matrix by "*", "#", and empty
//  spaces.
//  *
//  */
// void *Plotting(void *param) {
//   BallPosition *ball_pos;
//   ball_pos = (BallPosition *)param;
//   dx = dy = 1;

//   // Initialize Matrix with empty space, boundaries.
//   for (int i = 0; i < row; i++) {
//     for (int j = 0; j < col; j++) {
//       if (i == 0 || i == row - 1 || j == 0 || j == col - 1) {
//         matrix[i][j] = 1;
//       } else {
//         matrix[i][j] = 0;
//       }
//     }
//   }

//   // Fill the matrix's 0s with exactly `collision_amount` random 2s
//   int count = 0;
//   while (count < collision_amount) {
//     int rand_x = rand() % (col - 2) + 1; // Random column between 1 and col-2
//     int rand_y = rand() % (row - 2) + 1; // Random row between 1 and row-2
//     if (matrix[rand_y][rand_x] == 0) {
//       matrix[rand_y][rand_x] = 2;
//       count++;
//     }
//   }

//   // Add ball to matrix
//   matrix[ball_pos->x][ball_pos->y] = 3;

//   // Plotting Logic
//   sleep(1);
//   while (1) {
//     clear();
//     int r, c;
//     getmaxyx(stdscr, r, c); /* Gets number of rows and columns */

//     if (break_loop == 0) {
//       mvprintw(0, c - 6, "Manual");
//     } else {
//       mvprintw(0, c - 6, "Auto");
//     }

//     mvprintw(0, 0, "%d", counter_val);

//     for (int i = 0; i < row; i++) {
//       for (int j = 0; j < col; j++) {
//         int temp = matrix[i][j];
//         switch (temp) {
//         case 0:
//           mvprintw(i + 2, j, " ");
//           break;
//         case 1:
//           mvprintw(i + 2, j, "*");
//           break;
//         case 2:
//           mvprintw(i + 2, j, "#");
//           break;
//         case 3:
//           mvprintw(i + 2, j, "o");
//           break;
//         }
//       }
//     }

//     refresh();
//     usleep(100000);
//   }
// }

// /**
//  * @brief Handles key press for manual movement of the ball.
//  */
// void *movementController(void *param) {
//   int key_stroke;
//   BallPosition *ball = (BallPosition *)param;

//   while (1) {
//     key_stroke = getch();
//     if (break_loop == 1) {
//       switch (key_stroke) {
//       case KEY_BACKSPACE:
//         break_loop = 0;
//         break;
//       }
//     }

//     if (break_loop == 0) {
//       pthread_mutex_lock(&ball_mutex); // Lock before modifying ball position
//       switch (key_stroke) {
//       case ' ':
//         break_loop = 1;
//         break;
//       case KEY_LEFT:
//         ball->x -= 1;
//         counter_val++;
//         break;
//       case KEY_UP:
//         ball->y -= 1;
//         counter_val++;
//         break;
//       case KEY_DOWN:
//         ball->y += 1;
//         counter_val++;
//         break;
//       case KEY_RIGHT:
//         ball->x += 1;
//         counter_val++;
//         break;
//       }
//       pthread_mutex_unlock(&ball_mutex); // Unlock after modifying ball
//       position
//     }
//   }
// }

// /**
//  * @brief Checks for collisions with walls and updates direction.
//  */
// void *collisionChecker(void *param) {
//   BallPosition *ball_pos = (BallPosition *)param;

//   while (1) {
//     pthread_mutex_lock(&ball_mutex);
//     if (ball_pos->x >= row - 2) {
//       dx = -1;
//     }
//     if (ball_pos->x <= 1) {
//       dx = 1;
//     }
//     if (ball_pos->y >= col - 2) {
//       dy = -1;
//     }
//     if (ball_pos->y <= 2) {
//       dy = 1;
//     }
//     pthread_mutex_unlock(&ball_mutex);
//   }
// }

// /**
//  * @brief Updates ball position on the screen.
//  */
// void *updateBall(void *param) {
//   BallPosition *ball_pos = (BallPosition *)param;

//   while (1) {
//     pthread_mutex_lock(&ball_mutex);
//     matrix[ball_pos->x][ball_pos->y] = 0; // Clear previous ball position

//     ball_pos->x += dx; // Move ball
//     ball_pos->y += dy;

//     matrix[ball_pos->x][ball_pos->y] = 3; // Place ball at new position
//     counter_val++;
//     pthread_mutex_unlock(&ball_mutex);
//   }
// }
