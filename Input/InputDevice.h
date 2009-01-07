#pragma once


#include "InputSystem.h"

namespace base{
	class InputDevice{
	protected:
		LPDIRECTINPUT _pInputObject;
		LPDIRECTINPUTDEVICE _pDevice;
		HWND _hWnd;
	private:
		virtual BOOL InitializeDevice() = 0;
	public:
		InputDevice( InputSystem* pSystem );
		virtual ~InputDevice();
		virtual BOOL Update() = 0;
		virtual BOOL IsDown( const char* key ) = 0;
		virtual BOOL IsUp( const char* key ) = 0;
		virtual void DumpStatus( HDC hDC ) = 0;
	};
}

