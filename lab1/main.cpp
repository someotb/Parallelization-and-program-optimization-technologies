#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <iomanip>

double f(double x)
{
    return sin(x + 2) / (0.4 + cos(x));
}

double simpson(double a, double b, int n)
{
    double sum = 0.0;
    double step = (b - a) / n;

    sum += f(a);
    sum += f(b);

    for (int i = 1; i < n; i++)
    {
        double x = a + i * step;
        if (i & 1)
            sum += 4 * f(x);
        else
            sum += 2 * f(x);
    }

    return sum * step / 3;
}

double runge(double a, double b, double eps, int &n)
{
    n = 2;
    double prev = simpson(a, b, n);
    while (true)
    {
        n *= 2;
        double curr = simpson(a, b, n);
        if (fabs(curr - prev) / 15 < eps)
            return curr;
        prev = curr;
    }
}

int main()
{
    int n;
    double result = runge(-1, 1, 1e-6, n);
    std::cout << "Result: " << result << " | num of steps: " << n << "\n";
}