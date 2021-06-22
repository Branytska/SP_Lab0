#include "Header.h"


int main()
{
	HANDLE mutex = CreateMutexA(NULL, FALSE, "MyMutex");

	if (WaitForSingleObject(mutex, 0) == WAIT_OBJECT_0)
	{
		std::cout << "Is Running" << std::endl;
		std::cout << "Press any key" << std::endl;
		getchar();
		ReleaseMutex(mutex);
	}
	else
	{
		printf("Can't start program!");
		getchar();
	}

	CloseHandle(mutex);
	return 0;
}