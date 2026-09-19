// g++ -O2 -fopenmp simpson.cpp -o simpson
#include <cmath>
#include <cstdio>
#include <omp.h>

double f(double x) { return std::sin(x + 2.0) / (0.4 + std::cos(x)); }

// Составная формула Симпсона на n (чётном) равных отрезках
template <class F>
double simpson(F func, double a, double b, long n) {
    const double h = (b - a) / n;
    const long m = n / 2;
    double odd = 0.0, even = 0.0;

    #pragma omp parallel for reduction(+ : odd) schedule(static)
    for (long i = 1; i <= m; ++i)
        odd += func(a + (2 * i - 1) * h);

    #pragma omp parallel for reduction(+ : even) schedule(static)
    for (long i = 1; i < m; ++i)
        even += func(a + 2 * i * h);

    return h / 3.0 * (func(a) + func(b) + 4.0 * odd + 2.0 * even);
}

// Правило Рунге: |S_2n - S_n| / 15 < eps (метод Симпсона имеет 4-й порядок)
template <class F>
double simpson_runge(F func, double a, double b, double eps, long &n_out) {
    long n = 2;
    double prev = simpson(func, a, b, n);
    while (true) {
        n *= 2;
        double cur = simpson(func, a, b, n);
        if (std::fabs(cur - prev) / 15.0 < eps) {
            n_out = n;
            return cur;
        }
        prev = cur;
    }
}

int main() {
    const double a = -1.0, b = 1.0, eps = 1e-6;

    long n_eps;
    double I = simpson_runge(f, a, b, eps, n_eps);
    std::printf("I = %.10f, n = %ld (eps = %g)\n", I, n_eps, eps);

    // Для анализа масштабируемости: n нужно взять большим,
    // иначе задача считается за микросекунды и накладные расходы
    // на создание нитей скроют любое ускорение.
    const long N = 400'000'000;
    const int threads[] = {1, 2, 4, 16, 32, 64};
    double t1 = 0.0;

    std::printf("threads,time_s,speedup,efficiency\n");
    for (int p : threads) {
        omp_set_num_threads(p);
        simpson(f, a, b, N);                 // прогрев
        double best = 1e30;
        for (int r = 0; r < 3; ++r) {        // лучшее из 3 запусков
            double t0 = omp_get_wtime();
            volatile double res = simpson(f, a, b, N);
            (void)res;
            best = std::fmin(best, omp_get_wtime() - t0);
        }
        if (p == 1) t1 = best;
        std::printf("%d,%.4f,%.2f,%.2f\n", p, best, t1 / best, t1 / best / p);
    }
}
