#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void *Plotting(void *param);
void *readKeyboard(void *param);
void mainProcess();
typedef struct {
  int x;
  int y;
} BallPosition;

#endif