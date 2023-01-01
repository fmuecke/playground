#include "stdafx.h"

//
// Simple demo program to take a screen shot of the desktop and the current application
//
// Version: 0.4 (14:26 02.09.2010 F.Muecke)
//
// TODO:
//	- CHECK FOR RESSOURCE LEAKS
//	DONE: optionally capture mouse cursor
//	- capture full desktop (currently only the primary monitor is captured)
//  - support other color depths
//  - support compression and/or jpeg/png as output format
//


using namespace std;

class BFCScreenShot
{
public:
	BFCScreenShot(const TCHAR* baseFileName)
	: m_baseFileName(baseFileName)
	, m_fileCount(0)
	{
	};
	
	bool CaptureScreen( bool doCaptureMouse = true )
	{
		return do_capture( NULL, doCaptureMouse );
	}
	
	// if no window is specified, the foreground window will be used
	bool CaptureWindow( const HWND hWnd = NULL, bool doCaptureMouse = true )
	{
		return do_capture( hWnd ? hWnd : ::GetForegroundWindow(), doCaptureMouse );
	}

private:
	// If no window handle is specified, the entire screen will be captured
	// NOTE, this does only work for the primary monitor!
	// TODO: Use EnumDisplayMonitors to get the DC of all monitors.
	bool do_capture( 
		const HWND hWnd = NULL, 
		bool doCaptureMouse = false)
	{
		HDC hDC = ::GetWindowDC( hWnd );
		HDC hMemDC = CreateCompatibleDC(hDC);
		_ASSERTE( hMemDC && "CreateCompatibleDC failed!" );
		
		// Get the client area for size calculation
		int width(0);
		int height(0);
		POINT offset = {0};
		if( NULL == hWnd )
		{
			width = ::GetSystemMetrics(SM_CXSCREEN);
			height = ::GetSystemMetrics(SM_CYSCREEN);
		}
		else
		{
			RECT rcClient = {0};
			::GetWindowRect(hWnd, &rcClient);
			width = rcClient.right-rcClient.left;
			height = rcClient.bottom-rcClient.top;
			offset.x = rcClient.left;
			offset.y = rcClient.top;
		}

		// Create a compatible bitmap from the Window DC
		HBITMAP hbm = ::CreateCompatibleBitmap( hDC, width, height );
		_ASSERTE( hbm && "CreateCompatibleBitmap failed!" );
		
		// Select the compatible bitmap into the compatible memory DC.
		::SelectObject( hMemDC, hbm );
		
		// Bit block transfer into our compatible memory DC.
		const BOOL bRet = BitBlt( hMemDC, 0,0, width, height, hDC, 0,0, SRCCOPY );
		if( FALSE == bRet )
		{
			DWORD err = ::GetLastError();
			::DeleteObject(hbm);
			::ReleaseDC(hWnd, hMemDC);
			::ReleaseDC(hWnd, hDC);
			return false;
		}
		
		if( doCaptureMouse )
			draw_mouse_cursor(hMemDC, offset);

		// Get the BITMAP from the HBITMAP
		BITMAP bmpScreen = {0};
		::GetObject( hbm, sizeof(BITMAP), &bmpScreen );
		 
		BITMAPFILEHEADER bmfHeader = {0};
		BITMAPINFOHEADER bi = {0};
		
		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bmpScreen.bmWidth;    
		bi.biHeight = bmpScreen.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 16; // 16bit should be enough for screenshots
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;
		bi.biSizeImage = bi.biBitCount * width * height / 8;

		const DWORD dwBmpSize = bi.biSizeImage;

		HANDLE hDIB = ::GlobalAlloc(GHND, dwBmpSize); 
		char* lpbitmap = static_cast<char *>(::GlobalLock(hDIB));

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		::GetDIBits( 
			hDC, 
			hbm, 
			0, 
			bmpScreen.bmHeight,
			lpbitmap,
			reinterpret_cast<BITMAPINFO*>(&bi), 
			DIB_RGB_COLORS );

		// A file is created, this is where we will save the screen capture.
		const HANDLE hFile = ::CreateFile( 
			m_baseFileName,//get_outfile_name(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, 
			NULL);   
		
		// Add the size of the headers to the size of the bitmap to get the total file size
		const DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	 
		// Offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = 
			static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) + 
			static_cast<DWORD>(sizeof(BITMAPINFOHEADER)); 
		
		bmfHeader.bfSize = dwSizeofDIB; 
		bmfHeader.bfType = 0x4D42; //BM   
	 
		DWORD dwBytesWritten = 0;
		::WriteFile(hFile, reinterpret_cast<LPSTR>(&bmfHeader), sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
		::WriteFile(hFile, reinterpret_cast<LPSTR>(&bi), sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
		::WriteFile(hFile, static_cast<LPSTR>(lpbitmap), dwBmpSize, &dwBytesWritten, NULL);
		
		// Unlock and Free the DIB from the heap
		::GlobalUnlock(hDIB);    
		::GlobalFree(hDIB);

		// Close the handle for the file that was created
		::CloseHandle(hFile);
		
		// Clean up
		::DeleteObject(hbm);
		::ReleaseDC(hWnd, hMemDC);
		::ReleaseDC(hWnd, hDC);
		return true;
	}
	
private:
	void draw_mouse_cursor(HDC hDC, POINT const& offset)
	{
		CURSORINFO ci = {0};
		ci.cbSize = sizeof(CURSORINFO);
		if( FALSE == ::GetCursorInfo( &ci ) )
		{
			const DWORD err(::GetLastError());
			_ASSERTE(!"GetCursorInfo failed");
		}
		else
		{
			_ASSERTE( CURSOR_SHOWING == ci.flags );

			const HICON hIcon( ::CopyIcon(ci.hCursor) );
			_ASSERTE( hIcon );

			ICONINFO iconInfo = {0};
			if( FALSE == ::GetIconInfo( hIcon, &iconInfo ) )
			{
				const DWORD err(::GetLastError());
				_ASSERTE(!"GetIconInfo failed");
			}
			else
			{
				POINT p;
				::GetCursorPos(&p);
				int x = ci.ptScreenPos.x - static_cast<int>(iconInfo.xHotspot) - offset.x;
				int y = ci.ptScreenPos.y - static_cast<int>(iconInfo.yHotspot) - offset.y;
				::DrawIcon( hDC, x, y, hIcon );
			}
			::DestroyIcon(hIcon);	
		}
	}
	
	//const CString get_outfile_name()
	//{
	//	CString out;
	//	out.Format("%s%.3d.bmp", m_csBaseFileName, m_fileCount);
	//	m_fileCount++;
	//	return out;
	//}

	const TCHAR* m_baseFileName;
	unsigned int m_fileCount;
};

	
void print_help()
{
	cout << "SceenShot v0.4, (c) F.Muecke\n"
		 << "\n"
		 << "Writes the current desktop content to the specified file\n\n"
		 << "Usage:\tScreenShot.exe [options] <outfile.bmp>\n\n"
		 << "Options:\n"
		 << "  /m\tcapture mouse cursor\n"
		 << "  /w\tcapture current foreground window\n"
		 << "\n"
		 << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool captureCursor(false);
	if( argc > 1 && argv[1] == string("/m") || 
		argc > 2 && argv[2] == string("/m") )
		captureCursor = true;

	bool captureWindow(false);
	if( argc > 1 && argv[1] == string("/w") || 
		argc > 2 && argv[2] == string("/w") )
		captureWindow = true;
	
	if( argc == 1 ||
		argc > 1 && argv[0] == string("/?") || 
		argc > 2 && argv[1] == string("/?") )
	{
		print_help();
		return 0;
	}

	if( argv[argc-1][0] == '/' ) // file name check
	{
		print_help();
		return 0;
	}

	// Get proper file name
	string fileName(argv[argc-1]);
	const size_t pos = fileName.rfind('.');
	if( string::npos == pos )
	{
		fileName.append(".bmp");
	}
	else
	{
		const string ext(fileName.substr(pos+1));
		if( ext != "bmp" )
			fileName.replace(pos+1, ext.size(), "bmp");
	}

	BFCScreenShot camera(fileName.c_str());
	if( captureWindow )
		camera.CaptureWindow(NULL, captureCursor);
	else
		camera.CaptureScreen(captureCursor);

	return 0;
}