#include "GraphicsTest.h"
#include "GraphicsSystem.h"
#include "GraphicsTexture.h"
#include "GraphicsSprite.h"

using namespace base;

GraphicsSystem* pGraphicsSystem;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	/*--------------------------
		�ϐ��̐錾
	----------------------------*/
	static char psAppName[] = "WndApp";
	MSG			msg;					// ���b�Z�[�W
	WNDCLASS	wc;						// �E�C���h�E�N���X
	HWND		hwnd;					// �E�C���h�E�n���h��

	//	�E�B���h�E�N���X�̐ݒ�
	wc.lpszClassName = psAppName;				// �E�C���h�E�N���X�l�[���i�ڍוs���j
	wc.lpfnWndProc   = WndProc;					// �E�C���h�E�v���V�[�W�����iWndProc�ɂ���j
	wc.style         = NULL;		
	wc.cbClsExtra    = 0;						// ���g�p�B0 �ɂ���B
	wc.cbWndExtra    = 0;						// ���g�p�B0 �ɂ���B
	wc.lpszMenuName  = "MidiMenu";				// ���g�p�BNULL �ł��n�j
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);			// <-�w�i�F
	wc.hInstance     = hInstance;									// �C���X�^���X�n���h��
	wc.hIcon         = LoadIcon( 0, IDI_APPLICATION );				// �A�C�R��
	wc.hCursor       = LoadCursor( 0, IDC_ARROW );					// �J�[�\���^�C�v

	// �E�C���h�E�N���X��o�^
	RegisterClass( &wc );

	// �E�B���h�E�̍쐬
    hwnd = CreateWindowEx(
		0,
        psAppName,							// �E�B���h�E�����E�C���h�E�N���X�̖��O������
		"MidiSample",						// �A�v���P�[�V�����̖��O
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        GetSystemMetrics( SM_CXSCREEN ) / 2 - 320,
        GetSystemMetrics( SM_CYSCREEN ) / 2 - 240,
		640,
		480,
        NULL,								// �e�E�C���h�E����
        NULL,								// �N���X�l�[���w�莞�ANULL
        hInstance,							// �E�C���h�E�C���X�^���X�n���h��			
        NULL 								// NULL�łn�j
	);

	// �E�B���h�E��\��
	ShowWindow ( hwnd, nCmdShow );			// �E�C���h�E��\��
	UpdateWindow( hwnd );					// �E�C���h�E�̍X�V

	pGraphicsSystem = new GraphicsSystem( hwnd );


	// ���b�Z�[�W���[�v	�Q�[���I���܂ŉi�v���[�v
  	while( TRUE ){	
		// ���b�Z�[�W�̗L��̏ꍇ
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ){	
			// �v���O�����I���v��
			if( msg.message == WM_QUIT )	break;
			// ���b�Z�[�W�̉��
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return	0;
}



LRESULT CALLBACK WndProc ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	// Windows ����̃��b�Z�[�W�ɂ�蕪��
	switch (message){
	case WM_CREATE:
		break;
	case WM_KEYDOWN:				// �L�[����
        switch( wParam ){			// �L�[�ɉ����ď��� 
		case VK_ESCAPE:				// �I���{�^��
		case VK_F12:
			PostMessage( hwnd, WM_CLOSE, 0, 0 );	// Windows�� �v���O���������
			break;
		case 'D':
			{
				GraphicsTexture* pTexture = new GraphicsTexture( pGraphicsSystem );
				pTexture->Open( "test.jpg" );
				GraphicsSprite* pSprite = new GraphicsSprite( pGraphicsSystem );
				pSprite->Load( pTexture, 0, 0, 256, 256 );
				pGraphicsSystem->Clear( D3DCOLOR_XRGB(0,0,0) );
				pGraphicsSystem->BeginScene();
				pSprite->Begin();
				pSprite->Draw( 20, 20, 0 );
				pSprite->Draw( 30, 30, 0 );
				pSprite->Draw( 40, 40, 0 );
				pSprite->End();
				pGraphicsSystem->EndScene();
				pGraphicsSystem->Present();
				delete pSprite;
				delete pTexture;
				//MessageBox( hwnd, "hoge", "fuga", 0 );
			}
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



