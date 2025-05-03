#define _GNU_SOURCE

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

// --------------- Global Variables ---------------
int *array;
pthread_mutex_t mutex;
int NE, NICE_1, NICE_2;
int end_program_flag = 0;

// --------------- Thread 1: Set Array Values to -1 ---------------
void *thread1(void *arg) {
  // Set nice value to provided argument value
  nice(NICE_1);

  // Set to use CPU #1
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(1, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  // Set random number in array to -1
  while (end_program_flag != 1) {
    int r = rand() % NE;
    pthread_mutex_lock(&mutex);
    array[r] = -1;
    pthread_mutex_unlock(&mutex);
  }
}

// --------------- Thread 1: Set Array Values to 99 ---------------
void *thread2(void *arg) {
  // Set nice value to provided argument value
  nice(NICE_2);

  // Set to use CPU #1
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(1, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  // Set random number in array to 99
  while (end_program_flag != 1) {
    int r = rand() % NE;
    pthread_mutex_lock(&mutex);
    array[r] = 99;
    pthread_mutex_unlock(&mutex);
  }
}

// --------------- Thread 3: Plot values  ---------------
void *thread3(void *arg) {

  // Set to use CPU #0
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  int copy_arr[NE];
  while (end_program_flag != 1) {
    pthread_mutex_lock(&mutex);

    int th1_ctr = 0, th2_ctr = 0;

    // Copy all elements in array
    for (int i = 0; i < NE; i++) {
      copy_arr[i] = array[i];
    }

    // Get Percentage
    for (int i = 0; i < NE; i++) {
      if (array[i] == -1)
        th1_ctr++;
      else if (array[i] == 99)
        th2_ctr++;
    }
    if (th1_ctr == NE || th2_ctr == NE)
      end_program_flag = 1;
    pthread_mutex_unlock(&mutex);

    // Print Percentage
    float thread1_percent = (float)th1_ctr / NE * 100;
    float thread2_percent = (float)th2_ctr / NE * 100;

    printf("Thread 1 Counter:%f%%\n", thread1_percent);
    printf("Thread 2 Counter:%f%%\n", thread2_percent);
    printf("-------------\n");
    usleep(10000);
  }
}

int main(int argc, char *argv[]) {

  // --------------- Validate Parameters ---------------
  if (argc != 4) {
    // Print error message and exit if not enough arguments
    fprintf(stderr, "Error: Need to provide no more and no less than three "
                    "additional arguments.\n");
    return 1;
  }

  // --------------- Initialize Variables  ---------------
  pthread_t th1;
  pthread_t th2;
  pthread_t th3;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_mutex_init(&mutex, NULL);
  NE = atoi(argv[1]);
  NICE_1 = atoi(argv[2]);
  NICE_2 = atoi(argv[3]);

  // --------------- Initialize Memory  ---------------
  array = mmap(NULL, NE * sizeof(int), PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (array == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  // Set Random Seed
  srand(time(NULL));

  // --------------- Thread Creation & Time Measuring logic  ---------------
  double start_time_in_seconds;
  double end_time_in_seconds;
  double time_taken;
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  pthread_create(&th1, &attr, thread1, NULL);
  pthread_create(&th2, &attr, thread2, NULL);
  pthread_create(&th3, &attr, thread3, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  clock_gettime(CLOCK_MONOTONIC, &end);

  start_time_in_seconds = start.tv_sec + (start.tv_nsec / 1e9);
  end_time_in_seconds = end.tv_sec + (end.tv_nsec / 1e9);
  time_taken = end_time_in_seconds - start_time_in_seconds;

  // --------------- Print all starting elements (Up to 50 only) ---------------
  for (int i = 0; i < 50; i++) {
    if (i >= NE)
      break;
    printf("Element at Array Index %d: %d\n", i, array[i]);
  }
  printf("%lf\n", time_taken);
}
