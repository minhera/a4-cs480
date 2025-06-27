// Names: Minh Tran & Ryan Alakija
// Red ID: <Minh Red ID> && 826620776
//
// Created by User on 6/23/2025.
//
#include <stdio.h>
#include <iostream>
#include <unistd.h>         //for getopt function and access to POSIX OS API
#include <semaphore.h>
#include "bufferqueue.h"
#include "producer.h"
#include "consumer.h"
#include "seating.h"
#include "log.h"

//MAYBE CONSIDER MOVING THIS TO HEADER FILE OF CLASS (once we create it)
#define EXIT_NORMAL 0       //normal exit flag
#define EXIT_ERROR 1        //error exit flag

//some other functions to add (maybe)
sem_t main_barrier; // Declare semaphore, will use to signal main thread that the rest is done

int main(int argc, char* argv[]) {

    /*** COMMAND LINE USER INTERFACE + INITIALIZATION ***/
    
    //declare & initialize variables w/ default values
    unsigned int total_seat_reqs = 120;             //total number of seating requests that will take place in the simulation

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
                total_seat_reqs = temp_int;
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

    //TESTING: Command-line user interface
    printf("Total number of seats: %d\n", total_seat_reqs);
    printf("T-X robot average time: %d\n", tx_time);
    printf("Rev-9 robot average time: %d\n", rev9_time);
    printf("General table greeter average time: %d\n", general_time);
    printf("VIP room greeter average time: %d\n", vip_time);

        //TESTING: Command-line user interface
    printf("Total number of seats: %d\n", total_seat_reqs);
    printf("T-X robot average time: %d\n", tx_time);
    printf("Rev-9 robot average time: %d\n", rev9_time);
    printf("General table greeter average time: %d\n", general_time);
    printf("VIP room greeter average time: %d\n", vip_time);

    /*** THREAD CREATION ***/

    //declare pthread variables
    pthread_t gen_table_robot;                      //thread for general table greeting robot
    pthread_t vip_room_robot;                       //thread for VIP room robot
    pthread_t tx_robot;                             //thread for T-X robot
    pthread_t rev9_robot;                           //thread for Rev-9 robot

    sem_init(&main_barrier, 0, 0); //Initialize semaphore

    //producer thread creation
    pthread_create(&gen_table_robot, NULL, producer_general, &general_time);
    pthread_create(&vip_room_robot, NULL, producer_vip, &vip_time);

    //consumer thread creation
    pthread_create(&tx_robot, NULL, consumer_tx, &tx_time);
    pthread_create(&rev9_robot, NULL, consumer_rev9, &rev9_time);


    sem_wait(&main_barrier); //Wait until we get signal from the last consumer
    
    return 0;
}
