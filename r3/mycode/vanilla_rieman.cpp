// #include <math.h>

#include <cmath>
#include <iostream>
using namespace std;

#define ASCALE 0.236589076381454
#define TSCALE (1800.0 / (2.0 * M_PI))
#define VSCALE ((ASCALE * 1800.0) / (2.0 * M_PI))

double mid(double, double, double, int);
double trap(double, double, double, int);
float left(float, float, float, int);
float Fvelocity(float time);
double velocity(double);
int main() {
    struct timespec start_time, end_time;
    double dlower = 0;
    double dupper = 1800;
    float flower = 0, fupper = 1800;
    double dn = 0.001;
    float fn = 0.01;
    double dS = 1800 / dn;
    float fS = 1800 / fn;
    float area = 0.0;
    double dArea = 0.0;
    double time_taken;

    // cout << "boxes " << fS << "\n"
    //      << "delta " << fn << endl;
    cout << "boxes " << dS << "\n"
         << "delta " << dn << endl;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    // area = left(flower, fupper, fn, fS);
    // dArea = mid(dlower, dupper, dn, dS);
    dArea = trap(dlower, dupper, dn, dS);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);

    cout.precision(15);
    cout << "area using left: " << area << endl;
    cout << "area using trap/mid: " << dArea << endl;

    time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
    time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
    printf("time elapsed for program: %f\n", time_taken);
    return 0;
}

float left(float lower, float upper, float delta, int steps) {
    float lval, x, area = 0.0;
    lval = Fvelocity(lower);

    x = lower;

    for (int i = 1; i < steps; i++) {
        area += lval;
        x += delta;
        lval = Fvelocity(x);
    }
    area *= delta;
    return area;
}

double trap(double lower, double upper, double delta, int step) {
    double val, fx;
    double left, right;
    left = lower;
    right = lower + delta;
    val = (velocity(left) + velocity(right)) / 2.0;

    for (int i = 0; i < step; i++) {
        left += delta;
        right = lower + delta;
        fx = ((velocity(left) + velocity(right)) / 2.0);

        val += fx;
    }
    val* delta;
    return val;
}

double mid(double lower, double upper, double delta, int step) {
    double lval, x, area = 0.0;
    double midpoint = (double)(delta / 2);
    x = lower;
    lval = velocity(x + midpoint);

    for (int i = 1; i < step; i++) {
        area += lval;
        x += delta;
        lval = velocity(x + midpoint);
    }
    area *= delta;
    return area;
}

float Fvelocity(float time) { return (((float)-cos(time / TSCALE) + 1) * VSCALE); }

double velocity(double time) { return (((double)-cos(time / TSCALE) + 1) * VSCALE); }