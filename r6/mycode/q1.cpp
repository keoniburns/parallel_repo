/**
 * @file q1.cpp
 * @author keoni burns
 * @brief caluculates the polynomial root function -x^3+9.7x^2-1.3x-105.7
 * @version 0.1
 * @date 2023-11-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <float.h>
#include <math.h>
#include <time.h>

#include <iomanip>
#include <iostream>

double func(double x);
double derv(double x);
double NewtRaph(double x0, double err, double n);
double regula(int n, double x0, double x1, double x2, double x3, double err);
void regHelper(double *x, double x0, double x1, double fx0, double fx1, int *itr);

using namespace std;

int main(int argc, char *argv[]) {
    struct timespec start, stop;
    double fstart, fstop;
    int n;
    double value, x0, x1, x2, x3, totErr;
    int alg;
    cout << "enter x0, x1(if not applicable use -1), allowed error, and max iterations on individual lines" << endl;
    cin >> x0;
    cin >> x1;
    cin >> totErr;
    cin >> n;

    cout << "which method\n "
         << "1 - newton & 2 - regula" << endl;
    cin >> alg;
    if (alg == 1) {
        cout << "running newton" << endl;
        clock_gettime(CLOCK_MONOTONIC, &start);
        fstart = (double)start.tv_sec + ((double)start.tv_nsec / 1000000000.0);
        value = NewtRaph(x0, totErr, n);
        clock_gettime(CLOCK_MONOTONIC, &stop);
        fstop = (double)stop.tv_sec + ((double)stop.tv_nsec / 1000000000.0);
    }
    if (alg == 2) {
        cout << "running regula" << endl;
        clock_gettime(CLOCK_MONOTONIC, &start);
        fstart = (double)start.tv_sec + ((double)start.tv_nsec / 1000000000.0);
        value = regula(n, x0, x1, x2, x3, totErr);
        clock_gettime(CLOCK_MONOTONIC, &stop);
        fstop = (double)stop.tv_sec + ((double)stop.tv_nsec / 1000000000.0);
    }
    cout << setprecision(15) << "estimated root is " << value << endl;
    cout << setprecision(15) << "our verification on how zero it really is: " << func(value) << endl;
    cout << "monotonic time: " << (fstop - fstart) << endl;
    return 0;
}

/* -x^3+9.7x^2-1.3x-105.7 func */
double func(double x) { return (-pow(x, 3) + (9.7 * pow(x, 2)) - (1.3 * x) - 105.7); }

/* -3x^2+19.4x-1.3 derivative */
double derv(double x) { return ((-3 * pow(x, 2)) + (19.4 * x) - 1.3); }

double NewtRaph(double x0, double err, double n) {
    double root = 0, x1 = 0, h;
    if (fabs(derv(x0)) < err) {
        cout << "slope at " << x0 << " is " << derv(x0) << "\nit needs to be adjusted" << endl;

        x0 += err;
        for (int i = 0; i < n; i++) {
            if (fabs(derv(x0)) < err) {
                x0 += err;
            } else {
                break;
            }
        }

        cout << "new initial guess is " << x0 << endl;
    }

    if (fabs(derv(x0)) < err) {
        cout << "this aint the guess chief" << endl;
    }

    for (int i = 1; i <= n; i++) {
        h = func(x0) / derv(x0);
        x1 = x0 - h;
        if (fabs(func(x1)) < err) {
            printf("After %d iterations, root = %20.15lf\n", i, x1);
            return x1;
        }
        x0 = x1;
    }
    root = x0;
    return root;
}

double regula(int n, double x0, double x1, double x2, double x3, double err) {
    int itr = 0;
    regHelper(&x2, x0, x1, func(x0), func(x1), &itr);
    while (itr < n) {
        if (func(x0) * func(x2) < 0) {
            x1 = x2;
        } else {
            x0 = x2;
        }
        regHelper(&x3, x0, x1, func(x0), func(x1), &itr);
        if (fabs(x3 - x2) < err) {
            printf("After %d iterations, root = %20.15lf\n", itr, x3);
            return x3;
        }
        x2 = x3;
    }
    return 1;
}

void regHelper(double *x, double x0, double x1, double fx0, double fx1, int *itr) {
    *x = x0 - ((x1 - x0) / (fx1 - fx0)) * fx0;
    ++(*itr);
}