#include "WindowClass.h"

using namespace base;
char* WindowClass::_psClassName = "ClassName";
char* WindowClass::_psWindowName = "WindowName";
WNDPROC WindowClass::_pWndProc = NULL;

WindowClass::WindowClass() {
	_hInstance = GetModuleHandle( NULL );
	_loopAction = NULL;
	//TODO: �o�O����ł́H
	SetUserData( (LONG) this );
}

WindowClass::~WindowClass() {
}

HWND WindowClass::GetWindowHandle() {
	return _hWnd;
}

LONG WindowClass::GetUserData( HWND hWnd ) {
	return ::GetWindowLong(hWnd, GWL_USERDATA);
}

LONG WindowClass::GetUserData() {
	return ::GetWindowLong(_hWnd, GWL_USERDATA);
}

BOOL WindowClass::SetUserData( LONG data ) {
	if( ::SetWindowLong( _hWnd, GWL_USERDATA, data ) ){
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL WindowClass::SetWindowProcedure( WNDPROC WndProc ) {
	_pWndProc = WndProc;
	return TRUE;
}

BOOL WindowClass::RegisterClass(){
	_wndClass.lpszClassName = _psClassName;				// �E�C���h�E�N���X�l�[���i�ڍוs���j
	_wndClass.lpfnWndProc   = _pWndProc;					// �E�C���h�E�v���V�[�W�����iWndProc�ɂ���j
	_wndClass.style         = NULL;		
	_wndClass.cbClsExtra    = 0;						// ���g�p�B0 �ɂ���B
	_wndClass.cbWndExtra    = 0;						// ���g�p�B0 �ɂ���B
	_wndClass.lpszMenuName  = NULL;				// ���g�p�BNULL �ł��n�j
	_wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);			// <-�w�i�F
	_wndClass.hInstance     = _hInstance;									// �C���X�^���X�n���h��
	_wndClass.hIcon         = LoadIcon( 0, IDI_APPLICATION );				// �A�C�R��
	_wndClass.hCursor       = LoadCursor( 0, IDC_ARROW );					// �J�[�\���^�C�v
	// �E�C���h�E�N���X��o�^
	return ::RegisterClass( &_wndClass )?TRUE:FALSE;
}

BOOL WindowClass::Create() {
	if( !RegisterClass() ){
		return FALSE;
	};

	RECT rect={0, 0, 640, 480};
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect( &rect, style, FALSE );
	// �E�B���h�E�̍쐬
    _hWnd = CreateWindowEx(
		0,
        _psClassName,							// �E�B���h�E�����E�C���h�E�N���X�̖��O������
		_psWindowName,						// �A�v���P�[�V�����̖��O
        style,
        GetSystemMetrics( SM_CXSCREEN ) / 2 - (rect.right-rect.left)/2,
        GetSystemMetrics( SM_CYSCREEN ) / 2 - (rect.bottom-rect.top)/2,
		(rect.right-rect.left),
		(rect.bottom-rect.top),
        NULL,								// �e�E�C���h�E����
        NULL,								// �N���X�l�[���w�莞�ANULL
        _hInstance,							// �E�C���h�E�C���X�^���X�n���h��			
        NULL 								// NULL�łn�j
		);
	return _hWnd?TRUE:FALSE;
}


BOOL WindowClass::Show() {
	//ShowWindow ( _hWnd, nCmdShow );			// �E�C���h�E��\��
	ShowWindow ( _hWnd, SW_SHOWDEFAULT );			// �E�C���h�E��\��
	return UpdateWindow( _hWnd )?TRUE:FALSE;					// �E�C���h�E�̍X�V
}


VALUE WindowClass::CallBackAction() {
	if( _loopAction && GetActiveWindow() == _hWnd ) {
		return rb_funcall( _loopAction, rb_intern("action"), 0);
	}else {
		return Qnil;
	}
}

VALUE WindowClass::SetLoopAction( VALUE action ) {
	return _loopAction = action;
}

BOOL WindowClass::Main() {
	// ���b�Z�[�W���[�v	�Q�[���I���܂ŉi�v���[�v
	while( TRUE ){	
		// ���b�Z�[�W�̗L��̏ꍇ
		if( PeekMessage( &_msg, 0, 0, 0, PM_REMOVE ) ){	
			// �v���O�����I���v��
			if( _msg.message == WM_QUIT )	break;
			// ���b�Z�[�W�̉��
			TranslateMessage( &_msg );
			DispatchMessage( &_msg );
		}
		else{
			CallBackAction();
		}
	}
	return TRUE;
}





