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
	// �p�����[�^��ݒ�
	D3DPRESENT_PARAMETERS param;
	ZeroMemory( &param, sizeof(param) );
	param.BackBufferWidth  = 640;                    // �𑜓x�E��
	param.BackBufferHeight = 480;                    // �𑜓x�E����
	//param.BackBufferFormat = D3DFMT_X8R8G8B8;        // �s�N�Z���t�H�[�}�b�g
	param.BackBufferFormat = D3DFMT_UNKNOWN;        // �s�N�Z���t�H�[�}�b�g
	param.BackBufferCount  = 1;                      // �o�b�N�o�b�t�@�̐�
	param.SwapEffect       = D3DSWAPEFFECT_DISCARD;  // �\��������@
	//param.Windowed         = FALSE;                  // �t���X�N���[��
	param.Windowed       = TRUE;                   // �E�C���h�E���[�h

	// Z�o�b�t�@�L��
	param.EnableAutoDepthStencil = TRUE;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	
	// �f�o�C�X�쐬
	HRESULT hr = _pGraphicsObject->CreateDevice(
		D3DADAPTER_DEFAULT,                  // �v���C�}���f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,                      // �n�[�h�E�F�A�𗘗p
		_hWnd,                              // �E�C���h�E�n���h��
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // �\�t�g�E�F�A�ɂ�钸�_����
		// D3DCREATE_HARDWARE_VERTEXPROCESSING, // �n�[�h�E�F�A�ɂ�钸�_����
		&param,                              // �p�����[�^
		&_pDevice
		);

    if( FAILED( hr ) ){
		Log::Error("GraphicsSystem#InitializeDevice: Device�̍쐬�Ɏ��s�D");
	}

	hr = D3DXCreateSprite( _pDevice, &_pSprite );

    if( FAILED( hr ) ){
		Log::Error("GraphicsSystem#InitializeDevice: Sprite�̍쐬�Ɏ��s�D");
	}

    // �A���t�@�E�u�����f�B���O���s��
    _pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    // ���ߏ������s��
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
