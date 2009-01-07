#pragma once

#include "InputDevice.h"

namespace base{
	class KeyboardDevice : public InputDevice {
	private:
		BYTE* _pKeyState;
		BOOL InitializeDevice();
	public:
		KeyboardDevice( InputSystem* pSystem );
		~KeyboardDevice();
		BOOL Update();
		BOOL IsDown( const char* key );
		BOOL IsUp( const char* key );
		void DumpStatus( HDC hDC );
	};
}