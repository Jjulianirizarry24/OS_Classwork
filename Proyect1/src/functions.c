#include "functions.h"
#include <math.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;

// // Signal handler for SIGALRM
// void sigalrm_handler(int signum) {
//   mvprintw(0, 0, "Timer expired! The program will continue...\n");
//   refresh();
// }

/**
 * @brief Executes all program logic
 */
void mainProcess() {

  printf("start of program\n");

  pthread_t thread_1, thread_2;

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  pthread_create(&thread_1, NULL, posixTimer, NULL);
  // pthread_create(&thread_2, NULL, sleepFunction, NULL);

  pthread_join(thread_1, NULL);
  // pthread_join(thread_2, NULL);

  endwin();
}

/**
 * @brief Executes the posix timer thread logic
 */
// void *posixTimer() {
//   // sleep(1);

//   int signum = SIGALRM;
//   // Set Up Signal
//   struct sigaction sa;
//   sa.sa_handler = sigalrm_handler; // Define the handler function
//   sa.sa_flags = 0;
//   sigaction(SIGALRM, &sa, NULL); // Register the signal handler for SIGALRM

//   timer_t tmr;
//   struct itimerspec its;
//   struct sigevent sev;

//   sev.sigev_notify - SIGEV_SIGNAL;
//   sev.sigev_signo = signum;
//   sev.sigev_value.sival_ptr = &tmr;

//   // 50000000
//   its.it_value.tv_sec = 1;
//   its.it_value.tv_nsec = 0;
//   its.it_interval.tv_sec = 0;
//   its.it_interval.tv_nsec = 0;

//   // error checking omitted for brevity
//   // pause();

//   char *path = "../inputs/path_1.txt";
//   FILE *file = fopen(path, "r"); // Open the file for reading
//   if (file == NULL) {
//     perror("Error opening file");
//     exit(1);
//   }

//   timer_create(CLOCK_REALTIME, &sev, &tmr);

//   int x, y; // Variables to store the coordinates
//   while (fscanf(file, "%d,%d\n", &x, &y) == 2) {
//     pthread_mutex_lock(&screen_mutex); // Lock screen before updating
//     clear();
//     mvprintw(y, x, "*");
//     refresh();
//     pthread_mutex_unlock(&screen_mutex); // Unlock screen after update
//     // usleep(5000);

//     timer_settime(tmr, 0, &its, NULL);
//   }

//   fclose(file); // Close the file when done
//   return NULL;
// }

/**
 * @brief Executes the sleep function thread logic
 */
void *sleepFunction() {

  char *path = "../inputs/path_2.txt";
  FILE *file = fopen(path, "r"); // Open the file for reading
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }

  int x, y; // Variables to store the coordinates
  while (fscanf(file, "%d,%d\n", &x, &y) == 2) {
    pthread_mutex_lock(&screen_mutex); // Lock screen before updating
    clear();
    mvprintw(y, x, "*");
    refresh();
    pthread_mutex_unlock(&screen_mutex); // Unlock screen after update

    usleep(50000);
  }

  fclose(file); // Close the file when done
  return NULL;
}

// pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t timer_cond = PTHREAD_COND_INITIALIZER;
volatile sig_atomic_t timer_expired = 0;

// Signal handler for SIGALRM
void sigalrm_handler(int signum) {
  timer_expired = 1;
  pthread_cond_signal(&timer_cond);
}

/**
 * @brief Executes the posix timer thread logic
 */
void *posixTimer(void *arg) {
  int signum = SIGALRM;

  // Set Up Signal
  struct sigaction sa;
  sa.sa_handler = sigalrm_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(signum, &sa, NULL);

  timer_t tmr;
  struct itimerspec its;
  struct sigevent sev;

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = signum;
  sev.sigev_value.sival_ptr = &tmr;

  // Timer will expire after 1 second (single shot)
  its.it_value.tv_sec = 1;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0; // Single shot timer
  its.it_interval.tv_nsec = 0;

  if (timer_create(CLOCK_REALTIME, &sev, &tmr) == -1) {
    perror("timer_create");
    return NULL;
  }

  char *path = "../inputs/path_1.txt";
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("Error opening file");
    return NULL;
  }

  int x, y;
  while (fscanf(file, "%d,%d\n", &x, &y) == 2) {
    // Start the timer
    if (timer_settime(tmr, 0, &its, NULL) == -1) {
      perror("timer_settime");
      break;
    }

    // Wait for timer to expire
    pthread_mutex_lock(&screen_mutex);
    while (!timer_expired) {
      pthread_cond_wait(&timer_cond, &screen_mutex);
    }
    timer_expired = 0;

    // Update display
    clear();
    mvprintw(y, x, "*");
    refresh();
    pthread_mutex_unlock(&screen_mutex);
  }

  fclose(file);
  return NULL;
}