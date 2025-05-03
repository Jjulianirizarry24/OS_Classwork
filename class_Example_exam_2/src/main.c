#define _GNU_SOURCE

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

float* array;
pthread_mutex_t mutex;

void* thread_read(void* arg) {
  nice(5);

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(2, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  while (1) {
    int tem;
    scanf("%d", &tem);
    pthread_mutex_lock(&mutex);
    array[0] = tem;
    pthread_mutex_unlock(&mutex);
  }
}
void* thread_plot(void* arg) {
  nice(-8);

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(3, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  while (1) {
    pthread_mutex_lock(&mutex);
    int copy = array[0];
    pthread_mutex_unlock(&mutex);

    printf("%d\n", copy * 10);
  }
}

int main() {
  pthread_t th1;
  pthread_t th2;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_mutex_init(&mutex, NULL);

  array = mmap(NULL, 15 * sizeof(float), PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  pthread_create(&th1, &attr, thread_read, NULL);
  pthread_create(&th2, &attr, thread_plot, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
}
