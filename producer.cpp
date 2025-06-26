#include "producer.h"     
#include "queue.h"           
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads

void* producer_general(void* time){
  int general_time = *(int*)time; 
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (general_time > 0){
    usleep(general_time);
  }
  return NULL;
}

void *consumer_rev9(void* time){
  int rev9_time = *(int*)time; 
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (rev9_time > 0){
    usleep(rev9_time);
  }
  return NULL;
}
