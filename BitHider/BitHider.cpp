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
	AesFileSelection mode;
	char key_choice, iv_choice;

	wprintf(L"Select algorithm mode:\n0) Encrypt\n1) Decrypt\n");
	scanf("%d", &mode);

	printf("This program uses high quality entropy for generating the Key and the IV\nit is NOT recommended to manually input them\n\n");

	printf("Do you want to manually input the IV (HEX ONLY)? <y/n> :");
	scanf("%c", &iv_choice);
	getchar();

	printf("Do you want to manually input the Key (HEX ONLY)? <y/n> :");
	scanf("%c", &key_choice);
	getchar();

	wprintf(L"Select Input File\n");

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
		//GetIv();
	}
	else {
		Data.GenerateIv();
	}

	if (key_choice == 'y') {
		//GetKey();
	}
	else {
		Data.GenerateKey();
	}
	

	Data.ExecSelectedAction();

	
	system("PAUSE");
    return 0;
}

