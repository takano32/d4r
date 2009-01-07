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
		変数の宣言
	----------------------------*/
	static char psAppName[] = "WndApp";
	MSG			msg;					// メッセージ
	WNDCLASS	wc;						// ウインドウクラス
	HWND		hwnd;					// ウインドウハンドル

	//	ウィンドウクラスの設定
	wc.lpszClassName = psAppName;				// ウインドウクラスネーム（詳細不明）
	wc.lpfnWndProc   = WndProc;					// ウインドウプロシージャ名（WndProcにする）
	wc.style         = NULL;		
	wc.cbClsExtra    = 0;						// 未使用。0 にする。
	wc.cbWndExtra    = 0;						// 未使用。0 にする。
	wc.lpszMenuName  = "MidiMenu";				// 未使用。NULL でもＯＫ
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);			// <-背景色
	wc.hInstance     = hInstance;									// インスタンスハンドル
	wc.hIcon         = LoadIcon( 0, IDI_APPLICATION );				// アイコン
	wc.hCursor       = LoadCursor( 0, IDC_ARROW );					// カーソルタイプ

	// ウインドウクラスを登録
	RegisterClass( &wc );

	// ウィンドウの作成
    hwnd = CreateWindowEx(
		0,
        psAppName,							// ウィンドウを作るウインドウクラスの名前を入れる
		"MidiSample",						// アプリケーションの名前
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        GetSystemMetrics( SM_CXSCREEN ) / 2 - 320,
        GetSystemMetrics( SM_CYSCREEN ) / 2 - 240,
		640,
		480,
        NULL,								// 親ウインドウ無し
        NULL,								// クラスネーム指定時、NULL
        hInstance,							// ウインドウインスタンスハンドル			
        NULL 								// NULLでＯＫ
	);

	// サウンド機能テスト

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

	


	// ここまで


	// ウィンドウを表示
	ShowWindow ( hwnd, nCmdShow );			// ウインドウを表示
	UpdateWindow( hwnd );					// ウインドウの更新

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
	




	// メッセージループ	ゲーム終了まで永久ループ
  	while( TRUE ){	
		// メッセージの有りの場合
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ){	
			// プログラム終了要求
			if( msg.message == WM_QUIT )	break;
			// メッセージの解析
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return	0;
}



LRESULT CALLBACK WndProc ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	// Windows からのメッセージにより分岐
	switch (message){
	case WM_CREATE:
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



