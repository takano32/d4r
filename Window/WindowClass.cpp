#include "WindowClass.h"

using namespace base;
char* WindowClass::_psClassName = "ClassName";
char* WindowClass::_psWindowName = "WindowName";
WNDPROC WindowClass::_pWndProc = NULL;

WindowClass::WindowClass() {
	_hInstance = GetModuleHandle( NULL );
	_loopAction = NULL;
	//TODO: バグありでは？
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
	_wndClass.lpszClassName = _psClassName;				// ウインドウクラスネーム（詳細不明）
	_wndClass.lpfnWndProc   = _pWndProc;					// ウインドウプロシージャ名（WndProcにする）
	_wndClass.style         = NULL;		
	_wndClass.cbClsExtra    = 0;						// 未使用。0 にする。
	_wndClass.cbWndExtra    = 0;						// 未使用。0 にする。
	_wndClass.lpszMenuName  = NULL;				// 未使用。NULL でもＯＫ
	_wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);			// <-背景色
	_wndClass.hInstance     = _hInstance;									// インスタンスハンドル
	_wndClass.hIcon         = LoadIcon( 0, IDI_APPLICATION );				// アイコン
	_wndClass.hCursor       = LoadCursor( 0, IDC_ARROW );					// カーソルタイプ
	// ウインドウクラスを登録
	return ::RegisterClass( &_wndClass )?TRUE:FALSE;
}

BOOL WindowClass::Create() {
	if( !RegisterClass() ){
		return FALSE;
	};

	RECT rect={0, 0, 640, 480};
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect( &rect, style, FALSE );
	// ウィンドウの作成
    _hWnd = CreateWindowEx(
		0,
        _psClassName,							// ウィンドウを作るウインドウクラスの名前を入れる
		_psWindowName,						// アプリケーションの名前
        style,
        GetSystemMetrics( SM_CXSCREEN ) / 2 - (rect.right-rect.left)/2,
        GetSystemMetrics( SM_CYSCREEN ) / 2 - (rect.bottom-rect.top)/2,
		(rect.right-rect.left),
		(rect.bottom-rect.top),
        NULL,								// 親ウインドウ無し
        NULL,								// クラスネーム指定時、NULL
        _hInstance,							// ウインドウインスタンスハンドル			
        NULL 								// NULLでＯＫ
		);
	return _hWnd?TRUE:FALSE;
}


BOOL WindowClass::Show() {
	//ShowWindow ( _hWnd, nCmdShow );			// ウインドウを表示
	ShowWindow ( _hWnd, SW_SHOWDEFAULT );			// ウインドウを表示
	return UpdateWindow( _hWnd )?TRUE:FALSE;					// ウインドウの更新
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
	// メッセージループ	ゲーム終了まで永久ループ
	while( TRUE ){	
		// メッセージの有りの場合
		if( PeekMessage( &_msg, 0, 0, 0, PM_REMOVE ) ){	
			// プログラム終了要求
			if( _msg.message == WM_QUIT )	break;
			// メッセージの解析
			TranslateMessage( &_msg );
			DispatchMessage( &_msg );
		}
		else{
			CallBackAction();
		}
	}
	return TRUE;
}





