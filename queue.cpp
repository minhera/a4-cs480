// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#include "queue.h"
#include "log.h"
#include <pthread.h>
#include <queue>
#include <semaphore.h>

std::queue<RequestType> request_queue;
int count = 0;

unsigned int produced[RequestTypeN] = {0};
unsigned int inRequestQueue[RequestTypeN] = {0};
unsigned int* consumed[ConsumerTypeN];
unsigned int consumed_counts[ConsumerTypeN][RequestTypeN] = {{0}};

pthread_mutex_t queue_lock;
pthread_cond_t not_full, not_empty;

//initialize queue values
void queue_init() {
    pthread_mutex_init(&queue_lock, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
    consumed[TX] = consumed_counts[TX];
    consumed[Rev9] = consumed_counts[Rev9];
}

void queue_add(RequestType type) {
    //declare maximum queue size
    int max_queue_size = 18;

    //if the queue is full, wait till an item is removed before adding item
    while (count == max_queue_size)
        pthread_cond_wait(&not_full, &queue_lock);

    //add an item to the queue, increment the count
    request_queue.push(type);
    count++;

    //also update the number of the request type that is produced and in the request type
    produced[type]++;
    inRequestQueue[type]++;

    //call the output log function that displays the production of the request
    output_request_added(type, produced, inRequestQueue);

    //now that we carried out this function, we can also signal that the request is not empty
    pthread_cond_signal(&not_empty);
}

void queue_remove(ConsumerType who) {
    //if the queue is empty, wait till an item comes along
    while (count == 0)
        pthread_cond_wait(&not_empty, &queue_lock);

    //obtain the request (and its type) from the queue before removing it
    RequestType type = request_queue.front();
    request_queue.pop();

    //decrement the count and update the other values accordingly
    count--;

    //another request has been consumed, but now there is one
    //less request in the queue
    consumed[who][type]++;
    inRequestQueue[type]--;

    //call the output function that displays the removal / consumption of a request
    output_request_removed(who, type, consumed[who], inRequestQueue);

    //now that we carried this function out, we can also signal that the request is not full
    pthread_cond_signal(&not_full);
}
