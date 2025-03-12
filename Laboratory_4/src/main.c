#include <math.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;
double balance = 100;
void *Deposit(void *param);
void *Withdrawal(void *param);
// void *Menu(void *param);
// void *checkBalance(void *param);

pthread_mutex_t mutex;

/**
 * @brief Bank Account Simulator
 *
 *
 *
 */
int main() {
  // Initialize Mutex
  pthread_t thread_counter;
  pthread_t thread_counter_2;

  pthread_mutex_init(&mutex, NULL);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  // pthread_create(&thread_counter, &attr, Deposit, NULL);
  // pthread_create(&thread_counter_2, &attr, Withdrawal, NULL);
  // pthread_join(thread_counter, NULL);
  // pthread_join(thread_counter_2, NULL);

  // Initialize ncurses
  initscr();
  noecho();
  int key;
  int num_transactions;
  double amount;

  while (1) {
    clear();
    printw("Menu\n");
    printw("______________________\n");
    printw("1. Perform Deposits\n");
    printw("2. Perform Withdrawals\n");
    printw("3. Check Balance\n");
    printw("4. Exit\n");

    scanw("%d", &key);
    scanw("%d", &num_transactions);
    scanw("%lf", &amount);
    if (key == 1) {
      for (int i = 0; i < num_transactions; i++) {
        pthread_create(&thread_counter, &attr, Deposit, (void *)&amount);
      }
    } else if (key == 2) {
      for (int i = 0; i < num_transactions; i++) {
        pthread_create(&thread_counter, &attr, Withdrawal, (void *)&amount);
      }
    }
    pthread_join(thread_counter, NULL);
    pthread_join(thread_counter_2, NULL);

    refresh();
  }

  return 0;
}

void *Deposit(void *param) {
  pthread_mutex_lock(&mutex);
  double *temp;
  temp = (double *)param;
  balance += *temp;
  pthread_mutex_unlock(&mutex);
}

void *Withdrawal(void *param) {
  pthread_mutex_lock(&mutex);
  double *temp;
  temp = (double *)param;
  balance += *temp;
  pthread_mutex_unlock(&mutex);
}
