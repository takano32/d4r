#include "GraphicsTexture.h"
using namespace base;

GraphicsTexture::GraphicsTexture( GraphicsSystem* system ){
	_pTexture = NULL;
	_pDevice = system->GetDevice();
}

GraphicsTexture::~GraphicsTexture() {
	UnLoad();
}

LPDIRECT3DTEXTURE9 GraphicsTexture::GetTexture() {
	return _pTexture;
}

BOOL GraphicsTexture::Load( BYTE* data, int size ) {
	HRESULT hr = D3DXCreateTextureFromFileInMemory( _pDevice, data, size, &_pTexture );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsTexture::Open( char* file_name ) {
	UnLoad();
	HRESULT hr = D3DXCreateTextureFromFile( _pDevice, file_name, &_pTexture );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsTexture::UnLoad() {
	BOOL ret = _pTexture?TRUE:FALSE;
	RELEASE( _pTexture );
	return ret;
}

BOOL GraphicsTexture::Create(int width, int height)
{
	HRESULT hr = _pDevice->CreateTexture(width, height, 0, D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &_pTexture, NULL);
	return SUCCEEDED(hr);
}
