// Originally generated by ChatGPT 4.0 and refactored by Sam Siewert
//
// Match the style of hello_riemann and hello_simpson
//
#include <iostream>
#include <cmath>

#define RANGE (M_PI)
#define STEPS (1000000)

using namespace std;

double function_to_integrate(double x);

using namespace std;

double trapezoidal_rule(double a, double b, int n) 
{
    double h = (b - a) / n;
    double sum = (function_to_integrate(a) + function_to_integrate(b)) / 2.0;

    for (int i = 1; i < n; i++) 
    {
        double x = a + i * h;
        sum += function_to_integrate(x);
    }

    return h * sum;
}

int main() 
{
    const double a = 0.0;
    const double b = RANGE;
    const int n = STEPS;

    const double result = trapezoidal_rule(a, b, n);

    cout.precision(15);
    cout << "The integral of f(x) from 0.0 to " << b << " with " << n << " steps is " << result << endl;

    return 0;
}

double function_to_integrate(double x)
{
    return (sin(x));
}
