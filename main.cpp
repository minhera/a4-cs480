// Names: Minh Tran & Ryan Alakija
// Red ID: 129986763 && 826620776

#include <stdio.h>
#include <iostream>
#include <unistd.h>         //for getopt function and access to POSIX OS API
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "seating.h"
#include "producer.h"
#include "consumer.h"
#include "log.h"

unsigned int total_seating_reqs = 120; 				//total number of seating requests that will take place in the simulation

#define EXIT_NORMAL 0       //normal exit flag
#define EXIT_ERROR 1        //error exit flag

sem_t barrier_sem;  // Used to signal main thread when done

int main(int argc, char* argv[]) {

    /*** COMMAND LINE USER INTERFACE + INITIALIZATION ***/

    //declare & initialize variables w/ default values

    //consumers
    unsigned int tx_time = 0;                       //average time for T-X robot to process seating request + guide customers to proper seating
    unsigned int rev9_time = 0;                     //average time for Rev-9 robot (same as T-X robot)

    //producers
    unsigned int general_time = 0;                  //average time for general table greeter robot to produce and insert general table request
    unsigned int vip_time = 0;                      //average time for VIP room greeter robot to produce and insert VIP room request

    //declare other variables
    int option;                 //an int variable that obtains the return value of getopt() function
    int temp_int;               //a temporary int variable to store the converted int value of optarg

    /* process optional arguments; each of them take numerical arguments
     * note: getopt() uses unistd.h library, which also includes getopt.h
     * output variables:
     *    optarg - the argument passed with the command-line switch/flag
     *    optind - the index that represents the first argument in argv to process AFTER
     *             all the optional arguments are processed; we will not need this here
     */

    //if getopt() returns -1, there are no command-line flag arguments given, thus we skip
    while ((option = getopt(argc, argv, "s:x:r:g:v:")) != -1) {
        //error handling for any non-int arguments;
        //explicitly not required by the instructions, but implemented for good practice
        try {
            temp_int = std::stoi(optarg);

            //check if argument is 0 or greater (although 0 would be redundant for most flags besides -s)
            if (temp_int < 0) {
                //we could throw an exception as a part of this try-catch block,
                //but it's easier to just print an error statement here and exit
                fprintf(stderr, "Please enter a non-negative number\n");
                exit(EXIT_ERROR);
            }
        }
        catch (...) {
            fprintf(stderr, "Each flag argument must be a non-negative number\n");
            exit(EXIT_ERROR);
        }

        //now that we passed the error-detecting try-catch block, we will process these optional arguments
        switch (option) {
            case 's':
                total_seating_reqs = temp_int;
                break;
            case 'x':
                tx_time = temp_int;
                break;
            case 'r':
                rev9_time = temp_int;
                break;
            case 'g':
                general_time = temp_int;
                break;
            case 'v':
                vip_time = temp_int;
                break;
            default:
                //for if the user enters a flag with no argument after it
                fprintf(stderr, "usage: %s [ -s | -x | -r | -g | -v ]\n", argv[0]);
                exit(EXIT_ERROR);
        }
    }

    pthread_t gen_table_robot;                      //thread for general table greeting robot
    pthread_t vip_room_robot;                       //thread for VIP room robot
    pthread_t tx_robot;                             //thread for T-X robot
    pthread_t rev9_robot;                           //thread for Rev-9 robot

    //initialize queue and semaphore
    queue_init();
    sem_init(&barrier_sem, 0, 0);

    //create pthreads and pass the time variables to each respective functions
    pthread_create(&gen_table_robot, nullptr, producer_general, &general_time);
    pthread_create(&vip_room_robot, nullptr, producer_vip, &vip_time);
    pthread_create(&tx_robot, nullptr, consumer_tx, &tx_time);
    pthread_create(&rev9_robot, nullptr, consumer_rev9, &rev9_time);

    //wait for both production threads to terminate
    pthread_join(gen_table_robot, nullptr);
    pthread_join(vip_room_robot, nullptr);

    //block main from exiting until sem_post() is called
    //this is what terminates the consumer threads
    sem_wait(&barrier_sem);

    //call log.c function to display the history of number of requests both produced and consumed
    output_production_history(produced, consumed);
    return 0;
}
