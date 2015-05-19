/*
 * This is a proof of concept using fibers to solve
 * a complex problem.
 * chosen problem is ConvexHull.
 * first thread will read a number of points from text files.
 * then it will start a thread that calculate max/min 4 points.
 * thread 1 & 2 will run parallel.
 * when they 're finished, 4 other threads will be started in order to calculate the hull.
 */
