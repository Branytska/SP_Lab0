#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>

#define CHANGE_RECORD 1
#define DELETE_RECORD 2
#define ADD_RECORD 3
#define READ_ALL_RECORD 4
#define EXIT 0

using namespace std;
LPCWSTR RECB = L"records.txt";

struct rec {
	UINT number;
	FILETIME creationTime;
	TCHAR data[81];
	UINT changesCounter;
};
struct header {
	UINT not_Null;
	UINT SizeFile;
};

header ReadCounts() {
	HANDLE hFile1 = CreateFile(RECB, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	header h;
	DWORD jk;
	ReadFile(hFile1, &h, sizeof(header), &jk, NULL);
	if (jk == 0) {
		h.not_Null = 0;
		h.SizeFile = 0;
	}
	CloseHandle(hFile1);
	return h;
}

vector<rec> ReadRecs() {
	HANDLE hFile1 = CreateFile(RECB, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, sizeof(header), NULL, FILE_BEGIN);
	vector<rec> datas(0);

	rec i;
	DWORD jk = 0;
	for (;;) {
		bool k = ReadFile(hFile1, &i, sizeof(rec), &jk, NULL);
		if (jk == 0) break;
		datas.push_back(i);
	}
	CloseHandle(hFile1);

	return datas;
}
void PrintAllInformation() {
	SYSTEMTIME cm;
	vector <rec> datas = ReadRecs();
	header count = ReadCounts();
	cout << "Size of file: " << count.SizeFile << " bytes; Not null records: " << count.not_Null << endl;
	for (int i = 0; i < datas.size(); i++) {
		FileTimeToSystemTime(&datas[i].creationTime, &cm);
		cout << "Number = " << datas[i].number << endl;
		cout << cm.wDay << "." << cm.wMonth << "." << cm.wYear << " " << cm.wHour << ":" << cm.wMinute << endl;
		wcout << "Data = " << datas[i].data << endl;
		cout << "Counter = " << datas[i].changesCounter << endl;
		cout << "------------------------------" << endl;
		cout << endl;
	}
}

void Add_rec() {
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	SYSTEMTIME k, s;
	GetLocalTime(&k);

	FILETIME ft;
	rec f;
	SystemTimeToFileTime(&k, &ft);
	FileTimeToSystemTime(&ft, &s);
	cout << "Input string: ";
	scanf("%80S", f.data);

	if (lstrcmp(f.data, L"*") == 0) {
		f.number = data.size();
		f.creationTime = ft;
		lstrcpy(f.data, L"");
		f.changesCounter = 0;
	}
	else {
		f.number = data.size();
		f.creationTime = ft;
		f.changesCounter = 0;
		count.not_Null++;
	}

	data.push_back(f);

	count.SizeFile = sizeof(count) + sizeof(rec) * data.size();
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	LPDWORD temp = NULL;
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), temp, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), temp, NULL);
	}
	CloseHandle(hFile1);
}
void Change_rec() {
	SYSTEMTIME cm;
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	UINT n;
	cout << "Enter num of record you want to change: " << endl;
	cin >> n;
	cout << "\nIt's your record" << endl;

	FileTimeToSystemTime(&data[n].creationTime, &cm);

	cout << "Number = " << data[n].number << endl;
	cout << cm.wDay << "." << cm.wMonth << "." << cm.wYear << " " << cm.wHour << ":" << cm.wMinute << endl;
	wcout << "Data = " << data[n].data << endl;
	cout << "Counter = " << data[n].changesCounter << endl;
	cout << "------------------------------" << endl;
	cout << endl;

	cout << "\nEnter new string:";
	cout << "\nFor empty string enter '*'): ";
	TCHAR str[81];

	lstrcpy(str, data[n].data);
	scanf("%80S", data[n].data);
	if (lstrcmp(data[n].data, L"*") == 0) {
		lstrcpy(data[n].data, L"");
	}
	if (lstrcmp(str, L"") == 0 && lstrcmp(data[n].data, L"") != 0) {
		count.not_Null++;
	}
	if (lstrcmp(str, L"") != 0 && lstrcmp(data[n].data, L"") == 0) {
		count.not_Null--;
	}
	data[n].changesCounter++;
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;

	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);

	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);

}
void delete_rec() {
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	int i;
	cout << "Enter num record to delete: ";
	cin >> i;

	if (lstrcmp(data[i].data, L"") != 0)
		count.not_Null--;

	data.erase(data.begin() + i);
	vector<rec>(data).swap(data);
	for (int k = 0; k < data.size(); k++) {
		data[k].number = k;
	}
	count.SizeFile = sizeof(count) + sizeof(rec) * data.size();
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;

	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);

	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);
}
int main() {
	int key;
	while (true) {
		cout << "Programm for your document with records" << endl;
		cout << "Enter 1 to change record" << endl;
		cout << "Enter 2 to delete record" << endl;
		cout << "Enter 3 to add record" << endl;
		cout << "Enter 4 to read all records" << endl;
		cout << "Enter 0 to exit" << endl;
		cout << "Your choice: ";
		cin >> key;

		switch (key) {
		case EXIT:
			exit(EXIT_SUCCESS);
			break;
		case CHANGE_RECORD:
			Change_rec();
			break;
		case DELETE_RECORD:
			delete_rec();
			break;
		case ADD_RECORD:
			Add_rec();
			break;
		case READ_ALL_RECORD:
			PrintAllInformation();
			break;
		default:
			cout << "Change your choice" << endl;
			Sleep(100);
			system("cls");
		}
		system("pause");
		system("cls");
	}
}
