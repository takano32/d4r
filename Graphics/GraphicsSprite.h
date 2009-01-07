#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "GraphicsSystem.h"
#include "GraphicsTexture.h"
#include "GraphicsFader.h"

namespace base{
	class GraphicsSprite{
		LPDIRECT3DDEVICE9 _pDevice;
		LPD3DXSPRITE _pSprite;
		LPDIRECT3DTEXTURE9 _pTexture;
		GraphicsFader* _pFader;

		// テクスチャでの位置
		int _x;
		int _y;
		int _width;
		int _height;
	public:
		GraphicsSprite( GraphicsSystem* system, GraphicsFader* fader );
		~GraphicsSprite();
		BOOL Begin();
		BOOL Draw( int x, int y, int z );
		BOOL End();
		BOOL Load( GraphicsTexture* texture , int x, int y, int width, int height );
		BOOL UnLoad();
		int GetWidth(){return _width;}
		int GetHeight(){return _height;}
	};
}