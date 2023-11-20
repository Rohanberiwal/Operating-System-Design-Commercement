#ifndef SIMPLE_MULTITHREADER_H
#define SIMPLE_MULTITHREADER_H

#include <functional>

class SimpleMultithreader {
public:
    SimpleMultithreader();
    SimpleMultithreader(int numThreads); 
    void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads);
    void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads);

private:
    static void *parallel_for_worker(void *arguments);
    static void *parallel_for_2d_worker(void *arguments);
};

#endif  
