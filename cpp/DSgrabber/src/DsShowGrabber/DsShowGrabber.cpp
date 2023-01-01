// DsShowGrabber.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//
#include "DsShowGrabber.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#include <iostream>
#include <vector>

using namespace std;

#define SAFE_EXIT_FROM_INIT(hr) if (hr!=S_OK) \
{					 \
	TearDownGraph(); \
	return false;    \
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///   DsShowGrabber Class											//////
//////////////////////////////////////////////////////////////////////////


DsShowGrabber::DsShowGrabber() : 
#ifdef _DEBUG
    g_dwGraphRegister(0),
#endif
	_width(0), _height(0), _size(0), _pixelDepth(0), 
	_pBuffer(0),_newImage(false), m_frames(0), _deviceID(-1), _flipped(true),
    _eventThread(NULL), m_singleShot(false), m_avi(false), m_loopAvi(false)
{
	m_imageLoc = CreateMutex (NULL, FALSE, NULL);
	CoInitialize(NULL);
}

DsShowGrabber::~DsShowGrabber()
{
	TearDownGraph();
	CoUninitialize();	
}

bool DsShowGrabber::init(const char* fileName, bool loop, int pixelDepth /*= 24*/)
{
    return init(fileName, loop ? "loop=true" : "loop=false", 0, 0, pixelDepth);
}

bool DsShowGrabber::init(const char* friendlyName, const char* displayName, int width, int height, int pixelDepth)
{
    m_avi = false;
	m_frames = 0;
	m_framePos = 0;

	if (pGraph /*|| pBuilder*/)
		TearDownGraph();

	// Create the Filter Graph Manager.
	SAFE_EXIT_FROM_INIT(pGraph.CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER));
	
	// pMediaControl Handles media streaming in the filter graph. 
	pMediaControl = CComQIPtr<IMediaControl>(pGraph);
	// pMediaEvent Handles filter graph events. 
	pEvent   = CComQIPtr<IMediaEvent>(pGraph);
    if (friendlyName && strlen(friendlyName)>0
        && (_stricmp(displayName, "loop=true") == 0 || _stricmp(displayName, "loop=false")==0)
        && width == 0
        && height == 0)
    {    
        // convert const char to wchar
        wchar_t* fileName = new wchar_t[strlen(friendlyName) + 1];
        memset(fileName, 0, sizeof(wchar_t) * strlen(friendlyName) + 1);
		size_t convertedChars = 0;
#if _MSC_VER > 1310
		mbstowcs_s(&convertedChars, fileName, strlen(friendlyName) + 1, friendlyName, _TRUNCATE);
#else
        mbstowcs(fileName, friendlyName, strlen(friendlyName)+1);
        convertedChars = strlen(friendlyName)+1;
#endif
        //mbstowcs_s(&converted, fileName, friendlyName, strlen(friendlyName) + 1); 
        // Add video file filter to filter graph
	    if ((convertedChars!=strlen(friendlyName)+1) || (pGraph->AddSourceFilter(fileName, L"Capture Filter", &pCaptureDeviceFilter))!=S_OK)
        {
            delete [] fileName;
            TearDownGraph();
            return false;
        }
        delete [] fileName;
        // indicate that we have an avi file
        m_avi = true;
        // avi files are DIB images stored flipped 
        _flipped = true;
		pSeeking = CComQIPtr< IMediaSeeking, &IID_IMediaSeeking > (pGraph);
    }
    else
    {
        // Get capture device
	    SAFE_EXIT_FROM_INIT(getCaptureDeviceFilter(friendlyName, displayName ,pCaptureDeviceFilter));
	    // Add capture device to Filtergraph
	    SAFE_EXIT_FROM_INIT(pGraph->AddFilter(pCaptureDeviceFilter, L"Capture Filter"));
    }

	// get output pin from Video Source, dummy is not necessary and should be null
	CComPtr<IPin> dummy;
	SAFE_EXIT_FROM_INIT(
		getInputOutputPins(
		pCaptureDeviceFilter,
		dummy,
		pSourceOutputPin)
	);

	if (dummy)
		dummy.Release();

	if(!pSourceOutputPin)
	{
		#ifdef _DEBUG
			cout << "Source Outputpin couldn't be found" << endl;
		#endif
		TearDownGraph();
		return false;
	}
    
    if (!m_avi)
    	// set specified source output properties
	    SAFE_EXIT_FROM_INIT(setSourceProperties(pSourceOutputPin, width, height, pixelDepth));
	
    // Create SampleGrabber-Filter
	SAFE_EXIT_FROM_INIT(pSampleGrabberFilter.CoCreateInstance(
			CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER));

	// Getting Interface from SampleGrabberFilter and save it in pSampleGrabber
	SAFE_EXIT_FROM_INIT(pSampleGrabberFilter.QueryInterface(&pSampleGrabber));

	// Adding SampleGrabberFilter to FilterGraph
	SAFE_EXIT_FROM_INIT(pGraph->AddFilter(pSampleGrabberFilter, L"SampleGrabber"));

	// get input and output pin from SampleGrabber Filter
	SAFE_EXIT_FROM_INIT(
		getInputOutputPins(
			pSampleGrabberFilter,
			pSampleGrabberInputPin,
			pSampleGrabberOutputPin)
	);	

	if (!pSampleGrabberInputPin || !pSampleGrabberOutputPin) {
		#ifdef _DEBUG
			cout << "SampleGrabber Pins couldn't be found" << endl;
		#endif
		TearDownGraph();
		return false;
	}
	// Configure the Sample Grabber.
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	switch(pixelDepth) {
	case 8:
		mt.subtype = MEDIASUBTYPE_RGB8;
		break;
	//case 16:
	//	mt.subtype = MEDIASUBTYPE_RGB555;
	//	break;
	case 32:
		mt.subtype = MEDIASUBTYPE_ARGB32;
		break;
	case 24:		
	default:
		mt.subtype = MEDIASUBTYPE_RGB24;
	}
	mt.formattype = FORMAT_VideoInfo; 	
	SAFE_EXIT_FROM_INIT(pSampleGrabber->SetMediaType(&mt));  // Set the media type
	//deleteMediaType(&mt);
		
	/// Creating NullRenderer
	SAFE_EXIT_FROM_INIT(
		pNullRendererFilter.CoCreateInstance(
			CLSID_NullRenderer, 
			NULL, 
			CLSCTX_INPROC_SERVER)
	);

	SAFE_EXIT_FROM_INIT(pGraph->AddFilter(pNullRendererFilter, L"NullRenderer"));		

	// get input pin from nullrenderer, dummy is not necessary and should be null
	SAFE_EXIT_FROM_INIT(
		getInputOutputPins(
		pNullRendererFilter,
		pNullRendererInputPin,
		dummy)
		);
	if (dummy)
		dummy.Release();

	// Lets connect all pins
	SAFE_EXIT_FROM_INIT(pGraph->Connect(pSourceOutputPin, pSampleGrabberInputPin));
	SAFE_EXIT_FROM_INIT(pGraph->Connect(pSampleGrabberOutputPin, pNullRendererInputPin));

	// initialize grabber properties
	_width = width; _height = height; _pixelDepth = pixelDepth;

	// set buffered mode in samplegrabber, if a callback is wanted the setCallBack method
	// has to be called after the initialization
	SAFE_EXIT_FROM_INIT(pSampleGrabber->SetBufferSamples(true));

	// get real grabbber properties, because sometimes a conversion filter is inserted by 
	// directshow resulting in a mismatch between the specified width, height and pixeldepth
	// and the real grabber properties
	SAFE_EXIT_FROM_INIT(getGrabberProperties());

#ifdef _DEBUG
	SAFE_EXIT_FROM_INIT(AddGraphToRot(pGraph, &g_dwGraphRegister));
#endif

    if (m_avi && _stricmp(displayName, "loop=true")==0)
    {
		m_loopAvi = true;
       	DWORD dwThreadID;
        _eventThread = CreateThread(NULL,0,listeningEvent,(LPVOID) this,0,&dwThreadID);
       	if(_eventThread == NULL) return false;
    }
	else
		m_loopAvi = false;
    return true;	

}

unsigned long __stdcall DsShowGrabber::listeningEvent(void* grabber)
{   
    if (grabber == NULL)
        return -1;
    CComQIPtr<IMediaEvent> event = ((DsShowGrabber*) grabber)->pEvent;
	// Get the seeking interface, so we can seek to a location
	CComQIPtr< IMediaSeeking, &IID_IMediaSeeking > pSeeking( ((DsShowGrabber*) grabber)->pGraph );
	DWORD dwCaps = 0;
	HRESULT hr = pSeeking->GetCapabilities( &dwCaps );		
	if (hr!=S_OK || (dwCaps & AM_SEEKING_CanSeekAbsolute)==0)
		return -1;
	REFERENCE_TIME start = 0;
    long evCode, param1, param2;
    while (hr = event->GetEvent(&evCode, &param1, &param2, INFINITE), SUCCEEDED(hr))
    {		
        hr = event->FreeEventParams(evCode, param1, param2);
        if (EC_COMPLETE == evCode && !((DsShowGrabber*) grabber)->m_singleShot)        
            pSeeking->SetPositions( &start, AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning ); // reset to beginning
        else if (EC_USERABORT == evCode)
            break;
    }
    return 0;
}

bool DsShowGrabber::setOneShot(bool oneShot)
{
	if (!m_avi || !pSeeking)
		return false;
	DWORD dwCaps = 0;
	HRESULT hr = pSeeking->GetCapabilities( &dwCaps );		
	if (hr!=S_OK || (dwCaps & AM_SEEKING_CanSeekAbsolute)==0) return false;
	hr = pSampleGrabber->SetBufferSamples(oneShot ? TRUE : FALSE);
	if (SUCCEEDED(hr)) 
		hr = pSampleGrabber->SetOneShot(oneShot ? TRUE : FALSE);
	m_singleShot = oneShot;
	return hr==S_OK;	
}

bool DsShowGrabber::grabFrame(unsigned char* pBuffer, long size)
{
	if (!pBuffer || size > _size)
		return false;
	WaitForSingleObject( m_imageLoc, INFINITE );
	if (pCurrentSample)
	{
		BYTE* data = 0;
		pCurrentSample->GetPointer(&data);
		memcpy(pBuffer, data, size);
		// release the current sample
		pCurrentSample.Release();
		ReleaseMutex( m_imageLoc);
		return true;
	}
	else
	{
		ReleaseMutex( m_imageLoc);
		return false;
	}
}


bool DsShowGrabber::getBuffer(unsigned char* pBuffer, long size, long timeout /*= 0*/)
{
	if (!pBuffer || !pSampleGrabber)
		return false;	
	if (m_avi && pSeeking)
	{
		if (m_framePos==m_frames && !m_loopAvi )
			return false;
		if (m_singleShot)	
		{
			REFERENCE_TIME start = (m_framePos==m_frames) ? 0 : m_framePos++;
			// set position
			HRESULT hr = pSeeking->SetPositions( &start, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning );
			if (hr!=S_OK) return false;
			hr = pMediaControl->Run();
			if (hr!=S_OK) return false;
			long EvCode = 0;
			// wait for the graph to settle
			hr = pEvent->WaitForCompletion( timeout, &EvCode );
			if (hr!=S_OK) return false;		
		}
	}
	if FAILED(pSampleGrabber->GetCurrentBuffer(&size, (long*) pBuffer))
		return false;	
	return true;
}

HRESULT DsShowGrabber::getGrabberProperties()
{
	AM_MEDIA_TYPE pmt;
	HRESULT hr = pSampleGrabber->GetConnectedMediaType(&pmt);
	if (hr == S_OK)
	{
		_width  = ((VIDEOINFOHEADER *)pmt.pbFormat)->bmiHeader.biWidth;
		_height = ((VIDEOINFOHEADER *)pmt.pbFormat)->bmiHeader.biHeight;
		_pixelDepth  = ((VIDEOINFOHEADER *)pmt.pbFormat)->bmiHeader.biBitCount;
		
		// more likely to be too large: nothing more than a guess
		_size = abs(_width) * abs(_height) * static_cast<long>(_pixelDepth * 0.125f + 0.5f);  
		freeMediaType(pmt);		
	}	
	return hr;
}


HRESULT DsShowGrabber::getCaptureDeviceFilter(const char* friendlyName, const char* displayName, CComPtr<IBaseFilter>& pCaptureDeviceFilter)
{    
	CComPtr<ICreateDevEnum> pDevEnum;
	CComPtr<IEnumMoniker>   pEnum;
	if(pDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER)!=S_OK)
		return S_FALSE;
	if(pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum,0)!=S_OK)
	{
		pDevEnum.Release();
		return S_FALSE;
	}	
	pDevEnum.Release();	
	
	int index = 0;
	CComPtr<IMoniker> pMoniker;	

	pEnum->Reset();
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		CComPtr<IPropertyBag> pPropBag;

		if (FAILED(pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)(&pPropBag))))
		{
			pMoniker.Release();
			continue;  // Skip this one, maybe the next one will work.
		} 
		// if displayName is set, choose device by displayName
		if (displayName)
		{
			pPropBag.Release();
			WCHAR *szDisplayName=NULL;
			if(SUCCEEDED(pMoniker->GetDisplayName(NULL,NULL,&szDisplayName)))
			{
				LPSTR str = ConvertLPWSTRToLPSTR(szDisplayName);
				string deviceName = str;				
				delete[] str;
				size_t index = deviceName.find('#',0);
                if (index!=string::npos)
				{				
					deviceName = deviceName.substr(index, deviceName.length()-index);
					index = deviceName.find('{',0);
					if (index!=string::npos)
					{	
						deviceName = deviceName.substr(0, index);
						if (strcmp(deviceName.c_str(), displayName)==0)
						{
							HRESULT hr = pMoniker->BindToObject(
								0, 0, IID_IBaseFilter, (void**)&pCaptureDeviceFilter);						
							if (SUCCEEDED(hr))
							{								
								pMoniker.Release();	
								pEnum.Release();
								_deviceID = static_cast<int>(index);
								return hr;
							}					
						}
					}					
				}					
			}		
		}
		// else if friendlyName is set, choose device by friendlyname
		else if (friendlyName)
		{
			// Find the description or friendly name.
			VARIANT varName;
			VariantInit(&varName);
			HRESULT hr = pPropBag->Read(L"Description", &varName, 0);
			if (FAILED(hr))
			{
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
			}
			pPropBag.Release();
			if (SUCCEEDED(hr))
			{
				LPSTR str = ConvertLPWSTRToLPSTR(varName.bstrVal);
				VariantClear(&varName);
				// compare with given friendlyName
				if (strcmp(str,friendlyName)==0)
				{						
					delete[] str;
					HRESULT hr = pMoniker->BindToObject(
						0, 0, IID_IBaseFilter, (void**)&pCaptureDeviceFilter);						
					if (SUCCEEDED(hr))
					{								
						pMoniker.Release();	
						pEnum.Release();
						_deviceID = index;
						return hr;
					}															
				} // end if strcmp friendlyname
				else
                    delete[] str;
			} // end if succeeded(read friendly name)
			else
				VariantClear(&varName); 
		}
		// else choose the first available capture device
		else 
		{
			HRESULT hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCaptureDeviceFilter);							
			if (SUCCEEDED(hr))
			{				
				pMoniker.Release();	
				pEnum.Release();
				_deviceID = index;
				return hr;
			}		
		}
		++index;		
		pMoniker.Release();
	}
	// todo get a standard device if available
	pEnum.Release();
	return S_FALSE;
}
	
HRESULT DsShowGrabber::setSourceProperties(
	CComPtr<IPin>& pOutputPin, int width, int height, int pixelDepth)
{
	//The IEnumMediaTypes interface enumerates the preferred formats for a pin. 
	CComPtr<IEnumMediaTypes> pMedia; 

	// Saving enumerator of Videoformat-types of pCamaeraOutputPin in pMedia
	HRESULT hr = pOutputPin->EnumMediaTypes( &pMedia );
	if (hr!=S_OK)
		return hr;
	
    AM_MEDIA_TYPE *pmt = NULL;
	while(pMedia->Next(1, &pmt, 0) == S_OK) 
	{	
		if( pmt->formattype == FORMAT_VideoInfo ) 
		{			
			VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->pbFormat;
			#ifdef _DEBUG				
				cout << "Possible Resolution: " << vih->bmiHeader.biWidth << "x" << vih->bmiHeader.biHeight 
					<< " Bits/Pixel: " << vih->bmiHeader.biBitCount << endl;
			#endif
			// if selected resolution found, choose this one
			if (vih->bmiHeader.biWidth==width && 
				(vih->bmiHeader.biHeight==height || vih->bmiHeader.biHeight==-height)  && 
				vih->bmiHeader.biBitCount==pixelDepth) 
			{				
				// todo set framerate with external information
				vih->AvgTimePerFrame =(LONGLONG)(10000000.0 / 30.0);
				if (vih->bmiHeader.biHeight<0)
					_flipped = false;
				CComPtr<IAMStreamConfig> pConfig;
				hr = pOutputPin.QueryInterface(&pConfig);
				pConfig->SetFormat(pmt);
				deleteMediaType(pmt);
				pConfig.Release();
				pMedia.Release();
				return hr;
			}
			deleteMediaType(pmt);
			pmt = NULL;
		}  // end if pmt->formattype == FORMAT_Videoinfo                      
	} // end while...
	pMedia.Release();
	// this point is only reached if the selected resolution was not found
	return displayPinProperties(pOutputPin);		
}



void DsShowGrabber::freeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0)
    {
        CoTaskMemFree((PVOID)mt.pbFormat);
        mt.cbFormat = 0;
        mt.pbFormat = NULL;
    }
    if (mt.pUnk != NULL)
    {
        // Unecessary because pUnk should not be used, but safest.
        mt.pUnk->Release();
        mt.pUnk = NULL;
    }
}

void DsShowGrabber::deleteMediaType(AM_MEDIA_TYPE *pmt)
{
	// allow NULL pointers for coding simplicity
	if (pmt == NULL) {
		return;
	}
	freeMediaType(*pmt);
	CoTaskMemFree((PVOID)pmt);
}


HRESULT DsShowGrabber::getInputOutputPins(
	CComPtr<IBaseFilter>& filter, CComPtr<IPin>& pInputPin, CComPtr<IPin>& pOutputPin)
{
	CComPtr<IEnumPins> pEnum;
	// getting InputPin from SampleGrabber and save it in pGrabberInputPin
	
	if (!filter || pInputPin || pOutputPin) 
		return S_FALSE;
	
	HRESULT hr = filter->EnumPins(&pEnum);
	if (hr!=S_OK)
		return hr;

	pEnum->Reset();	

	CComPtr<IPin> pP;
	while (pEnum->Next(1, &pP, NULL) == S_OK) {		
		PIN_INFO pininfo;
		if(pP->QueryPinInfo(&pininfo)==S_OK)
		{
			// todo if pininfo.achName begins with a "~" it is probably the capture pin 
			// instead of the preview pin
			// TODO if more then one output or input pin exists (???)
			pininfo.pFilter->Release();		
			if(pininfo.dir == PINDIR_INPUT && !pInputPin) {
				#ifdef _DEBUG
				cout << "Found InputPin on Filter"<< endl;
				#endif
				// todo check if it's the right input pin
				pP.CopyTo(&pInputPin);								
			}
			else if(pininfo.dir == PINDIR_OUTPUT && !pOutputPin)
			{
				#ifdef _DEBUG
				cout << "Found OutputPin on Filter"<< endl;
				#endif
				// todo check if it's the right output pin
				pP.CopyTo(&pOutputPin);								
			}
		}
		pP.Release();		
	}
	pEnum.Release();
	return S_OK;
}

HRESULT DsShowGrabber::displayPinProperties(CComPtr<IPin>& pSrcPin, HWND hWnd /* = NULL*/)
{
	CComPtr<ISpecifyPropertyPages> pPages;

	HRESULT hr = pSrcPin.QueryInterface(&pPages);	
	if (SUCCEEDED(hr))
	{
		CAUUID caGUID;
		hr = pPages->GetPages(&caGUID);
		if (SUCCEEDED(hr))
		{
		OleCreatePropertyFrame(
			hWnd,
			0,
			0,
			L"Property Sheet",
			1,
			(IUnknown **)&(pSrcPin.p),
			caGUID.cElems,
			caGUID.pElems,
			0,
			0,
			NULL);
		CoTaskMemFree(caGUID.pElems);
		}
	}
	return(hr);	
}


HRESULT DsShowGrabber::displayFilterProperties(CComPtr<IBaseFilter>& pFilter, HWND hWnd /* = NULL */)
{
	ISpecifyPropertyPages *pSpec;
	CAUUID cauuid;

	HRESULT hr = pFilter.QueryInterface(&pSpec);
	if(hr == S_OK)
	{
		hr = pSpec->GetPages(&cauuid);

		hr = OleCreatePropertyFrame(
			hWnd, 
			30, 
			30, 
			NULL, 
			1,
			(IUnknown **)&(pFilter.p), 
			cauuid.cElems,
			(GUID *)cauuid.pElems, 
			0, 
			0, 
			NULL);
		CoTaskMemFree(cauuid.pElems);
		pSpec->Release();
	}
	return hr;
}

#ifdef _DEBUG
/// Adds a DirectShow filter graph to the Running Object Table,
/// allowing GraphEdit to "spy" on a remote filter graph.
HRESULT DsShowGrabber::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
	CComPtr<IMoniker> pMoniker;
	CComPtr<IRunningObjectTable> pROT;
	WCHAR wsz[128];
	HRESULT hr;

	if (!pUnkGraph || !pdwRegister)
		return E_POINTER;

	if (FAILED(GetRunningObjectTable(0, &pROT)))
		return E_FAIL; 
	
//#if _MSC_VER > 1310
//	hr = StringCchPrintfW(wsz, 128, L"FilterGraph %08x pid %08x\0", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
//#else
	wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
//#endif
    hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if(SUCCEEDED(hr))
    {
        // Use the ROTFLAGS_REGISTRATIONKEEPSALIVE to ensure a strong reference
        // to the object.  Using this flag will cause the object to remain
        // registered until it is explicitly revoked with the Revoke() method.
        //
        // Not using this flag means that if GraphEdit remotely connects
        // to this graph and then GraphEdit exits, this object registration
        // will be deleted, causing future attempts by GraphEdit to fail until
        // this application is restarted or until the graph is registered again.
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
                            pMoniker, pdwRegister);
		//hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
		pMoniker.Release();
	}
	pROT.Release();
	return hr;
}

// Removes a filter graph from the Running Object Table
void DsShowGrabber::RemoveGraphFromRot(DWORD pdwRegister)
{
	IRunningObjectTable *pROT;

	if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
	{
		pROT->Revoke(pdwRegister);
		pROT->Release();
	}
}
#endif

bool DsShowGrabber::start()
{
	HRESULT hr = S_FALSE;
	if (m_avi && pSeeking)
	{
	pSeeking->SetTimeFormat(&TIME_FORMAT_FRAME);
	pSeeking->GetAvailable(&m_framePos, &m_frames);
	}
	if (pMediaControl)
        hr = pMediaControl->Run();
	if (FAILED(hr))
	{
		cout << "Error starting filter graph" << endl;
		return false;
	}
	else
		return true;
}

bool DsShowGrabber::stop()
{
	if (!pMediaControl)
		return false;	
	if (FAILED(pMediaControl->StopWhenReady()))
		return false;
	else
		return true;
}

void DsShowGrabber::TearDownGraph()
{
#ifdef _DEBUG
	if (g_dwGraphRegister) {
		RemoveGraphFromRot(g_dwGraphRegister);
		g_dwGraphRegister=0;
	}
#endif
	if (pMediaControl)
		pMediaControl->Stop();
    if (_eventThread!=NULL)
        CloseHandle(_eventThread);
	if (pSourceOutputPin)
		pSourceOutputPin.Release();
	if (pCaptureDeviceFilter)
		pCaptureDeviceFilter.Release();
	if (pSampleGrabberInputPin)
		pSampleGrabberInputPin.Release();
	if (pSampleGrabberOutputPin)
		pSampleGrabberOutputPin.Release();
	if (pSampleGrabber)
		pSampleGrabber.Release();
	if (pSampleGrabberFilter)
		pSampleGrabberFilter.Release();
	if (pNullRendererInputPin)
		pNullRendererInputPin.Release();
	if (pNullRendererFilter)
		pNullRendererFilter.Release();
	if (pEvent)
		pEvent.Release();
	if (pMediaControl)
		pMediaControl.Release();	
	if (pGraph)
        pGraph.Release();
}

STDMETHODIMP DsShowGrabber::QueryInterface(REFIID riid, void ** ppv)
    {
        //CheckPointer(ppv, E_POINTER);
        
        if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) 
        {
            *ppv = (void *) static_cast<ISampleGrabberCB *>(this);
            return NOERROR;
        }    
        return E_NOINTERFACE;
    }


STDMETHODIMP DsShowGrabber::SampleCB( double SampleTime, IMediaSample * pSample ) {

//#ifdef _DEBUG
//	static long lastTime = timeGetTime( );
//	long time = timeGetTime();
//	_frames++;
//	if (time-lastTime>1000)
//	{
//		float framesPerSecond = (float)(_frames * 1000) / (float)(time-lastTime);
//		lastTime = time;
//		_frames = 0;
//		//cout << "Device " << _deviceID <<" Frames per second " << framesPerSecond << endl; 
//	}	
//#endif
	WaitForSingleObject( m_imageLoc, INFINITE );
	if (!pCurrentSample)
		// adds a reference to the current sample
		pCurrentSample = pSample;	
	ReleaseMutex(m_imageLoc);
	return S_OK;
}


STDMETHODIMP DsShowGrabber::BufferCB( double SampleTime, BYTE * pBuffer, long BufferLen )
{
	return E_NOTIMPL;
}

HRESULT DsShowGrabber::SetCallback(ISampleGrabberCB *pCallback,long WhichMethodToCallback)
{
	pSampleGrabber->SetBufferSamples(false);
	return pSampleGrabber->SetCallback(pCallback, WhichMethodToCallback);
}
