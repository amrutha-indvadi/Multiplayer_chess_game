#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include<windows.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<time.h>

#define Pipe TEXT("\\\\.\\pipe\\Pipe")
//#define PIPENAME TEXT("\\\\192.168.50.70\\pipe\\chess")
#define BUFFERSIZE 500

typedef enum
{
	White = 0,
	Black = 1
}Colour;

typedef enum
{
	WhitePawn = 'P', BlackPawn = 'p',
	WhiteRook = 'R', BlackRook = 'r',
	WhiteKnight = 'N', BlackKnight = 'n',
	WhiteBishop = 'B', BlackBishop = 'b',
	WhiteQueen = 'Q', BlackQueen = 'q',
	WhiteKing = 'K', BlackKing = 'k',
	empty = ' '
} Piece;

char board[8][8];
int SrcRow;
int DstRow;
int SrcCol;
int DstCol;

char Player[50];
int CurrentPlayer = White;
int WhiteKingRow, WhiteKingCol, BlackKingRow, BlackKingCol;
//--------------------------------------------------------------------------------------------------------------------------------
void InitializeBoard()
{
	for (int i = 2; i <= 5; i++) // empty square
	{
		for (int j = 0; j < 8; j++)
			board[i][j] = empty;
	}
	for (int j = 0; j < 8; j++) // pawns
	{
		board[1][j] = BlackPawn;
		board[6][j] = WhitePawn;
	}
	board[0][0] = board[0][7] = BlackRook; // black pieces
	board[0][1] = board[0][6] = BlackKnight;
	board[0][2] = board[0][5] = BlackBishop;
	board[0][3] = BlackQueen;
	board[0][4] = BlackKing;

	board[7][0] = board[7][7] = WhiteRook; // white pieces
	board[7][1] = board[7][6] = WhiteKnight;
	board[7][2] = board[7][5] = WhiteBishop;
	board[7][3] = WhiteQueen;
	board[7][4] = WhiteKing;
}
//--------------------------------------------------------------------------------------------------------------------------------
void PrintBoard()
{
	if (stricmp(Player, "player1") == 0 || stricmp(Player, "0") == 0)
	{
		printf("    "); // column lable
		for (char ColLable = 'a'; ColLable <= 'h'; ColLable++)
			printf("\033[1;30m %c \033[0m", ColLable);
		printf("\033[1;30m \n   --------------------------\033[0m\n");

		for (int i = 0; i < 8; i++)
		{
			printf("\033[1;30m %d |\033[0m", 8 - i); // row lable

			for (int j = 0; j < 8; j++)
			{
				if ((i + j) % 2 == 0) //square colour
					printf("\033[47m");
				else
					printf("\033[40m");

				if (board[i][j] == WhitePawn || board[i][j] == WhiteRook || board[i][j] == WhiteKnight || board[i][j] == WhiteBishop || board[i][j] == WhiteQueen || board[i][j] == WhiteKing)
					printf("\033[1;34m %c \033[0m", board[i][j]); // white pieces (bule)
				if (board[i][j] == BlackPawn || board[i][j] == BlackRook || board[i][j] == BlackKnight || board[i][j] == BlackBishop || board[i][j] == BlackQueen || board[i][j] == BlackKing)
					printf("\033[1;35m %c \033[0m", board[i][j]); // black piece (pink)
				if (board[i][j] == empty)
					printf(" %c ", board[i][j]);
				printf("\033[0m");
			}
			printf("\033[1;30m| %d \n\033[0m", 8 - i); // row lable
		}
		printf("\033[1;30m   --------------------------\033[0m\n"); // column lable
		printf("    ");
		for (char ColLable = 'a'; ColLable <= 'h'; ColLable++)
			printf("\033[1;30m %c \033[0m", ColLable);
		printf("\n");
	}
	else
	{
		printf("    "); // column lable
		for (char ColLable = 'h'; ColLable >= 'a'; ColLable--)
			printf("\033[1;30m %c \033[0m", ColLable);
		printf("\033[1;30m \n   --------------------------\033[0m\n");

		for (int i = 7; i >= 0; i--)
		{
			printf("\033[1;30m %d |\033[0m", 8 - i); // row lable

			for (int j = 7; j >= 0; j--)
			{
				if ((i + j) % 2 == 0) //square colour
					printf("\033[47m");
				else
					printf("\033[40m");

				if (board[i][j] == WhitePawn || board[i][j] == WhiteRook || board[i][j] == WhiteKnight || board[i][j] == WhiteBishop || board[i][j] == WhiteQueen || board[i][j] == WhiteKing)
					printf("\033[1;34m %c \033[0m", board[i][j]); // white pieces (bule)
				if (board[i][j] == BlackPawn || board[i][j] == BlackRook || board[i][j] == BlackKnight || board[i][j] == BlackBishop || board[i][j] == BlackQueen || board[i][j] == BlackKing)
					printf("\033[1;35m %c \033[0m", board[i][j]); // black piece (pink)
				if (board[i][j] == empty)
					printf(" %c ", board[i][j]);
				printf("\033[0m");
			}
			printf("\033[1;30m| %d \n\033[0m", 8 - i); // row lable
		}
		printf("\033[1;30m   --------------------------\033[0m\n"); // column lable
		printf("    ");
		for (char ColLable = 'h'; ColLable >= 'a'; ColLable--)
			printf("\033[1;30m %c \033[0m", ColLable);
		printf("\n");
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Pawn(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	char pawn = board[*SrcRow][*SrcCol];

	if (pawn == WhitePawn)
	{
		if (*SrcCol == *DstCol && board[*DstRow][*DstCol] == empty)
		{
			if (*DstRow == *SrcRow - 1)
				return  true; // 1 step
			if (*SrcRow == 6 && *DstRow == 4 && board[5][*SrcCol] == empty)
				return true; // 2 step (1st move)
		}
		if ((*DstCol == *SrcCol + 1 || *DstCol == *SrcCol - 1) && *DstRow == *SrcRow - 1 && islower(board[*DstRow][*DstCol]))
			return true; //capture
	}
	if (pawn == BlackPawn)
	{
		if (*SrcCol == *DstCol && board[*DstRow][*DstCol] == empty)
		{
			if (*DstRow == *SrcRow + 1)
				return  true; // 1 step
			if (*SrcRow == 1 && *DstRow == 3 && board[2][*SrcCol] == empty)
				return true; // 2 step (1st move)
		}
		if ((*DstCol == *SrcCol + 1 || *DstCol == *SrcCol - 1) && *DstRow == *SrcRow + 1 && isupper(board[*DstRow][*DstCol]))
			return true; // capture
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Rook(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	int RowStep = (*DstRow > *SrcRow) - (*DstRow < *SrcRow);
	int ColStep = (*DstCol > *SrcCol) - (*DstCol < *SrcCol);
	int Row = *SrcRow;
	int Col = *SrcCol;

	if (*SrcRow == *DstRow || *SrcCol == *DstCol)
	{
		do
		{
			if (board[Row + RowStep][Col + ColStep] == empty)
			{
				Row += RowStep;
				Col += ColStep;
			}
			else if (CurrentPlayer == White && board[Row + RowStep][Col + ColStep] == board[*DstRow][*DstCol])
				return true;
			else if (CurrentPlayer == Black && board[Row + RowStep][Col + ColStep] == board[*DstRow][*DstCol])
				return true;
			else
				return false;
		} while (Row != *DstRow || Col != *DstCol);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Knight(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	int RowDiff = abs(*DstRow - *SrcRow);
	int ColDiff = abs(*DstCol - *SrcCol);

	if ((RowDiff == 1 && ColDiff == 2) || (RowDiff == 2 && ColDiff == 1))
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Bishop(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	int RowStep = (*DstRow > *SrcRow) - (*DstRow < *SrcRow);
	int ColStep = (*DstCol > *SrcCol) - (*DstCol < *SrcCol);
	int Row = *SrcRow;
	int Col = *SrcCol;

	if (abs(*SrcRow - *DstRow) == abs(*SrcCol - *DstCol))
	{
		do
		{
			if (board[Row + RowStep][Col + ColStep] == empty)
			{
				Row += RowStep;
				Col += ColStep;
			}
			else if (CurrentPlayer == White && board[Row + RowStep][Col + ColStep] == board[*DstRow][*DstCol])
				return true;
			else if (CurrentPlayer == Black && board[Row + RowStep][Col + ColStep] == board[*DstRow][*DstCol])
				return true;
			else
				return false;
		} while (Row != *DstRow || Col != *DstCol);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Queen(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	if (Rook(SrcRow, SrcCol, DstRow, DstCol) || Bishop(SrcRow, SrcCol, DstRow, DstCol))
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool King(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	int RowDiff = abs(*DstRow - *SrcRow);
	int ColDiff = abs(*DstCol - *SrcCol);

	if ((RowDiff <= 1 && ColDiff <= 1))
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool ValidMove(int* SrcRow, int* SrcCol, int* DstRow, int* DstCol)
{
	char piece = board[*SrcRow][*SrcCol];

	if (piece == WhitePawn || piece == BlackPawn)
		return (Pawn(SrcRow, SrcCol, DstRow, DstCol));
	else if (piece == WhiteRook || piece == BlackRook)
		return (Rook(SrcRow, SrcCol, DstRow, DstCol));
	else if (piece == WhiteKnight || piece == BlackKnight)
		return (Knight(SrcRow, SrcCol, DstRow, DstCol));
	else if (piece == WhiteBishop || piece == BlackBishop)
		return (Bishop(SrcRow, SrcCol, DstRow, DstCol));
	else if (piece == WhiteQueen || piece == BlackQueen)
		return (Queen(SrcRow, SrcCol, DstRow, DstCol));
	else if (piece == WhiteKing || piece == BlackKing)
		return (King(SrcRow, SrcCol, DstRow, DstCol));
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool Check(int CurrentPlayer)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == WhiteKing)
			{
				WhiteKingRow = i;
				WhiteKingCol = j;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == BlackKing)
			{
				BlackKingRow = i;
				BlackKingCol = j;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (CurrentPlayer == White)
			{
				if (ValidMove(&i, &j, &WhiteKingRow, &WhiteKingCol) && islower(board[i][j]))
					return true;
			}
			if (CurrentPlayer == Black)
			{
				if (ValidMove(&i, &j, &BlackKingRow, &BlackKingCol) && isupper(board[i][j]))
					return true;
			}
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool KingToSafeSquare(int CurrentPlayer)
{
	if (CurrentPlayer == White)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == WhiteKing)
				{
					char WhiteKing = board[i][j];

					for (int r = 0; r < 8; r++)
					{
						for (int c = 0; c < 8; c++)
						{
							char Piece = board[r][c];

							if (King(&i, &j, &r, &c) && (islower(Piece) || Piece == empty))
							{
								board[i][j] = empty;
								board[r][c] = WhiteKing;

								if (!Check(CurrentPlayer == true))
								{
									board[i][j] = WhiteKing;
									board[r][c] = Piece;
									return true;
								}
								board[i][j] = WhiteKing;
								board[r][c] = Piece;
							}
						}
					}
				}
			}
		}
	}
	if (CurrentPlayer == Black)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == BlackKing)
				{
					char BlackKing = board[i][j];

					for (int r = 0; r < 8; r++)
					{
						for (int c = 0; c < 8; c++)
						{
							char Piece = board[r][c];

							if (King(&i, &j, &r, &c) && (board[r][c] == empty || isupper(board[r][c])))
							{
								board[i][j] = empty;
								board[r][c] = BlackKing;

								if (!Check(CurrentPlayer == true))
								{
									board[i][j] = BlackKing;
									board[r][c] = Piece;
									return true;
								}
								board[i][j] = BlackKing;
								board[r][c] = Piece;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool BlockCheck(int CurrentPlayer)
{
	if (CurrentPlayer == White)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				char WhitePiece = board[i][j];

				if (isupper(WhitePiece) && WhitePiece != WhiteKing)
				{
					for (int r = 0; r < 8; r++)
					{
						for (int c = 0; c < 8; c++)
						{
							char Piece = board[r][c];

							if (ValidMove(&i, &j, &r, &c) && (islower(Piece) || Piece == empty))
							{
								board[i][j] = empty;
								board[r][c] = WhitePiece;

								if (!Check(CurrentPlayer == true))
								{
									board[i][j] = WhitePiece;
									board[r][c] = Piece;
									return true;
								}
								board[i][j] = WhitePiece;
								board[r][c] = Piece;
							}
						}
					}
				}
			}
		}
	}
	if (CurrentPlayer == Black)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				char BlackPiece = board[i][j];

				if (islower(BlackPiece) && BlackPiece != BlackKing)
				{
					for (int r = 0; r < 8; r++)
					{
						for (int c = 0; c < 8; c++)
						{
							char Piece = board[r][c];

							if (isupper(Piece) || Piece == empty)
							{
								board[i][j] = empty;
								board[r][c] = BlackPiece;

								if (!Check(CurrentPlayer == true))
								{
									board[i][j] = BlackPiece;
									board[r][c] = Piece;
									return true;
								}
								board[i][j] = BlackPiece;
								board[r][c] = Piece;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------
bool CheckMate(int CurrentPlayer)
{
	if (!Check(CurrentPlayer == true))
		return false;
	if (KingToSafeSquare(CurrentPlayer == true))
		return false;
	if (BlockCheck(CurrentPlayer == true))
		return false;
	else
		return true;
}
//--------------------------------------------------------------------------------------------------------------------------------
char* ReadLineInBuffer(const char* Buffer, int Line)
{
	int currentLine = 1;
	const char* lineStart = Buffer;
	const char* lineEnd = NULL;

	while (*lineStart != '\0')
	{
		lineEnd = strchr(lineStart, '\n');
		if (currentLine == Line)
		{
			int lineLength;
			if (lineEnd != NULL)
				lineLength = lineEnd - lineStart;
			else
				lineLength = strlen(lineStart);

			char* result = (char*)malloc(lineLength + 1);
			strncpy(result, lineStart, lineLength);
			result[lineLength] = '\0';
			return result;
		}
		if (lineEnd == NULL)
			break;

		lineStart = lineEnd + 1;
		currentLine++;
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------
int main()
{
	char Src[5], Dst[5];
	HANDLE hPipe1;
	BOOL Success;
	char Buffer[BUFFERSIZE];
	DWORD Read1 = 0, Write1 = 0;

	char PlayerName[50], OppPlayerName[50];
	char PlayerFile[50];

	while (!WaitNamedPipe(Pipe, NMPWAIT_WAIT_FOREVER)) // pipe IPC
	//while (!WaitNamedPipe(PIPENAME, NMPWAIT_WAIT_FOREVER))
		printf("Waiting for server.\n");
	system("cls");

	hPipe1 = CreateFile(Pipe, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	//hPipe1 = CreateFile(PIPENAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	printf("Server connected.\n\n");
	
	printf("Player Name: ");
	scanf("%s", PlayerName);
	Success = WriteFile(hPipe1, PlayerName, (DWORD)strlen(PlayerName) + 1, &Write1, NULL); // player name
	Success = ReadFile(hPipe1, OppPlayerName, sizeof(OppPlayerName), &Read1, NULL); // oponent player name
	printf("Opponent  player name: %s\n", OppPlayerName);

	while (1)
	{
		printf("\nEnter \"Player1\" to choose White \n");
		printf("Enter \"Player2\" to choose Black \n");
		printf("Enter \"Resume\" to resume old game\n");
		printf("\nEnter your choice: ");
		scanf("%s", &Player);
		printf("\n");
		if (stricmp(Player, "player1") == 0 || stricmp(Player, "player2") == 0) // new game
		{
			Success = WriteFile(hPipe1, Player, (DWORD)strlen(Player) + 1, &Write1, NULL); // player1, player2
			Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL);
			strcpy(Player, Buffer); // player1, player2

			if (CurrentPlayer == White)
			{
				strcpy(Buffer, "White");
				Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // White
			}
			if (CurrentPlayer == Black)
			{
				strcpy(Buffer, "Black");
				Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // Black
			}
			printf("You are: %s\n\n", (stricmp(Player, "player1") == 0) ? "\033[1;34m Player1 (White) \033[0m" : "\033[1;35m Player2 (Black) \033[0m");
			InitializeBoard();
			break;
		}
		else if (stricmp(Player, "Resume") == 0) // resume (file mapping)
		{
			char OppPlayerNameA[50];

			Success = WriteFile(hPipe1, Player, (DWORD)strlen(Player) + 1, &Write1, NULL); // resume

			printf("Enter opponent player name: ");
			scanf("%s", OppPlayerNameA);

			if (stricmp(OppPlayerName, OppPlayerNameA) == 0)
			{
				sprintf(PlayerFile, "%s.txt", OppPlayerName);

				const char* PlayerfilePath = PlayerFile;

				HANDLE PlayerFilePtr = NULL;
				HANDLE PlayerMap = NULL;
				char* PlayerView = NULL;

				PlayerFilePtr = CreateFileA(PlayerfilePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				PlayerMap = CreateFileMapping(PlayerFilePtr, NULL, PAGE_READWRITE, 0, 0, NULL);
				PlayerView = (char*)MapViewOfFile(PlayerMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				strcpy(Buffer, PlayerView);

				char* ReadDateTime = ReadLineInBuffer(PlayerView, 1);
				if (ReadDateTime != NULL)
				{
					Success = WriteFile(hPipe1, ReadDateTime, (DWORD)strlen(ReadDateTime) + 1, &Write1, NULL); // date & time
					Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL);
				}
				char* ReadPlayer = ReadLineInBuffer(PlayerView, 2);
				if (ReadPlayer != NULL)
				{
					if (stricmp(ReadPlayer, "Player1\r") == 0)
					{
						strcpy(Player, "Player1");
						Success = WriteFile(hPipe1, Player, (DWORD)strlen(Player) + 1, &Write1, NULL); // player1
					}
					else if (stricmp(ReadPlayer, "Player2\r") == 0)
					{
						strcpy(Player, "Player2");
						Success = WriteFile(hPipe1, Player, (DWORD)strlen(Player) + 1, &Write1, NULL); // player2
					}
					free(ReadPlayer);
				}
				char* ReadCurrentPlayer = ReadLineInBuffer(PlayerView, 3);
				if (ReadCurrentPlayer != NULL)
				{
					if (stricmp(ReadCurrentPlayer, "White\r") == 0)
					{
						CurrentPlayer = White;
						strcpy(Buffer, "White");
						Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // White
					}

					if (stricmp(ReadCurrentPlayer, "Black\r") == 0)
					{
						CurrentPlayer = Black;
						strcpy(Buffer, "Black");
						Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // Black
					}
					free(ReadCurrentPlayer);
				}
				int LineNo = 4;
				char FileBuffer[4];
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						char* ReadCurrentLine = ReadLineInBuffer(PlayerView, LineNo);
						strcpy(FileBuffer, ReadCurrentLine);

						if (FileBuffer[0] == '.')
							board[i][j] = empty;
						else
							board[i][j] = FileBuffer[0];
						LineNo++;
						free(ReadCurrentLine);
					}
				}
				UnmapViewOfFile(PlayerView);
				CloseHandle(PlayerMap);
				CloseHandle(PlayerFilePtr);
				break;
			}
			else
				printf("File not found!\n");
		}
		else
			printf("Enter valid choice.\n");
	}

	while (1)
	{
		// Player1 White - write ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (stricmp(Player, "player1") == 0 || stricmp(Player, "0") == 0)
		{
			if (CurrentPlayer == White)
			{
				PrintBoard();

				if (CheckMate(CurrentPlayer == true)) // checkmate 
				{
					printf("\033[1;31m CHECKMATE! \n Game Over\033[0m");
					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
				else if (Check(CurrentPlayer == true)) // check
					printf("\033[1;31m King is in check! \n\033[0m");

				printf("\n\033[1;30mIf you want to exit game, Enter \" QUIT GAME \"\n\033[0m");
				printf("\033[1;30mIf you want to save and exit game, Enter \" SAVE GAME \" \n\n\033[0m");
				printf("Current player: %s\n", (CurrentPlayer == White) ? "\033[1;34m White \033[0m" : "\033[1;35m Black \033[0m");
				printf("Enter source and destination: \n");

				DWORD StartTime = GetTickCount(); // timer
				DWORD EndTime = 60000;

				while ((GetTickCount() - StartTime) < EndTime)
				{
					if (_kbhit())
					{
						scanf("%s %s", Src, Dst); // source and destination input
						printf("\n");

						if (stricmp("quit", Src) == 0 && stricmp("game", Dst) == 0) // quit game
						{
							char* Data = strcat(Src, ",");
							strcat(Data, Dst);
							strcpy(Buffer, Data);
							Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

							printf("Game over!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");

							FlushFileBuffers(hPipe1);
							CloseHandle(hPipe1);
							exit(0);
						}
						if (stricmp("save", Src) == 0 && stricmp("game", Dst) == 0) // save and quit game (file handling)
						{
							printf("Game saved!\n");

							char* Data = strcat(Src, ",");
							strcat(Data, Dst);
							strcpy(Buffer, Data);
							Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // save,game
							Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // date & time

							sprintf(PlayerFile, "%s.txt", OppPlayerName);
							
							FILE* PlayerFilePtr = freopen(PlayerFile, "w", stdout);
							printf("%s\n", Buffer);
							printf("%s\n", (stricmp(Player, "Player1")) == 0 ? "Player1" : "Player2");
							printf("%s\n", (CurrentPlayer == White) ? "White" : "Black");
							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 8; j++)
								{
									if (board[i][j] == empty)
										printf(".\n");
									else
										printf("%c\n", board[i][j]);
								}
							}
							FlushFileBuffers(hPipe1);
							CloseHandle(hPipe1);
							exit(0);
						}
						if (SrcRow >= 0 && SrcRow < 8 && SrcCol >= 0 && SrcCol < 8 && DstRow >= 0 && DstRow < 8 && DstCol >= 0 && DstCol < 8) // move piece
						{
							SrcCol = Src[0] - 'a';
							SrcRow = 8 - (Src[1] - '0');
							DstCol = Dst[0] - 'a';
							DstRow = 8 - (Dst[1] - '0');

							if (isupper(board[SrcRow][SrcCol]) && ValidMove(&SrcRow, &SrcCol, &DstRow, &DstCol))
							{
								if (board[DstRow][DstCol] == empty || islower(board[DstRow][DstCol]))
								{
									board[DstRow][DstCol] = board[SrcRow][SrcCol];
									board[SrcRow][SrcCol] = empty;
									system("cls");

									char* Data = strcat(Src, ","); // write data
									strcat(Data, Dst);
									strcpy(Buffer, Data);
									Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

									CurrentPlayer = 1 - CurrentPlayer;
								}
							}
						}
						break;
					}
				}
				if ((GetTickCount() - StartTime) == EndTime) // time up
				{
					strcpy(Buffer, "quit,game");
					Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

					printf("Time Out!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");

					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
			}
			// Player1 Black - read ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			else if (CurrentPlayer == Black)
			{
				PrintBoard();

				if (CheckMate(CurrentPlayer == true)) // checkmate
				{
					printf("\033[1;31m CHECKMATE! \n Game Over\033[0m");

					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
				else if (Check(CurrentPlayer == true)) // check
					printf("\033[1;31m King is in check \n\033[0m");

				printf("Current player: %s\n", (CurrentPlayer == White) ? "\033[1;34m White \033[0m" : "\033[1;35m Black \033[0m");

				Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // read data
				if (Success && Read1 > 0)
				{
					if (stricmp("quit,game", Buffer) == 0) // quit game
					{
						printf("Game over!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");

						FlushFileBuffers(hPipe1);
						CloseHandle(hPipe1);
						exit(0);
					}
					if (stricmp("save,game", Buffer) == 0) // save game
					{
						printf("Game saved!\n");

						Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // file name (dat & time)

						sprintf(PlayerFile, "%s.txt", OppPlayerName);

						FILE* PlayerFilePtr = freopen(PlayerFile, "w", stdout);
						printf("%s\n", Buffer);
						printf("%s\n", (stricmp(Player, "Player1")) == 0 ? "Player1" : "Player2");
						printf("%s\n", (CurrentPlayer == White) ? "White" : "Black");
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 8; j++)
							{
								if (board[i][j] == empty)
									printf(".\n");
								else
									printf("%c\n", board[i][j]);
							}
						}
						FlushFileBuffers(hPipe1);
						CloseHandle(hPipe1);
						exit(0);
					}
					char* Token = strtok(Buffer, ","); // move piece
					while (Token != NULL)
					{
						strcpy(Src, Token);
						Token = strtok(NULL, ",");
						strcpy(Dst, Token);
						break;
					}
				}
				printf("\n");

				if (SrcRow >= 0 && SrcRow < 8 && SrcCol >= 0 && SrcCol < 8 && DstRow >= 0 && DstRow < 8 && DstCol >= 0 && DstCol < 8)
				{
					SrcCol = Src[0] - 'a';
					SrcRow = 8 - (Src[1] - '0');
					DstCol = Dst[0] - 'a';
					DstRow = 8 - (Dst[1] - '0');

					if (islower(board[SrcRow][SrcCol]) && ValidMove(&SrcRow, &SrcCol, &DstRow, &DstCol))
					{
						if (board[DstRow][DstCol] == empty || isupper(board[DstRow][DstCol]))
						{
							board[DstRow][DstCol] = board[SrcRow][SrcCol];
							board[SrcRow][SrcCol] = empty;
							system("cls");
							CurrentPlayer = 1 - CurrentPlayer;
						}
					}
				}
			}
		}
		// Player2 White - read ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (stricmp(Player, "player2") == 0 || stricmp(Player, "1") == 0)
		{
			if (CurrentPlayer == White)
			{
				PrintBoard();

				if (CheckMate(CurrentPlayer == true)) // checkmate
				{
					printf("\033[1;31m CHECKMATE! \n Game Over\033[0m");

					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
				else if (Check(CurrentPlayer == true)) // check
					printf("\033[1;31mKing is in check \n\033[0m");

				printf("Current player: %s\n", (CurrentPlayer == White) ? "\033[1;34m White \033[0m" : "\033[1;35m Black \033[0m");

				Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // read data
				if (Success && Read1 > 0)
				{
					if (stricmp("quit,game", Buffer) == 0) // quit game
					{
						printf("Game over!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");

						FlushFileBuffers(hPipe1);
						CloseHandle(hPipe1);
						exit(0);
					}
					if (stricmp("save,game", Buffer) == 0) // save game
					{
						printf("Game saved!\n");

						Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // file name (date & time)

						sprintf(PlayerFile, "%s.txt", OppPlayerName);

						FILE* PlayerFilePtr = freopen(PlayerFile, "w", stdout);
						printf("%s\n", Buffer);
						printf("%s\n", (stricmp(Player, "Player1")) == 0 ? "Player1" : "Player2");
						printf("%s\n", (CurrentPlayer == White) ? "White" : "Black");
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 8; j++)
							{
								if (board[i][j] == empty)
									printf(".\n");
								else
									printf("%c\n", board[i][j]);
							}
						}
						FlushFileBuffers(hPipe1);
						CloseHandle(hPipe1);
						exit(0);
					}
					char* Token = strtok(Buffer, ","); // move piece
					while (Token != NULL)
					{
						strcpy(Src, Token);
						Token = strtok(NULL, ",");
						strcpy(Dst, Token);
						break;
					}
				}
				printf("\n");

				if (SrcRow >= 0 && SrcRow < 8 && SrcCol >= 0 && SrcCol < 8 && DstRow >= 0 && DstRow < 8 && DstCol >= 0 && DstCol < 8)
				{
					SrcCol = Src[0] - 'a';
					SrcRow = 8 - (Src[1] - '0');
					DstCol = Dst[0] - 'a';
					DstRow = 8 - (Dst[1] - '0');

					if (isupper(board[SrcRow][SrcCol]) && ValidMove(&SrcRow, &SrcCol, &DstRow, &DstCol))
					{
						if (board[DstRow][DstCol] == empty || islower(board[DstRow][DstCol]))
						{
							board[DstRow][DstCol] = board[SrcRow][SrcCol];
							board[SrcRow][SrcCol] = empty;
							system("cls");
							CurrentPlayer = 1 - CurrentPlayer;
						}
					}
				}
			}
			// Player2 Black - write ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			else  if (CurrentPlayer == Black)
			{
				PrintBoard();

				if (CheckMate(CurrentPlayer == true)) // checkmate
				{
					printf("\033[1;31m CHECKMATE! \n Game Over\033[0m");
					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
				else if (Check(CurrentPlayer == true)) // check
					printf("\033[1;31m King is in check \n\033[0m");

				printf("\n\033[1;30mIf you want to exit game, Enter \" QUIT GAME \"\n\033[0m");
				printf("\033[1;30mIf you want to save and exit game, Enter \" SAVE GAME \" \n\n\033[0m");
				printf("Current player: %s\n", (CurrentPlayer == White) ? "\033[1;34m White \033[0m" : "\033[1;35m Black \033[0m");
				printf("Enter source and destination: ");

				DWORD StartTime = GetTickCount(); //timer
				DWORD EndTime = 60000;

				while ((GetTickCount() - StartTime) < EndTime)
				{
					if (_kbhit())
					{
						scanf("%s %s", Src, Dst); // source and destination input
						printf("\n");

						if (stricmp("quit", Src) == 0 && stricmp("game", Dst) == 0) // quit game
						{
							char* Data = strcat(Src, ",");
							strcat(Data, Dst);
							strcpy(Buffer, Data);
							Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

							printf("Game over!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");

							FlushFileBuffers(hPipe1);
							CloseHandle(hPipe1);
							exit(0);
						}
						if (stricmp("save", Src) == 0 && stricmp("game", Dst) == 0) // save and quit game (file handling)
						{
							printf("Game saved!\n");

							char* Data = strcat(Src, ",");
							strcat(Data, Dst);
							strcpy(Buffer, Data);
							Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL); // save,game
							Success = ReadFile(hPipe1, Buffer, sizeof(Buffer), &Read1, NULL); // date & time

							sprintf(PlayerFile, "%s.txt", OppPlayerName);

							FILE* PlayerFilePtr = freopen(PlayerFile, "w", stdout);
							printf("%s\n", Buffer);
							printf("%s\n", (stricmp(Player, "Player1")) == 0 ? "Player1" : "Player2");
							printf("%s\n", (CurrentPlayer == White) ? "White" : "Black");
							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 8; j++)
								{
									if (board[i][j] == empty)
										printf(".\n");
									else
										printf("%c\n", board[i][j]);
								}
							}
							FlushFileBuffers(hPipe1);
							CloseHandle(hPipe1);
							exit(0);
						}
						if (SrcRow >= 0 && SrcRow < 8 && SrcCol >= 0 && SrcCol < 8 && DstRow >= 0 && DstRow < 8 && DstCol >= 0 && DstCol < 8)
						{
							SrcCol = Src[0] - 'a';
							SrcRow = 8 - (Src[1] - '0');
							DstCol = Dst[0] - 'a';
							DstRow = 8 - (Dst[1] - '0');

							if (islower(board[SrcRow][SrcCol]) && ValidMove(&SrcRow, &SrcCol, &DstRow, &DstCol))
							{
								if (board[DstRow][DstCol] == empty || isupper(board[DstRow][DstCol]))
								{
									board[DstRow][DstCol] = board[SrcRow][SrcCol];
									board[SrcRow][SrcCol] = empty;
									system("cls");

									char* Data = strcat(Src, ","); // write data
									strcat(Data, Dst);
									strcpy(Buffer, Data);
									Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

									CurrentPlayer = 1 - CurrentPlayer;
								}
							}
						}
						break;
					}
				}
				if ((GetTickCount() - StartTime) == EndTime)
				{
					strcpy(Buffer, "quit,game");
					Success = WriteFile(hPipe1, Buffer, (DWORD)strlen(Buffer) + 1, &Write1, NULL);

					printf("Time Out!\n%s is the winner\n", (CurrentPlayer == White) ? "\033[1;35m Black \033[0m" : "\033[1;34m White \033[0m");
					FlushFileBuffers(hPipe1);
					CloseHandle(hPipe1);
					exit(0);
				}
			}
		}
	}
	return 0;
}