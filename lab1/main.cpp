#include <omp.h>
#include <iostream>
#include <stdio.h>

int main()
{
    std::cout << "Start of programm\n";

#pragma omp parallel
    {
        printf("thread %d/%d\n", omp_get_thread_num(), omp_get_num_threads());
    }

    std::cout << "End of programm\n";
}