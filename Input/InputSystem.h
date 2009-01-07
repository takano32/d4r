#pragma once

#define DIRECTINPUT_VERSION 0x700
#pragma comment(lib, "dinput.lib")
#pragma comment(lib, "dxguid.lib")
#define RELEASE(x) if(x){ x->Release(); x=NULL; }


#include <windows.h>
#include <dinput.h>


namespace base{
	class InputSystem{
		LPDIRECTINPUT _pInputObject;
		HWND _hWnd;
		HINSTANCE _hInstance;
	private:
		BOOL InitializeSystem();
	public:
		InputSystem( HWND hWnd );
		~InputSystem();
		LPDIRECTINPUT GetInputObject();
		HWND GetWindow();
	};
}