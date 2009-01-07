#pragma once


#include "InputSystem.h"
#include "InputDevice.h"

namespace base{
	class JoystickDevice : public InputDevice{
	private:
		DIJOYSTATE* _pJoyState;
		BOOL InitializeDevice();
	public:
		JoystickDevice( InputSystem* pSystem );
		~JoystickDevice();
		BOOL Update();
		BOOL IsDown( const char* key );
		BOOL IsUp( const char* key );
		void DumpStatus( HDC hDC );
		BOOL EnumDevicesProcess( const DIDEVICEINSTANCE* pdidInstance );
		static BOOL CALLBACK EnumDevicesCallBack( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
		BOOL EnumAxesProcess( LPCDIDEVICEOBJECTINSTANCE lpddoi );
		static BOOL CALLBACK EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef );
	};
}

