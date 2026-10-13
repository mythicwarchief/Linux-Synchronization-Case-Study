#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 1000000

static long long counter = 0;
static pthread_spinlock_t spinlock;

void *increment(void *arg)
{
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_spin_lock(&spinlock);
        counter++;
        pthread_spin_unlock(&spinlock);
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    if (pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0)
    {
        fprintf(stderr, "Failed to initialize spinlock\n");
        return 1;
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, increment, NULL) != 0)
        {
            fprintf(stderr, "Failed to create thread\n");
            return 1;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    long long expected = (long long)NUM_THREADS * ITERATIONS;
    printf("Synchronization Mechanism: Spinlock\n");
    printf("Expected counter: %lld\n", expected);
    printf("Actual counter:   %lld\n", counter);
    printf("Result: %s\n",
           counter == expected ? "PASS" : "FAIL");
    pthread_spin_destroy(&spinlock);
    return counter == expected ? 0 : 1;
}
