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
void Get_input(int curRank, int numProcs, float* lower, float* upper, int* n);

int main() {
    int curRank, numProcs, n;
    float lower = 0;
    float upper = 1800;
    float delta = 0.01;
    float area, total;
    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &curRank);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    /* Find out how many processes are being used */
    Get_input(curRank, numProcs, &lower, &upper, &n);

    float ratio = (upper - lower) / n;
    int numBoxes = n / numProcs;
    float start = lower + curRank * ratio;
    float end = start + numBoxes * ratio;

    printf("my_rank=%d, start a=%lf, end b=%lf, number of quadratures = %d, step_size=%lf\n", curRank, start, end,
           numBoxes, ratio);
    // float* results;
    area = Lriemann(start, end, ratio, numBoxes);

    printf("my_rank=%d, integrated area = %lf, step_size * number quadratures=%lf\n", curRank, area,
           (ratio * numBoxes));

    // cout << "the area calculated from " << start << " to " << end << " for process " << curRank << " is: " << area
    //      << endl;

    MPI_Reduce(&area, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (curRank == 0) {
        printf("With n = %f quadratures, our estimate\n", 1 / delta);
        printf("of the integral from %f to %f = %15.14lf\n", lower, upper, total);
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}

// left riemann
// float Lriemann(float lower, float upper, float delta, int quads) {
//     static float result;
//     float x = 0.0;
//     int newUp = (upper - lower) / delta;

//     result = velocity(lower);
//     // result[1] = acceleration(lower);
//     for (int time = lower; time < newUp; time++) {
//         // cout << time << endl;
//         x += delta;
//         result += velocity(x);
//         // result[1] += acceleration(x) * delta;
//         // cout.precision(18);
//         // cout << "time: " << x << ", position: " << result << endl;
//     }
//     // result *= delta;
//     return result;
// }

float Lriemann(float lower, float upper, float delta, int rectangles) {
    float lval, x, area = 0.0;
    lval = velocity(lower);
    x = lower;

    for (int i = 1; i < rectangles; i++) {
        area += lval * delta;
        x += delta;
        lval = velocity(x);
    }

    return area;
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
float acceleration(float time) { return (sin(time / TSCALE) * ASCALE); }     // returns acceleration at time t
float velocity(float time) { return ((-cos(time / TSCALE) + 1) * VSCALE); }  // returns velocity at time t