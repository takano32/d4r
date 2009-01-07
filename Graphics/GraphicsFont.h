#pragma once

#include "GraphicsSystem.h"

namespace base{
	class GraphicsFont{
		GraphicsSystem *_pSystem;
		LPD3DXFONT _pFont;
    D3DCOLOR _color;

	private:
		BOOL InitializeDevice();
	public:
		GraphicsFont( GraphicsSystem *pSystem );
		~GraphicsFont();
		LPD3DXFONT GetFont();
    BOOL Draw(char* string, int x, int y);
    BOOL SetFontColor(int red, int green, int blue, int alpha);
	};
}