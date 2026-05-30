#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <tchar.h>
#include <time.h>

#define Pipe TEXT("\\\\.\\pipe\\Pipe") 
#define BUFFERSIZE 512

typedef struct
{
    HANDLE hPipe1;
    HANDLE hPipe2;
} PipePair;
//--------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI PipePairThread(LPVOID lpvParam)
{
    PipePair* pair = (PipePair*)lpvParam;
    HANDLE hPipe1 = pair->hPipe1;
    HANDLE hPipe2 = pair->hPipe2;
    CHAR Buffer1[BUFFERSIZE], Buffer2[BUFFERSIZE];
    DWORD Read1{}, Read2{}, Write1, Write2;
    BOOL Success;

    int Toss;
    srand(time(NULL));
    char PlayerA[10];
    char ColourA[10];

    Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // player name
    Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // player name
    Success = WriteFile(hPipe1, Buffer2, Read2, &Write1, NULL); // opponent player name
    Success = WriteFile(hPipe2, Buffer1, Read1, &Write2, NULL); // opponent player name

    Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // player1, player2, resume
    Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // player1, player2, resume

    if ((stricmp(Buffer1, "Player1") == 0 && stricmp(Buffer2, "Player2") == 0) || (stricmp(Buffer1, "Player2") == 0 && stricmp(Buffer2, "Player1") == 0))
    {
        Success = WriteFile(hPipe1, Buffer1, (DWORD)strlen(Buffer1) + 1, &Write1, NULL); // player1, player2
        Success = WriteFile(hPipe2, Buffer2, (DWORD)strlen(Buffer2) + 1, &Write2, NULL); // player1, player2
        strcpy(PlayerA, Buffer1);

        Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // white, black
        Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // white, black
        strcpy(ColourA, Buffer1);
        //Success = WriteFile(hPipe1, Buffer1, (DWORD)strlen(Buffer1) + 1, &Write1, NULL);
        //Success = WriteFile(hPipe2, Buffer2, (DWORD)strlen(Buffer2) + 1, &Write2, NULL);
    }
    else if ((stricmp(Buffer1, "Player1") == 0 && stricmp(Buffer2, "Player1") == 0) || (stricmp(Buffer1, "Player2") == 0 && stricmp(Buffer2, "Player2") == 0))
    {
        printf("Both the player Selected same colour, So players will be assigned by the system.\n");

        Toss = rand() % 2;

        if (Toss == 0)
        {
            strcpy(Buffer1, "Player1");
            strcpy(Buffer2, "Player2");
            Success = WriteFile(hPipe1, Buffer1, (DWORD)strlen(Buffer1) + 1, &Write1, NULL);
            Success = WriteFile(hPipe2, Buffer2, (DWORD)strlen(Buffer2) + 1, &Write2, NULL);
        }
        else
        {
            strcpy(Buffer1, "Player2");
            strcpy(Buffer2, "Player1");
            Success = WriteFile(hPipe1, Buffer1, (DWORD)strlen(Buffer1) + 1, &Write1, NULL);
            Success = WriteFile(hPipe2, Buffer2, (DWORD)strlen(Buffer2) + 1, &Write2, NULL);
        }
        strcpy(PlayerA, Buffer1);

        Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // Buffer =white, black
        Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // Buffer =white, black
        strcpy(ColourA, Buffer1);
        //Success = WriteFile(hPipe1, Buffer1, (DWORD)strlen(Buffer1) + 1, &Write1, NULL);
        //Success = WriteFile(hPipe2, Buffer2, (DWORD)strlen(Buffer2) + 1, &Write2, NULL);
    }
    else if (stricmp(Buffer1, "resume") == 0 && stricmp(Buffer2, "resume") == 0)
    {
        Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // date & time
        Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // date & time
        Success = WriteFile(hPipe1, Buffer2, Read2, &Write1, NULL); // date & time
        Success = WriteFile(hPipe2, Buffer1, Read1, &Write2, NULL); // date & time

        Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // player1, player2
        Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // player1, player2
        strcpy(PlayerA, Buffer1);

        Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL); // white, black
        Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL); // white, black
        strcpy(ColourA, Buffer1);
    }
    FlushFileBuffers(hPipe1);
    FlushFileBuffers(hPipe2);

    while (1)
    {
        if ((stricmp(PlayerA, "Player1") == 0 && stricmp(ColourA, "White") == 0) || (stricmp(PlayerA, "Player2") == 0 && stricmp(ColourA, "Black") == 0))
        {
            Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL);
            Success = WriteFile(hPipe2, Buffer1, Read1, &Write2, NULL);

            if (stricmp(Buffer1, "save,game") == 0 || stricmp(Buffer2, "save,game") == 0)
            {
                time_t Time;
                struct tm* UtcTime;
                char TimeBuffer[50];
                time(&Time);
                UtcTime = gmtime(&Time);
                strftime(TimeBuffer, sizeof(TimeBuffer), "%Y,%m,%d,%H,%M,%S", UtcTime);

                Success = WriteFile(hPipe1, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write1, NULL); // date & time
                Success = WriteFile(hPipe2, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write2, NULL); // date & time
            }
            Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL);
            Success = WriteFile(hPipe1, Buffer2, Read2, &Write1, NULL);

            if (stricmp(Buffer1, "save,game") == 0 || stricmp(Buffer2, "save,game") == 0)
            {
                time_t Time;
                struct tm* UtcTime;
                char TimeBuffer[50];

                time(&Time);
                UtcTime = gmtime(&Time);
                strftime(TimeBuffer, sizeof(TimeBuffer), "%Y,%m,%d,%H,%M,%S", UtcTime);

                Success = WriteFile(hPipe1, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write1, NULL); // date & time
                Success = WriteFile(hPipe2, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write2, NULL); // date & time
            }
        }
        else
        {
            Success = ReadFile(hPipe2, Buffer2, BUFFERSIZE, &Read2, NULL);
            Success = WriteFile(hPipe1, Buffer2, Read2, &Write1, NULL);

            if (stricmp(Buffer1, "save,game") == 0 || stricmp(Buffer2, "save,game") == 0)
            {
                time_t Time;
                struct tm* UtcTime;
                char TimeBuffer[50];

                time(&Time);
                UtcTime = gmtime(&Time);
                strftime(TimeBuffer, sizeof(TimeBuffer), "%Y,%m,%d,%H,%M,%S", UtcTime);

                Success = WriteFile(hPipe1, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write1, NULL); // date & time
                Success = WriteFile(hPipe2, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write2, NULL); // date & time
            }
            Success = ReadFile(hPipe1, Buffer1, BUFFERSIZE, &Read1, NULL);
            Success = WriteFile(hPipe2, Buffer1, Read1, &Write2, NULL);

            if (stricmp(Buffer1, "save,game") == 0 || stricmp(Buffer2, "save,game") == 0)
            {
                time_t Time;
                struct tm* UtcTime;
                char TimeBuffer[50];

                time(&Time);
                UtcTime = gmtime(&Time);
                strftime(TimeBuffer, sizeof(TimeBuffer), "%Y,%m,%d,%H,%M,%S", UtcTime);

                Success = WriteFile(hPipe1, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write1, NULL); // date & time
                Success = WriteFile(hPipe2, TimeBuffer, (DWORD)strlen(TimeBuffer) + 1, &Write2, NULL); // date & time
            }
        }
    }
    FlushFileBuffers(hPipe1);
    DisconnectNamedPipe(hPipe1);
    CloseHandle(hPipe1);

    FlushFileBuffers(hPipe2);
    DisconnectNamedPipe(hPipe2);
    CloseHandle(hPipe2);

    free(pair);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------
int main()
{
    HANDLE hPipe1, hPipe2;
    PipePair* pair;
    BOOL Success;
    char Buffer[BUFFERSIZE];

    while (1)
    {
        hPipe1 = CreateNamedPipe(Pipe, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(Buffer), sizeof(Buffer), 0, NULL);
        hPipe2 = CreateNamedPipe(Pipe, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(Buffer), sizeof(Buffer), 0, NULL);

        Success = ConnectNamedPipe(hPipe1, NULL);
        _tprintf(TEXT("Client connected.\n"));
        Success = ConnectNamedPipe(hPipe2, NULL);
        _tprintf(TEXT("Client connected.\n"));
        printf("\n");

        pair = (PipePair*)malloc(sizeof(PipePair));
        pair->hPipe1 = hPipe1;
        pair->hPipe2 = hPipe2;

        if (CreateThread(NULL, 0, PipePairThread, pair, 0, NULL) == NULL)
        {
            _tprintf(TEXT("CreateThread failed.\n"));
            CloseHandle(hPipe1);
            CloseHandle(hPipe2);
            free(pair);
        }
    }
    return 0;
}