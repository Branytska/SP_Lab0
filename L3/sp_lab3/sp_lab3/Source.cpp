#include <stdio.h>
#include "windows.h"
#include "iostream"
#include "tchar.h"
#include "processthreadsapi.h"

#define _CRT_SECURE_NO_WARNINGS

#define MAX_KEY 255
#define MAX_VALUE 16383

#define CLASSES_ROOT 1
#define CURRENT_USER 2
#define LOCAL_MACHINE 3
#define USERS 4
#define CURRENT_CONFIG 5
#define PERFORMANCE_DATA 6

const char PRINT_SUBKEYS = 'a';
const char PRINT_KEYS = 'b';
const char SEARCH = 'c';
const char SAVE = 'd';
const char EXIT = 'e';

using namespace std;

typedef struct {
	TCHAR    subkeyName[MAX_KEY];
	DWORD    sizeOfString;
	TCHAR    className[MAX_PATH] = TEXT("");
	DWORD    classString = MAX_PATH;
	DWORD    numberOfSubkeys = 0;
	DWORD    maxSize;
	DWORD    maxString;
	DWORD    numberValues;
	DWORD    maxValueName;
	DWORD    maxValueData;
	DWORD    sizeOfDescription;
	FILETIME lastTime;
} KEY_INFO, * pKEY_INFO;

bool MyGetKeyInfo(HKEY key, KEY_INFO* keyInfo);

void MyReadString(CHAR sBuffNewPath[], DWORD maxBuffSize, BOOL isUsedBeforeInputChar);

bool FindInReg(HKEY hKey, LPCWSTR reqStr, LPWSTR fullPath)
{
	KEY_INFO keyInfo = { 0 };
	DWORD retCode = ERROR_SUCCESS;
	LPWSTR newSubkeyPath;

	if (!MyGetKeyInfo(hKey, &keyInfo))
	{
		return false;
	}

	if (keyInfo.numberOfSubkeys)
	{
		for (int i = 0; i < keyInfo.numberOfSubkeys; i++)
		{
			keyInfo.sizeOfString = MAX_KEY;
			retCode = RegEnumKeyEx(hKey, i, keyInfo.subkeyName, &keyInfo.sizeOfString, NULL, NULL, NULL, NULL);
			if (retCode == ERROR_SUCCESS)
			{
				if (lstrcmpi(keyInfo.subkeyName, reqStr) == 0)
				{
					std::cout << "FOUND: " << fullPath << "\\" << keyInfo.subkeyName << endl;
				}

				newSubkeyPath = (LPWSTR)malloc(MAX_VALUE * sizeof(TCHAR));
				lstrcpy(newSubkeyPath, fullPath);
				lstrcat(newSubkeyPath, L"\\");
				lstrcat(newSubkeyPath, keyInfo.subkeyName);
				HKEY newKey = { 0 };

				if (RegOpenKeyEx(hKey, keyInfo.subkeyName, 0, KEY_ALL_ACCESS, &newKey) == ERROR_SUCCESS)
				{
					FindInReg(newKey, reqStr, newSubkeyPath);
				}
				free(newSubkeyPath);
			}
		}
	}

	if (keyInfo.numberValues)
	{
		LPWSTR lpValue = NULL;
		DWORD dwValue = keyInfo.maxValueName + 1;

		DWORD dwType = 0;

		LPBYTE lpData = NULL;
		DWORD dwData = 0;

		lpValue = (LPWSTR)malloc((keyInfo.maxValueName + 1) * sizeof(BYTE));

		for (int i = 0; i < keyInfo.numberValues; i++)
		{
			retCode = RegEnumValueA(hKey, i, (LPSTR)lpValue, &dwValue, NULL, NULL, NULL, &dwData);
			lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));

			dwValue = keyInfo.maxValueName + 1;

			retCode = RegEnumValueA(hKey, i, (LPSTR)lpValue, &dwValue, NULL, &dwType, lpData, &dwData);

			if (retCode == ERROR_SUCCESS)
			{
				if (lstrcmpi(lpValue, reqStr) == 0)
				{
					std::cout << "FOUND: " << fullPath << "\t" << lpValue << endl;
				}
				if (((dwType & REG_EXPAND_SZ) == REG_EXPAND_SZ) || ((dwType & REG_SZ) == REG_SZ))
				{
					if (lstrcmpi((LPWSTR)lpData, reqStr) == 0)
					{
						std::cout << "FOUND:" << fullPath << "\t " << lpValue << "\ndata:" << lpData << endl;
					}
				}
			}
		}
	}

	RegCloseKey(hKey);
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		printf("ERROR OF PRIV: %u\t", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		printf("Adjust error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("DO NOT HAVE PRIV\n");
		return FALSE;
	}
	return TRUE;
}

bool SaveIntoFile(HKEY hKey)
{
	CHAR filePath[MAX_PATH];
	DWORD retCode = ERROR_SUCCESS;
	std::cout << "Input path to new file:\n";

	MyReadString(filePath, MAX_PATH, false);

	retCode = RegSaveKey(hKey, (LPCWSTR)filePath, NULL);
	switch (retCode)
	{
	case ERROR_SUCCESS:
		std::cout << "OK:\n" << endl;
		RegCloseKey(hKey);
		return true;
		break;
	case ERROR_ALREADY_EXISTS:
		std::cout << "ERROR! File already exists!\t" << endl;
		break;
	default:
		std::cout << "ERROR! Cant save\t Error code:" << retCode << endl;
	}

	RegCloseKey(hKey);

	return false;
}

void PrintParams(HKEY key)
{
	DWORD i, retCode = ERROR_SUCCESS;
	KEY_INFO keyInfo = { 0 };

	DWORD dwType = 0;

	LPBYTE lpData = NULL;
	DWORD dwData = 0;

	LPSTR lpValue = NULL;
	DWORD dwValue = 0;

	MyGetKeyInfo(key, &keyInfo);

	if (keyInfo.numberValues)
	{
		std::cout << "\t Values count:" << keyInfo.numberValues << endl;
		lpValue = (LPSTR)malloc((keyInfo.maxValueName + 1) * sizeof(BYTE));
		dwValue = keyInfo.numberValues + 1;

		for (int i = 0; i < keyInfo.numberValues; i++)
		{
			retCode = RegEnumValueA(key, i, lpValue, &dwValue, NULL, NULL, NULL, &dwData);
			lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));

			dwValue = keyInfo.numberValues + 1;

			retCode = RegEnumValueA(key, i, lpValue, &dwValue, NULL, &dwType, lpData, &dwData);

			if (retCode == ERROR_SUCCESS)
			{
				if (strcmp(lpValue, "") == 0)
				{
					printf("\n(%d) Value name(default): %s\n", i + 1);
				}
				else
				{
					printf("\n(%d) Value name: %s\n", i + 1, lpValue);
				}

				DWORD data = *(DWORD*)(lpData);

				switch (dwType)
				{
				case REG_BINARY:
					printf("\tValue type: REG_BINARY\tValue data: binary\n");
					break;
				case REG_DWORD:
					printf("\tValue type: REG_DWORD\tValue data: %#x|%u\n", data, data);
					break;
				case REG_EXPAND_SZ:
					printf("\tValue type: REG_EXPAND_SZ\tValue data: %s\n", lpData);
					break;
				case REG_LINK:
					wprintf(L"\tValue type: REG_LINK\tValue data: %ws\n", lpData);
					break;
				case REG_SZ:
					printf("\tValue type: REG_SZ\tValue data: %s\n", lpData);
					break;
				case REG_NONE:
				{
					printf("\tValue type: REG_NONE\n    Value data: %x\n", *(DWORD*)(lpData));
				} break;
				default:
					printf("\tValue type: unknown\n    Value data: %x\n", *(DWORD*)(lpData));
					break;
				}
			}

			free(lpData);
		}

		free(lpValue);
	}

	RegCloseKey(key);
}

void PrintSubkey(HKEY key)
{
	DWORD i, retCode;
	KEY_INFO keyInfo = { 0 };

	MyGetKeyInfo(key, &keyInfo);

	if (keyInfo.numberOfSubkeys)
	{
		std::cout << "\tCount:" << keyInfo.numberOfSubkeys << endl;
		for (int i = 0; i < keyInfo.numberOfSubkeys; i++)
		{
			keyInfo.sizeOfString = MAX_VALUE;
			retCode = RegEnumKeyEx(key, i, keyInfo.subkeyName, &keyInfo.sizeOfString, NULL, NULL, NULL, NULL);
			if (retCode == ERROR_SUCCESS)
			{
				std::cout << "OK" << endl;
				std::wcout << "Index: " << i + 1 << "\tKEY: " << keyInfo.subkeyName << endl;
			}
			else
			{
				std::cout << "Error";
			}
		}
	}

	RegCloseKey(key);
}

void MyReadString(CHAR sBuffNewPath[], DWORD maxBuffSize, BOOL isUsedBeforeInputChar)
{
	memset(sBuffNewPath, '\0', sizeof(sBuffNewPath));
	if (isUsedBeforeInputChar)
		getchar();
	fgets(sBuffNewPath, maxBuffSize, stdin);
	if ((strlen(sBuffNewPath) > 0) && (sBuffNewPath[strlen(sBuffNewPath) - 1] == '\n'))
		sBuffNewPath[strlen(sBuffNewPath) - 1] = '\0';
}

bool MyGetKeyInfo(HKEY key, KEY_INFO* keyInfo)
{
	DWORD retCode = RegQueryInfoKey(key, (*keyInfo).className, &(*keyInfo).classString, NULL, &(*keyInfo).numberOfSubkeys, &(*keyInfo).maxSize,
		&(*keyInfo).maxString, &(*keyInfo).numberValues, &(*keyInfo).maxValueName, &(*keyInfo).maxValueData, &(*keyInfo).sizeOfDescription, &(*keyInfo).lastTime);

	if (retCode == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool OpenKey(HKEY** hKey, DWORD dwOpenAccess, LPWSTR fullPath)
{
	HKEY predKey;
	if (fullPath != NULL)  memset(fullPath, '\0', sizeof(fullPath));
	int choice = 0;

	std::cout << "Predefined keys:" << endl;
	std::cout << "1 - HKEY_CLASSES_ROOT" << endl;
	std::cout << "2 - HKEY_CURRENT_USER" << endl;
	std::cout << "3 - HKEY_LOCAL_MACHINE" << endl;
	std::cout << "4 - HKEY_USERS" << endl;
	std::cout << "5 - HKEY_CURRENT_CONFIG" << endl;
	std::cout << "6 - HKEY_PERFORMANCE_DATA" << endl;
	std::cout << "Your key:";

	cin >> choice;

	switch (choice)
	{
	case CLASSES_ROOT:
	{
		predKey = HKEY_CLASSES_ROOT;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_CLASSES_ROOT\\");
	} break;
	case CURRENT_USER:
	{
		predKey = HKEY_CURRENT_USER;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_CURRENT_USER\\");
	} break;
	case LOCAL_MACHINE:
	{
		predKey = HKEY_LOCAL_MACHINE;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_LOCAL_MACHINE\\");
	} break;
	case USERS:
	{
		predKey = HKEY_USERS;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_USERS\\");
	} break;
	case CURRENT_CONFIG:
	{
		predKey = HKEY_CURRENT_CONFIG;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_CURRENT_CONFIG\\");
	} break;
	case PERFORMANCE_DATA:
	{
		predKey = HKEY_PERFORMANCE_DATA;
		if (fullPath != NULL) lstrcpy(fullPath, L"HKEY_PERFORMANCE_DATA\\");
	} break;
	default:
		return false;
	}

	CHAR keyArr[MAX_KEY] = { '\0' };
	LPCWSTR key = (LPCWSTR)keyArr;
	std::cout << "Path to subkey:\n";
	MyReadString((CHAR*)key, MAX_KEY, true);

	if (RegOpenKeyEx(predKey, key, 0, dwOpenAccess, *hKey) == ERROR_SUCCESS)
	{
		if (fullPath != NULL) lstrcat(fullPath, key);
		return true;
	}
	return false;
}

void PrintMenu()
{
	cout << "PRINT_SUBKEYS - Print a list of subkeys by key name\n";
	cout << "PRINT_KEYS - Print a list of keys parameters with their value and type\n";
	cout << "SEARCH - Searches the registry for a given string in the key names, key values and their types.\n\t Base key set user\n";
	cout << "SAVE - Save key as a file\n";
	cout << "EXIT - Exit\n";
}

int main()
{

	while (true)
	{
		char choice = 0;
		HKEY hKey = { 0 };
		PHKEY phKey = &hKey;

		PrintMenu();
		cin >> choice;
		switch (choice)
		{
		case PRINT_SUBKEYS:
		{
			if (OpenKey(&phKey, KEY_READ, NULL) == true)
			{
				PrintSubkey(hKey);
			}
		}break;
		case PRINT_KEYS:
		{
			if (OpenKey(&phKey, KEY_QUERY_VALUE, NULL) == true)
			{
				PrintParams(hKey);
			}
		} break;
		case SEARCH:
		{
			CHAR fullPath[MAX_PATH];
			if (OpenKey(&phKey, KEY_ALL_ACCESS, (LPWSTR)fullPath) == true)
			{
				CHAR reqString[MAX_PATH] = { '\0' };
				cout << "Input string for searching:";
				MyReadString(reqString, MAX_PATH, false);
				FindInReg(hKey, (LPCWSTR)reqString, (LPWSTR)fullPath);
			}
		} break;
		case SAVE:
		{
			HANDLE hToken;

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			{
				cout << "Cant get access rights (SE_BACKUP_NAME)\n Error code:" << GetLastError() << endl;
			}
			if (SetPrivilege(hToken, SE_BACKUP_NAME, true))
			{
				HKEY hKey = { 0 };
				PHKEY phKey = &hKey;
				if (OpenKey(&phKey, KEY_ALL_ACCESS, NULL) == true)
				{
					SaveIntoFile(hKey);
				}
			}
		} break;
		case EXIT:
		{
			return 1;
		} break;
		default:
			cout << "Error choice, try again\n";
			break;
		}
	}

	return 0;
}