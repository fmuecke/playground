#include "DsShowGrabber.h"

#include <vector>
#include <iostream>

class Resolution {
public:
	Resolution() : width(0), height(0), pixeldepth(0) {}
	Resolution(int w, int h, int d) : width(w), height(h), pixeldepth(d) {}
	int width, height, pixeldepth;
};

VideoDevice* returnDevices (int& numDevices)
{
	numDevices = -1;
	int		temp = 0;
	HRESULT hr;
	BOOL bCheck = FALSE;

	std::vector<VideoDevice> sources;

	CoInitialize(NULL);
	// enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	hr = pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER);
	if(hr != NOERROR) {
		std::cout << "Error Creating Device Enumerator" <<std::endl;
		CoUninitialize();
		return 0;
	}

	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if(hr != NOERROR) {
		pCreateDevEnum.Release();
		#ifdef _DEBUG
				std::cout << "Sorry, you have no video capture hardware" << std::endl;  
		#endif
		CoUninitialize();
		return 0;
	}
	pEm->Reset();	
	CComPtr<IMoniker> pM;	
	while(pEm->Next(1, &pM, NULL)==S_OK && sources.size()<9) 
	{
		CComPtr<IPropertyBag> pBag;
		VideoDevice device;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) {
			VARIANT varName;
			VariantInit(&varName);
			hr = pBag->Read(L"Description", &varName, 0);			
			if (FAILED(hr))
				hr = pBag->Read(L"FriendlyName", &varName, 0);
			if (SUCCEEDED(hr))
				// Add it to the application's list box.
				device.friendlyName = ConvertLPWSTRToLPSTR(varName.bstrVal);
			VariantClear(&varName); 	
			pBag.Release();
			WCHAR *szDisplayName=NULL;
			if(SUCCEEDED(pM->GetDisplayName(NULL,NULL,&szDisplayName)))
			{				
				LPSTR str = ConvertLPWSTRToLPSTR(szDisplayName);
				std::string deviceName = str;				
				delete[] str;
				size_t index = deviceName.find('#',0);
				if (index!=std::string::npos)
				{				
					deviceName = deviceName.substr(index, deviceName.length()-index);
					index = deviceName.find('{',0);
					if (index!=std::string::npos)
					{	
						deviceName = deviceName.substr(0, index);
						device.displayName = new char[deviceName.length()+1];
#if _MSC_VER > 1310
						strcpy_s(device.displayName, deviceName.length()+1, deviceName.c_str());
#else
                        strcpy(device.displayName, deviceName.c_str());
#endif
					}
					else
					{
						pM.Release();
						continue;
					}
				}
				else
				{
					pM.Release();
					continue;
				}
			}		
		}
		else
		{
			pM.Release();
			continue;
		}
		CComPtr<IBaseFilter> VideoSrc    = NULL;	
		CComPtr<IPin>	     VideoSrcPin = NULL;
		hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)&VideoSrc);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			std::cout << "Error binding Videosource" << std::endl;
#endif
			pM.Release();
			continue;
		}				
		CComPtr<IEnumPins> pEnum;
		VideoSrc->EnumPins(&pEnum);
		hr = pEnum->Reset();
		CComPtr<IPin> pP;
		while (hr=pEnum->Next(1, &pP, NULL)==S_OK) {			
			PIN_INFO pininfo;				
			hr = pP->QueryPinInfo(&pininfo);
			if(hr == NOERROR) {
				if(pininfo.dir == PINDIR_OUTPUT) {					
#ifdef _DEBUG
					std::cout << "Found OutputPin on VideoSource"<< std::endl;
#endif
					pP.CopyTo(&VideoSrcPin);	
					pininfo.pFilter->Release();
					pP.Release();
					break;
		        }
			}
			pininfo.pFilter->Release();
			pP.Release();
		}		
		pEnum.Release();

		if (!VideoSrcPin) {
#ifdef _DEBUG
			std::cout << "Couldn't save VideoSource-OutputPin" << std::endl;					
#endif
			VideoSrc.Release();		
			pM.Release();
			continue;
		}

		CComPtr<IEnumMediaTypes> pMedia; //The IEnumMediaTypes interface enumerates the preferred formats for a pin. 
		AM_MEDIA_TYPE *pmt = NULL;
		// Saving all prefered Videoformat-types of pCamaeraOutputPin in pMedia
		std::vector<Resolution> resolutions;
		hr = VideoSrcPin->EnumMediaTypes( &pMedia );
		if(SUCCEEDED(hr)) {
			while(pMedia->Next(1, &pmt, 0) == S_OK) {
				if( pmt->formattype == FORMAT_VideoInfo ) {	
					VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->pbFormat;
#ifdef _DEBUG
					std::cout << "Possible Resolution: " << vih->bmiHeader.biWidth << "x" << vih->bmiHeader.biHeight 
							<< " Bits/Pixel: " << vih->bmiHeader.biBitCount << std::endl ;	
#endif
					resolutions.push_back(Resolution(vih->bmiHeader.biWidth,vih->bmiHeader.biHeight,vih->bmiHeader.biBitCount));					
		        }
				if (pmt != NULL) 
				{
					if (pmt->cbFormat != 0) {
						CoTaskMemFree((PVOID)pmt->pbFormat);

						// Strictly unnecessary but tidier
						pmt->cbFormat = 0;
						pmt->pbFormat = NULL;
					}
					if (pmt->pUnk != NULL) {
						pmt->pUnk->Release();
						pmt->pUnk = NULL;
					}
					CoTaskMemFree((PVOID)pmt);
				}
				pmt=NULL;
		    }
			device.widths = new int[resolutions.size()];
			device.heights = new int[resolutions.size()];
			device.depths = new int[resolutions.size()];
			device.nrResolutions = resolutions.size();
			for (size_t i=0; i<resolutions.size(); ++i)
			{
				device.widths[i] = resolutions.at(i).width;
				device.heights[i] = resolutions.at(i).height;
				device.depths[i] = resolutions.at(i).pixeldepth;
			}
		}		

		VideoSrcPin.Release();
		VideoSrc.Release();
		pMedia.Release();    
		pM.Release();
		sources.push_back(device);
	}
	pCreateDevEnum.Release();
	pEm.Release();
#ifdef _DEBUG
	//std::cout << "Number of Capture Devices: " << uIndex << std::endl;
#endif
	CoUninitialize();
	VideoDevice* retVal = new VideoDevice[sources.size()];
	for (size_t i=0; i<sources.size(); ++i)
		retVal[i] = sources.at(i);
	numDevices = sources.size();
	return retVal;
}


void releaseDevices(VideoDevice*& devices)
{
	delete[] devices;
	devices = 0;
}


char* ConvertLPWSTRToLPSTR(wchar_t* lpwszStrIn)
{
	LPSTR pszOut = NULL;

	if (lpwszStrIn != NULL)
	{
		int nInputStrLen = static_cast<int>(wcslen (lpwszStrIn));

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;    

		pszOut = new char [nOutputStrLen];

		if (pszOut)
		{
			memset (pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	return pszOut;
}