#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <assert.h>

#define NUM_THREADS 8

static void* func(void*);

static atomic_int counter;

int main()
{
    pthread_t threads[NUM_THREADS];
    int result_code = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        result_code = pthread_create(&threads[i], NULL, func, NULL);
        assert(!result_code);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        result_code = pthread_join(threads[i], NULL);
        assert(!result_code);
    }

    return 0;
}

void* func(void* args)
{
    atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);
    int tmp = atomic_load(&counter);
    printf("%d\n", tmp);
}

void setThreadPrioriy(std::thread& t)
{
    sched_param sch;
    sch.sched_priority = 20; 
    if (pthread_setschedparam(t.native_handle(), SCHED_FIFO, &sch)) {
        puts(strerror(errno));
    }
    int policy; 
    pthread_getschedparam(t.native_handle(), &policy, &sch);
    printf("Thread executing at priority %d\n", sch.sched_priority);
}

