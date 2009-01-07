
#pragma once

#pragma comment(lib,"msacm32.lib")
#include <windows.h>
#include <mmreg.h>
#include <msacm.h>

namespace base{
	class SoundACM {
	public:
		LRESULT			Open(WAVEFORMATEX*pWFormat,LPVOID lpSrcBuf,DWORD dwSrcLength);
		LRESULT			Open(BYTE*p,DWORD size);			//	直接MP3のファイルをオープン！
		
		DWORD			GetSize(void) const;				//	これで変換後サイズを取得
		WAVEFORMATEX*	GetFormat(void);					//	これで変換後フォーマットを取得
		LRESULT			Convert(LPVOID dstBuf);				//	これで変換する
		LRESULT			Close(void);						//	Closeはデストラクタがやる
		
		bool			IsOpen(void) const;					//	Openしたんか？
		
		SoundACM(void);
		virtual ~SoundACM();
		
	protected:
		WAVEFORMATEX*	m_lpSrcWFormat;		//	変換前のフォーマット
		LPVOID			m_lpSrcBuf;			//	変換前のバッファ位置
		DWORD			m_dwSrcLength;		//	変換前のバッファサイズ
		
		WAVEFORMATEX	m_destWFormat;		//	変換後フォーマット
		HACMSTREAM		m_hAcm;				//	handle of acmStream
		ACMSTREAMHEADER	m_acmheader;		//	header of acmStream
		DWORD			m_dwDestLength;		//	変換後のバッファサイズ
		
		MPEGLAYER3WAVEFORMAT m_WFormat;		//	無理矢理mp3のヘッダーを用意する
		bool			m_bOpen;			//	オープンは成功したのか？
	};
}