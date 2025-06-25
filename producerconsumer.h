#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "seating.h"       
#include "bufferqueue.h"
#include "log.h"

void *producer_general(int time);
void *producer_vip(int time);
void *consumer_tx(int time);
void *consumer_rev9(int time);
