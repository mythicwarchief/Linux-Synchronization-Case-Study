#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define ITERATIONS 500000

typedef enum
{
    USE_MUTEX,
    USE_SPINLOCK
} LockType;

static long long counter = 0;
static pthread_mutex_t mutex;
static pthread_spinlock_t spinlock;
static LockType lock_type;

static double elapsed_seconds(struct timespec start,
                              struct timespec end)
{
    return (end.tv_sec - start.tv_sec)
         + (end.tv_nsec - start.tv_nsec) / 1e9;
}
static void *worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++)
    {
        if (lock_type == USE_MUTEX)
        {
            pthread_mutex_lock(&mutex);
            counter++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_spin_lock(&spinlock);
            counter++;
            pthread_spin_unlock(&spinlock);
        }
    }
    return NULL;
}
static int run_benchmark(LockType type, FILE *csv)
{
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;
    lock_type = type;
    counter = 0;
    const char *name =
        type == USE_MUTEX ? "Mutex" : "Spinlock";
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
    {
        perror("clock_gettime");
        return 1;
    }
    int created = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int rc = pthread_create(
            &threads[i], NULL, worker, NULL);
        if (rc != 0)
        {
            fprintf(stderr, "Failed to create thread\n");
            for (int j = 0; j < created; j++)
            {
                pthread_join(threads[j], NULL);
            }
            return 1;
        }
        created++;
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
    {
        perror("clock_gettime");
        return 1;
    }
    long long expected =
        (long long)NUM_THREADS * ITERATIONS;
    double seconds = elapsed_seconds(start, end);
    printf("%-10s | Expected: %lld | Actual: %lld"
           " | Time: %.6f seconds | %s\n",
           name, expected, counter, seconds,
           counter == expected ? "PASS" : "FAIL");
    fprintf(csv, "%s,%d,%d,%lld,%lld,%.9f\n",
            name, NUM_THREADS, ITERATIONS,
            expected, counter, seconds);
    return counter == expected ? 0 : 1;
}
int main(void)
{
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Failed to initialize mutex\n");
        return 1;
    }
    if (pthread_spin_init(&spinlock,
                          PTHREAD_PROCESS_PRIVATE) != 0)
    {
        fprintf(stderr, "Failed to initialize spinlock\n");
        pthread_mutex_destroy(&mutex);
        return 1;
    }
    FILE *csv = fopen("results/benchmark_results.csv", "w");
    if (csv == NULL)
    {
        perror("Cannot open results/benchmark_results.csv");
        pthread_mutex_destroy(&mutex);
        pthread_spin_destroy(&spinlock);
        return 1;
    }
    fprintf(csv,
            "mechanism,threads,iterations_per_thread,"
            "expected,actual,time_seconds\n");
    printf("Synchronization Benchmark\n");
    printf("Threads: %d | Iterations per thread: %d\n\n",
           NUM_THREADS, ITERATIONS);
    int mutex_result = run_benchmark(USE_MUTEX, csv);
    int spin_result = run_benchmark(USE_SPINLOCK, csv);
    fclose(csv);
    pthread_mutex_destroy(&mutex);
    pthread_spin_destroy(&spinlock);
    printf("\nResults saved to results/benchmark_results.csv\n");
    return (mutex_result || spin_result) ? 1 : 0;
}
