// Simple demo that prints the command line.
#include "Windows.h"
#pragma comment( lib, "kernel32" )
void myMain() {
	HANDLE std_out = ::GetStdHandle( STD_OUTPUT_HANDLE );	
	char* lpCommandLine = ::GetCommandLine();
	DWORD bytes_written = 0;
	::WriteFile( std_out, lpCommandLine, lstrlen( lpCommandLine ), 
		&bytes_written, NULL );
	::ExitProcess( 0 );
}