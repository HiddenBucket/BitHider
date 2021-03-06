// CryptoMaster.cpp : Defines the entry point for the console application.



#include "stdafx.h"					//Precompiled Header

#include "CBBS.hpp"					//Blum Blum Shub Random Number Generator
#include "cryptoTK.hpp"				//Crypto Tool Kit
#include "aes.hpp"					//AES algorithm library
#include "aes_file.h"







int main() {

	TCHAR path[MAX_STR_LEN] = { NULL };
	OPENFILENAME ofn;					// common dialog box structure
	HWND hwnd = NULL;					// owner window
	AesFileSelection mode = Encrypt;
	CHAR key_choice = 0, iv_choice = 0 ;
	HANDLE hStdout, hStdin, hIn, hOut;
	DWORD cRead, cWritten, fdwMode, fdwOldMode;

	LPCSTR prompt1 = "BitHider is a nice little low-footprint encrypting\\decrypting tool that will help you to protect your important files.\nLet's get started!\n\n1\\5\n\n";
	LPCSTR prompt2 = "Select algorithm mode, just insert the number corresponding to the mode:\n0) Encrypt\n1) Decrypt\n\n2\\5\n\n";
	LPCSTR prompt3 = "Mode:";
	LPCSTR prompt4 = "This program uses high quality entropy for generating the Key and the IV\nit is not advised to manually input them\n";
	LPCSTR prompt5 = "Do you want to manually input the IV (HEX ONLY)? <y/n>\n\n3\\5\n\n";
	LPCSTR prompt6 = "Do you want to manually input the KEY (HEX ONLY)? <y/n>\n\n4\\5\n\n";
	LPCSTR prompt7 = "Select Input File\n\n5\\5\n\n";

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		ErrorExit( (LPTSTR)L"GetStdInputHandle");
	}

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		ErrorExit((LPTSTR)L"GetStdOutputHandle");
	}

	hIn = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hIn == INVALID_HANDLE_VALUE) {
		ErrorExit((LPTSTR)L"CreateConsoleInputHandle");
	}

	hOut = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		ErrorExit((LPTSTR)L"CreateConsoleOutputHandle");
	}

	GetConsoleMode(hStdin, &fdwOldMode);
	fdwMode = fdwOldMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	SetConsoleMode(hStdin, fdwMode);


	//1st stage

	WriteConsoleA(hStdout, prompt1, lstrlenA(prompt1), &cWritten, NULL);

	PressAnyKey(hStdin, hStdout, L"\nPress any key to proceed to the next stage");

	ClearOutputBuffer(hStdout);

	//2nd stage

	WriteConsoleA(hStdout, prompt2, lstrlenA(prompt2), &cWritten, NULL);

	do {
	WriteConsoleA(hStdout, prompt3, lstrlenA(prompt3), &cWritten, NULL);
	ReadConsoleA(hStdin, &mode, 1, &cRead, NULL);
	WriteConsoleA(hStdout, &mode, cRead, &cWritten, NULL);
	mode = AesFileSelection(mode - 48);
	NewLine(hStdout);
	} while ( (mode != 0) && (mode != 1));

	PressAnyKey(hStdin, hStdout, L"\nPress any key to proceed to the next stage");

	ClearOutputBuffer(hStdout);

	//3rd stage
	
	SetColor(hStdout, 12);
	WriteConsoleA(hStdout, prompt4, lstrlenA(prompt4), &cWritten, NULL);
	SetColor(hStdout, 15);
	WriteConsoleA(hStdout, prompt5, lstrlenA(prompt5), &cWritten, NULL);

	do {
	WriteConsoleA(hStdout, prompt3, lstrlenA(prompt3), &cWritten, NULL);
	ReadConsoleA(hStdin, &iv_choice, 1, &cRead, NULL);
	WriteConsoleA(hStdout, &iv_choice, cRead, &cWritten, NULL);
	NewLine(hStdout);
	} while ((iv_choice != 'y') && (iv_choice != 'n'));

	PressAnyKey(hStdin, hStdout, L"\nPress any key to proceed to the next stage");

	ClearOutputBuffer(hStdout);

	//4th stage
	
	WriteConsoleA(hStdout, prompt6, lstrlenA(prompt6), &cWritten, NULL);

	do {
	WriteConsoleA(hStdout, prompt3, lstrlenA(prompt3), &cWritten, NULL);
	ReadConsoleA(hStdin, &key_choice, 1, &cRead, NULL);
	WriteConsoleA(hStdout, &key_choice, cRead, &cWritten, NULL);
	NewLine(hStdout);
	} while ((key_choice != 'y') && (key_choice != 'n'));

	PressAnyKey(hStdin, hStdout, L"\nPress any key to proceed to the next stage");

	ClearOutputBuffer(hStdout);

	//5th stage

	WriteConsoleA(hStdout, prompt7, lstrlenA(prompt7), &cWritten, NULL);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = NULL;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);


	

 	AesFile Data(path,mode);


	if (key_choice != 'y' || iv_choice != 'y') {
		Data.InitGen();
	}


	if (iv_choice == 'y') {
		Data.GetIv();
	}
	else {
		Data.GenerateIv();
	}

	if (key_choice == 'y') {
		Data.GetKey();
	}
	else {
		Data.GenerateKey();
	}
	

	Data.ExecSelectedAction();

	Data.PrintInfo();
	system("PAUSE");
    return 0;
}

