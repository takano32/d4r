#pragma once

#include <windows.h>
#include <map>

using namespace std;

#pragma comment (lib, "msvcrt-ruby18.lib")
#include <ruby.h>

/*
namespace base{
	class WindowClass;
}
*/

namespace base{
	class WindowClass{
	private:
		static char* _psClassName;
		static char* _psWindowName;
		//static map<HWND,WindowClass> _map;
		static WNDPROC _pWndProc;
		HINSTANCE _hInstance;
		HWND _hWnd;
		MSG _msg;
		WNDCLASS _wndClass;
		VALUE _loopAction;

		BOOL RegisterClass();
	public:
		WindowClass();
		~WindowClass();
		HWND GetWindowHandle();
		LONG GetUserData();
		static LONG GetUserData( HWND hWnd );
		BOOL SetUserData( LONG data );
		BOOL SetWindowProcedure( WNDPROC WndProc );
		BOOL Create();
		BOOL Show();
		VALUE CallBackAction();
		VALUE SetLoopAction( VALUE action );
		BOOL Main();
	};
}



