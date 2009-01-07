#include "InputDevice.h"

using namespace base;

InputDevice::InputDevice( InputSystem* pSystem ) {
	_pDevice = NULL;
	_pInputObject = pSystem->GetInputObject();
	_hWnd = pSystem->GetWindow();
}

InputDevice::~InputDevice() {
	_pDevice->Unacquire();
	RELEASE( _pDevice );
}

