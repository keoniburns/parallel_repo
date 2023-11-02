#include <math.h>
#include <stdio.h>

#include <iomanip>
// #include <omp.h>

#include <iostream>
#define PI (3.14159265358979323846)
#define NUM_THREADS (8)
using namespace std;
int main(int argc, char *argv[]) {
    double first_sum = 0.0, sec_sum = 0.0;
    unsigned int length = 0;
    double constant = 1.0;
    int i = 0;

    if (argc < 2) {
        cerr << "usage: ./ex2 <number of iterations, n>" << endl;
        exit(-1);
    } else {
        length = atoi(argv[1]);
    }

#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : first_sum) reduction(+ : sec_sum) private(i, factor) \
    shared(length)
    for (i = 0; i < length; i++) {
        first_sum += constant / ((2.0 * (double)i) + 1.0);
        sec_sum += 2.0 / (((4.0 * (double)i) + 1.0) * (4.0 * (double)i + 3.0));
        constant = -constant;
    }
    double Merr = 1000000000.0 * ((M_PI - (4.0 * first_sum)));
    double Eerr = 1000000000.0 * ((M_PI - (4.0 * sec_sum)));
    cout << setprecision(16);
    cout << "Madvha-Leibniz pi: \n" << (4 * first_sum) << endl;
    cout << "Euler pi: \n" << (4 * sec_sum) << endl;
    cout << "% error for the two in their respective order are:\n" << Merr << "\n" << Eerr << endl;
    return 0;
}