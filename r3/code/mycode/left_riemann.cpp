#include <mpi.h>
#include <time.h>

#include <cmath>
#include <iostream>
using namespace std;

// scale macros
#define ASCALE 0.236589076381454
#define TSCALE (1800.0 / (2.0 * M_PI))
#define VSCALE ((ASCALE * 1800.0) / (2.0 * M_PI))

// declare function protos
float acceleration(float time);
float velocity(float time);
float Lriemann(float lower, float upper, float delta, int rectangles);
float trap(float, float, float, int);
void Get_input(int curRank, int numProcs, float* lower, float* upper, int* n);

int main() {
    int curRank, numProcs, n;
    float lower, upper;
    float area = 0, total = 0;
    struct timespec start_time, end_time;
    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &curRank);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    /* Find out how many processes are being used */
    Get_input(curRank, numProcs, &lower, &upper, &n);

    float step = (upper - lower) / n;
    int numBoxes = n / numProcs;
    int increment = step * numBoxes;
    float start = lower + (curRank * increment);
    float end = start + (increment);
    double time_taken;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);

    // area = Lriemann(start, end, step, numBoxes);
    area = trap(start, end, step, numBoxes);
    MPI_Reduce(&area, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    /* Print the result */
    if (curRank == 0) {
        printf("With n = %d quadratures, our estimate\n", n);
        printf("of the integral from %f to %f = %15.14lf\n", lower, upper, total);
    }

    time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
    time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
    printf("time elapsed for program: %f\n", time_taken);
    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}

float Lriemann(float lower, float upper, float delta, int rectangles) {
    float lval, x, area = 0.0;
    // lval = velocity(lower);
    lval = acceleration(lower);
    x = lower;

    for (int i = 1; i < rectangles; i++) {
        area += lval;
        x += delta;
        // lval = velocity(x);
        lval = acceleration(x);
    }
    area *= delta;
    return area;
}

float trap(float lower, float upper, float delta, int rectangle) {
    float val, fx;
    float left, right;
    left = lower;
    right = lower + delta;
    val = (velocity(left) + velocity(right)) / 2.0;
    // val = (acceleration(left) + acceleration(right)) / 2.0;

    for (int i = 1; i < rectangle; i++) {
        left += delta;
        right = lower + delta;
        fx = ((velocity(left) + velocity(right)) / 2.0);
        // fx = ((acceleration(left) + acceleration(right)) / 2.0);
        val += fx;
    }
    val *= delta;
    return val;
}

void Get_input(int curRank, int numProcs, float* lower, float* upper, int* n) {
    int rc = 0;

    if (curRank == 0) {
        printf("Enter a, b, n\n");
        rc = scanf("%f %f %d", lower, upper, n);
        if (rc < 0) perror("Get_input");
    }
    MPI_Bcast(lower, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(upper, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);

} /* Get_input */

// these all return the respective attribute value using the antiderivative
float acceleration(float time) { return ((float)sin(time / TSCALE) * ASCALE); }     // returns acceleration at time t
float velocity(float time) { return (((float)-cos(time / TSCALE) + 1) * VSCALE); }  // returns velocity at time t
