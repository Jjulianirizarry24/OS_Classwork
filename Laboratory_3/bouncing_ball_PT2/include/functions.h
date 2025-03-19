#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void *Plotting(void *param);
void *movementController(void *param);
void *collisionChecker(void *param);
void *updateBall(void *param);
void mainProcess();
typedef struct {
  int x;
  int y;
} BallPosition;
#endif