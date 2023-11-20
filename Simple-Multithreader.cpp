// \simple_multithreader.cpp

#include "simple_multithreader.h"
#include <chrono>
#include <iostream>

struct ParallelForArgs {
    int low;
    int high;
    std::function<void(int)> lambda;
};

struct ParallelFor2DArgs {
    int low1;
    int high1;
    int low2;
    int high2;
    std::function<void(int, int)> lambda;
};

SimpleMultithreader::SimpleMultithreader() {
    std::cout << "SimpleMultithreader default constructor called.\n";
}

SimpleMultithreader::SimpleMultithreader(int numThreads) {
    std::cout << "SimpleMultithreader parameterized constructor called with numThreads = " << numThreads << "\n";
}

void SimpleMultithreader::parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_t threads[numThreads];
    ParallelForArgs args[numThreads];

    int range = (high - low + 1) / numThreads;
    int remainder = (high - low + 1) % numThreads;
    int current_low = low;

    for (int i = 0; i < numThreads; ++i) {
        args[i].low = current_low;
        args[i].high = current_low + range - 1 + (i < remainder ? 1 : 0);
        args[i].lambda = lambda;

        pthread_create(&threads[i], nullptr, parallel_for_worker, &args[i]);

        current_low = args[i].high + 1;
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Execution time: taken for 1d " << duration.count() << " microseconds\n";
}

void SimpleMultithreader::parallel_for(int low1, int high1, int low2, int high2,
                                       std::function<void(int, int)> &&lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_t threads[numThreads];
    ParallelFor2DArgs args[numThreads];

    int range1 = (high1 - low1 + 1) / numThreads;
    int remainder1 = (high1 - low1 + 1) % numThreads;
    int current_low1 = low1;

    for (int i = 0; i < numThreads; ++i) {
        args[i].low1 = current_low1;
        args[i].high1 = current_low1 + range1 - 1 + (i < remainder1 ? 1 : 0);
        args[i].low2 = low2;
        args[i].high2 = high2;
        args[i].lambda = lambda;

        pthread_create(&threads[i], nullptr, parallel_for_2d_worker, &args[i]);

        current_low1 = args[i].high1 + 1;
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Execution time: for 2d " << duration.count() << " microseconds\n";
}

void *SimpleMultithreader::parallel_for_worker(void *arguments) {
    ParallelForArgs *args = static_cast<ParallelForArgs *>(arguments);

    for (int i = args->low; i <= args->high; ++i) {
        args->lambda(i);
    }

    pthread_exit(nullptr);
}

void *SimpleMultithreader::parallel_for_2d_worker(void *arguments) {
    ParallelFor2DArgs *args = static_cast<ParallelFor2DArgs *>(arguments);

    for (int i = args->low1; i <= args->high1; ++i) {
        for (int j = args->low2; j <= args->high2; ++j) {
            args->lambda(i, j);
        }
    }

    pthread_exit(nullptr);
}
