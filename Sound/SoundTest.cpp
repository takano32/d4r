#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "ddraw.lib" )
#pragma comment( lib, "dinput.lib" )
#pragma comment( lib, "dsound.lib" )
#pragma comment( lib, "d3drm.lib" )
#pragma comment( lib, "wsock32.lib" )

#include <windows.h>		// Windows
#include <winnls32.h>		// IME
#include <mmsystem.h>		//
#include <stdlib.h>			//
#include <stdio.h>			//
#include <conio.h>			// 
#include <string.h>			//
#include <memory.h>			//
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "../Log.h"
#include "SoundSystem.h"
#include "SoundBuffer.h"
#include "SoundClass.h"
#include "../File.h"

using namespace base;

SoundSystem* pSoundSystem_;

SoundClass* pSoundClass;
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

	// �T�E���h�@�\�e�X�g

	/*
	SoundBuffer* pSoundBufferSample1 = new SoundBuffer( pSoundSystem_ );
	pSoundBufferSample1->Open( "smoke_l.mp3" );
	pSoundBufferSample1->Play( TRUE );

	SoundBuffer* pSoundBufferSample2 = new SoundBuffer( pSoundSystem_ );
	pSoundBufferSample2->Open( "sample03.MP3" );
	pSoundBufferSample2->Play(TRUE);
	*/

	pSoundClass = new SoundClass( hwnd );
	pSoundClass->Open( 1, "xeno.wav" );
	pSoundClass->Open( 2, "smoke_l.mp3" );

	


	// �����܂�


	// �E�B���h�E��\��
	ShowWindow ( hwnd, nCmdShow );			// �E�C���h�E��\��
	UpdateWindow( hwnd );					// �E�C���h�E�̍X�V

	pSoundClass->PlayWithFade( 1, 4000, TRUE );
	pSoundClass->PlayWithFade( 2, 4000 ,TRUE );
	pSoundClass->BackScene( 4000, TRUE );
	pSoundClass->BackScene( 2000, TRUE );
	pSoundClass->BackScene( 2000, TRUE );


	//pSoundClass->Play( 1, TRUE );
	
	//Sleep(5000);
	//pSoundClass->PlayWithFade( 2, TRUE, 2500 );

	/*
	
	Sleep(7000);
	for(int i = 25; 0 <= i; i--) {
		int vol = pSoundBufferSample1->GetVolume();
		pSoundBufferSample1->SetVolume(i * 4);
		Sleep(100);
	}
	*/

	//pSoundBufferSample1->FadeOut( 8000 );
	//pSoundBufferSample1->FadeIn( TRUE, 8000 );
	//Sleep(1000);
	




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



