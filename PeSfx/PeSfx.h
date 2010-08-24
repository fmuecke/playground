//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Filename: PeSfx.h
// Created:  2009/05/10 22:16
// Modified: 2010/08/25 0:27 
// Author:   Florian Muecke
//	
// Description: 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifndef PE_SFX_HEADER_INCLUDED
#define PE_SFX_HEADER_INCLUDED

#pragma once

#include <Windows.h>

#define VERSION					"0.1"

enum 
{ 
	eINTERNAL_ERROR = 1,
	eOPEN_FILE_ERROR,
	eCREATE_FILE_ERROR,
	eMEM_ALLOC_ERROR,
	eCREATE_CAB_FILE_ERROR,
	eCAB_OPEN_ERROR,
	eDLL_LOAD_ERROR
};

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