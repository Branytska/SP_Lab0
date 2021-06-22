#include "Header.h"


int find_largest_divisor(int num)
{
	int j = num / 2;
	for (int i = j; i >= 2; i--)
	{
		if (num % i == 0)
			return i;
	}
}

int sum_elements(int* arr)
{
	int result = 0;

	for (int i = 0; i < ARRAY_MAX; i++)
	{
		result += arr[i];
	}

	return result;
}

void print_array(int* arr)
{
	for (int i = 0; i < ARRAY_MAX; i++) {
		printf("%i ", arr[i]);
	}
}


void start(int count)
{
	InitializeCriticalSection(&criticalSection);
	int** arrays = new int* [count];
	tlsIndex = TlsAlloc();

	for (int i = 0; i < count; i++)
	{
		arrays[i] = new int[ARRAY_MAX];
		for (int j = 0; j < ARRAY_MAX; j++)
			arrays[i][j] = rand() % 90 + 10;

		threads[i] = CreateThread(NULL, 0, thread_function, arrays[i], NULL, NULL);
	}
}

DWORD WINAPI thread_function(LPVOID param)
{
	EnterCriticalSection(&criticalSection);
	printf("My ID: %u\n", GetCurrentThreadId());

	int* array = (int*)param;
	TlsSetValue(tlsIndex, (LPVOID)(new int[ARRAY_MAX]));
	int sum = 0;

	printf("My array: ");
	print_array(array);

	process_array(array, (int*)TlsGetValue(tlsIndex));

	printf("\nProcessed array into: ");
	print_array((int*)TlsGetValue(tlsIndex));

	sum = sum_elements((int*)TlsGetValue(tlsIndex));
	printf("\nSum = %d\n\n", sum);

	LeaveCriticalSection(&criticalSection);
	return 0;
}

void process_array(int* mainArr, int* resultArr)
{
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		resultArr[i] = find_largest_divisor(mainArr[i]);
	}
}

int main()
{
	srand(time(0));
	int count;

	printf("Input count of threads\n");
	std::cout << "NUmber of threds = ";
	std::cin >> count;

	threads = new HANDLE[count];

	InitializeCriticalSection(&criticalSection);
	start(count);
	WaitForMultipleObjects(count, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);
	return 0;
}

