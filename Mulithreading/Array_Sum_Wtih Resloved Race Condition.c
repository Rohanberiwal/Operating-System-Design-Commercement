#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int A[SIZE], result=0;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
void array_sum(int low, int high) {
int sum = 0;
for (int i=low; i<high; i++) {
sum += A[i];
}
pthread_mutex_lock(&m);
result += sum;
pthread_mutex_unlock(&m);
}
typedef struct {
int low;
int high;
} thread_args;
void *thread_func(void *ptr) {
thread_args * t = ((thread_args *) ptr);
array_sum(t->low, t->high);
return NULL;
}

int main(int argc, char *argv[]) {
if (SIZE < 1024) {
array_sum(0, SIZE);
} else {
pthread_t tid[NTHREADS];
thread_args args[NTHREADS];
int chunk = SIZE/NTHREADS;
for (int i=0; i<NTHREADS; i++) {
args[i].low=i*chunk; args[i].high=(i+1)*chunk;
pthread_create(&tid[i],
NULL,

thread_func,
(void*) &args[i]);

}
for (int i=0; i<NTHREADS; i++) {
pthread_join(tid[i]);
}
}
printf(”Total Sum is %d\n", result);
return 0;
