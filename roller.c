#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PASSENGERS_PER_RIDE         32
#define TOTAL_PASSENGERS_COUNT      64
#define MAX_RIDES_COUNT             0x7FFFFFFF

HANDLE  hThreadPassengers[TOTAL_PASSENGERS_COUNT];
HANDLE  hThreadCar;
DWORD   dwPassengersThreadID[TOTAL_PASSENGERS_COUNT];
DWORD   dwCarThreadID;
LONG    nPassengersOnBoard;
HANDLE  hSemQueueAllowed;
HANDLE  hSemCheckInAllowed;
HANDLE  hSemAllSeatsTaken;
HANDLE  hSemGetoffAllowed;
HANDLE  hSemGetoff;

DWORD WINAPI PassengerThreadFunc(LPVOID lpParam);
DWORD WINAPI CarThreadFunc(LPVOID lpParam);

int main(int argc, char **argv)
{
    int     i;

    hSemQueueAllowed = CreateSemaphore(NULL, 0, PASSENGERS_PER_RIDE, NULL);
    hSemCheckInAllowed = CreateSemaphore(NULL, 0, 1, NULL);
    hSemAllSeatsTaken = CreateSemaphore(NULL, 0, 1, NULL);
    hSemGetoffAllowed = CreateSemaphore(NULL, 0, 1, NULL);
    hSemGetoff = CreateSemaphore(NULL, 0, 1, NULL);
    assert(hSemQueueAllowed);
    assert(hSemCheckInAllowed);
    assert(hSemAllSeatsTaken);
    assert(hSemGetoffAllowed);
    assert(hSemGetoff);

    nPassengersOnBoard = 0;
    for (i = 0; i < TOTAL_PASSENGERS_COUNT; i++)
    {
/*      hThreadPassengers[i] = CreateThread(NULL, 0, PassengerThreadFunc, 
            (LPVOID)i, 0, &dwPassengersThreadID[i]);
*/
        hThreadPassengers[i] = (HANDLE)_beginthreadex(NULL, 0, PassengerThreadFunc,
            (void*)i, 0, &dwPassengersThreadID[i]);
        assert(hThreadPassengers[i]);
    }

/*  hThreadCar = CreateThread(NULL, 0, CarThreadFunc, NULL, 0, &dwCarThreadID);
    assert(hThreadCar);
*/
    hThreadCar = (HANDLE)_beginthreadex(NULL, 0, CarThreadFunc, NULL, 0, 
        &dwCarThreadID);
    WaitForSingleObject(hThreadCar, INFINITE);

    for (i = 0; i < TOTAL_PASSENGERS_COUNT; i++)
    {
        TerminateThread(hThreadPassengers[i], EXIT_SUCCESS);
    }

    CloseHandle(hSemQueueAllowed);
    CloseHandle(hSemCheckInAllowed);
    CloseHandle(hSemAllSeatsTaken);
    CloseHandle(hSemGetoffAllowed);
    CloseHandle(hSemGetoff);

    return EXIT_SUCCESS;
}

DWORD WINAPI PassengerThreadFunc(LPVOID lpParam)
{
    int     i;
    int     iPassengerID = (int)lpParam;

    for (i = 0; i < MAX_RIDES_COUNT; i++)
    {
        printf("%d waiting for queue. \n", iPassengerID);
        WaitForSingleObject(hSemQueueAllowed, INFINITE);

        printf("%d was allowed to check in. \n", iPassengerID);
        WaitForSingleObject(hSemCheckInAllowed, INFINITE);
        
        ++nPassengersOnBoard;
        if (nPassengersOnBoard < PASSENGERS_PER_RIDE)
        {
        
            ReleaseSemaphore(hSemCheckInAllowed, 1, NULL);
        }
        else
        {
            printf("All seats taken. \n");
            ReleaseSemaphore(hSemAllSeatsTaken, 1, NULL);
        }

        printf("%d was allowed to get off. \n", iPassengerID);
        WaitForSingleObject(hSemGetoffAllowed, INFINITE);

        printf("%d got off the car. \n", iPassengerID);
        ReleaseSemaphore(hSemGetoff, 1, NULL);
    }

    return EXIT_SUCCESS;
}

DWORD WINAPI CarThreadFunc(LPVOID lpParam)
{
    int     i, j;

    for (i = 0; i < MAX_RIDES_COUNT; i++)
    {
        printf(">> Queue. \n");
        ReleaseSemaphore(hSemQueueAllowed, PASSENGERS_PER_RIDE, NULL);

        printf(">> Check in. \n");
        ReleaseSemaphore(hSemCheckInAllowed, 1, NULL);

        printf(">> Wait for passengers. \n");
        WaitForSingleObject(hSemAllSeatsTaken, INFINITE);

        Sleep(2000);

        printf(">> Door opened. \n");
        for (j = 0; j < PASSENGERS_PER_RIDE; j++)
        {
            ReleaseSemaphore(hSemGetoffAllowed, 1, NULL);

            WaitForSingleObject(hSemGetoff, INFINITE);
        }
        nPassengersOnBoard = 0;
    }

    return EXIT_SUCCESS;
}
