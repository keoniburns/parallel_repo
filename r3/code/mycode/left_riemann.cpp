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
float Lriemann(float lower, float upper, float delta);
void Get_input(int curRank, int numProcs, float* lower, float* upper, int* numQuads);

int main() {
    int curRank, numProcs, numQuads;
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
    Get_input(curRank, numProcs, &lower, &upper, &numQuads);

    float ratio = upper / numProcs;
    float start = curRank * ratio;
    float end = start + ratio;

    float* results;
    area = Lriemann(lower, upper, delta);

    cout << "the area calculated from " << start << " to " << end << " for process " << curRank << " is: " << area
         << endl;

    MPI_Reduce(&area, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (curRank == 0) {
        printf("With n = %d quadratures, our estimate\n", 1 / delta);
        printf("of the integral from %f to %f = %15.14lf\n", start, end, total);
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}

// left riemann
float Lriemann(float lower, float upper, float delta) {
    static float result;  // res[0] = pos, res[1] = vel
    float x = 0.0;
    float newUp = upper / delta;

    result = velocity(lower);
    // result[1] = acceleration(lower);
    for (int time = 1; time < newUp; time++) {
        x += delta;
        result += velocity(x) * delta;
        // result[1] += acceleration(x) * delta;
        // cout.precision(18);
        // cout << "time: " << x << ", position: " << result[0] << ", velocity: " << result[1] << endl;
    }
    return result;
}

void Get_input(int curRank, int numProcs, float* lower, float* upper, int* numQuads) {
    int rc = 0;

    if (curRank == 0) {
        printf("Enter a, b, and n\n");
        rc = scanf("%lf %lf %d", lower, upper, numQuads);
        if (rc < 0) perror("Get_input");
    }
    MPI_Bcast(lower, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(upper, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(numQuads, 1, MPI_INT, 0, MPI_COMM_WORLD);
} /* Get_input */

// these all return the respective attribute value using the antiderivative
float acceleration(float time) { return (sin(time / TSCALE) * ASCALE); }     // returns acceleration at time t
float velocity(float time) { return ((-cos(time / TSCALE) + 1) * VSCALE); }  // returns velocity at time t