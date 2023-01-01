#ifndef DEVICES_H_
#define DEVICES_H_

#include <iostream>
// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem DSSHOWGRABBER_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// DSSHOWGRABBER_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
#ifdef DSSHOWGRABBER_EXPORTS
	#define DSSHOWGRABBER_API __declspec(dllexport)		
#else
	#define DSSHOWGRABBER_API __declspec(dllimport)		
    #ifdef _DEBUG
    #pragma comment(lib,"DsShowGrabberD.lib")
    #else
    #pragma comment(lib,"DsShowGrabber.lib")
    #endif
#endif

/** \brief This struct describes an available video device
* 
*/
struct DSSHOWGRABBER_API VideoDevice {
	VideoDevice() : friendlyName(0), displayName(0), widths(0), heights(0), depths(0), nrResolutions(0), preferedMode(0) {}
	~VideoDevice() 
	{
		delete[] friendlyName;
		delete[] displayName;
		delete[] widths;
		delete[] heights;
		delete[] depths;
	}
	VideoDevice(const VideoDevice& copy) : friendlyName(0), displayName(0), widths(0), heights(0), depths(0), nrResolutions(copy.nrResolutions), preferedMode(copy.preferedMode)
	{
		if (copy.friendlyName)
		{
			friendlyName = new char[strlen(copy.friendlyName)+1];
#if _MSC_VER > 1310
			strcpy_s(friendlyName, strlen(copy.friendlyName)+1, copy.friendlyName);
#else
       		strcpy(friendlyName, copy.friendlyName);
#endif
		}
		if (copy.displayName)
		{
			displayName = new char[strlen(copy.displayName)+1];
#if _MSC_VER > 1310
			strcpy_s(displayName, strlen(copy.displayName)+1, copy.displayName);
#else
            strcpy(displayName, copy.displayName);
#endif
        }
		if (copy.widths && nrResolutions > 0)
		{
			widths = new int[nrResolutions];
			memcpy(widths, copy.widths, sizeof(int)*nrResolutions);
		}
		if (copy.heights && nrResolutions > 0)
		{
			heights = new int[nrResolutions];
			memcpy(heights, copy.heights, sizeof(int)*nrResolutions);
		}
		if (copy.depths && nrResolutions > 0)
		{
			depths = new int[nrResolutions];
			memcpy(depths, copy.depths, sizeof(int)*nrResolutions);
		}
	}

	VideoDevice& operator=(const VideoDevice& copy)
	{
		if (this == &copy)
			return *this;
		if (copy.friendlyName)
		{
			friendlyName = new char[strlen(copy.friendlyName)+1];
#if _MSC_VER > 1310
			strcpy_s(friendlyName, strlen(copy.friendlyName)+1, copy.friendlyName);
#else
            strcpy(friendlyName, copy.friendlyName);
#endif
		}
		else
			friendlyName = 0;
		if (copy.displayName)
		{
			displayName = new char[strlen(copy.displayName)+1];
#if _MSC_VER > 1310
			strcpy_s(displayName, strlen(copy.displayName)+1, copy.displayName);
#else
            strcpy(displayName, copy.displayName);
#endif
		}
		else
			displayName = 0;
		preferedMode = copy.preferedMode;
		nrResolutions = copy.nrResolutions;
		if (copy.widths && nrResolutions > 0)
		{
			widths = new int[nrResolutions];
			memcpy(widths, copy.widths, sizeof(int)*nrResolutions);
		}
		else widths = 0;
		if (copy.heights && nrResolutions > 0)
		{
			heights = new int[nrResolutions];
			memcpy(heights, copy.heights, sizeof(int)*nrResolutions);
		}
		else
			 heights = 0;
		if (copy.depths && nrResolutions > 0)
		{
			depths = new int[nrResolutions];
			memcpy(depths, copy.depths, sizeof(int)*nrResolutions);
		}
		else
			depths = 0;		
		return *this;
	}

    /// the friendlyname of the device
	char* friendlyName;
    /// the display name of the device's moniker
	char* displayName;
    /// all width settings of the possible resolutions
	int*	 widths;
    /// all height settings of the possible resolutions
	int*	 heights;
    /// all pixel depth settings of the possible resolutions
	int*     depths;
    /// number of possible resolutions
	int	nrResolutions;    
	int preferedMode;
};

/**
\defgroup Global Getting Device Information
*/

/** @ingroup Global releases the memory allocated during the call of returnDevices
 * @param devices pointer to an array allocated by calling returnDevices(int& numDevices) 
 */
DSSHOWGRABBER_API void releaseDevices(VideoDevice*& devices);

/** @ingroup Global returns the video devices connected to this computer
 * @param numDevices (out) the number of connected devices will be saved into this variable
 * @return VideoDevice* pointer to an array of connected VideoDevices.<br>The allocated memory
 * has to be released by calling the releaseDevices(VideoDevice*& devices) function.
 */
DSSHOWGRABBER_API VideoDevice* returnDevices (int& numDevices);



// convert function for wchar to char strings
char* ConvertLPWSTRToLPSTR (wchar_t* lpwszStrIn);


#endif