#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;
void* Count(void* param);

pthread_mutex_t mutex;

int main() {
  pthread_t thread_counter;
  pthread_t thread_counter_2;

  pthread_mutex_init(&mutex, NULL);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  int initial_value = 56;

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  pthread_create(&thread_counter, &attr, Count, NULL);
  pthread_create(&thread_counter_2, &attr, Count, NULL);

  pthread_join(thread_counter, NULL);
  pthread_join(thread_counter_2, NULL);

  clock_gettime(CLOCK_MONOTONIC, &end);

  printf("the final number is %d\n", counter);

  double elapsed_time =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
  printf("the elapsed time is %f\n", elapsed_time);

  return 0;
}

void* Count(void* param) {
  for (int i = 0; i < 1000000; i++) {
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
  }
}
