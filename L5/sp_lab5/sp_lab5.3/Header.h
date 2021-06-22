#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#define ARRAY_MAX 134217727
CRITICAL_SECTION section;
int* array;

DWORD WINAPI thread_function_min(LPVOID use_critical_section);
DWORD WINAPI thread_function_max(LPVOID use_critical_section);
DWORD WINAPI thread_function_avg(LPVOID use_critical_section);
void generate_array(int* array);
int random_int(int min, int max);