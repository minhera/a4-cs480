// Names: Minh Tran & Ryan Alakija
// Red ID: 
//
// Created by User on 6/23/2025.
//
//
// Created by User on 6/23/2025.
//
#include <stdio.h>
#include <iostream>
#include <unistd.h>         //for getopt function and access to POSIX OS API

#include "seating.h"
#include "log.h"

//MAYBE CONSIDER MOVING THIS TO HEADER FILE OF CLASS (once we create it)
#define EXIT_NORMAL 0       //normal exit flag
#define EXIT_ERROR 1        //error exit flag

//remove these global variables later, ONLY FOR REFERENCE
//default values for command line arguments


int main(int argc, char* argv[]) {

    /*** COMMAND LINE USER INTERFACE + INITIALIZATION ***/
    
    //declare & initialize variables w/ default values
    unsigned int production_limit = 120;                //total number of seating requests

    unsigned int consumer_tx = 0;                       //average time for T-X robot to process seating request + guide customers to proper seating
    unsigned int consumer_rev9 = 0;                     //average time for Rev-9 robot (same as T-X robot)
    unsigned int producer_gt = 0;                       //average time for general table greeter robot to produce and insert general table request
    unsigned int producer_vip = 0;                      //average time for VIP room greeter robot to produce and insert VIP room request

    /* some alternate variable names I am proposing
     *    numSeats / num_seats;
     *    txTime / tx_time;
     *    rev9Time / rev9_time / rev_9_time;
     *    tableGreeterTime / table_greeter_time;
     *    vipTime / vip_time;
     */

    //declare other variables
    int option;

    /* process optional arguments; each of them take numerical arguments
     * note: getopt() uses unistd.h library, which also includes getopt.h
     * output variables:
     *    optarg - the argument passed with the command-line switch/flag
     *    optind - the index that represents the first argument in argv to process AFTER
     *             all the optional arguments are processed; we will not need this here
     */
    while ((option = getopt(argc, argv, "s:x:r:g:v:")) != -1) {
        switch (option) {
            case 's':
                production_limit = std::stoi(optarg);
                break;
            case 'x':
                consumer_tx = std::stoi(optarg);
                break;
            case 'r':
                consumer_rev9 = std::stoi(optarg);
                break;
            case 'g':
                producer_gt = std::stoi(optarg);
                break;
            case 'v':
                producer_vip = std::stoi(optarg);
                break;
            default:
                fprintf(stderr, "usage: %s [ -s | -x | -r | -g | -v ]\n", argv[0]);
                exit(EXIT_ERROR);
        }
    }

    //TESTING: Command-line user interface
    printf("Total number of seats: %d\n", production_limit);
    printf("T-X robot average time: %d\n", consumer_tx);
    printf("Rev-9 robot average time: %d\n", consumer_rev9);
    printf("General table greeter average time: %d\n", producer_gt);
    printf("VIP room greeter average time: %d\n", producer_vip);

    return 0;
}
