/*
 * This is a proof of concept using fibers to solve
 * a complex problem.
 * I choose ConvexHull problem to demonstrade fiber technique.
 * The first thread will read a number of points from text files.
 * With each point read, it will start a thread with the point as parameter.
 * The calculation thread will calculate with all read points and then hold to wait
 * for more points to be read.
 * With each point A, if there is another point B that all other points when rotating
 * with A as root counter clockwise from B, then A (and also B) belong to the convexhull
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
  int cx = crossProduct(v1, v2);
  if (cx == 0)
    return 0;
  if (cx > 0)
    return 1;
  return -1;
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
  int hasRunning, i;
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
FiberController* fiberctrl;
// Array of points
int2d points[100];
int numPoints;
int readPoints;

// index of border points
int topP, leftP, bottomP, rightP;
// flags for finished calculation threads
int topFin, leftFin, bottomFin, rightFin;

// 2nd Thread: calculate rotation with 1 specific point as root
// Wait for new point(s) to be read from 1st Thread
// trigger max/min finished signal
typedef struct calculationFiberData_t {
  Fiber fiber;
  int calculatePt;
  int currentPt, maxPt, firstPt;
} calculationFiberData;

int calculationFiber(Fiber* fbdata) {
  calculationFiberData *data = (calculationFiberData*) fbdata;
  switch(data->fiber.cPoint) {
  case 0:
    data->currentPt = 0;
    data->maxPt = -1;
    data->firstPt = -1;
    return data->fiber.cPoint = 1;
  case 1:
    while (data->currentPt < numPoints) {
    return data->fiber.cPoint = 2;
  case 2:
      if (data->currentPt < readPoints) {
        if (data->calculatePt != data->currentPt) {
          if (-1 == data->maxPt) {
            data->maxPt = data->currentPt;
            data->firstPt = data->maxPt;
            printf("set max pt %d for pt <%d>\n", data->maxPt, data->calculatePt);
          } else {
            if (checkRotateDirection(points[data->calculatePt], points[data->maxPt], points[data->currentPt]) > 0) {
              data->maxPt = data->currentPt;
              printf("change max pt %d for pt <%d>\n", data->maxPt, data->calculatePt);
              if (checkRotateDirection(points[data->calculatePt], points[data->maxPt], points[data->firstPt]) > 0) {
                printf("full circle rotated with pt <%d>\n", data->calculatePt);
                data->maxPt = -1;
                return data->fiber.cPoint = 3;
              }
            }
          }
        }
        data->currentPt++;
      }
    }
    return data->fiber.cPoint = 3;
  case 3:
    if (data->maxPt == -1) {
      printf(" * point : <%d> inside.\n", data->calculatePt);
    } else {
      printf(" * * point : <%d> convex, max : %d\n", data->calculatePt, data->maxPt);
    }
    return data->fiber.cPoint = 4;
  case -1:
  default:
    return data->fiber.cPoint = -1;
  }
}

// 1st Thread: Read points from files
// Start 2nd, 3rd, 4th, 5th, 6th thread
// trigger finished read signal
typedef struct readFileFiberData_t {
  Fiber fiber;
  FILE* file;
  char* filename;
} readFileFiberData;

int readFileFiber(Fiber *fbdata) {
  readFileFiberData *data = (readFileFiberData*) fbdata;
  switch (data->fiber.cPoint) {
  case 0:
    // variable init
    data->file = fopen(data->filename, "r");
    numPoints = 0;
    readPoints = 0;
    if (NULL == data->file)
      return data->fiber.cPoint = 0;
    return data->fiber.cPoint = 1;
  case 1:
    fscanf(data->file, "%d", &numPoints);
    printf("num points : %d\n", numPoints);
    return data->fiber.cPoint = 2;
  case 2:
    for (readPoints = 0; readPoints<numPoints; readPoints++) {
    return data->fiber.cPoint = 3;
  case 3:
      // read files
      fscanf(data->file, "%d %d", &(points[readPoints].x), &(points[readPoints].y) );
      printf("read %d %d\n", points[readPoints].x, points[readPoints].y);
      // Start thread
      calculationFiberData* fbc = (calculationFiberData*)startFiber(fiberctrl, &calculationFiber, sizeof(calculationFiberData));
      fbc->calculatePt = readPoints;
    }
    return data->fiber.cPoint = 4;
  case -1:
  default:
    return data->fiber.cPoint = -1;
  }
}

// main Thread loop
int main(int argc, char* argv[]) {
  fiberctrl = initFiberController(101);
  readFileFiberData* fb1 = (readFileFiberData*) startFiber(fiberctrl, &readFileFiber, sizeof(readFileFiberData));
  fb1->filename = argv[1];
  loopFiber(fiberctrl);
  finalizeFiberController(fiberctrl);
  return 0;
}
