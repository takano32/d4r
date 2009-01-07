#include "GraphicsFader.h"

#include "../Util/Log.h"
#include <tchar.h>

using namespace base;

GraphicsFader::GraphicsFader( GraphicsSystem *pSystem ) {
	_pSystem = pSystem;
	_color = 0xffffffff;
}

GraphicsFader::~GraphicsFader() {
}

BOOL GraphicsFader::SetFade(int red, int green, int blue, int alpha)
{
	_color = MAKELONG(MAKEWORD(blue, green), MAKEWORD(red, alpha));
/*	LPDIRECT3DDEVICE9 device = _pSystem->GetDevice();
	static float p1 = 0.0f;
	static float p2 = 0.0001f;
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);                // フォグ ブレンディングを有効にする。
	device->SetRenderState(D3DRS_FOGCOLOR, _color);           // フォグの色を設定する。
	device->SetRenderState(D3DRS_FOGSTART, *((DWORD *)(&p1)));
	device->SetRenderState(D3DRS_FOGEND, *((DWORD *)(&p2)));*/
	return TRUE;
}

DWORD GraphicsFader::GetFade()
{
	return _color;
}
