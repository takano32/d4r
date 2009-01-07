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

	// ウィンドウを表示
	ShowWindow ( hwnd, nCmdShow );			// ウインドウを表示
	UpdateWindow( hwnd );					// ウインドウの更新

	pGraphicsSystem = new GraphicsSystem( hwnd );


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



