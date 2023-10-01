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
float* Lriemann(double lower, double upper, double delta);
double* Mriemann(double lower, double upper, double delta);
double* trap(double lower, double upper, double delta);
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);

int main() {
    // int curRank, numProcs, n = 1024;
    double lower = 0;
    double upper = 1800;
    double delta = 0.01;
    float* results;
    double* Mresults;
    results = Lriemann(lower, upper, delta);

    Mresults = Mriemann(lower, upper, delta);
    double* trapRes = trap(lower, upper, delta);
    cout.precision(20);
    cout << "The integral of f(x) from 0.0 to " << upper << " with " << delta << " n is " << results[0] << "\n"
         << "the integral of acceleration from 0.0 to " << upper << " with " << delta << " n is " << results[1] << endl;
    cout << "The integral of f(x) from 0.0 to " << upper << " using midpoint riemann with " << delta << " n is "
         << Mresults[0] << "\n"
         << "the integral of acceleration from 0.0 to " << upper << " using midpoint riemann with " << delta << " n is "
         << Mresults[1] << endl;
    cout << "The integral of f(x) from 0.0 to " << upper << " using trapezoidal riemann with " << delta << " n is "
         << trapRes[0] << "\n"
         << "the integral of acceleration from 0.0 to " << upper << " using trapezoidal riemann with " << delta
         << " n is " << trapRes[1] << endl;

    return 0;
}

// left riemann
float* Lriemann(double lower, double upper, double delta) {
    static float result[2];  // res[0] = pos, res[1] = vel
    double x = 0.0;
    double newUp = upper / delta;

    result[0] = velocity(lower);
    result[1] = acceleration(lower);
    for (int time = 1; time < newUp; time++) {
        x += delta;
        result[0] += velocity(x);
        result[1] += acceleration(x);
    }
    result[0] *= delta;
    result[1] *= delta;
    return result;
}

// midpoint riemann sum
double* Mriemann(double lower, double upper, double delta) {
    double pos, vel;
    static double result[2];
    // double delta_ratio = (delta / 2);
    double newUp = upper / delta;
    // iterates using the step siae (delta) as the unit of incrementation
    for (int time = 1; time < newUp; time++) {
        double midpoint = (double)(time - delta);
        pos = velocity(midpoint);
        vel = acceleration(midpoint);
        result[0] += pos;
        result[1] += vel;
    }
    result[0] *= delta;
    result[1] *= delta;
    return result;
}

// trapezoidal riemann
double* trap(double lower, double upper, double delta) {
    double pos, vel;
    static double result[2];
    double newUp = upper / delta;
    // double pratio, vratio;

    // iterates using the step siae (delta) as the unit of incrementation
    for (int time = 1; time < newUp; time++) {
        pos = (velocity(time - 1) + velocity(time)) / 2.0;
        vel = (acceleration(time - 1) + acceleration(time)) / 2.0;
        result[0] += pos;
        result[1] += vel;
    }
    result[0] *= delta;
    result[1] *= delta;
    return result;
}

double f(double x, bool calcPos) {
    if (calcPos) return (velocity(x));
    return (acceleration(x));
}

double acceleration(double time) { return (sin(time / TSCALE) * ASCALE); }     // returns acceleration at time t
double velocity(double time) { return ((-cos(time / TSCALE) + 1) * VSCALE); }  // returns velocity at time t