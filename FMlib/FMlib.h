//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Filename: FMlib.h
// Created:  06:35 11.05.2009
// Author:   Florian Muecke
//	
// Description: Various helper functions for Win32 API
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef FMlib_h_INCLUDED_
#define FMlib_h_INCLUDED_

namespace FM
{

void MbxInfo( const char* msg, const char* caption );
void MbxError( const char* msg, const char* caption );


//int GetNumberOfArgs();

//--------------------
// PrepareCommandLine
//--------------------
// Modifies command line so that each argument ends with a terminating zero.
// Delimiters like " or ' will be removed as well.
// Returns the total number of arguments.
int PrepareCommandLine();

//-------------------
// GetCommandLineArg
//-------------------
// Returns the specified argument from the prepared (!) command line.
// Returns Module name if pos exceed argument list.
const char* GetCommandLineArg( int pos );

//--------
// StrCmp
//--------
// Simple case sensitive string compare.
bool StrCmp( const char* lpSrc, const char* lpDst );


// 
// Defines for GetPEFileSize
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
// GetPEFileSize
//---------------
// Calculates the file size via the PE header.
int GetPEFileSize( char const* lpFileName );


} // namespace FM

#endif // FMlib_h_INCLUDED_