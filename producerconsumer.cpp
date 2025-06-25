#include "producerconsumer.h"

void producer_general(int time){
  pthread_mutex_lock( &mutex1 );
  queue_add(general);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}

void producer_vip(int time){
  pthread_mutex_lock( &mutex1 );
  queue_add(vip);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}

void consumer_tx(int time){
  int delay = time;
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}

void consumer_rev9(int time){
  pthread_mutex_lock( &mutex1 );
  queue_remove(item);
  pthread_mutex_unlock( &mutex1 );

  if (time > 0){
    usleep(time);
  }
  return NULL;
}
