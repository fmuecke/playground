#include <iostream>
#include <Windows.h>
#include <string>
#include <map>

using namespace std;


unsigned int isRegMonPresent()
{
	map<unsigned int, string> A;
	A['e'] = "E";
	A['f'] = 'F';
	A['i'] = "I";
	A['l'] = "L";
	A['m'] = "M";
	A['n'] = "N";
	A['o'] = "O";

	string names[] = {
		"BW2K",
		A['f'] + A['i'] + A['l'] + A['e'] + A['m'] + A['o'] + A['n'],//"FILEMON",
		"FILEVXD",
		"FILEMON701",
		"ICEDUMP",
		"NTICE",
		"REGMON",
		"REGVXD",
		"REGMON701",
		"SICE",
		"TWX2002"
	};
	HANDLE hRegMon;
	for( unsigned int i = 0; i < sizeof(names)/sizeof(string); i++ )
	{
		string tmp = "\\\\.\\";
		tmp.append(names[i]);
		hRegMon = CreateFileA( tmp.c_str(), 
			GENERIC_READ, 
			FILE_SHARE_READ,
			NULL, 
			OPEN_EXISTING, 
			0, 
			NULL );
		if( hRegMon != INVALID_HANDLE_VALUE )
		{	
			CloseHandle(hRegMon);
			cout << "Found: " << tmp << endl;
		}
		else
		{
			DWORD err = GetLastError();
			if( err == ERROR_ACCESS_DENIED )
				cout << "Found: " << tmp << endl;
			else 
			if( err != ERROR_FILE_NOT_FOUND )
				cout << "Found: " << tmp << endl;
		}
	}
	return 0;
}

bool isOllyPresent()
{
	int ollyPresent = 0;
	__asm
	{
		;ASSUME FS:NOTHING
		MOV ECX, FS:[18h]
		MOV ECX, DWORD PTR DS:[ECX+30h]
		MOV EBX, ECX
		MOVZX ECX, BYTE PTR DS:[ECX+2h]
		TEST ECX, ECX
	    JNE _OllyDetected
		//MOV DWORD PTR[EBX],12345678h
		//CALL IsDebuggerPresent
		//CMP EAX,34h
	    //JNE _OllyDetected
		JMP _OllyCheckEnd
_OllyDetected:
		mov ollyPresent, 1;
_OllyCheckEnd:
	}
	return ollyPresent != 0;
}
int main()
{
	isRegMonPresent();
	cout << isOllyPresent() << endl;
	char fmon[8] = "FILEMON";
	char xfmon[8] = "xxxxxxx";
	char yfmon[8] = "yyyyyyy";
	
	for( unsigned int i=0; i<7; i++)
	{
		xfmon[i] = ~fmon[i];
		yfmon[i] = ~xfmon[i];
	}
		cout << fmon << endl;
		cout << xfmon << endl;
		cout << yfmon << endl;
	
	system("pause");

	return 0;
}

//int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd )
//{
//	isRegMonPresent();
//	return 0;
//
//}