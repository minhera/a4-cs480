#include "producer.h"     
#include "queue.h"           
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads

void* producer_general(void* time){
  int general_time = *(int*)time; 
  while (true){ // Makes sure this thread run indefinitely
    pthread_mutex_lock( &mutex1 );
    queue_add(item);
    pthread_mutex_unlock( &mutex1 );
  
    usleep(general_time);
  }
  return nullptr;
}

void *producer_vip(void* time){
  int vip_time = *(int*)time; 
  while (true){ // Makes sure this thread run indefinitely
    pthread_mutex_lock( &mutex1 );
    queue_add(item);
    pthread_mutex_unlock( &mutex1 );
  
  
    usleep(vip_time);
  }
  return nullptr;
}
