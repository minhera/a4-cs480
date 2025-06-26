#include "consumer.h"     
#include "queue.h"           
#include <unistd.h>                 // For usleep
#include <pthread.h>                // POSIX threads

void *consumer_tx(int time){
  int delay = time;
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}

void *consumer_rev9(int time){
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}
