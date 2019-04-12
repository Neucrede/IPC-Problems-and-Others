#include <windows.h>
#include <process.h>
#include <stdio.h>

typedef struct __SmokerInfo
{
    int         iSmokerID;
    int         nIterations;
    HANDLE      hSemIngredient;
} SmokerInfo_t;

HANDLE  hSemIngredients[3];
HANDLE  hSemAgent;
HANDLE  hMutexIngredients;

DWORD WINAPI SmokerThreadFunc(LPVOID lpParam)
{
    int             nItrnCount = 0;
    HANDLE          hSemToWait[2];
    SmokerInfo_t*   pInfo = (SmokerInfo_t*)lpParam;
    int             i = 0, j = 0;

    printf("Smoker's thread %d created. \n", pInfo->iSmokerID);
    WaitForSingleObject(hMutexIngredients, INFINITE);
    while ((i < 3) || (j < 2))
    {
        if (pInfo->iSmokerID != i)
        {
            hSemToWait[j] = hSemIngredients[i];
            printf("Smoker %d waits for semaphore 0x%08X. \n",
                pInfo->iSmokerID, hSemIngredients[i]);
            ++j;
        }
        ++i;
    }
    ReleaseMutex(hMutexIngredients);

    while (nItrnCount++ <= pInfo->nIterations)
    {
        printf("Smoker %d is waiting. \n", pInfo->iSmokerID);
        WaitForMultipleObjects(2, hSemToWait, TRUE, INFINITE);
        ReleaseSemaphore(hSemAgent, 1, NULL);
        printf("SemAgent has been released by smoker %d. \n",
            pInfo->iSmokerID);
        printf("Smoker %d is smoking now. \n", pInfo->iSmokerID);

        Sleep(1);
    }

    return 0;
}

DWORD WINAPI AgentThreadFunc(LPVOID lpParam)
{
    int     nIntrCount = 0;
    int     iIngredient;
    int     i;

    printf("Agent's thread created. \n");
    while (nIntrCount++ <= (int)lpParam)
    {
        iIngredient = rand()%3;

        printf("Agent is waiting. \n");
        WaitForSingleObject(hSemAgent, INFINITE);
        printf("Ingredients for smoker %d is ready. \n", iIngredient);

        for (i = 0; i < 3; i++)
        {
            if (i != iIngredient)
            {
                ReleaseSemaphore(hSemIngredients[i], 1, NULL);
                printf("Semaphore has been 0x%08X released. \n", 
                    hSemIngredients[i]);
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    HANDLE          hThrdSmokers[3];
    DWORD           dwSmokersThreadID[3];
    HANDLE          hThrdAgent;
    DWORD           dwAgentThreadID;
    SmokerInfo_t    sSmokersInfo[3];
    int             i;

    const int       nItrnCount = 0x7FFFFFFF;

    for (i = 0; i < 3; i++)
    {
        hSemIngredients[i] = CreateSemaphore(NULL, 0, 1, NULL);
    }
    hSemAgent = CreateSemaphore(NULL, 1, 1, NULL);
    hMutexIngredients = CreateMutex(NULL, FALSE, NULL);

    for (i = 0; i < 3; i++)
    {
        sSmokersInfo[i].nIterations = nItrnCount;
        sSmokersInfo[i].iSmokerID = i;
        sSmokersInfo[i].hSemIngredient = hSemIngredients[i];
    }

    hThrdAgent = /*CreateThread*/_beginthreadex(NULL, 0, AgentThreadFunc, 
        (LPVOID)nItrnCount, 0, &dwAgentThreadID);
    for (i = 0; i < 3; i++)
    {
        hThrdSmokers[i] = /*CreateThread*/_beginthreadex(NULL, 0, SmokerThreadFunc, 
            &sSmokersInfo[i], 0, &dwSmokersThreadID[i]);
    }

    WaitForSingleObject(hThrdAgent, INFINITE);

    printf("All %d iterations done. \n", nItrnCount);

    for (i = 0; i < 3; i++)
    {
        CloseHandle(hThrdSmokers[i]);
    }
    CloseHandle(hThrdAgent);
    CloseHandle(hMutexIngredients);

    return 0;
}
