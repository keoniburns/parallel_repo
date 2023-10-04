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
double acceleration(double time);
double velocity(double time);
double Mriemann(double, double, double, int);
double trap(double, double, double, int);
void Get_input(int curRank, int numProcs, double* lower, double* upper, int* n);

int main() {
    int curRank, numProcs, n;
    double lower, upper;
    double area = 0, total = 0;
    struct timespec start_time, end_time;
    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &curRank);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    /* Find out how many processes are being used */
    Get_input(curRank, numProcs, &lower, &upper, &n);

    double step = (upper - lower) / n;
    int numBoxes = n / numProcs;
    int increment = step * numBoxes;
    double start = lower + (curRank * increment);
    double end = start + (increment);
    double time_taken;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);

    area = Mriemann(start, end, step, numBoxes);
    // area = trap(start, end, step, numBoxes);
    MPI_Reduce(&area, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    /* Print the result */
    if (curRank == 0) {
        cout << "step size is : " << step << endl;
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

double Mriemann(double lower, double upper, double delta, int rectangles) {
    double lval, x, area = 0.0;
    double midpoint = (double)(delta / 2);
    x = lower;
    lval = velocity(x + midpoint);
    // lval = acceleration(x + midpoint);
    for (int i = 1; i < rectangles; i++) {
        area += lval;
        x += delta;
        lval = velocity(x + midpoint);
        // lval = acceleration(x + midpoint);
    }
    area *= delta;
    return area;
}

double trap(double lower, double upper, double delta, int rectangle) {
    double val, fx;
    double left, right;
    left = lower;
    right = lower + delta;
    val = (velocity(left) + velocity(right)) / 2.0;
    // val = (acceleration(left) + acceleration(right)) / 2.0;

    for (int i = 1; i < rectangle; i++) {
        left += delta;
        right = lower + delta;
        fx = ((velocity(left) + velocity(right)) / 2.0);
        // fx = (acceleration(left) + acceleration(right)) / 2.0;

        val += fx;
    }
    val *= delta;
    return val;
}

void Get_input(int curRank, int numProcs, double* lower, double* upper, int* n) {
    int rc = 0;

    if (curRank == 0) {
        printf("Enter a, b, n\n");
        rc = scanf("%lf %lf %d", lower, upper, n);
        if (rc < 0) perror("Get_input");
    }
    MPI_Bcast(lower, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(upper, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);

} /* Get_input */

// these all return the respective attribute value using the antiderivative
double acceleration(double time) { return ((double)sin(time / TSCALE) * ASCALE); }  // returns acceleration at time t
double velocity(double time) { return (((double)-cos(time / TSCALE) + 1) * VSCALE); }