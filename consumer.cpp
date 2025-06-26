#include "consumer.h"     
#include "queue.h"           
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads

void *consumer_tx(void* time){
  int tx_time = *(int*)time; 
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(tx_time);
  }
  return NULL;
}

void *consumer_rev9(void* time){
  int rev9_time = *(int*)time; 
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(rev9_time);
  }
  return NULL;
}
