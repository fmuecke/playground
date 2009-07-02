//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Filename: FMlib.cpp
// Created:  06:34 11.05.2009
// Author:   Florian Muecke
//	
// Description: Various helper functions for Win32 API
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "FMlib.h"
#include <Windows.h>

#ifdef FM_WRITE_DEBUG_INFO
#include <iostream>
#endif

namespace FM
{


void MbxInfo( const char* msg, const char* caption )
{
	MessageBox( NULL, msg, caption, MB_ICONINFORMATION );
}

void MbxError( const char* msg, const char* caption )
{
	MessageBox( NULL, msg, caption, MB_ICONERROR );
}
	
	
//int GetNumberOfArgs()
//{
//	int argc = 0;
//	char* lpCommandLine = GetCommandLine();
//	while( *lpCommandLine )
//	{
//		++lpCommandLine;
//		if( *lpCommandLine == '\0' )
//		{
//			++argc;
//			++lpCommandLine;
//		}
//	}
//	return argc;
//}

//--------------------
// PrepareCommandLine
//--------------------
// Modifies command line so that each argument ends with a terminating zero.
// Delimiters like " or ' will be removed as well.
// Returns the total number of arguments.
int PrepareCommandLine()
{
	const char delimiters_[3] = { ' ', '\'', '\"' };

	char* lpCommandLine = GetCommandLine();

	// Calculate size of command line and allocate memory
	// (needed for copy later)
	while ( *lpCommandLine++ );
	int cmdLineSize = lpCommandLine - GetCommandLine() + 1;
	lpCommandLine = GetCommandLine();
	char* pMem = (char*)VirtualAlloc( 0, cmdLineSize+1, MEM_COMMIT, PAGE_READWRITE ); // allocate one more byte for the double zero termination
	if( pMem == NULL )
	{
		return -1;
	}
	int argc = 0;
	//------------------------------------------------------
	// Iterate through the command line and count arguments
	// First argument is the program name itself
	//------------------------------------------------------
	char* pos = lpCommandLine;
	char* lpCleanCommandLine = pMem;
	while( *pos )
	{
		char delimiter = delimiters_[0]; // ' '
		if( *pos == delimiter ) 
		{
			// skip spaces until "*pos" is the next char
			// or end of string
			while( *pos == delimiter )
			{
				*pos = 0x00; // zero delimiter
				++pos;
			}
			continue;
		}
		if( *pos == delimiters_[1] ) // '\"'
		{
			delimiter = delimiters_[1];
			*pos = 0x00; // zero delimiter
			++pos;
		}
		else
		if( *pos == delimiters_[2] ) // '\''
		{
			delimiter = delimiters_[2];
			*pos = 0x00; // zero delimiter
			++pos;
		}

		// Iterate until next delimiter or end of string.
		// "*pos" must be the first char after the delimiter!
		++argc; // we have a new arg
		while( *pos != delimiter )
		{
			if( *pos == 0x00 )
				break;
			*lpCleanCommandLine = *pos;
			++pos;
			++lpCleanCommandLine;
		}
		if( *pos == 0x00 )
		{
			if( delimiter != delimiters_[0] ) // check for space
			{
				VirtualFree( pMem, 0, MEM_RELEASE );
				return -1;
			}
			break;
		}
		*pos = 0x00; // zero delimiter
		*lpCleanCommandLine = 0x00;
		++pos;
		++lpCleanCommandLine;
	}

	// Write command line back
	lpCleanCommandLine = pMem;
	while( lpCleanCommandLine < pMem+cmdLineSize )
	{
		*lpCommandLine = *lpCleanCommandLine;
		++lpCleanCommandLine;
		++lpCommandLine;
		if( *lpCleanCommandLine == '\0' )
		{
			*lpCommandLine = '\0';
			++lpCleanCommandLine;
			++lpCommandLine;
		}
	}

	VirtualFree( pMem, 0, MEM_RELEASE );
	return argc;
}

//-------------------
// GetCommandLineArg
//-------------------
// Returns the specified argument from the prepared (!) command line.
// Returns Module name if pos exceed argument list.
const char* GetCommandLineArg( int pos )
{
	const char* lpCommandLine = GetCommandLine();

	while( --pos >= 0 )
	{
		if( *lpCommandLine == '\0' )
			return GetCommandLine();
		while( *lpCommandLine )
			++lpCommandLine;
		++lpCommandLine;
	}

	return lpCommandLine;
}

//--------
// StrCmp
//--------
// Simple case sensitive string compare.
bool StrCmp( const char* lpSrc, const char* lpDst )
{
	bool equal = false;
	if( *lpSrc == *lpDst ) 
		equal = true;
	while( *lpSrc )
	{
		if( *lpSrc != *lpDst )
		{
			equal = false;
			break;
		}
		++lpSrc;
		++lpDst;
	}
	return equal;
}

//---------------
// GetPEFileSize
//---------------
// Calculates the file size via the PE header.
int GetPEFileSize( char const* lpFileName )
{
	HANDLE hFile = ::CreateFileA( lpFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	if( !hFile )
	{
		return PE_ERROR_FILE_OPEN;
	}

#ifdef FM_WRITE_DEBUG_INFO
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

#ifdef FM_WRITE_DEBUG_INFO
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

} // namespace FM

