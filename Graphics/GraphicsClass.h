#pragma once

#define MAX_TEXTURE 128
#define MAX_SPRITE 4096

#include <windows.h>
#include "GraphicsTexture.h"
#include "GraphicsSprite.h"
#include "GraphicsSystem.h"
#include "GraphicsFont.h"
#include "GraphicsFader.h"

namespace base{
	class GraphicsClass {
		GraphicsSystem* _pSystem;
		GraphicsFont* _pFont;
		GraphicsTexture* _pTextures[MAX_TEXTURE];
		GraphicsSprite* _pSprites[MAX_SPRITE];
		GraphicsFader* _pFader;
	public:
		GraphicsClass( HWND hWnd );
		~GraphicsClass();
		BOOL Begin( int red, int green, int blue );
		BOOL End();
		BOOL CreateTexture( int texture_id, int width, int height );
		BOOL RegistTexture( int texture_id, BYTE* data, int size );
		BOOL UnRegistTexture( int texture_id );
		BOOL RegistSprite( int sprite_id, int texture_id, 
			int x, int y, int width, int heigth );
		BOOL UnRegistSprite( int sprite_id );
		BOOL DrawSprite( int sprite_id, int x, int y, int z );
		BOOL DrawText( char* string, int x, int y );
		BOOL SetFontColor( int red, int green, int blue, int alpha );
		int GetSpriteWidth( int sprite_id );
		int GetSpriteHeight( int sprite_id );
		BOOL SetFade( int red, int green, int blue, int alpha );
		// texture( texture_id, resource.get("hoge.bmp"))
		// sprite( sprite_id, texture_id, x, y, width, height)
		// draw( sprite_id, x, y, z ) 0<= z <= 999999
	};
}
