#include "GraphicsSprite.h"
#include "GraphicsSystem.h"
#include "GraphicsTexture.h"

using namespace base;

GraphicsSprite::GraphicsSprite( GraphicsSystem* system, GraphicsFader* fader ) {
	_pFader = fader;
	_pDevice = system->GetDevice();
	_pSprite = system->GetSprite();
	_pSprite->AddRef();
	_x = _y = _width = _height = 0;
}

GraphicsSprite::~GraphicsSprite() {
	UnLoad();
}


BOOL GraphicsSprite::Draw( int x, int y , int z ) {
	// •`‰æ”ÍˆÍ
	RECT		Rect = { _x, _y, _x + _width, _y + _height };
	// ‰ñ“]Žž‚Ì’†SÀ•W
	static D3DXVECTOR3 Center( 0 , 0 , 0 ); //À•W³‚µ‚¢
	// •`‰æˆÊ’u
	D3DXVECTOR3	Translation( (float)x, (float)y, (float)(z / 10000.0) );
	HRESULT hr = _pSprite->Draw( _pTexture, &Rect, &Center, &Translation, _pFader->GetFade() );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSprite::Begin() {
	HRESULT hr = _pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK );
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSprite::End() {
	HRESULT hr = _pSprite->End();
	return hr==D3D_OK?TRUE:FALSE;
}

BOOL GraphicsSprite::Load( GraphicsTexture* texture , int x, int y, int width, int height ) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	UnLoad();
	//D3DXCreateSprite( _pDevice, &_pSprite );
	_pTexture = texture->GetTexture();
	return _pTexture?TRUE:FALSE;
}

BOOL GraphicsSprite::UnLoad() {
	//BOOL ret = _pSprite?TRUE:FALSE;
	//RELEASE( _pSprite );
	BOOL ret = TRUE;
	return ret;
}

