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
    // �f�o�C�X�̗� (CreateDevice)
    hr = _pInputObject->EnumDevices( DIDEVTYPE_JOYSTICK, EnumDevicesCallBack, this, DIEDFL_ATTACHEDONLY );
    if(FAILED(hr) || _pInputObject == NULL) return FALSE;

	// �f�o�C�X������Ȃ������璆�f
	if(!_pDevice) return FALSE;

    // �f�[�^�t�H�[�}�b�g�̐ݒ�
    hr = _pDevice->SetDataFormat(&c_dfDIJoystick);
    if(FAILED(hr)) return FALSE;
	
    // �������[�h�̐ݒ�
    hr = _pDevice->SetCooperativeLevel( _hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
    if(FAILED(hr)) return FALSE;
	
    // ���̐ݒ�
    hr = _pDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
    if(FAILED(hr)) return FALSE;
	
    // �|�[�����O�̐ݒ�
    //lpDInputDEV->Poll();
	
    // ���͐���̊J�n
    _pDevice->Acquire();
	return TRUE;
}


BOOL JoystickDevice::EnumDevicesProcess( const DIDEVICEINSTANCE* pdidInstance ) {
	// �f�o�C�X�̐���
    HRESULT hr = _pInputObject->CreateDevice(pdidInstance->guidInstance, &_pDevice, NULL);
    if(FAILED(hr)) return DIENUM_CONTINUE;
	
    // �W���C�X�e�B�b�N�\�͂̎擾
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
	// ���̗�
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize       = sizeof(diprg);            // DIPROPDWORD �T�C�Y
    diprg.diph.dwHeaderSize = sizeof(diprg);            // DIPROPHEADER �T�C�Y
    diprg.diph.dwObj        = lpddoi->dwType;           // �I�u�W�F�N�g
    diprg.diph.dwHow        = DIPH_BYID;                // ���ߕ��@
    diprg.lMin              = -1000;                    // �͈͂̉���
    diprg.lMax              = +1000;                    // �͈͂̏��
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
		// ���͐���̍ĊJ
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
	// �W���C�X�e�B�b�N��Ԃ̎擾
    DIJOYSTATE dijs;
    HRESULT hr = _pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs);
    if(SUCCEEDED(hr))
    {
        // �W���C�X�e�B�b�N��Ԃ̕\��
        char str[256];
        wsprintf(str, "�w�� �F %d", (short)dijs.lX + 1);
        TextOut(hDC, 20, 20, str, strlen(str));
        wsprintf(str, "�x�� �F %d", (short)dijs.lY + 1);
        TextOut(hDC, 20, 40, str, strlen(str));
        wsprintf(str, "�{�^���P �F %s", (dijs.rgbButtons[0])?("ON"):("OFF"));
        TextOut(hDC, 20, 80, str, strlen(str));
        wsprintf(str, "�{�^���Q �F %s", (dijs.rgbButtons[1])?("ON"):("OFF"));
        TextOut(hDC, 20, 100, str, strlen(str));
        wsprintf(str, "�{�^���R �F %s", (dijs.rgbButtons[2])?("ON"):("OFF"));
        TextOut(hDC, 20, 120, str, strlen(str));
        wsprintf(str, "�{�^���S �F %s", (dijs.rgbButtons[3])?("ON"):("OFF"));
        TextOut(hDC, 20, 140, str, strlen(str));
    } else{
        // ���͐���̍ĊJ
        _pDevice->Acquire();
    }
}

