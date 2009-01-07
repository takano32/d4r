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
	// Windows ����̃��b�Z�[�W�ɂ�蕪��
	// Listener�𓱓�����\��D
	switch (message){
	case WM_CREATE:
		MessageBox(NULL, "hoge", "fuga", MB_OK );
		break;
	case WM_KEYDOWN:				// �L�[����
        switch( wParam ){			// �L�[�ɉ����ď��� 
		case VK_ESCAPE:				// �I���{�^��
		case VK_F12:
			PostMessage( hwnd, WM_CLOSE, 0, 0 );	// Windows�� �v���O���������
			break;
		}
		break;	
		case WM_DESTROY:		// �v���O�����I��
			PostQuitMessage(0);
			return 0;
			break;
		default:
			InvalidateRect( hwnd, NULL, FALSE );
			return DefWindowProc( hwnd, message, wParam, lParam );
			break;
	}
	
	// �f�t�H���g
	return DefWindowProc( hwnd, message, wParam, lParam);
}


