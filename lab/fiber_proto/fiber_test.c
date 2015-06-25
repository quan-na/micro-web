/*
 * This is a proof of concept using fibers to solve
 * a complex problem.
 * chosen problem is ConvexHull.
 * first thread will read a number of points from text files.
 * then it will start a thread that calculate max/min 4 points.
 * thread 1 & 2 will run parallel.
 * when they 're finished, 4 other threads will be started in order to calculate the hull.
 */

#include <stdio.h>
#include <stdlib.h>

// point structure with int type values
typedef struct int2d_t {
  int x;
  int y;
} int2d;

int crossProduct(int2d a, int2d b) {
  return a.x*b.y - a.y*b.x;
}

void point2Vector(int2d a, int2d b, int2d* v) {
  v->x = b.x - a.x;
  v->y = b.y - a.y;
}

// check if with root point, rotating from point A to point B is clockwise(0) or counter-clockwise(1) 
int checkRotateDirection(int2d root, int2d a, int2d b) {
  int2d v1, v2;
  point2Vector(root, a, &v1);
  point2Vector(root, b, &v2);
  return (crossProduct(v1, v2) > 0);
}

// these will be part of fiber library
typedef struct Fiber_t {
  int cPoint;
  int (*fiberFunc)(struct Fiber_t*);
} Fiber;

typedef struct FiberController_t {
  int fiberCapac;
  int fiberNum;
  Fiber** fibers;
  int emergeFlg;
  //Fiber dummyFiber;
} FiberController;

FiberController* initFiberController(int numFibers) {
  FiberController* fiberController = (FiberController*) malloc(sizeof(FiberController));
  if (NULL == fiberController)
    return NULL;
  fiberController->emergeFlg = 0;
  //fiberController->dummyFiber.cPoint = -1;
  fiberController->fiberCapac = numFibers;
  fiberController->fiberNum = 0;
  fiberController->fibers = (Fiber**) malloc(numFibers * sizeof(Fiber*));
  if (NULL == fiberController->fibers) {
    free(fiberController);
    return NULL;
  }
  return fiberController;
}

void finalizeFiberController(FiberController* controller) {
  if (NULL == controller)
    return;
  if (NULL == controller->fibers) {
    free(controller);
    return;
  }
  int i;
  for (i=0; i<controller->fiberNum; i++) {
    if (controller->fibers[i]->cPoint > 0) {
      controller->fibers[i]->cPoint = -1;
      controller->fibers[i]->fiberFunc(controller->fibers[i]);
    }
    //if (controller->fibers[i] != &(controller->dummyFiber))
    free(controller->fibers[i]);
  }
  free(controller->fibers);
  controller->fibers = NULL;
  free(controller);
  return;
}

// to pass parameters to fibers, return fiberData pointer and let starter assign params
// do not use dummy fiber
Fiber* startFiber(FiberController* controller, int (*fiberFunc)(Fiber*), int dataSize) {
  int i;
  for (i=0; i<controller->fiberNum; i++) {
    if (controller->fibers[i]->cPoint == -1)
      break;
  }
  Fiber* newFiber = (Fiber*) malloc(dataSize);
  if (NULL == newFiber)
    return NULL;
  if (i<controller->fiberNum) {
    free(controller->fibers[i]);
  } else {
    if (controller->fiberNum < controller->fiberCapac)
      controller->fiberNum++;
    else {
      free(newFiber);
      return NULL;
    }
  }
  controller->fibers[i] = newFiber;
  controller->fibers[i]->cPoint = 0;
  controller->fibers[i]->fiberFunc = fiberFunc;
  return controller->fibers[i];
}

void loopFiber(FiberController* controller) {
  if (controller->fiberNum <= 0)
    return;
  int hasRunning;
  do {
    hasRunning = 0;
    if (controller->emergeFlg)
      break;
    for (i=0; i<controller->fiberNum; i++) {
      if (controller->fibers[i]->cPoint != -1) {
        hasRunning = 1;
        controller->fibers[i]->fiberFunc(controller->fibers[i]);
      }
    }
  } while (hasRunning);
}

// shared fiber data
// Array of points
int2d points[100];
int numPoints;
int readPoints;

// index of border points
int topP, leftP, bottomP, rightP;
// flags for finished calculation threads
int topFin, leftFin, bottomFin, rightFin;

// 1st Thread: Read points from files
// Start 2nd, 3rd, 4th, 5th, 6th thread
// trigger finished read signal
typedef struct readFileFiberData_t {
  Fiber fiber;
  FILE* file;
} readFileFiberData;

int readFileFiber(readFileFiberData *data) {
  switch (data->fiber.cPoint) {
  case 0:
    // variable init
    data->file = fopen("fiber_proto/input.txt", "r");
    numPoints = 0;
    readPoints = 0;
    return data->fiber.cPoint = 1;
  case 1:
    // TODO: start other threads
    return data->fiber.cPoint = 2;
  case 2:
    // TODO: read files
    return data->fiber.cPoint = 3;
  case -1:
  default:
    return data->fiber.cPoint = -1;
  }
}

// 2nd Thread: calculate max, min 4 points
// Wait for new point(s) to be read from 1st Thread
// trigger max/min finished signal

// 3rd Thread: calculate from top (max y)
// Wait for max/min finished signal

// 4th Thread: calculate from bottom (min y)
// Wait for max/min finished signal

// 5th Thread: calculate from left (min x)
// Wait for max/min finished signal

// 6th Thread: calculate from right (max x)
// Wait for max/min finished signal

// main Thread loop
int main(void) {
  return 0;
}
