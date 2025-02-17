/*
* Timer example using POSIX timer API
* DIT633, lecture 2
* (c) Miroslaw Staron, 2025
*/

// Standard libraries
// please note that we use two new ones: signal.h and time.h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// POSIX libraries for timers
#include <signal.h>
#include <time.h>

// for readability we define true
#define true 1

/* Our ISR - Interrupt Service Routine
* This function will be called when the timer expires/fires
*/
void timer_ISR(int signum) {

    // simple counter so that we know how many times it was fired
    // please note the keyword static
    // it means that the variable is not destroyed after the function ends
    // we need it here, because the function is called multiple times, but we want to keep the value
    // between the calls
    // 
    // change it to non-static and see what happens
    static int count = 0;

    // print the message just to know what happened
    printf("Timer expired: %d\n", ++count);
}

/* Main function
* The main function sets up and starts a POSIX timer that sends 
* a real-time signal (SIGRTMIN) at regular intervals (every second).
*/
int main(int argc, char *argv[]) {
    
    // Variables for timer setup
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its;

    // setting the actual timer
    timer_t timerid;

    // we set the interval of the timer to be 1 second
    int interval_sec = 1;

    // and nano seconds to 0
    int interval_nsec = 0;

    // Set up signal handler
    // first, we say that we will fill the parts of the SIGINFO structure
    sa.sa_flags = SA_SIGINFO;

    // then we say that we want to call the function timer_handler when the signal is received
    // this is where we link the action to the interrupt
    // since the name of the function is a pointer, we can use it directly
    // and assign it to the variable sa.sa_handler
    // note that we do not use the parentheses, because we do not want to call the function
    // we want to assign the address of the function to the variable
    sa.sa_handler = timer_ISR;

    // this line sets the interrupt to be non-blocking
    sigemptyset(&sa.sa_mask);

    // we link the signal SIGRTMIN to the function timer_handler
    // by calling the function sigaction
    // it takes as the parameter the structure sa, which we just filled in
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up timer and sets the timer to send a signal
    sev.sigev_notify = SIGEV_SIGNAL;

    // we set the signal to be sent to SIGRTMIN
    // which is a real-time signal
    sev.sigev_signo = SIGRTMIN;

    // we set the value of the timer to be the address of the timerid
    sev.sigev_value.sival_ptr = &timerid;

    // Create the timer
    // we create the timer with the function timer_create
    // it takes as the first parameter the type of the clock
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Start timer
    // we set the initial expiration of the time, i.e., the first firing
    // and 0 nanoseconds
    its.it_value.tv_sec = interval_sec;
    its.it_value.tv_nsec = interval_nsec;

    // we set the interval of the timer to be 1 second
    // and 0 nanoseconds
    its.it_interval.tv_sec = interval_sec;
    its.it_interval.tv_nsec = interval_nsec;


    // and here we set the timer to start
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // Wait for timer events
    // this is an infinite loop, the program will wait for the timer to expire
    // nothing happens here
    // all logic is handled in the ISR
    while (true) {
        pause(); 
    }

    // finish up the program
    return 0;
}