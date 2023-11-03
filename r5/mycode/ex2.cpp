#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>

#include <iomanip>
#include <iostream>

#define PI (3.14159265358979323846)
// #define NUM_THREADS (8)

using namespace std;
int main(int argc, char *argv[]) {
    struct timespec start, stop;
    double fstart, fstop;
    double first_sum = 0.0, sec_sum = 0.0;
    unsigned int length = 0;
    double constant = 1.0;
    unsigned int i = 0;
    int threads = 4;  // for default

    if (argc < 2) {
        cerr << "usage: ./ex2 <number of iterations, n> <number of threads>" << endl;
        exit(-1);
    } else {
        length = atoi(argv[1]);
    }
    if (argc == 3) {
        threads = atoi(argv[2]);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    fstart = (double)start.tv_sec + ((double)start.tv_nsec / 1000000000.0);

#pragma omp parallel for num_threads(threads) reduction(+ : first_sum) reduction(+ : sec_sum) private(i) shared(length)
    for (i = 0; i < length; i++) {
        if (i % 2 == 0) {
            first_sum += constant / ((2.0 * (double)i) + 1.0);
        } else {
            first_sum += -constant / ((2.0 * (double)i) + 1.0);
        }
        sec_sum += 2.0 / (((4.0 * (double)i) + 1.0) * (4.0 * (double)i + 3.0));
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    fstop = (double)stop.tv_sec + ((double)stop.tv_nsec / 1000000000.0);

    double Merr = 1000000000.0 * ((M_PI - (4.0 * first_sum)));
    double Eerr = 1000000000.0 * ((M_PI - (4.0 * sec_sum)));
    cout << setprecision(16);
    cout << "Madvha-Leibniz pi: \n" << (4 * first_sum) << endl;
    cout << "Euler pi: \n" << (4 * sec_sum) << endl;
    cout << "% error for the two in their respective order are:\n" << Merr << "\n" << Eerr << endl;
    cout << "\n\n"
         << "monotonic time: " << (fstop - fstart) << endl;
    return 0;
}
