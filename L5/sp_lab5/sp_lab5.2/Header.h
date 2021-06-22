#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

HANDLE semaphore;

int random_int(int min, int max);
DWORD WINAPI thread_function(LPVOID param);