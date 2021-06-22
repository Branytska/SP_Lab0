#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>;
#include <iostream>
#include <locale.h>

#define SET_DIRECTORY 0
#define PRINT_DIRECTORY 1
#define COPY_FILE 2
#define CREATE_DIRECTORY 3
#define FULL_INFORMATION 4
#define DELETE_DIRECTORY 5

using namespace std;

void SetDirectory(TCHAR* buffer) {
	for (;;) {
		cout << "Enter full way to the directory" << endl;
		cout << "Use double slashes like this C://" << endl;
		TCHAR temp[MAX_PATH];
		cin.get();
		scanf("%80S", temp);
		BOOL res = SetCurrentDirectory(temp);
		if (!res)
			cout << "\nNot correct name of directory" << endl;
		else {
			GetCurrentDirectory(sizeof(temp), temp);
			wcout << "Your current directory is: " << temp << endl;
			lstrcpy(buffer, L"");
			lstrcpy(buffer, temp);
			return;
		}
	}
}

void PrintDirectory(TCHAR* buffer) {
	cout << "Files in current directory: " << endl;
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	TCHAR find[MAX_PATH];
	lstrcpy(find, buffer);
	lstrcat(find, L"\\*");
	hf = FindFirstFile((LPCWSTR)find, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			SYSTEMTIME systime;
			FILETIME localfiletime;
			FileTimeToLocalFileTime(&FindFileData.ftCreationTime, &localfiletime);
			FileTimeToSystemTime(&localfiletime, &systime);
			LONGLONG size = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
			wcout << FindFileData.cFileName << "\t" << size << " bytes " << "\t" << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

void CopyFileInCurDir(TCHAR* buffer) {
	BOOL k = true;
	for (;;) {
		cout << "Input file name in current directory: ";
		TCHAR find[MAX_PATH], s[MAX_PATH], newDirectory[MAX_PATH];
		lstrcpy(find, buffer);
		cin.get();
		scanf("%80S", s);
		lstrcat(find, L"\\");
		lstrcat(find, s);
		cout << "\nInput directory to copy (E://...): ";
		//cin.get();
		scanf("%80S", newDirectory);
		lstrcat(newDirectory, L"\\");
		lstrcat(newDirectory, s);
		k = CopyFile(find, newDirectory, false);
		if (k) return;
		else cout << "Not correct name(s)\n";
	}
}
void MyCreateDirectory(TCHAR* buffer) {
	TCHAR newDir[MAX_PATH], way[MAX_PATH];
	cout << "Input new directory name: ";
	cin.get();
	scanf("%80S", newDir);
	lstrcpy(way, buffer);
	lstrcat(way, L"\\");
	lstrcat(way, newDir);
	if (CreateDirectory(way, NULL))
		cout << "Directory created" << endl;
	else
		cout << "error create directory" << endl;
}

void FullInformation(TCHAR* buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name: ";
	cin.get();
	scanf("%80S", file);
	lstrcpy(way, buffer);
	lstrcat(way, L"\\");
	lstrcat(way, file);
	HANDLE hFile1 = CreateFile(way, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BY_HANDLE_FILE_INFORMATION infa;
	BOOL f = GetFileInformationByHandle(hFile1, &infa);
	CloseHandle(hFile1);
	if (!f) {
		cout << "Not correct name of file\n";
	}
	else {
		cout << "File is ";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_HIDDEN)
			cout << "hidden";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_SYSTEM)
			cout << "system";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			cout << "archive";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_COMPRESSED)
			cout << "compressed";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_NORMAL)
			cout << "normal";
		cout << ".\n";
		SYSTEMTIME systime;
		FILETIME localfiletime;
		FileTimeToLocalFileTime(&infa.ftCreationTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Creation time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;

		FileTimeToLocalFileTime(&infa.ftLastWriteTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Last Write time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;
	}
}
void MyDelete(TCHAR* buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name to delete: ";
	cin.get();
	scanf("%80S", file);
	lstrcpy(way, buffer);
	lstrcat(way, L"\\");
	lstrcat(way, file);
	BOOL g = DeleteFile(way), k;
	if (!g) {
		k = RemoveDirectory(way);
	}
	if (!g && !k) {
		DWORD lastErrorCode = GetLastError();
		void* cstr;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&cstr,
			0,
			NULL
		);
		PTCHAR res((LPTSTR)cstr);
		printf("%s", res);
	}
}

int main() {

	setlocale(LC_ALL, "rus");

	while (true) {
		TCHAR buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);

		int choice;

		system("cls");

		wcout << "Your current directory: " << buffer << endl;
		cout << "\t\tMenu" << endl;
		cout << "0 - Set Directory" << endl;
		cout << "1 - Print Directory" << endl;
		cout << "2 - Copy File" << endl;
		cout << "3 - Make Directory" << endl;
		cout << "4 - More information about file" << endl;
		cout << "5 - Delete file or empty directory" << endl;
		cout << "Your choice: ";

		cin >> choice;

		switch (choice) {
		case SET_DIRECTORY:
			SetDirectory(buffer);
			break;
		case PRINT_DIRECTORY:
			PrintDirectory(buffer);
			break;
		case COPY_FILE:
			CopyFileInCurDir(buffer);
			break;
		case CREATE_DIRECTORY:
			MyCreateDirectory(buffer);
			break;
		case FULL_INFORMATION:
			FullInformation(buffer);
			break;
		case DELETE_DIRECTORY:
			MyDelete(buffer);
			break;
		}
		system("pause");
	}
}
