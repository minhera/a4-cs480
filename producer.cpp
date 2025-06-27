#include "producer.h"
#include "queue.h"
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads
#include <semaphore.h>

//GET RID OF THIS GLOBAL VARIABLE, PUT IT SOMEWHERE ELSE
extern sem_t barrier_sem;

// Producer thread for general table requests
void* producer_general(void* arg) {
    int delay = *((int*)arg);       // Delay in milliseconds
    while (true) {
        pthread_mutex_lock(&queue_lock);                                 // Lock queue for safe access
        if (produced[GeneralTable] + produced[VIPRoom] >= production_limit) {
            pthread_mutex_unlock(&queue_lock);                           // Unlock and exit if limit reached
            break;
        }
        queue_add(GeneralTable);                                          // Add general table request
        pthread_mutex_unlock(&queue_lock);                               // Unlock after adding
        if (delay > 0) usleep(delay * 1000);                              // Simulate production delay
    }
    return nullptr;
}

// Producer thread for VIP room requests
void* producer_vip(void* arg) {
    int delay = *((int*)arg);
    while (true) {
        pthread_mutex_lock(&queue_lock);                                 // Lock queue
        if (produced[GeneralTable] + produced[VIPRoom] >= production_limit) {
            pthread_mutex_unlock(&queue_lock);                           // Exit if production limit reached
            break;
        }
        if (inRequestQueue[VIPRoom] >= 5) {                               // Respect VIP room max constraint
            pthread_mutex_unlock(&queue_lock);                           // Unlock and retry
            usleep(delay * 1000);
            continue;
        }
        queue_add(VIPRoom);                                              // Add VIP request
        pthread_mutex_unlock(&queue_lock);                               // Unlock
        if (delay > 0) usleep(delay * 1000);                              // Simulate delay
    }
    return nullptr;
}
