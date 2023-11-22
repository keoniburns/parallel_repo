#include <omp.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
using namespace std;

double f(double x) { return cos(x * x); }

int main() {
    unsigned long long itr, maxitr, total_itr = 0;
    unsigned int rootfound = 0;
    double max_error = 0.0, fstart = 0.0, fstop = 0.0;
    double step = 2.0 * numeric_limits<double>::epsilon(), x0, x1, xfinal, sign_changed = 0, start;
    timespec start_t, stop_t;

    cout << "smallest step is 2x epsilon is " << (double)(2.0 * numeric_limits<double>::epsilon()) << endl;

    cout << "Enter x0 start, xfinal end, step size and maximum iterations to search for each root on sub-interval of "
            "1.0 "
         << endl;
    cin >> x0 >> xfinal >> step >> maxitr;
    start = x0;

    cout << "Will step at " << step << " from " << x0 << " to " << xfinal << " to find each root, with "
         << (((unsigned)(xfinal - x0) + 1) * maxitr) << " total search steps" << endl;
    clock_gettime(CLOCK_MONOTONIC, &start_t);
    fstart = (double)start_t.tv_sec + (double)start_t.tv_nsec / 1000000000.0;

// do {
//     for (itr = 1; itr <= maxitr; itr++) {
//         x1 = x0 + step;

//         // Any difference in sign between x0 and x1 will result in a sign_changed less than zero - a crossing
//         sign_changed = f(x0) * f(x1);

//         if (sign_changed < 0.0) {
//             // cout << "\n*** Sign change at " << total_itr + itr << " iterations: estimated root at "
//             //      << (x1 + x0) / 2.0 << ", f[root]=" << f((x1 + x0) / 2.0) << endl;

//             if (fabs(f((x1 + x0) / 2.0)) > max_error) max_error = fabs(f((x1 + x0) / 2.0));

//             rootfound++;

//             // We can exit on the first root (zero crossing found) or keep looking
//             break;
//         }

//         x0 = x1;  // advance the step interval
//     }

//     // Re-start search for the next root one step beyond the last one found
//     total_itr += itr;

//     x0 = x1;
//     itr = 1;

// } while ((x0 < xfinal) &&
//          (itr < maxitr));  // exit the outer loop search for all roots if the end is reached or max iterations
#pragma omp parallel reduction(+ : rootfound, total_itr) private(itr, x1, sign_changed)
    {
        do  // while there are potentially more roots on the search interval
        {
            int terminate = 0;

#pragma omp for
            for (itr = 1; itr <= maxitr; itr++)  // find the next root on the sub-interval
            {
                if (terminate) continue;  // Skip iterations if termination signal is set

                x1 = x0 + step;

                // Any difference in sign between x0 and x1 will result in a sign_changed less than zero - a crossing
                sign_changed = f(x0) * f(x1);

                if (sign_changed < 0.0) {
#pragma omp critical
                    {
                        // cout << "\n*** Sign change at " << total_itr + itr << " iterations: estimated root at "
                        //      << (x1 + x0) / 2.0 << ", f[root]=" << f((x1 + x0) / 2.0) << endl;

                        if (fabs(f((x1 + x0) / 2.0)) > max_error) max_error = fabs(f((x1 + x0) / 2.0));

                        rootfound++;

                        // Set termination signal
                        terminate = 1;
                    }
                }

                x0 = x1;  // advance the step interval
            }

// Re-start search for the next root one step beyond the last one found
#pragma omp single
            {
                total_itr += itr;
                x0 = x1;
                itr = 1;
            }

        } while ((x0 < xfinal) &&
                 (itr < maxitr));  // exit the outer loop search for all roots if the end is reached or max iterations
    }

    clock_gettime(CLOCK_MONOTONIC, &stop_t);
    fstop = (double)stop_t.tv_sec + (double)stop_t.tv_nsec / 1000000000.0;
    cout << "\n\nSearch Results (completed in " << (fstop - fstart) * 1000.0 << " milliseconds):" << endl;

    if (!rootfound) {
        cout << "After " << itr << " iterations: No solution (zero crossing) found on interval " << start << " to "
             << x0 << endl;
    } else {
        cout << rootfound << " roots found on interval " << start << " to " << x0 << ", with max error=" << max_error
             << " in " << total_itr << " iterations" << endl;
    }

    return 0;
}
