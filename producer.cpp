// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#include "producer.h"
#include "queue.h"
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads
#include <semaphore.h>

extern sem_t barrier_sem;

/* General note
 * The delay time is being measured in milliseconds, and usleep is in microseconds
 * So we will multiply by 1000 (10^3) => (10^-6) * (10^3) = 10^-3 (milliseconds)
 */

// Producer thread for general table requests
void* producer_general(void* arg) {
    //how long the request takes
    int delay = *((int*)arg);       //cast the argument to int* and dereference it to obtain the value

    //run while loop indefinitely until a break statement can be reached
    while (true) {
        pthread_mutex_lock(&mutex1);                                 // Lock queue for safe access

        //check if the total requests produced has reached the number of seat requests
        if (produced[GeneralTable] + produced[VIPRoom] >= total_seating_reqs) {
            pthread_mutex_unlock(&qmutex1);                           // Unlock and exit if limit reached
            break;
        }
        queue_add(GeneralTable);                                         // Add general table request
        pthread_mutex_unlock(&mutex1);                               // Unlock after adding
        // Simulate production delay in milliseconds
        if (delay > 0)
            usleep(delay * 1000);
    }
    return nullptr;                                     //once we get here, the thread is done
}

// Producer thread for VIP room requests
void* producer_vip(void* arg) {
    int delay = *((int*)arg);       //cast the argument to int* and dereference it to obtain the value
    unsigned int max_vip_size = 5;           //threshold value for the maximum number of VIP requests in queue

    //run while loop indefinitely until a break statement can be reached
    while (true) {
        pthread_mutex_lock(&mutex1);                                 // Lock queue for safe access

        //check if the total requests produced has reached the number of seat requests
        if (produced[GeneralTable] + produced[VIPRoom] >= total_seating_reqs) {
            // if so, unlock mutex and then exit (hence the break statement)
            pthread_mutex_unlock(&mutex1);
            break;
        }
        // Respect VIP room max constraint
        if (inRequestQueue[VIPRoom] >= max_vip_size) {
            pthread_mutex_unlock(&queue_lock);                          // Unlock and retry
            usleep(delay * 1000);                                       // delay in milliseconds instead of microseconds
            continue;
        }
        queue_add(VIPRoom);                                              // Add VIP request
        pthread_mutex_unlock(&mutex1);                               // Unlock queue
        // Simulate production delay in milliseconds
        if (delay > 0)
            usleep(delay * 1000);
    }
    return nullptr;
}
