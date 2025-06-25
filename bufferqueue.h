#ifndef QUEUE_H              
#define QUEUE_H

#include "seating.h"       

pthread_mutex_t mutex1;  
pthread_cond_t not_full;          
pthread_cond_t not_empty; 

void queue_init();                  
void queue_add(RequestType table);   
void queue_remove(ConsumerType machine);
