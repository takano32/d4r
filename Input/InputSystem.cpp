#include "InputSystem.h"

using namespace base;

InputSystem::InputSystem( HWND hWnd ) {
	_hWnd = hWnd;
	_hInstance = GetModuleHandle( NULL );
	InitializeSystem();
}

InputSystem::~InputSystem() {
	RELEASE( _pInputObject );
}


BOOL InputSystem::InitializeSystem() {
	HRESULT hr;

    // DirectInput オブジェクトの生成
    hr = DirectInputCreate(_hInstance, DIRECTINPUT_VERSION,
        &_pInputObject, NULL);
    if(FAILED(hr)) return FALSE;
    return TRUE;
}

LPDIRECTINPUT InputSystem::GetInputObject() {
	return _pInputObject;
}

HWND InputSystem::GetWindow() {
	return _hWnd;
}
