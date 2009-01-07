#include <windows.h>		// Windows

#include "WindowClass.h"
using namespace base;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WindowClass* window = new WindowClass();
	window->SetWindowProcedure( WndProc );
	window->Create();
	window->Show();
	window->Main();
	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
	//WindowClass* window = (WindowClass*)GetUserData( hwnd );
	// Windows からのメッセージにより分岐
	// Listenerを導入する予定．
	switch (message){
	case WM_CREATE:
		MessageBox(NULL, "hoge", "fuga", MB_OK );
		break;
	case WM_KEYDOWN:				// キー入力
        switch( wParam ){			// キーに応じて処理 
		case VK_ESCAPE:				// 終了ボタン
		case VK_F12:
			PostMessage( hwnd, WM_CLOSE, 0, 0 );	// Windowsへ プログラムおわり
			break;
		}
		break;	
		case WM_DESTROY:		// プログラム終了
			PostQuitMessage(0);
			return 0;
			break;
		default:
			InvalidateRect( hwnd, NULL, FALSE );
			return DefWindowProc( hwnd, message, wParam, lParam );
			break;
	}
	
	// デフォルト
	return DefWindowProc( hwnd, message, wParam, lParam);
}


