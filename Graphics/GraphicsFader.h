#pragma once

#include "GraphicsSystem.h"

namespace base{
	class GraphicsFader{
		GraphicsSystem *_pSystem;
		DWORD _color;
	public:
		GraphicsFader( GraphicsSystem *pSystem );
		~GraphicsFader();
		BOOL SetFade( int red, int green, int blue, int alpha );
		DWORD GetFade();
	};
}