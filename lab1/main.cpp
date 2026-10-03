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

#pragma omp parallel for reduction(+ : sum)
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
    int threads[6] = {1, 2, 4, 16, 32, 64};
    int n = 2 * 10e7;

    std::cout << std::setprecision(10);

    for (auto thread : threads)
    {
        omp_set_num_threads(thread);
        double t0 = omp_get_wtime();
        double result = simpson(-1, 1, n);
        double t1 = omp_get_wtime();
        std::cout << thread << " threads: " << t1 - t0 << "s, answer " << result << "\n";
    }
}