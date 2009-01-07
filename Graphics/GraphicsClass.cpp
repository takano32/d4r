#include "GraphicsClass.h"
#include "../Util/Log.h"
#include <ruby.h>

using namespace base;

GraphicsClass::GraphicsClass( HWND hWnd ) {
	_pSystem = new GraphicsSystem( hWnd );
	_pFont = new GraphicsFont( _pSystem );
	_pFader = new GraphicsFader( _pSystem );
	memset( _pTextures, 0x00, sizeof( GraphicsTexture* ) * MAX_TEXTURE );
	memset( _pSprites, 0x00, sizeof( GraphicsSprite* ) * MAX_SPRITE );
}

GraphicsClass::~GraphicsClass() {
  delete _pFont;
  delete _pSystem;
  delete _pFader;
}

BOOL GraphicsClass::Begin( int red, int green, int blue ) {
	_pSystem->Clear( D3DCOLOR_XRGB(red,green,blue) );
	return _pSystem->BeginScene();
}

BOOL GraphicsClass::End() {
	_pSystem->EndScene();
	return _pSystem->Present();
}

BOOL GraphicsClass::CreateTexture(int texture_id, int width, int height ) {
	UnRegistTexture( texture_id );
	_pTextures[texture_id] = new GraphicsTexture( _pSystem );
	return _pTextures[texture_id]->Create( width, height );
}

BOOL GraphicsClass::RegistTexture( int texture_id, BYTE *data, int size ) {
	UnRegistTexture( texture_id );
	_pTextures[texture_id] = new GraphicsTexture( _pSystem );
	return _pTextures[texture_id]->Load( data, size );
}

BOOL GraphicsClass::UnRegistTexture( int texture_id ) {
	if( _pTextures[texture_id] ) {
		delete _pTextures[texture_id];
		_pTextures[texture_id] = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL GraphicsClass::RegistSprite( int sprite_id, int texture_id, 
								 int x, int y, int width, int height ) {
	_pSprites[sprite_id] = new GraphicsSprite( _pSystem, _pFader );
	_pSprites[sprite_id]->Load( _pTextures[texture_id], x, y, width, height );
	return TRUE;
}

BOOL GraphicsClass::UnRegistSprite( int sprite_id ) {
	if( _pSprites[sprite_id] ) {
		_pSprites[sprite_id]->UnLoad();
		delete _pSprites[sprite_id];
		_pSprites[sprite_id] = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL GraphicsClass::DrawSprite( int sprite_id, int x, int y, int z ) {
	GraphicsSprite* sprite = _pSprites[sprite_id];
	if( !sprite ) {
		Log::Error("GraphicsClass#DrawSprite: 不正なスプライトIDが指定されました．");
		return FALSE;
	}
	//sprite->Begin();
	BOOL ret = sprite->Draw(x, y, z);
	//sprite->End();
	return ret;
}

BOOL GraphicsClass::DrawText(char* string, int x, int y)
{
  return _pFont->Draw( string, x, y );
}

BOOL GraphicsClass::SetFontColor(int red, int green, int blue, int alpha)
{
  return _pFont->SetFontColor( red, green, blue, alpha );
}

int GraphicsClass::GetSpriteWidth( int sprite_id ) {
	GraphicsSprite* sprite = _pSprites[sprite_id];
	if( !sprite ) {
		Log::Error("GraphicsClass#DrawSprite: 不正なスプライトIDが指定されました．");
		return FALSE;
	}
	return sprite->GetWidth();
}

int GraphicsClass::GetSpriteHeight( int sprite_id ) {
	GraphicsSprite* sprite = _pSprites[sprite_id];
	if( !sprite ) {
		Log::Error("GraphicsClass#DrawSprite: 不正なスプライトIDが指定されました．");
		return FALSE;
	}
	return sprite->GetHeight();
}

BOOL GraphicsClass::SetFade(int red, int green, int blue, int alpha)
{
	_pFader->SetFade(red, green, blue, alpha);
	return TRUE;
}
