/*
* Attaching an interrupt to a desktop OS
* 
* Example from MSDN
*/

// we need this for IO
#include <stdio.h>

// Note the use of windows library
#include <windows.h>

// ISR - Interrupt Service Routine
// This is a function that is invoked when the interrupt happens
// the signature of this function is specified by the OS
//
// lpParam -- this is the pointer to the array of parameters
// TimerOrWaitFired -- flag that shows whether the task has ended correctly
// or was pre-empted by the OS
VOID CALLBACK isr1(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    // here, we just read the parameter to write it 
    int arg = * (int*) lpParam;
    printf("ISR 1: arg %d\n", arg);

}

// ISR - Interrupt Service Routine
// This is a function that is invoked when the interrupt happens
// the signature of this function is specified by the OS
//
// lpParam -- this is the pointer to the array of parameters
// TimerOrWaitFired -- flag that shows whether the task has ended correctly
// or was pre-empted by the OS
VOID CALLBACK isr2(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    // read the parameter passed
    char* arg = (char *)lpParam;

    printf("ISR2: arg %s\n", arg);
}

/*
* Function to assign an ISR to a specific IRQ
* 
* In this example, we use the timer as the interrupt. Simply to avoid setting up 
* new devices. We do that in TinkerCad and Arduino
* 
* period - number of seconds to wait
* isr - pointer to the isr routine/callback
*/
HANDLE TimerTask(unsigned int period, WAITORTIMERCALLBACK isr, void* arg)
{

    HANDLE HTimer = NULL;

    //set the timer to call the timer routine in 2ms
    if (!CreateTimerQueueTimer(&HTimer, NULL, (WAITORTIMERCALLBACK)isr, (PVOID)arg, 0, period * 1000, 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }


    return HTimer;
}

int main()
{
    // variables to hold parameters passed to ISR
    int arg1 = 1, 
        arg3 = 3;

    // one more parameter, this time it is a string
    char helloString[] = "Hello World!"; 

    // setting up three different interrupt handlers
    HANDLE h1 = NULL,
           h2 = NULL,
           h3 = NULL;

    //set the timer to call the timer routine in 2s
    // link to MSDN documentation: https://docs.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/nf-threadpoollegacyapiset-createtimerqueuetimer
    if (!CreateTimerQueueTimer(&h1, 
                                NULL, 
                                (WAITORTIMERCALLBACK)isr1, (PVOID) &arg1, 
                                0, 
                                2000, 
                                0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    } 

    //set the timer to call the timer routine in 2s
    if (!CreateTimerQueueTimer(&h2,
        NULL,
        (WAITORTIMERCALLBACK)isr2, (PVOID) helloString,
        0,
        10000,
        0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }

    //set the timer to call the timer routine in 2s
    if (!CreateTimerQueueTimer(&h3,
        NULL,
        (WAITORTIMERCALLBACK)isr1, (PVOID)&arg3,
        0,
        1000,
        0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }

    // just to stop the window from closing
    getchar();

    return 0;
}
