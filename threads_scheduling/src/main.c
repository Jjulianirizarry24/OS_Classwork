#define _GNU_SOURCE

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

struct timespec start;

void* thread_func(void* arg) {
  int num = *(int*)arg;

  cpu_set_t mask;

  CPU_ZERO(&mask);

  if (num == 1) {
    nice(10);
    CPU_SET(2, &mask);
  } else {
    nice(10);
    CPU_SET(3, &mask);
  }

  sched_setaffinity(0, sizeof(mask), &mask);

  sched_getaffinity(0, sizeof(cpu_set_t), &mask);
  long nproc = sysconf(_SC_NPROCESSORS_ONLN);
  printf("sched_getaffinity = ");
  for (int i = 0; i < nproc; i++) {
    printf("%d ", CPU_ISSET(i, &mask));
  }
  printf("\n");

  int i = 0;
  while (i < 500) {
    for (volatile long j = 0; j < 1000000; j++) {
      volatile float e = sqrt(cos((float)j));
    }
    printf("Thread %d running. iteration %d on cpu %d\n", num, i,
           sched_getcpu());
    i++;
  }

  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed_time =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

  printf("thread %d finished in %lf seconds\n", num, elapsed_time);
}

int main() {
  pthread_t t1, t2;
  int arg1 = 1, arg2 = 2;

  clock_gettime(CLOCK_MONOTONIC, &start);

  struct sched_param param;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = 90;
  pthread_attr_setschedparam(&attr, &param);

  pthread_create(&t1, &attr, thread_func, &arg1);

  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = 10;
  pthread_attr_setschedparam(&attr, &param);

  pthread_create(&t2, &attr, thread_func, &arg2);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return 0;
}
