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
pthread_cond_t timer_cond = PTHREAD_COND_INITIALIZER;
int timer_expired = 0;

// Signal handler for SIGALRM
void sigalrm_handler(int signum) {
  timer_expired = 1;
  pthread_cond_signal(&timer_cond);
}

/**
 * @brief Executes all program logic
 */
void mainProcess(char *path) {

  printf("start of program\n");

  pthread_t thread_1, thread_2;

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  pthread_create(&thread_1, NULL, posixTimer, (void *)path);
  pthread_join(thread_1, NULL);
  pthread_create(&thread_2, NULL, sleepFunction, (void *)path);
  pthread_join(thread_2, NULL);

  endwin();
}

/**
 * @brief Executes the sleep function thread logic
 *
 * Represented with '*'
 */
void *sleepFunction(void *ptr) {
  char *path = (char *)ptr;

  FILE *file = fopen(path, "r"); // Open the file for reading
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }

  FILE *output_file = fopen("../outputs/sleepFile.txt", "w");
  if (output_file == NULL) {
    printf("Error opening the file!\n");
    exit(1); // Exit if the file can't be opened
  }

  int x, y; // Variables to store the coordinates
  while (fscanf(file, "%d,%d\n", &x, &y) == 2) {

    pthread_mutex_lock(&screen_mutex); // Lock screen before updating
    clear();
    mvprintw(y, x, "*");
    refresh();
    pthread_mutex_unlock(&screen_mutex); // Unlock screen after update

    usleep(1);

    fprintf(output_file, "Sleep Function: \n");
  }

  // Close the file
  fclose(output_file);
  fclose(file); // Close the file when done
  return NULL;
}

/**
 * @brief Executes the posix timer thread logic
 *
 * Represented with '#'
 */
void *posixTimer(void *ptr) {

  char *path = (char *)ptr;
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
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 5000;
  its.it_interval.tv_sec = 0; // Single shot timer
  its.it_interval.tv_nsec = 0;

  if (timer_create(CLOCK_REALTIME, &sev, &tmr) == -1) {
    perror("timer_create");
    return NULL;
  }
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("Error opening file");
    return NULL;
  }

  FILE *output_file = fopen("../outputs/posixFile.txt", "w");
  if (output_file == NULL) {
    printf("Error opening the file!\n");
    exit(1); // Exit if the file can't be opened
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
    mvprintw(y, x, "#");
    refresh();
    pthread_mutex_unlock(&screen_mutex);

    fprintf(output_file, "Posix Timer: \n");
  }

  fclose(output_file);
  fclose(file);
  return NULL;
}