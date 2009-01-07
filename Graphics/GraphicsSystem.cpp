#include "GraphicsSystem.h"
#include "../Util/Log.h"
#include <tchar.h>

using namespace base;

GraphicsSystem::GraphicsSystem( HWND hWnd ) {
	_hWnd = hWnd;
	_pGraphicsObject = Direct3DCreate9( D3D_SDK_VERSION );
	_pDevice = NULL;
	InitializeDevice();
}

GraphicsSystem::~GraphicsSystem() {
	RELEASE( _pDevice );
	RELEASE( _pGraphicsObject );
	RELEASE( _pSprite );
}

BOOL GraphicsSystem::InitializeDevice() {
	// パラメータを設定
	D3DPRESENT_PARAMETERS param;
	ZeroMemory( &param, sizeof(param) );
	param.BackBufferWidth  = 640;                    // 解像度・幅
	param.BackBufferHeight = 480;                    // 解像度・高さ
	//param.BackBufferFormat = D3DFMT_X8R8G8B8;        // ピクセルフォーマット
	param.BackBufferFormat = D3DFMT_UNKNOWN;        // ピクセルフォーマット
	param.BackBufferCount  = 1;                      // バックバッファの数
	param.SwapEffect       = D3DSWAPEFFECT_DISCARD;  // 表示する方法
	//param.Windowed         = FALSE;                  // フルスクリーン
	param.Windowed       = TRUE;                   // ウインドウモード

	// Zバッファ有効
	param.EnableAutoDepthStencil = TRUE;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	
	// デバイス作成
	HRESULT hr = _pGraphicsObject->CreateDevice(
		D3DADAPTER_DEFAULT,                  // プライマリディスプレイアダプタ
		D3DDEVTYPE_HAL,                      // ハードウェアを利用
		_hWnd,                              // ウインドウハンドル
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // ソフトウェアによる頂点処理
		// D3DCREATE_HARDWARE_VERTEXPROCESSING, // ハードウェアによる頂点処理
		&param,                              // パラメータ
		&_pDevice
		);

    if( FAILED( hr ) ){
		Log::Error("GraphicsSystem#InitializeDevice: Deviceの作成に失敗．");
	}

	hr = D3DXCreateSprite( _pDevice, &_pSprite );

    if( FAILED( hr ) ){
		Log::Error("GraphicsSystem#InitializeDevice: Spriteの作成に失敗．");
	}

    // アルファ・ブレンディングを行う
    _pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    // 透過処理を行う
    _pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	return hr==D3D_OK?TRUE:FALSE;
}

LPDIRECT3DDEVICE9 GraphicsSystem::GetDevice() {
	return _pDevice;
}

LPD3DXSPRITE GraphicsSystem::GetSprite() {
	return _pSprite;
}

BOOL GraphicsSystem::Clear( D3DCOLOR color ) {
	//D3DCOLOR_XRGB(0,0,0)
	HRESULT hr = _pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		color , 1.0, 0 );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSystem::BeginScene() {
	HRESULT hr = _pDevice->BeginScene();
	if( FAILED(hr) ) return FALSE;
	hr = _pSprite->Begin( D3DXSPRITE_ALPHABLEND );//D3DXSPRITE_SORT_DEPTH_FRONTTOBACK );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSystem::EndScene() {
	HRESULT hr = _pSprite->End();
	if( FAILED(hr) ) return FALSE;
	hr = _pDevice->EndScene();
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSystem::Present() {
	HRESULT hr = _pDevice->Present( NULL, NULL, NULL, NULL );
	return hr==D3D_OK?TRUE:FALSE;
}
