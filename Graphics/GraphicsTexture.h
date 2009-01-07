#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "GraphicsSystem.h"

namespace base{
	class GraphicsTexture {
		LPDIRECT3DTEXTURE9 _pTexture;
		LPDIRECT3DDEVICE9 _pDevice;
	public:
		GraphicsTexture( GraphicsSystem* system );
		~GraphicsTexture();
		LPDIRECT3DTEXTURE9 GetTexture();
		BOOL Load( BYTE* data, int size );
		BOOL Open( char* file_name );
		BOOL UnLoad();
		BOOL Create( int width, int height );
	};
}
