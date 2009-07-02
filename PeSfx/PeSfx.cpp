//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Filename: pesfx.cpp
// Created:  2009/05/09 13:20
// Author:   Florian Muecke
//	
// Description: 
//              Original idea by Andrew Klinkhamer (MASM32 stubinstall).
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "PeSfx.h"

#include <SetupAPI.h> // will require at least Windows 2000
#include <shellapi.h>
#include "../FMlib/FMlib.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "shell32.lib")
#ifdef _DEBUG
#	pragma comment(lib, "../FMlib/FMlibd.lib")
#else
#	pragma comment(lib, "../FMlib/FMlib.lib")
#endif

// data
const char* caption = "PeSfx";
const char* szDirFormat = "~psr%x";
const char* szInternalError = "No data found!";
const char* szFileCreationError = "Unable to create temporary file(s)!";
const char* szMemAllocError = "Unable to allocate memory!";
const char* szCabOpenError = "Unable to open compressed file(s)!";
const char* szDllLoadError = "Error starting process!";
const char* szTmpDirError = "Error creating temporary directory. Please check your %TMP% configuration.";

// uninitialized data
//align 4
char szExePath[MAX_PATH];
char szPath[MAX_PATH];
char szSubDirName[sizeof(szDirFormat)+6+1]; //enough space for 8 hex chars + terminating 0 
HANDLE hSfxFile = NULL;
HANDLE hCabFile = NULL;
LPVOID pMem = NULL;
int fileSize = 0;
DWORD dwBytes = 0;
DWORD dwPathLen = 0;


void cleanUpAndExit( int error_code )
{
	if( pMem )
		::VirtualFree( pMem, 0, MEM_RELEASE );
	if( hSfxFile )
		::CloseHandle( hSfxFile );
	if( hCabFile )
		::CloseHandle( hCabFile );
	ExitProcess( error_code );
}

void main()
{
	int argc = FM::PrepareCommandLine();
	PeSfxRun psr;
	if( argc > 1 )
	{
		// Parse command line
		if( FM::StrCmp( FM::GetCommandLineArg(1), "/?" ) )
			FM::MbxInfo( psr.GetHelpString(), caption );
	}

	// Get the name and path of this executable
	::GetModuleFileName( NULL, szExePath, MAX_PATH );

	hSfxFile = ::CreateFile( szExePath, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hSfxFile == INVALID_HANDLE_VALUE )
	{
		hSfxFile = NULL;
		FM::MbxError( szFileCreationError, caption );
		cleanUpAndExit( CREATE_FILE_ERROR );
	}

	// Get the size of this stub plus the appended cab file
	int stubFileSize = FM::GetPEFileSize( szExePath );
	fileSize = ::GetFileSize( hSfxFile, NULL );
	fileSize -= stubFileSize;
	if( fileSize <= 0 )
	{
		FM::MbxError( szInternalError, caption );
		cleanUpAndExit( INTERNAL_ERROR );
	}

	pMem = ::VirtualAlloc( 0, fileSize+1, MEM_COMMIT, PAGE_READWRITE );
	if( pMem == NULL )
	{
		FM::MbxError( szMemAllocError, caption );
		cleanUpAndExit( MEM_ALLOC_ERROR );
	}

	// Read the appended cab into the allocated memory
	::SetFilePointer( hSfxFile, stubFileSize, 0, FILE_BEGIN );
	::ReadFile( hSfxFile, pMem, fileSize, &dwBytes, NULL );
	::CloseHandle( hSfxFile );
	hSfxFile = 0;

	// Get the path to the temporary directory
	::GetTempPath( MAX_PATH, szPath );
	
	// Get the string length
	dwPathLen = ::lstrlen( szPath );

	// Create %TMP% dir if it does not exist
	if( !::CreateDirectory( szPath, NULL ) )
	{
		if( ::GetLastError() == ERROR_PATH_NOT_FOUND )
		{
			FM::MbxError( szTmpDirError, caption );
			cleanUpAndExit( ERROR_PATH_NOT_FOUND );
		}
		// ERROR_ALREADY_EXISTS --> ok!
	}

	// Try to create the temp subdirectory for packed files
	bool retCode = false;
	while( !retCode ) 
	{
		// Create name for own tmp dir
		::wsprintf( szSubDirName, szDirFormat, ::GetTickCount() );
		//*(szPath + dwStrLen) = 0;
		::lstrcpy( szPath + dwPathLen, szSubDirName );
		retCode = ::CreateDirectory( szPath, NULL ) > 0;
	}	
	dwPathLen += sizeof(szSubDirName);

	// Copy temp dir name to szExePath (is not needed anymore)
	*(szPath+dwPathLen+1) = '\\';
	::lstrcpy( szExePath, szPath );
	//SetCurrentDirectory( szPath );

	// Append desired cab file name to path
	::lstrcpy( szPath+dwPathLen+2, szSubDirName );

	// Write the cab to disc
	hCabFile = ::CreateFile( szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL );
	if( hCabFile == INVALID_HANDLE_VALUE )
	{
		hCabFile = NULL;
		FM::MbxError( szFileCreationError, caption );
		cleanUpAndExit( CREATE_CAB_FILE_ERROR );
	}
	::WriteFile( hCabFile, pMem, fileSize, &dwBytes, NULL );
	::CloseHandle( hCabFile );
	hCabFile = NULL;
	::VirtualFree( pMem, 0, MEM_RELEASE );
	pMem = NULL;

	// Unpack the cab
	if( ::SetupIterateCabinet( szPath, 0, (PSP_FILE_CALLBACK)CabFileCallback, 0 ) == 0 )
	{
		FM::MbxError( szCabOpenError, caption );
		::DeleteFile( szPath );
		*(szPath + dwPathLen + 1) = '\0';
		::RemoveDirectory( szPath );
		cleanUpAndExit( CAB_OPEN_ERROR );
	}

	// Load the installer dll. The dll should create the actual
	// installer in response to the DLL_PROCESS_ATTACH message.
	HMODULE hDLL = ::LoadLibrary( szExePath );
	if( hDLL == NULL )
	{
		FM::MbxError( szDllLoadError, caption );
		::DeleteFile( szPath );
		::DeleteFile( szExePath );
		//TODO: remove directory
		cleanUpAndExit(	DLL_LOAD_ERROR );
	}
	::FreeLibrary( hDLL );

	// Remove cab file from from path string
	*(szPath+dwPathLen+1) = '\0';
	*(szPath+dwPathLen+2) = '\0'; // needed for SHFileOperation

	// Recursively delete temporary directory
	SHFILEOPSTRUCT fileOp;
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = szPath;
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	fileOp.pTo = szPath + dwPathLen;
	fileOp.hNameMappings = NULL;
	fileOp.lpszProgressTitle = NULL;
	if( ::SHFileOperation( &fileOp ) != 0 )
	{
		// error, check fileOp.fAnyOperationsAborted
	}

	// Exit this process
	::ExitProcess( 0 );
}

LRESULT WINAPI CabFileCallback( 
	PVOID pInstallData, UINT uNotification, UINT_PTR uParam1, UINT_PTR uParam2 )
{
	pInstallData;	// just for warning 4100
	uParam2;		// just for warning 4100
	if( uNotification == SPFILENOTIFY_FILEINCABINET )
	{
		lstrcpy( szExePath + dwPathLen + 2, ((FILE_IN_CABINET_INFO*)uParam1)->NameInCabinet );
		lstrcpy( ((FILE_IN_CABINET_INFO*)uParam1)->FullTargetName, szExePath );
		return FILEOP_DOIT;
	}
	return NO_ERROR;
}
