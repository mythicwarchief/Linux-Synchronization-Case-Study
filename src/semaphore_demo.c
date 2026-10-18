
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define NUM_THREADS 5
#define MAX_RESOURCES 2

sem_t resource_sem;
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

void *use_resource(void *arg)
{
    int id = *(int *)arg;

    pthread_mutex_lock(&output_mutex);
    printf("Thread %d is waiting for a resource.\n", id);
    pthread_mutex_unlock(&output_mutex);

    /* Wait until a resource slot becomes available. */
    while (sem_wait(&resource_sem) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            return NULL;
        }
    }

    pthread_mutex_lock(&output_mutex);
    printf("Thread %d acquired a resource.\n", id);
    pthread_mutex_unlock(&output_mutex);

    /* Simulate using the resource for one second. */
    struct timespec delay = {1, 0};
    nanosleep(&delay, NULL);

    pthread_mutex_lock(&output_mutex);
    printf("Thread %d released the resource.\n", id);
    pthread_mutex_unlock(&output_mutex);

    /* Release the resource slot. */
    if (sem_post(&resource_sem) == -1) {
        perror("sem_post");
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    if (sem_init(&resource_sem, 0, MAX_RESOURCES) == -1) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    printf("Counting Semaphore Demonstration\n");
    printf("Threads: %d | Available resources: %d\n\n",
           NUM_THREADS, MAX_RESOURCES);

    int created = 0;

    /* Create the worker threads. */
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;

        int error = pthread_create(
            &threads[i], NULL, use_resource, &ids[i]
        );

        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n",
                    strerror(error));
            break;
        }

        created++;
    }

    /* Wait for every successfully created thread. */
    for (int i = 0; i < created; i++) {
        int error = pthread_join(threads[i], NULL);

        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n",
                    strerror(error));
        }
    }

    if (sem_destroy(&resource_sem) == -1) {
        perror("sem_destroy");
        pthread_mutex_destroy(&output_mutex);
        return EXIT_FAILURE;
    }

    pthread_mutex_destroy(&output_mutex);

    printf("\nAll created threads have finished.\n");

    return EXIT_SUCCESS;
}
