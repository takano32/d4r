#include "GraphicsFont.h"
#include "../Util/Log.h"
#include <tchar.h>

using namespace base;

GraphicsFont::GraphicsFont( GraphicsSystem *pSystem ) {
	_pSystem = pSystem;
	_pFont = NULL;
	InitializeDevice();
}

GraphicsFont::~GraphicsFont() {
	RELEASE( _pFont );
}

BOOL GraphicsFont::InitializeDevice() {
	D3DXFONT_DESC font;
    ZeroMemory(&font, sizeof(font));
	font.Height = 24;
	font.Width = 0;
	font.Weight = FW_NORMAL;
	font.Italic = FALSE;
	font.CharSet = SHIFTJIS_CHARSET;
	font.OutputPrecision = OUT_DEFAULT_PRECIS;
	font.Quality = DEFAULT_QUALITY;
	font.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(font.FaceName, _T("‚l‚r ƒSƒVƒbƒN"));

    if( FAILED( D3DXCreateFontIndirect(_pSystem->GetDevice(), &font, &_pFont) ) ){
		Log::Error("GraphicsSystem#InitializeDevice: ƒtƒHƒ“ƒg‚Ìì¬‚ÉŽ¸”sD");
	}

	return TRUE;
}

LPD3DXFONT GraphicsFont::GetFont() {
	return _pFont;
}

BOOL GraphicsFont::Draw(char* string, int x, int y)
{
	RECT rect = {x, y, x, y};
  HRESULT hr;
	hr = _pFont->DrawText(
		_pSystem->GetSprite(), string, -1, &rect, DT_NOCLIP, _color );
  return SUCCEEDED(hr);
}

BOOL GraphicsFont::SetFontColor(int red, int green, int blue, int alpha)
{
  _color = MAKELONG(MAKEWORD(blue, green), MAKEWORD(red, alpha));
  return TRUE;
}
