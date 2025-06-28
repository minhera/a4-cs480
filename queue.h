// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#ifndef QUEUE_H
#define QUEUE_H

#include "seating.h"
#include <pthread.h>

void queue_init();
void queue_add(RequestType type);
void queue_remove(ConsumerType who);

unsigned int produced[];                    //array to keep track of number of each type that has been produced
unsigned int inRequestQueue[];              //array to keep track of number of each type currently in the queue
unsigned int* consumed[];                   //array to keep track of number of each type that has been consumed
unsigned int total_seat_reqs;               //the total number of seat requests
pthread_mutex_t mutex1;                 //mutex variable to lock the queue and keep it mutually exclusive
pthread_cond_t not_full;                    //checks if the queue is no longer full
pthread_cond_t not_empty;                   //checks if the queue is no longer empty

#endif
