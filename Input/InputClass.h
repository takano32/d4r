#pragma once

#include <map>
#include "InputSystem.h"
#include "KeyboardDevice.h"
#include "JoystickDevice.h"

using namespace std;



namespace base {
	class InputClass{
	private:
		HWND _hWnd;
		InputSystem* _pSystem;
		KeyboardDevice* _pKeyboardDevice;
		JoystickDevice* _pJoystickDevice;
	public:
		InputClass( HWND hWnd );
		~InputClass();
		BOOL Update();
		BOOL IsDown( const char* key );
		BOOL IsUp( const char* key );
	};
}

