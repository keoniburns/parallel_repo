#include <iomanip>
#include <iostream>
#include <vector>

#include "ex6lin.h"

using namespace std;
double table_accel(int timeidx);

double faccel(double time);

// This is the number of interpolation steps (data to generate) between table data values
const int STEPS_PER_SEC = 10;

int main(int argc, char *argv[]) {
    int steps_per_sec = STEPS_PER_SEC;
    double time, dt;

    cout << "argc=" << argc << ", argv[0]=" << argv[0] << endl;

    if (argc == 2) {
        steps_per_sec = atoi(argv[1]);
        cout << "Will use " << steps_per_sec << " steps per sec" << endl;
    } else {
        cout << "Use: trainprofile [steps_per_sec]" << endl;
        cout << "Will use default time profile" << endl;
    }

    dt = 1.0 / steps_per_sec;

    cout << "Step size of " << dt << endl;

    // Verify the static initializer or loaded CSV array for size and values
    //
    // And use it to generate values between 1-second given values.
    //
    vector<double> DefaultProfile;  // Define this vector with your actual data

    cout << "Number of values in profile = " << DefaultProfile.size() << " for 1801 expected" << endl;

    for (int idx = 0; idx <= (STEPS_PER_SEC * 1800); idx++) {
        // time you would use in your integrator and faccel(time) is the function to integrate
        time = 0.0 + (dt * (double)idx);
        cout << fixed << setprecision(14) << time << ", " << faccel(time) << endl;
    }

    return 0;
}

// Simple look-up in acceleration profile array
//
// Added array bounds check for the known size of train arrays
double table_accel(int timeidx) {
    // vector<double> DefaultProfile;  // Define this vector with your actual data
    long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

    // Check array bounds for look-up table
    if (timeidx > tsize) {
        printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize - 1);
        exit(-1);
    }

    return DefaultProfile[timeidx];
}

// Simple linear interpolation example for table_accel(t) for any floating point t value
// for a table of accelerations that are 1 second apart in time, evenly spaced in time.
//
// accel[timeidx] <= accel[time] < accel[timeidx_next]
double faccel(double time) {
    // The timeidx is an index into the known acceleration profile at a time <= time of interest passed in
    //
    // Note that the conversion to an integer truncates the double to the next lowest integer value or floor(time)
    int timeidx = (int)time;

    // The timeidx_next is an index into the known acceleration profile at a time > time of interest passed in
    //
    // Note that the conversion to an integer truncates the double and the +1 is added for ceiling(time)
    int timeidx_next = ((int)time) + 1;

    // delta_t = time of interest - time at a known value < time
    //
    // For a more general case:
    // double delta_t = (time - static_cast<dousble>(timeidx)) / static_cast<double>(timeidx_next - timeidx);
    //
    // If time in the table is always 1 second apart, then we can simplify since (timeidx_next - timeidx) = 1.0 by
    // definition here
    double delta_t = time - (double)(int)time;

    return (
        // The accel[time] is a linear value between accel[timeidx] and accel[timeidx_next]
        //
        // The accel[time] is a value that can be determined by the slope of the interval and accel[timeidx]
        //
        // I.e. accel[time] = accel[timeidx] + ( (accel[timeidx_next] - accel[timeidx]) /
        // static_cast<double>(timeidx_next - timeidx) ) * delta_t
        //
        // ((double)(timeidx_next - timeidx)) = 1.0
        //
        // accel[time] = accel[timeidx] + (accel[timeidx_next] - accel[timeidx]) * delta_t
        //
        table_accel(timeidx) + ((table_accel(timeidx_next) - table_accel(timeidx)) * delta_t));
}
