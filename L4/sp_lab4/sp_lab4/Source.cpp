#include "Header.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	int input;

	while (true)
	{
		printf("1 - Unpack file\n");
		printf("2 - Pack files\n");

		std::cin >> input;

		switch (input)
		{
		case ACTION_UNPACK:
		{
			LPSTR unpackFile = new CHAR[MAX_PATH];
			LPSTR resultFile = new CHAR[MAX_PATH];

			printf("Path to zip\n");
			std::cout << "Your input" << std::endl;
			scanf("%s", unpackFile);

			printf("Path to result\n");
			std::cout << "Your input" << std::endl;
			scanf("%s", resultFile);

			unpack_files(unpackFile, resultFile);
			break;
		}
		case ACTION_PACK:
		{
			LPSTR packFile = new CHAR[MAX_PATH];
			LPSTR resultFile = new CHAR[MAX_PATH];

			printf("Path to File/Directory\n");
			std::cout << "Your input" << std::endl;
			scanf("%s", packFile);

			printf("Path to result (.zip)\n");
			std::cout << "Your input" << std::endl;
			scanf("%s", resultFile);
			pack_files(packFile, resultFile);
			break;
		}

		default: printf("Invalid input!\n"); break;
		}

		system("pause");
		system("cls");
	}
}

void unpack_files(LPSTR unpackFile, LPSTR resultFile)
{
	LPSTR commandLine = new CHAR[MAX_PATH];
	ZeroMemory(commandLine, MAX_PATH);

	// set path to 7z and .zip archive (-e)
	strncpy(commandLine, unpack_7z, MAX_PATH - strlen(commandLine));
	strncat(commandLine, unpackFile, MAX_PATH - strlen(commandLine));

	// -o set result directory
	strncat(commandLine, " -o", MAX_PATH - strlen(commandLine));
	strncat(commandLine, resultFile, MAX_PATH - strlen(commandLine));

	// -y auto answer YES
	strncat(commandLine, " -y", MAX_PATH - strlen(commandLine));

	//Create pipes
	HANDLE hReadPipe;
	HANDLE hWritePipe;

	//security attributes for pipes
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0);
	if (hReadPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE)
		exit(1);

	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
		exit(1);

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(si);
	//Need only errors
	//si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		print_error();
	}
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD readed = 0;
		LPSTR result = new CHAR[1024];
		ZeroMemory(result, 1024);
		OVERLAPPED overlapped;
		while (ReadFile(hReadPipe, result, 1024, &readed, &overlapped))
		{
			printf("%s", result);
		}
	}

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
void pack_files(LPSTR packFile, LPSTR resultFile)
{

	LPSTR commandLine = new CHAR[MAX_PATH];
	ZeroMemory(commandLine, MAX_PATH);
	// set path to 7z and result archive (a)
	strncpy(commandLine, pack_7z, MAX_PATH - strlen(commandLine));
	strncat(commandLine, resultFile, MAX_PATH - strlen(commandLine));

	strncat(commandLine, " ", MAX_PATH - strlen(commandLine));
	strncat(commandLine, packFile, MAX_PATH - strlen(commandLine));

	//Create pipes
	HANDLE hReadPipe;
	HANDLE hWritePipe;

	//security attributes for pipes
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0);
	if (hReadPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE)
		exit(1);

	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
		exit(1);

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(si);
	//Need only errors
	//si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));


	if (!CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		print_error();
	}
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD readed = 0;
		LPSTR result = new CHAR[1024];
		ZeroMemory(result, 1024);
		OVERLAPPED overlapped;
		while (ReadFile(hReadPipe, result, 1024, &readed, &overlapped))
		{
			printf("%s", result);
		}
	}
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void print_error()
{
	printf("\nSomething went wrong(\n");
	LPVOID e_mess;
	DWORD e_code = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		e_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&e_mess, 0, NULL);
	char* err_mess = (char*)e_mess;
	wprintf(L"ERROR code: 0x%x\n", e_code);
	wprintf(L"ERROR message: %s\n", err_mess);
}