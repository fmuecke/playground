/*
	- Basic structure
		.-------------------.
		| PE Heder          |
		|-------------------|
		| PESFX Info Header |
		|-------------------|
		| PESFX File Header |
		|-------------------|
		| Archive File 1    |
		|-------------------|
		| ...               |
		|-------------------|
		| Archive File n    |
		'-------------------'
	
	- Options
		* RemoveFiles: files will not be kept after program termination
		* Silent: no output
		* Overwrite: overwrite existing files (should not be necessary)
		* ExecMode: CreateProcess, ShellExecute, LoadLibrary
		* View: maximized, minimized, hidden
		* Priority: high, normal, low
		* Use default temp directory (if not, the temp dir has to be specified;
			If the specified dir does not exist or can not be created the default
			temp is used)
*/

#ifndef PESFX_INFO_HEADER_INCLUDED
#define PESFX_INFO_HEADER_INCLUDED


namespace OptionFlags
{
	enum PESFX_Option_Flags = 
	{
		None = 0x00,
		Silent = 0x01,
		RemoveFiles = 0x02,
		Exec_CreateProcess = 0x00,
		Exec_ShellExecute = 0x04,
		Exec_LoadLibrary = 0x08,
		View_Normal = 0x00,
		View_Minimized = 0x10,
		View_Maximized = 0x20,
		View_Hidden = 0x30,
		Priority_Low = 0x40, 
		Priority_High = 0x50,
		Priority_Normal = 0x00,
		Output_UseTmp = 0x00;
		Output_Specifed = 0x60
	};
}

struct PESFX_Info_Header
{
	char magic_bytes[5];		// "PESFX" or something
	unsigned int version;		// consists of low order and high order short
	unsigned int options;
	unsigned char output_dir_length; // lenght of the string that follows this struct (including terminating zero?)
	// output dir 
};

// EXAMPLE DATA
PESFX_Info_Header info_header = {
	// magic bytes
	{ 'P','E','S','F','X' },
	// version
	(0&0xFF000000) | (0x00010000&0x00FF0000) | (4&0x0000FFFF), //0.1.4
	// option flags
	OptionFlags::Silent | OptionFlags::RemoveFiles | OptionFlags::Exec_CreateProcess | 
	 OptionFlags::View_Normal | OptionFlags::Priority_Normal | OptionFlags::Output_UseTmp,
	// dir length
	38
};
	
#endif // PESFX_INFO_HEADER_INCLUDED