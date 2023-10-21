#include <math.h>
#include <stdio.h>

#include <iomanip>
#include <iostream>

using namespace std;
// Define the function to be integrated (sin(x) in this case)
double f(double x) { return sin(x); }

// Calculate the integral using the left endpoint Riemann sum
double LeftRiemann(double left_endpt, double right_endpt, int rect_count, double base_len) {
    double left_value, x, area = 0.0;

    // Estimate the function value at the left endpoint
    x = left_endpt;
    left_value = f(x);

    // Calculate the Riemann sum by summing the areas of the rectangles
    for (int i = 0; i < rect_count; i++) {
        area += left_value * base_len;
        x += base_len;
        left_value = f(x);
    }

    return area;
}

int main() {
    double left_endpt = 0.0;      // Left endpoint of the interval
    double right_endpt = 1800;    // Right endpoint of the interval (π)
    int rect_count = 1000000000;  // Number of rectangles for Riemann sum
    double base_len = (right_endpt - left_endpt) / rect_count;

    // Calculate the integral using the Left Riemann sum
    double integral = LeftRiemann(left_endpt, right_endpt, rect_count, base_len);

    cout << fixed << setprecision(14) << "The approximate integral of sin(x) from 0 to π is: " << integral << endl;

    return 0;
}
