// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#include "consumer.h"
#include "queue.h"
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads
#include <semaphore.h>

int count = 0;
extern sem_t barrier_sem;

/* General note
 * The delay time is being measured in milliseconds, and usleep is in microseconds
 * So we will multiply by 1000 (10^3) => (10^-6) * (10^3) = 10^-3 (milliseconds)
 */

// Consumer thread for T-X robot
void* consumer_tx(void* arg) {
    //how long the request takes
    int delay = *((int*)arg);       //cast the argument to int* and dereference it to obtain the value
    
    //run while loop indefinitely until there are no more requests left
    while (true) {
        pthread_mutex_lock(&queue_lock);
        queue_remove(TX);
        
        //check if the total requests produced has reached the number of seat requests
        //AND that there are no more requests in the queue
        if (produced[GeneralTable] + produced[VIPRoom] >= total_seat_reqs && inRequestQueue[GeneralTable] + inRequestQueue[VIPRoom] == 0)
            sem_post(&barrier_sem);             //once that happens, we can signal the main thread that we are done
        
        pthread_mutex_unlock(&queue_lock);                              // Remove request from queue

        // Simulate consumption delay in milliseconds
        if (delay > 0)
            usleep(delay * 1000);

    }
    return nullptr;                 //terminate the thread
}

// Consumer thread for Rev-9 robot
void* consumer_rev9(void* arg) {
    int delay = *((int*)arg);
    
    //run while loop indefinitely
    while (true) {
        pthread_mutex_lock(&queue_lock);
        queue_remove(Rev9);
        if (produced[GeneralTable] + produced[VIPRoom] >= total_seat_reqs && inRequestQueue[GeneralTable] + inRequestQueue[VIPRoom] == 0)
            sem_post(&barrier_sem);
        pthread_mutex_unlock(&queue_lock);              // Remove request from queue
        // Simulate consumption delay in milliseconds
        if (delay > 0)
            usleep(delay * 1000);

    }
    return nullptr;             //terminate thread
}
