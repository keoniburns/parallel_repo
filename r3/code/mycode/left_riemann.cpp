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

    printf("my_rank=%d, start a=%lf, end b=%lf, number of quadratures = %d, step_size=%lf\n", curRank, start, end,
           numBoxes, step);

    // area = Lriemann(start, end, step, numBoxes);
    area = trap(start, end, step, numBoxes);
    printf("my_rank=%d, integrated area = %lf, step_size * number quadratures=%lf\n", curRank, area, (step * numBoxes));

    MPI_Reduce(&area, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (curRank == 0) {
        printf("With n = %d quadratures, our estimate\n", n);
        printf("of the integral from %f to %f = %15.14lf\n", lower, upper, total);
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}

float Lriemann(float lower, float upper, float delta, int rectangles) {
    float lval, x, area = 0.0;
    lval = velocity(lower);
    x = lower;

    for (int i = 1; i < rectangles; i++) {
        area += lval;
        x += delta;
        lval = velocity(x);
    }
    area *= delta;
    return area;
}

float trap(float lower, float upper, float delta, int rectangle) {
    float val, x;
    float left, right;
    left = lower;
    right = lower + delta;
    val = (velocity(left) - velocity(right)) / 2.0;

    for (int i = 1; i < rectangle; i++) {
        x = left + (i * delta);
        val += velocity(x);
    }
    val *= delta;
    return delta;
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
