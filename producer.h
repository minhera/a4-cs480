#ifndef PRODUCER_H           
#define PRODUCER_H

void* producer_general(void* arg);   // Function to create and insert general table requests
void* producer_vip(void* arg);       // Function to create and insert VIP room requests

#endif
