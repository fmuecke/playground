//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Filename: PeSfx.h
// Created:  2009/05/10 22:16
// Author:   Florian Muecke
//	
// Description: 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifndef PE_SFX_HEADER_INCLUDED
#define PE_SFX_HEADER_INCLUDED

#pragma once

#include <Windows.h>

#define VERSION					"0.1"

#define INTERNAL_ERROR			1
#define CREATE_FILE_ERROR		2
#define MEM_ALLOC_ERROR			3
#define CREATE_CAB_FILE_ERROR	4
#define CAB_OPEN_ERROR			5
#define DLL_LOAD_ERROR			6
//#define SPFILENOTIFY_FILEINCABINET	0x11

//#define FIXED_FILESIZE					13824 // bytes

LRESULT WINAPI CabFileCallback( PVOID pInstallData, UINT uNotification, UINT_PTR uParam1, UINT_PTR uParam2 );

enum ExecuteType
{
	ET_none = 0,
	ET_create_process,
	ET_shell_execute,
	ET_load_library
};

//struct PeSfxRunConfig
//{
//	//unsigned short  
//	char[] programName;
//	char[] parameter;
//	ExecuteType exec_type;
//};

class PeSfxRun
{
public:
	PeSfxRun(){}
	~PeSfxRun(){}

	const char* GetHelpString()
	{
		return "PeSfx v" VERSION "\n"\
			"\n"\
			"(c) 2009 Florian Muecke\n"\
			"http://flo.mueckeimnetz.de";

	}

};


#endif //PE_SFX_HEADER_INCLUDED