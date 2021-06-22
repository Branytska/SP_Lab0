#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <locale.h>
#include <iostream>

#define ACTION_UNPACK 1
#define ACTION_PACK 2

LPCSTR unpack_7z = "C:\\Program Files\\NVIDIA Corporation\\NVIDIA GeForce Experience\\7z.exe e ";
LPCSTR pack_7z = "C:\\Program Files\\NVIDIA Corporation\\NVIDIA GeForce Experience\\7z.exe a -tzip ";

void unpack_files(LPSTR unpackFile, LPSTR resultFile);
void pack_files(LPSTR unpackFile, LPSTR resultFile);
void print_error(); 
