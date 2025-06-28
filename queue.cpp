// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#include "queue.h"
#include "log.h"
#include <pthread.h>
#include <queue>
#include <semaphore.h>

//declare the request queue
std::queue<RequestType> request_queue;

//declare and initialize the variable we will use to
//store the number of requests in the queue
int count = 0;

//inititialize arrays
unsigned int produced[RequestTypeN] = {0};
unsigned int inRequestQueue[RequestTypeN] = {0};
unsigned int* consumed[ConsumerTypeN];

//initialize a double array to help initialize the above int* array
unsigned int consumed_counts[ConsumerTypeN][RequestTypeN] = {{0}};

//declare pthread mutex and conditions
pthread_mutex_t mutex1;
pthread_cond_t not_full, not_empty;

//initialize pthread mutex and conditions
void queue_init() {
    pthread_mutex_init(&mutex1, nullptr);               //mutex variable we will use to lock the queue
    pthread_cond_init(&not_full, nullptr);              //not full condition
    pthread_cond_init(&not_empty, nullptr);             //not empty condition

    //initialize consumed array
    int i;
    for (i = 0; i < ConsumerTypeN; ++i)
        consumed[i] = consumed_counts[i];
}

void queue_add(RequestType type) {
    //declare maximum queue size
    int max_queue_size = 18;

    //if the queue is full, wait till an item is removed before adding item
    while (count == max_queue_size)
        pthread_cond_wait(&not_full, &mutex1);

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

void queue_remove(ConsumerType robot) {
    //if the queue is empty, wait till an item comes along
    while (count == 0)
        pthread_cond_wait(&not_empty, &mutex1);

    //obtain the request (and its type) from the queue before removing it
    RequestType type = request_queue.front();
    request_queue.pop();

    //decrement the count and update the other values accordingly
    count--;

    //another request has been consumed, but now there is one
    //less request in the queue
    consumed[robot][type]++;
    inRequestQueue[type]--;

    //call the output function that displays the removal / consumption of a request
    output_request_removed(robot, type, consumed[robot], inRequestQueue);

    //now that we carried this function out, we can also signal that the request is not full
    pthread_cond_signal(&not_full);
}
