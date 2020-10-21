#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdalign.h>
#include <assert.h>
#include <memory.h>

#define NUM_THREADS 2
#define BUF_SIZE 0x200

static int buffer[BUF_SIZE];
alignas (64) static atomic_intptr_t ptr_shrd;
alignas (64) static atomic_int stop;

void* generate(void*);
void* accumulate(void*);

int main()
{
    int result_code = 0;
    char ch;
    time_t t;
    pthread_t threads[NUM_THREADS];
    atomic_store(&ptr_shrd, (intptr_t)buffer);
    atomic_store(&stop, 1);
    srand((unsigned int)time(&t)); 
    int *ptr_gen = calloc(sizeof(int), BUF_SIZE);
    int *ptr_accum = calloc(sizeof(int), BUF_SIZE);

    result_code = pthread_create(&threads[0], NULL, generate, (void*)ptr_gen);
    assert(!result_code);
    result_code = pthread_create(&threads[1], NULL, accumulate, (void*)ptr_accum);
    assert(!result_code);
    puts("Press any key to quit.");
    ch = getchar();
    atomic_store_explicit(&stop, 0, memory_order_release);
    result_code = pthread_join(threads[0], NULL);
    assert(!result_code);
    result_code = pthread_join(threads[1], NULL);
    assert(!result_code);
    free(ptr_gen);
    free(ptr_accum);

    return 0;
}

void* generate(void* ptr_gen)
{
    int *ptr = (int*)ptr_gen;
    while (atomic_load_explicit(&stop, memory_order_acquire)) { 
        for (size_t i = 0; i < BUF_SIZE; i++) {
            ptr[i] = (int)(rand() & 0xffff);
        }
        ptr = (int*)atomic_exchange(&ptr_shrd, (intptr_t)ptr);
    }
}

void* accumulate(void* ptr_accum)
{
    int *ptr = (int*)ptr_accum;
    static const size_t incr = BUF_SIZE >> 4; 
    while (atomic_load_explicit(&stop, memory_order_acquire)) { 
        ptr = (int*)atomic_exchange(&ptr_shrd, (intptr_t)ptr);
        long int accum = 0lu;
        for (size_t i; i < BUF_SIZE; i += incr) {
            accum += (long int)(ptr[i] & 0xff); 
        }
        //if (accum & 0x1)
        //    printf("%ld\t", accum);
    }
}
