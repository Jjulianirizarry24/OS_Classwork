#define _GNU_SOURCE
#include <math.h>
#include <ncurses.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

// Variable initialization
volatile int end_program_flag = 0;
pthread_t thread_1, thread_2, thread_3, thread_4, input;
int affinity_values[4] = {0, 1, 2, 3}; // Default vals
int nice_values[4] = {0, 0, 0, 0};     // initialized at 0
pthread_mutex_t terminal_lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Change CPU affinity for a thread
 *
 * Prompts user for thread number and CPU core, then updates the affinity.
 * Validates input and provides feedback.
 */
void changeAffinity() {
  int thread_num, cpu;
  long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
  // Get user input
  pthread_mutex_lock(&terminal_lock);
  echo();      // Enable echo for input
  timeout(-1); // Make input blocking

  printw("\nEnter thread number (0-3): ");
  refresh();
  scanw("%d", &thread_num);

  printw("Enter CPU core (0-%ld): ", number_of_processors - 1);
  refresh();
  scanw("%d", &cpu);

  // Validate and apply input
  if (thread_num >= 0 && thread_num < 4 && cpu >= 0 &&
      cpu < number_of_processors) {
    affinity_values[thread_num] = cpu;
    printw("Thread %d now on CPU %d\n", thread_num, cpu);
  } else {
    printw("Invalid input!\n");
  }

  noecho();     // Disable echo
  timeout(100); // Restore non-blocking timeout (100ms)
  refresh();
  pthread_mutex_unlock(&terminal_lock);

  sleep(1);
}

/**
 * @brief Change nice value for a thread
 *
 * Prompts user for thread number and nice value, then updates the priority.
 * Validates input and provides feedback.
 */
void changeNice() {
  int thread_num;
  int nice_val;

  // Get user input
  pthread_mutex_lock(&terminal_lock);
  echo();      // Enable echo for input
  timeout(-1); // Make input blocking (remove timeout)

  printw("\nEnter thread number (0-3): ");
  refresh();
  scanw("%d", &thread_num);

  printw("Enter nice value (-20 to 19): ");
  refresh();
  scanw("%d", &nice_val);

  // Validate input
  if (thread_num >= 0 && thread_num < 4 && nice_val >= -20 && nice_val <= 19) {
    nice_values[thread_num] = nice_val;
    printw("Set thread %d nice value to %d\n", thread_num, nice_val);
  } else {
    printw("Invalid input! Values not changed.\n");
  }

  noecho();     // Disable echo
  timeout(100); // Restore non-blocking timeout
  refresh();
  pthread_mutex_unlock(&terminal_lock);

  sleep(1);
}

/**
 * @brief Thread function to check for prime numbers
 *
 * Continuously generates random numbers and checks if they are prime.
 * Respects CPU affinity and nice value settings.
 * Outputs found primes to the terminal.
 *
 * @param ptr Thread number cast as void pointer
 * @return NULL
 */
void *primeChecker(void *ptr) {

  int thread_num = (int)ptr;
  int random_num;
  int isPrime;
  cpu_set_t mask;
  // Seed rng
  srand(time(0));

  // Initial thread message
  pthread_mutex_lock(&terminal_lock);
  printw("Thread %d initialized\n", thread_num);
  refresh();
  pthread_mutex_unlock(&terminal_lock);

  while (end_program_flag == 0) {
    pthread_join(input, NULL);
    // Set CPU affinity
    CPU_ZERO(&mask);
    CPU_SET(affinity_values[thread_num], &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
      perror("sched_setaffinity failed");
    }

    // Set nice value
    nice(nice_values[thread_num]);

    // Generate and check number
    random_num = rand();
    isPrime = random_num > 1; // 0 and 1 are not primes

    for (int i = 2; i <= random_num / 2; i++) {
      if (random_num % i == 0) {
        isPrime = 0;
        break;
      }
    }

    pthread_mutex_lock(&terminal_lock);
    if (isPrime) {
      printw("Thread %d: Prime %d\n", thread_num, random_num);
    }
    refresh();
    pthread_mutex_unlock(&terminal_lock);

    usleep(100000);
  }
}

/**
 * @brief Main process function
 *
 * Initializes ncurses, creates threads, and handles user input.
 * Displays status information and processes menu options.
 */
void mainProcess() {

  int user_input;
  pthread_attr_t attr;
  struct sched_param param;

  // Ncurses initialization
  initscr();
  // cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  printw("Starting program...\n");
  refresh();
  sleep(1); // Let initial message display

  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
  param.sched_priority = 0;
  pthread_attr_setschedparam(&attr, &param);

  // Create threads
  pthread_create(&thread_1, &attr, primeChecker, (void *)0);
  pthread_create(&thread_2, &attr, primeChecker, (void *)1);
  pthread_create(&thread_3, &attr, primeChecker, (void *)2);
  pthread_create(&thread_4, &attr, primeChecker, (void *)3);
  // pthread_create(&input, &attr, changeNice, NULL);

  // Main loop
  while (end_program_flag == 0) {
    pthread_mutex_lock(&terminal_lock);
    mvprintw(
        0, 0,
        "Press 0 => exit, Press 1 => Change CPU, Press 2 => Change Nice Val\n");
    mvprintw(1, 0, "Thread 1: CPU Affinity %d, Nice %d\n", affinity_values[0],
             nice_values[0]);
    mvprintw(2, 0, "Thread 2: CPU Affinity %d, Nice %d\n", affinity_values[1],
             nice_values[1]);
    mvprintw(3, 0, "Thread 3: CPU Affinity %d, Nice %d\n", affinity_values[2],
             nice_values[2]);
    mvprintw(4, 0, "Thread 4: CPU Affinity %d, Nice %d\n", affinity_values[3],
             nice_values[3]);
    mvprintw(5, 0, "-----------------------\n");

    refresh();
    pthread_mutex_unlock(&terminal_lock);

    sleep(1);
    clear();

    timeout(100); // Wait 100ms for input
    user_input = getch();
    switch (user_input) {

    // Somewhat unresponsive. Can take multiple tries for some reason
    case '0':
      end_program_flag = 1;
      break;

    case '1':
      changeAffinity();
      break;

    case '2':
      changeNice();

      break;
    }
  }

  // Cleanup
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
  pthread_join(thread_3, NULL);
  pthread_join(thread_4, NULL);
  sleep(1);
  printw("\n End Program\n");
  sleep(1);
  endwin();
}
