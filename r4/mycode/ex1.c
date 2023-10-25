/**
 * @file ex1.c
 * @author Keoni Burns
 * @brief adapted from code provided by Sam Siewert
 * interpolates data provided using a default step size of 1/10 of a second
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ex4accel.h"

double table_accel(int timeidx);
double faccel(double time);

#define STEPS_PER_SEC (10)

void main(int argc, char *argv[]) {
    int idx;
    int steps_per_sec = STEPS_PER_SEC;
    double time, dt;

    printf("argc=%d, argv[0]=%s\n", argc, argv[0]);

    // user defined step size
    if (argc == 2) {
        sscanf(argv[1], "%d", &steps_per_sec);
        printf("Will use %d steps per sec\n", steps_per_sec);
    } else {
        printf("Use: trainprofile [steps_per_sec]\n");
        printf("Will use default time profile\n");
    }

    dt = 1.0 / steps_per_sec;  // change in time ratio
    int newSize = steps_per_sec * 1800;

    for (idx = 0; idx <= newSize; idx++) {
        time = 0.0 + (dt * (double)idx);
        printf("%4.1lf, %015.14lf\n", time, faccel(time));
    }
}

// returns the acceleration at the time value passed in
// also checks whether the value is in the bounds of the lookup table
double table_accel(int timeidx) {
    long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

    // Check array bounds for look-up table
    if (timeidx > tsize) {
        printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize - 1);
        exit(-1);
    }

    return DefaultProfile[timeidx];
}

/**
 * @brief takes the value of acceleration at two known times and multiples the difference of them by the ratio of
 * 1/steps_per_sec
 *
 * @param time
 * @return double
 */
double faccel(double time) {
    int timeidx = (int)time;
    int timeidx_next = ((int)time) + 1;
    double delta_t = time - (double)((int)time);

    return (table_accel(timeidx) + ((table_accel(timeidx_next) - table_accel(timeidx)) * delta_t));
}
