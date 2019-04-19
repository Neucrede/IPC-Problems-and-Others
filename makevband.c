#include <windows.h>

int main(int argc, char **argv[])
{
    HANDLE      hFileIn;
    HANDLE      hFileOut;
    DWORD       dwFileInSize;
    HANDLE      hHeap;
    char        *pInBuffer;
    DWORD       dwRead;
    char        OutBuffer[16];
    int         temp;
    int         i;
    unsigned char   c;

    hFileIn = CreateFile(argv[1],
                         GENERIC_READ,
                         FILE_SHARE_READ,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    hFileOut = CreateFile("vband.out",
                          GENERIC_WRITE,
                          FILE_SHARE_READ,
                          NULL,
                          CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    dwFileInSize = GetFileSize(hFileIn, NULL);

    hHeap = GetProcessHeap();
    pInBuffer = (char*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwFileInSize);

    ReadFile(hFileIn, pInBuffer, dwFileInSize, &dwRead, NULL);

    i = 0;
    while (i < dwFileInSize)
    {
        c = pInBuffer[i];
        sprintf(OutBuffer, "%02x", (unsigned int)c);
        SetEndOfFile(hFileOut);
        WriteFile(hFileOut, &OutBuffer, strlen(OutBuffer), &dwRead, NULL);
        i++;
    }

    CloseHandle(hFileIn);
    CloseHandle(hFileOut);

    return 0;
}

