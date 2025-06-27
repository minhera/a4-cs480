#include "consumer.h"
#include "queue.h"
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads
#include <semaphore.h>

//GET RID OF THIS GLOBAL VARIABLE

extern sem_t barrier_sem;

// Consumer thread for T-X robot
void* consumer_tx(void* arg) {
    int delay = *((int*)arg);
    while (true) {
        pthread_mutex_lock(&queue_lock);
        queue_remove(TX);
        if (produced[GeneralTable] + produced[VIPRoom] == production_limit && inRequestQueue[GeneralTable] + inRequestQueue[VIPRoom] == 0)
            sem_post(&barrier_sem);
        pthread_mutex_unlock(&queue_lock);                            // Remove request from queue
        if (delay > 0) usleep(delay * 1000);                              // Simulate consumption delay

    }
    return NULL;
}

// Consumer thread for Rev-9 robot
void* consumer_rev9(void* arg) {
    int delay = *((int*)arg);
    while (true) {
        pthread_mutex_lock(&queue_lock);
        queue_remove(Rev9);
        if (produced[GeneralTable] + produced[VIPRoom] == production_limit && inRequestQueue[GeneralTable] + inRequestQueue[VIPRoom] == 0)
            sem_post(&barrier_sem);
        pthread_mutex_unlock(&queue_lock);              // Remove request from queue
        if (delay > 0)
            usleep(delay * 1000);                           // Simulate delay

    }
    return NULL;
}
