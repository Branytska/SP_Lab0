#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>

const int BytesToRead = 2048;

void FromUnicode(char*);
void ToUnicode(char*);
bool OpenAndCreateFiles(char*, HANDLE*, HANDLE*);

void FromUnicode(char* path) {
	HANDLE readFile;
	HANDLE writeFile;

	if (!OpenAndCreateFiles(path, &readFile, &writeFile))
		return;

	BYTE buffer[BytesToRead];
	DWORD dwBytes;

	while (ReadFile(readFile, buffer, BytesToRead, (LPDWORD)&dwBytes, NULL))
	{
		if (dwBytes == 0)
			break;

		int uLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buffer, dwBytes, NULL, 0);
		wchar_t* uStr = new wchar_t[uLength];
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buffer, dwBytes, uStr, uLength);

		int aLength = WideCharToMultiByte(CP_ACP, 0, uStr, uLength, NULL, 0, NULL, NULL);
		char* aStr = (char*)calloc(aLength, sizeof(char));

		WideCharToMultiByte(CP_ACP, 0, uStr, uLength, aStr, aLength, NULL, NULL);

		if (!WriteFile(writeFile, aStr, aLength * sizeof(char), &dwBytes, NULL)) {
			printf("Error of encoding");
			break;
		}
	}
	CloseHandle(readFile);
	CloseHandle(writeFile);
}

void ToUnicode(char* path) {
	HANDLE readFile;
	HANDLE writeFile;

	if (!OpenAndCreateFiles(path, &readFile, &writeFile))
		return;

	BYTE buffer[BytesToRead];
	DWORD dwBytes;

	while (ReadFile(readFile, buffer, BytesToRead, (LPDWORD)&dwBytes, NULL))
	{
		if (dwBytes == 0)
			break;

		int uLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buffer, dwBytes, NULL, 0);
		wchar_t* uStr = (wchar_t*)calloc(uLength, sizeof(wchar_t));

		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buffer, dwBytes, uStr, uLength);

		if (!WriteFile(writeFile, uStr, uLength * sizeof(wchar_t), (LPDWORD)&dwBytes, NULL)) {
			printf("Error of encoding");
			break;
		}
	}
}

bool OpenAndCreateFiles(char* path, HANDLE* readFile, HANDLE* writeFile) {
	*readFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	*writeFile = CreateFileA("result.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (*readFile == INVALID_HANDLE_VALUE || *writeFile == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		return true;
	}
}

int main(int argc, char* argv[]) {

	if (argc == 3) {
		if (strcmp(argv[1], "-u") == 0) {
			FromUnicode(argv[2]);
		}
		else if (strcmp(argv[1], "-a") == 0) {
			ToUnicode(argv[2]);
		}
		else {
			printf("Invalid arguments!");
		}
	}
	else {
		printf("Invalid arguments!");
	}
	return 0;
}