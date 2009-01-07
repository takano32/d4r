#include <windows.h>
#include "InputSystem.h"
#include "JoystickDevice.h"
#include "KeyboardDevice.h"

using namespace base;

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // ウインドウプロシージャ


// グローバル変数
BOOL end = FALSE;                                           // 終了判定


//==============================================================================================
// メイン処理
//==============================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウインドウクラス構造体
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.style         = CS_HREDRAW | CS_VREDRAW;             // ウインドウスタイル
    wc.lpfnWndProc   = (WNDPROC)WndProc;                    // ウインドウプロシージャアドレス
    wc.cbClsExtra    = 0;                                   // 補助領域サイズ
    wc.cbWndExtra    = 0;                                   // 補助領域サイズ
    wc.hInstance     = hInstance;                           // インスタンスハンドル
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);     // アイコン
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // マウスカーソル
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);          // クライアント領域背景色
    wc.lpszMenuName  = NULL;                                // ウインドウメニュー
    wc.lpszClassName = "Windows";                           // ウインドウクラス名

    // ウインドウクラスの登録
    if(!RegisterClass(&wc)) return FALSE;

    // ウインドウの作成
    HWND hWnd = CreateWindow(
        wc.lpszClassName,                                   // ウインドウクラス名
        "タイトル",                                         // ウインドウタイトル
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,        // ウインドウスタイル
        CW_USEDEFAULT,                                      // 表示Ｘ座標
        CW_USEDEFAULT,                                      // 表示Ｙ座標
        640,                                              // 幅
        480,                                             // 高さ
        NULL,                                               // 親ウインドウ
        NULL,                                               // ウインドウメニュー
        hInstance,                                          // インスタンスハンドル
        NULL);                                              // WM_CREATE情報

    // DirectInput の初期化
    //if(!InitDInput(hWnd, hInstance)) return FALSE;
	InputSystem* pSystem= new InputSystem( hWnd );
	//KeyboardDevice* pDevice = new KeyboardDevice( pSystem );
	JoystickDevice* pDevice = new JoystickDevice( pSystem );

    // ウインドウの表示
    ShowWindow(hWnd, nCmdShow);                             // 表示状態の設定
    UpdateWindow(hWnd);                                     // クライアント領域の更新

    // デバイスコンテキストの取得
    HDC hDC = GetDC(hWnd);

    // メッセージループ
    MSG msg;
    while(!end)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);                         // 仮想キーメッセージの変換
            DispatchMessage(&msg);                          // ウインドウプロシージャへ転送
        } else{
            PatBlt(hDC, 0, 0, 640, 480, WHITENESS);         // 画面のクリア
            pDevice->DumpStatus(hDC);                             // ジョイスティック入力処理
            Sleep(60);                                      // 待機
        }
    }

    // デバイスコンテキストの解放
    ReleaseDC(hWnd, hDC);

    return msg.wParam;
}

//==============================================================================================
// ウインドウプロシージャ
//==============================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        // ウインドウが破棄されたときの処理
        end = TRUE;
        PostQuitMessage(0);
        return 0;

    default:
        // デフォルト処理
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
 