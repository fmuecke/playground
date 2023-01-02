//=========================================================
// Copyright: Florian Muecke <dev[:]mueckeimnet.de>
// Created: 23:24 10.04.2009
// Description: Simple program to calculate the file size
//              via the PE file header. This can be handy
//              if you have additional data attached to 
//              exe or dll and want to know its offset.
//=========================================================

#include <windows.h>
#include <iostream>

//
// Defines
//
#define PE_FILE_OFFSET_ADDR		0x3E
#define PE_SIGNITURE			0x00004550		//"PE\0\0", Little Endian

// error codes
#define PE_ERROR_GENERIC				-500
#define PE_ERROR_FILE_OPEN				-501	// unable to open file
#define PE_ERROR_READ_DOS_HEADER		-502	// unable to read DOS header
#define PE_ERROR_WRONG_PE_SIGNATURE		-503	// wrong PE signature
#define PE_ERROR_READ_FILE_HEADER		-504	// unable to read file header
#define PE_ERROR_READ_OPT_HEADER		-505	// unable to read optional header
#define PE_ERROR_READ_SECTION_HEADER	-506	// unable to read section header

//---------------
// getPEFileSize
//---------------
// Returns the PE size of the specified file or a negative value as error code.
//
static int getPEFileSize( char const* lpFileName )
{
	HANDLE hFile = ::CreateFileA( lpFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	if( !hFile )
	{
		return PE_ERROR_FILE_OPEN;
	}

#ifdef _DEBUG
	// get real file size
	std::cout << "GetFileSize: " << ::GetFileSize( hFile, NULL ) << " bytes" << std::endl;
#endif

	// Read DOS header
	_IMAGE_DOS_HEADER dosHeader;
	DWORD bytesRead = 0;
	if( !::ReadFile( hFile, &dosHeader, sizeof(_IMAGE_DOS_HEADER), &bytesRead, NULL ) 
		|| bytesRead != sizeof(_IMAGE_DOS_HEADER) )
	{
		::CloseHandle( hFile );
		return PE_ERROR_READ_DOS_HEADER;
	}
	int pe_file_offset = dosHeader.e_lfanew;

	// Move file ptr to PE-Signature
	::SetFilePointer( hFile, pe_file_offset, 0, FILE_BEGIN );

	// Check PE signature
	int tmp = 0;
	if( !::ReadFile( hFile, &tmp, 4, &bytesRead, NULL ) 
		|| tmp != PE_SIGNITURE 
		|| bytesRead != 4 )
	{
		::CloseHandle( hFile );
		return PE_ERROR_WRONG_PE_SIGNATURE;
	}

	// Read file header
	_IMAGE_FILE_HEADER fileHeader;
	if( !::ReadFile( hFile, &fileHeader, sizeof(_IMAGE_FILE_HEADER), &bytesRead, NULL ) 
		|| bytesRead != sizeof(_IMAGE_FILE_HEADER) )
	{
		::CloseHandle( hFile );
		return PE_ERROR_READ_FILE_HEADER;
	}

	// Read optional header
	_IMAGE_OPTIONAL_HEADER optionalHeader;
	if( !::ReadFile( hFile, &optionalHeader, sizeof(_IMAGE_OPTIONAL_HEADER), &bytesRead, NULL ) 
		|| bytesRead != sizeof(_IMAGE_OPTIONAL_HEADER) )
	{
		::CloseHandle( hFile );
		return PE_ERROR_READ_OPT_HEADER;
	}

	// Calculate header size
	int header_size = pe_file_offset + 4 + sizeof(_IMAGE_FILE_HEADER) + sizeof(_IMAGE_OPTIONAL_HEADER);

	// Read section headers and accumulate size
	int n_sections = fileHeader.NumberOfSections;
	int section_size = 0;
	while( n_sections-- )
	{
		_IMAGE_SECTION_HEADER sectionHeader;
		if( !::ReadFile( hFile, &sectionHeader, sizeof(IMAGE_SECTION_HEADER), &bytesRead, NULL ) 
			|| bytesRead != sizeof(IMAGE_SECTION_HEADER) )
		{
			::CloseHandle( hFile );
			return PE_ERROR_READ_SECTION_HEADER;
		}
		section_size += sectionHeader.SizeOfRawData;
		header_size += sizeof(IMAGE_SECTION_HEADER);
	}

	// Round header size to a multiple of "FileAlignment"
	// ---
	// NOTE: this calculation is needed, because the provided
	// optionalHeader.SizeOfHeaders is sometimes too big 
	// (UPX does not set it right!)
	int aligned_header_size = optionalHeader.FileAlignment;
	if( header_size > aligned_header_size )
	{
		while( header_size > aligned_header_size )
		{
			aligned_header_size = aligned_header_size << 1;
		}
	}
	header_size = aligned_header_size;

#ifdef _DEBUG
	//std::cout << "Size of Code: " << optionalHeader.SizeOfCode << std::endl;
	//std::cout << "Size of Initialized Data: " << optionalHeader.SizeOfInitializedData << std::endl;
	//std::cout << "Size of Uninitialized Data: " << optionalHeader.SizeOfUninitializedData << std::endl;
	std::cout << "Size of Headers: " << optionalHeader.SizeOfHeaders << std::endl;
	std::cout << "Calc. Size of Sections: " << section_size << std::endl;
	std::cout << "Calc. Size of Headers: " << header_size << std::endl;
	std::cout << "---" << std::endl;
	std::cout << "Calculated Size: " << header_size+section_size << std::endl;
#endif
	::CloseHandle( hFile );
	return header_size+section_size;
}

//------
// main
//------
int main()
{
	// get module file name
	HMODULE hModule = ::GetModuleHandle( NULL );
	char lpFileName[MAX_PATH];
	::GetModuleFileNameA( hModule, lpFileName, MAX_PATH );
#ifdef _DEBUG
	std::cout << "Module: " << lpFileName << std::endl;
#endif
	return getPEFileSize( lpFileName );

/////////////////////////////////////
	LPVOID lpMem = ::VirtualAlloc( 0, dwFileSize, MEM_COMMIT, PAGE_READWRITE );
	//TODO: if( lpMem == NULL )
	
	// Read the appended file into the allocated memory
	//HANDLE hFile;
	//int dwFileSize = 0;
	//int bytesRead = 0;
	//::ReadFile( hFile, lpMem, dwFileSize, &bytesRead, NULL );
	//::CloseHandle( hFile );
	//hFile = NULL;
	
	char buffer[MAX_PATH];
	// Get the path to the temporary directory
	::GetTempPath( MAX_PATH, &path );

	//// Get the string length
	//::lstrlen( &path );
	//	mov		dwStrLen, eax
	//	lea		edx, [szCab + eax]

	//// Append a search filter
	//	mov		byte ptr [edx], '*'
	//	mov		byte ptr [edx + 1], 0

	// Check that the temp folder exists (this is not guaranteed under Win 2000/XP)
	// OR: We just always try to create the dir (will not result in error).
	::CreateDirectory( &path, NULL );

	// Write the file to disc
	hFile = ::CreateFile( &szCab, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		//TODO ERROR
	}
	::WriteFile( hFile, pMem, dwFileSize, &bytesRead, NULL );
	::CloseHandle( hFile );
	::VirtualFree( pMem, 0, MEM_RELEASE )

	// Unpack the cab
	//::SetupIterateCabinet( &szCab, 0, &CabFileCallback, 0 );
	//TODO ERROR "CAB_OPEN_ERROR"

	// Load DLL
	HMODULE hDll = ::LoadLibrary( &path );
	//TODO ERROR
	::FreeLibrary( hDll );

	// Delete Dll
	::DeleteFile( path );s
}