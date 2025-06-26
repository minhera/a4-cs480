#include "bufferqueue.h"
#include "log.h"
#include <pthread.h>

int maximum_size = 18;
RequestType queue_size[maximum_size];

pthread_mutex_t mutex1;
pthread_cond_t  not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;

void queue_create() {
    pthread_mutex_init(&mutex1, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);

}

void queue_add(RequestType table) {
    while (queue_count == queue_max){
        pthread_cond_wait(&not_full, &queue_lock);
    }    

  queue_count++;
}

void queue_remove(ConsumerType machine) {
    while (queue_count == 0){
        pthread_cond_wait(&not_empty, &queue_lock); 
    }

  queue_count--;
}
