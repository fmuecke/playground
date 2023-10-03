#include "stdafx.h"
#include <exception>
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

static int InitStdOut()
{
    auto handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if (!handle || handle == INVALID_HANDLE_VALUE)
    {
        if (!::AttachConsole(ATTACH_PARENT_PROCESS))
        {
            const auto err = ::GetLastError();
            if (err == ERROR_INVALID_HANDLE)
            {
                if (!::AllocConsole()) return ::GetLastError();
            }
            else return err;
        }
    }

    FILE* fp{ nullptr };
    freopen_s(&fp, "CONOUT$", "w", stdout);
    //freopen_s(&fp, "CONERR$", "w", stderr);
    //freopen_s(&fp, "CONIN$", "w", stdin);

    return NO_ERROR;
}

DWORD WriteStdOut(const string& str)
{
    auto stdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD bytesWritten{ 0 };
    ::WriteConsoleA(stdOut, str.c_str(), str.size(), &bytesWritten, nullptr);
    return bytesWritten;
}

int main()
{
    //cout << "before console init" << endl;
    //cout << "after console init" << endl;
    //WriteStdOut("\nblablub");

    return NO_ERROR;
}

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
    InitStdOut();

    cout << "\nasd" << endl;

    auto retCode = main();
    cout << "\npress ENTER to continue..." << endl;
    
    auto consoleWnd = ::GetConsoleWindow();
    cout << consoleWnd;
    
    //::PostMessageA(consoleWnd, WM_KEYDOWN, VK_RETURN, 10);
    //::PostMessageA(consoleWnd, WM_KEYUP, VK_RETURN, 10);
    //::SendMessageA(consoleWnd, WM_SETTEXT, 0, (LPARAM)"fuck!");

    FreeConsole();
    FreeConsole();
    FreeConsole();
    cerr << endl;

    return retCode;
}
