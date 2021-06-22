#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

#define ARRAY_MAX 10

CRITICAL_SECTION criticalSection;
HANDLE* threads;
int tlsIndex;

void start(int count);
DWORD WINAPI thread_function(LPVOID arr);
void process_array(int* mainArr, int* resultArr);
int find_largest_divisor(int num);
int sum_elements(int* arr);
void print_array(int* arr);