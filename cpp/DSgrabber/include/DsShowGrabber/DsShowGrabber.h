#ifndef DSSHOWGRABBER_H_
#define DSSHOWGRABBER_H_

#include "Devices.h"

#pragma warning(push)	// disable for this header only
#pragma warning(disable:4251) // disable warning about dll-interface for templates
#pragma warning (disable:4995)
#pragma warning (disable:4996)

#include <atlbase.h>

#include <qedit.h>
#include <dshow.h>

/** \brief This class offers control for a direct show video capture device
* 
* The class provides a simple interface to the DirectShow Extension of DirectX to
* get images from several video sources. You have the choice to capture images either from a
* video camera or a video file. TV Cards are not supported yet.<br>
* The following example code is sufficient to get images from a directshow video camera:
* \code
#include "DsShowGrabber/DsShowGrabber.h"

int main(int argc, char* argv[])
{
    // desired image properties for video capturing
    int width = 640, height = 480, pixelDepth = 24;
    // create new grabber object instance
    DsShowGrabber* grabber = new DsShowGrabber();
    // try to initialize the grabber by choosing the first available video device
    if (!grabber->init(NULL, NULL, width, height, pixelDepth))          
    {
        printf("Grabber initialization failed! No video capture device?\n");
        delete grabber;
        return -1;
    }
    // capture images by the grabber object itself
    if (grabber->SetCallback(grabber,0)!=S_OK)
    {
        delete grabber;
        return -1;
    }
    // get real image properties in case the desired ones were not available
    pixelDepth = grabber->getDepth();
    width = grabber->getWidth();
    height = grabber->getHeight();
    unsigned int size = grabber->getBufferSize();
    unsigned char* data  = new unsigned char[size];
    bool run = grabber->start();
    int i=0;
    // do image processing on the first 100 images
    while (run && i < 100)
    {
        // grab new image
        bool newImage = grabber->grabFrame(data, size);
        // do image processing on data 
        if (newImage)
        {
           printf("Processing new image %d\n", i++);
           // ...           
        }                
        // set run = false to stop   
    }
    grabber->stop();
    delete grabber;
    grabber = 0;
    delete[] data;
    data = 0;
    return 0;
}
    \endcode
* <br><br>The next example shows how to capture video images from a file without dropping frames <br>
* \code
#include "DsShowGrabber/DsShowGrabber.h"

int main(int argc, char* argv[])
{
    // create new grabber object instance
    DsShowGrabber* grabber = new DsShowGrabber();
    // try to initialize the grabber, loading a test video without looping
    if (!grabber->init("test.avi", false))          
    {
        printf("Grabber initialization failed! Video file not available?\n");
        delete grabber;
        return -1;
    }
    // if no frames should be dropped, you are not allowed to set a callback
    // and have to set one shot mode to true
    if (!grabber->setOneShot(true))
    {
        printf("Setting one shot mode failed\n");
        delete grabber;
        return -1;
    }
    // get real image properties in case the desired ones were not available
    unsigned int size = grabber->getBufferSize();
    unsigned char* data  = new unsigned char[size];
    bool run = grabber->start();
    int i=0;
    // do image processing on the whole video
    while (run)
    {
        // get next video image, waiting endlessly
        if (!grabber->getBuffer(data, size, INFINITE))
        {
            printf("Getting video buffer failed or video is at it's end\n");
            break;
        }
        printf("Processing new image %d\n", i++);        
    }
    grabber->stop();
    delete grabber;
    grabber = 0;
    delete[] data;
    data = 0;
    return 0;
}
\endcode
* @author wiendl, April 30 12:10 2005
*/

// Diese Klasse wird aus DsShowGrabber.dll exportiert
class DSSHOWGRABBER_API DsShowGrabber : public ISampleGrabberCB
{
public:

    /**
     * \brief Creates a new instance and initializes the Windows COM and variables
     */
	DsShowGrabber();
    /*
     * Stops filtergraph, cleaning up the allocated memory and uninitialize the Windows COM
     */
	~DsShowGrabber();

	/// ISampleGrabberCB override method
	STDMETHODIMP_(ULONG) AddRef()  { return 2; }
	/// ISampleGrabberCB override method
	STDMETHODIMP_(ULONG) Release() { return 1; }
	/// ISampleGrabberCB override method
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);

	/** ISampleGrabberCB override method
	* Sample Callback function, receives a pointer to the current media sample and a timestamp of this sample
	* @param SampleTime the timestamp of the sample
	* @param pSample pointer to the current IMediaSample
	* @return HRESULT S_OK
	*/
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample );
	/**  ISampleGrabberCB override method. <br> CAUTION, this function is not implemented	*/
	STDMETHODIMP BufferCB( double SampleTime, BYTE * pBuffer, long BufferLen );

	/** \brief Builds the filtergraph for a video camera device. 
     *
	 * This method intializes the grabber object. It'll search for a device with the given displayname or - if set to NULL -
	 * the given friendlyname. If the friendlyname is also NULL, the first available device will be chosen.
     * @see Devices<br>
	 * After adding the source filter to the graph, the output pin of the video device will be set to width, height and
	 * pixeldepth if possible. Otherwise the video device's pin dialog will pop up. <br>
	 * If the given pixeldepth is not equal to 8, 24 or 32 the sample grabbers pin will be set to 24bit by default to assure that there 
	 * will be an image, libraries like the OpenCV can handle.
	 *   @param friendlyName the directshow filters friendlyname of the video device
	 *   @param displayName the directshow moniker displayname of the video device (choose this instead of the friendlyname
	 *   if you have several devices with the same friendlyname)
	 *   @param width the desired image width
	 *   @param height the desired image height 
	 *   @param pixelDepth the desired image depth in bits
     *   @return true on success, otherwise false
	*/
	bool init(const char* friendlyName, const char* displayName, int width, int height, int pixelDepth);

    /** \brief Builds the filtergraph for a video file. 
      *
	 * This method intializes the grabber object. It'll try to load the given file by selecting
     * an appropriate direct show codec filter. <br>
	 *   @param fileName the filename (including path if necessary) of the video file to load
	 *   @param loop if set to true the video will be repeated endlessly
     *   @param pixelDepth if you want a specific color depth, you can specify values of 8, 24 or 32bit to automatically convert
     *    video images to this format. By default all images are converted to 24bit
     *   @return true on success, otherwise false
     */
    bool init(const char* fileName, bool loop, int pixelDepth = 24);

	/**
	The size of the image buffer in bytes
	*/
	long  getBufferSize() {return _size;}
	/**
	The image width of the video device's image
	*/
	int	  getWidth() {return _width;}
	/**
	The image height of the video device's image
	*/
	int	  getHeight() {return _height;}
	/**
	The image depth of the video device's image
	*/
	int	  getDepth() {return _pixelDepth;}
	/// call this method to start the filtergraph
	bool start();
	/// call this method to stop the filtergraph
	bool stop();
	/// returns true if the image is flipped (CAUTION not yet finished)
	bool isFlipped() {return _flipped;}
	
	/** \brief if avi file is captured, this avoids droping frames.
     *
	 * If disabled the number of dropped frames depends on the performance of the machine the grabber is running on.
     * Only related to avi file capturing: <br>
     * @param oneShot if set to true, no frame will be dropped
	 * @return false in case of an error	
     */
	bool setOneShot(bool oneShot);
	
	/// Only related to avi file capturing: <br>
	/// @return the number of all frames of the avi file
	__int64 getNumberOfFrames() {return m_frames;}

	/// Only related to avi file capturing: <br>    
	/// @param loop specifies if the avi file be repeated
	void setAviLoop(bool loop) {m_loopAvi = loop;}

	/** \brief gets the current video buffer. 
     * If you want to use this method NO Callback must be set
	 * @param pBuffer specifies an allocated array of <i>size</i> bytes
	 * @param size the size of the given image buffer
	 * @param timeout if not 0, this will be the duration the grabber waits for a new frame 
     */	
	bool getBuffer(unsigned char* pBuffer, long size, long timeout = 0);

	/** \brief grabs a new frame if there's one available. 
	 * If you want to use this function, you MUST set the instance of this class as callback. \n
	 * e.g. \n
	 * \code	
        DsVideoGrabber* grabber = new DsVideoGrabber();
        grabber->init(...);
        grabber->SetCallback(grabber, 0);
        grabber->start();
        grabber->grabFrame(buffer, size);
	   \endcode
      * @param buffer specifies an allocated array of size bytes
	  * @param size the size of the given image buffer
	  * @return false if the given size does not match with the current video buffer size or there is no new image. 
      * 
      */
	bool grabFrame(unsigned char* buffer, const long size);

	/// the best method to get the current sample is to take this method and syncronize all
	/// access to the image buffer within the given callback function of the ISampleGrabberCB object
	HRESULT SetCallback(ISampleGrabberCB *pCallback,long WhichMethodToCallback);

	/// shows the current video capture source filter dialog
	void showFilterProperties() {if (pCaptureDeviceFilter) displayFilterProperties(pCaptureDeviceFilter);}

private:

	CComPtr<IGraphBuilder>			pGraph;
	CComQIPtr<IMediaControl>		pMediaControl;
	CComQIPtr<IMediaEvent>			pEvent;
	// Get the seeking interface, so we can seek to a location
	CComQIPtr< IMediaSeeking, &IID_IMediaSeeking > pSeeking;

	// Filter
	CComPtr<IBaseFilter>			pCaptureDeviceFilter;
	CComPtr<IBaseFilter>			pSampleGrabberFilter;
	CComPtr<IBaseFilter>			pNullRendererFilter;

	// Interfaces
	CComPtr<ISampleGrabber>			pSampleGrabber;

	// Pins
	CComPtr<IPin>					pSourceOutputPin;
	CComPtr<IPin>					pSampleGrabberInputPin;
	CComPtr<IPin>					pSampleGrabberOutputPin;
	CComPtr<IPin>					pNullRendererInputPin;

	CComPtr<IMediaSample>			pCurrentSample;

	HANDLE							m_imageLoc;

	int	_width, _height, _pixelDepth;
	// number of frames of the loaded avi file
	__int64 m_frames;

    HANDLE _eventThread;

	int	_deviceID;

	long	_size;
	unsigned char* _pBuffer;
	bool	_newImage;
	bool	_flipped;

	REFERENCE_TIME m_framePos;
	bool	m_singleShot;
	bool	m_avi;
	bool	m_loopAvi;

	HRESULT getCaptureDeviceFilter(		  
		const char* friendlyName,
		const char* displayName,
		CComPtr<IBaseFilter>& pCaptureDeviceFilter);

  // 	HRESULT getVideoFileFilter(		  
		//const char* fileName,
		//CComPtr<IBaseFilter>& pCaptureDeviceFilter);

	HRESULT getInputOutputPins(
		CComPtr<IBaseFilter>& filter,
		CComPtr<IPin>& pInputPin, 
		CComPtr<IPin>& pOutputPin);

	HRESULT setSourceProperties(
		CComPtr<IPin>& pOutputPin,
		int width, 
		int height, 
		int pixelDepth);

	HRESULT getGrabberProperties();

	HRESULT displayPinProperties(
		CComPtr<IPin>& pSrcPin, 
		HWND hWnd = NULL);

	HRESULT displayFilterProperties(CComPtr<IBaseFilter>& pFilter, HWND hWnd = NULL);

	void freeMediaType(AM_MEDIA_TYPE& mt);
	void deleteMediaType(AM_MEDIA_TYPE* mt);

	void TearDownGraph();

#ifdef _DEBUG
    HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
	void RemoveGraphFromRot(DWORD pdwRegister);

	DWORD g_dwGraphRegister;
#endif

    static unsigned long __stdcall listeningEvent(void* pEvent);
};
#pragma warning(pop)  	// restore original warning level
#endif