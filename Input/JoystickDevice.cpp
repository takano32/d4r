#include "JoystickDevice.h"

using namespace base;

JoystickDevice::JoystickDevice( InputSystem* pSystem ):InputDevice( pSystem ){
	InitializeDevice();
	_pJoyState = new DIJOYSTATE();
}

JoystickDevice::~JoystickDevice() {
	if( _pJoyState ) {
		delete _pJoyState;
		_pJoyState = NULL;
	}
}

BOOL JoystickDevice::InitializeDevice() {
	HRESULT hr;
    // デバイスの列挙 (CreateDevice)
    hr = _pInputObject->EnumDevices( DIDEVTYPE_JOYSTICK, EnumDevicesCallBack, this, DIEDFL_ATTACHEDONLY );
    if(FAILED(hr) || _pInputObject == NULL) return FALSE;

	// デバイスが一つもなかったら中断
	if(!_pDevice) return FALSE;

    // データフォーマットの設定
    hr = _pDevice->SetDataFormat(&c_dfDIJoystick);
    if(FAILED(hr)) return FALSE;
	
    // 協調モードの設定
    hr = _pDevice->SetCooperativeLevel( _hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
    if(FAILED(hr)) return FALSE;
	
    // 軸の設定
    hr = _pDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
    if(FAILED(hr)) return FALSE;
	
    // ポーリングの設定
    //lpDInputDEV->Poll();
	
    // 入力制御の開始
    _pDevice->Acquire();
	return TRUE;
}


BOOL JoystickDevice::EnumDevicesProcess( const DIDEVICEINSTANCE* pdidInstance ) {
	// デバイスの生成
    HRESULT hr = _pInputObject->CreateDevice(pdidInstance->guidInstance, &_pDevice, NULL);
    if(FAILED(hr)) return DIENUM_CONTINUE;
	
    // ジョイスティック能力の取得
    DIDEVCAPS didevcaps;
    ZeroMemory(&didevcaps, sizeof(didevcaps));
    didevcaps.dwSize = sizeof(DIDEVCAPS);
    hr = _pDevice->GetCapabilities(&didevcaps);
    if(FAILED(hr)){
        RELEASE(_pDevice);
        return DIENUM_CONTINUE;
    }
    return DIENUM_STOP;
}



BOOL CALLBACK JoystickDevice::EnumDevicesCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	JoystickDevice* pJoystickDevice = (JoystickDevice*)pContext;
	return pJoystickDevice->EnumDevicesProcess( pdidInstance );
}


BOOL JoystickDevice::EnumAxesProcess( LPCDIDEVICEOBJECTINSTANCE lpddoi ) {
	// 軸の列挙
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize       = sizeof(diprg);            // DIPROPDWORD サイズ
    diprg.diph.dwHeaderSize = sizeof(diprg);            // DIPROPHEADER サイズ
    diprg.diph.dwObj        = lpddoi->dwType;           // オブジェクト
    diprg.diph.dwHow        = DIPH_BYID;                // 解釈方法
    diprg.lMin              = -1000;                    // 範囲の下限
    diprg.lMax              = +1000;                    // 範囲の上限
    HRESULT hr = _pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    if(FAILED(hr)) return DIENUM_STOP;
    return DIENUM_CONTINUE;
}

BOOL CALLBACK JoystickDevice::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef ) {
	JoystickDevice* pJoystickDevice = (JoystickDevice*)pvRef;
	return pJoystickDevice->EnumAxesProcess( lpddoi );
}

BOOL JoystickDevice::Update() {
	if(!_pDevice) return FALSE;
	HRESULT hr = _pDevice->GetDeviceState(sizeof(DIJOYSTATE), _pJoyState );
	if( FAILED(hr) ) {
		// 入力制御の再開
        _pDevice->Acquire();
	}
	return SUCCEEDED(hr);
}

BOOL JoystickDevice::IsDown( const char* key ) {
	BOOL down = FALSE;
	if(!_pDevice) return down;
	if( !stricmp(key, "joy 0") ) {
		down = _pJoyState->rgbButtons[0]?TRUE:FALSE;
	}
	if( !stricmp(key, "joy 1") ) {
		down = _pJoyState->rgbButtons[1]?TRUE:FALSE;
	}

	if( !stricmp(key, "joy up") ) {
		down = (short)_pJoyState->lY + 1 == 1;
	}

	if( !stricmp(key, "joy down") ) {
		down = (short)_pJoyState->lY + 1 == 0;
	}

	if( !stricmp(key, "joy left") ) {
		down = (short)_pJoyState->lX + 1 == 1;
	}

	if( !stricmp(key, "joy right") ) {
		down = (short)_pJoyState->lX + 1 == 0;
	}

	down =  down && (GetActiveWindow() == _hWnd);
	return down;
}

BOOL JoystickDevice::IsUp( const char* key ) {
	return !IsDown( key );
}


void JoystickDevice::DumpStatus( HDC hDC ) {
	// ジョイスティック状態の取得
    DIJOYSTATE dijs;
    HRESULT hr = _pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs);
    if(SUCCEEDED(hr))
    {
        // ジョイスティック状態の表示
        char str[256];
        wsprintf(str, "Ｘ軸 ： %d", (short)dijs.lX + 1);
        TextOut(hDC, 20, 20, str, strlen(str));
        wsprintf(str, "Ｙ軸 ： %d", (short)dijs.lY + 1);
        TextOut(hDC, 20, 40, str, strlen(str));
        wsprintf(str, "ボタン１ ： %s", (dijs.rgbButtons[0])?("ON"):("OFF"));
        TextOut(hDC, 20, 80, str, strlen(str));
        wsprintf(str, "ボタン２ ： %s", (dijs.rgbButtons[1])?("ON"):("OFF"));
        TextOut(hDC, 20, 100, str, strlen(str));
        wsprintf(str, "ボタン３ ： %s", (dijs.rgbButtons[2])?("ON"):("OFF"));
        TextOut(hDC, 20, 120, str, strlen(str));
        wsprintf(str, "ボタン４ ： %s", (dijs.rgbButtons[3])?("ON"):("OFF"));
        TextOut(hDC, 20, 140, str, strlen(str));
    } else{
        // 入力制御の再開
        _pDevice->Acquire();
    }
}

