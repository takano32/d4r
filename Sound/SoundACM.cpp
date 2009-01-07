#include "SoundACM.h"

#include <winbase.h>
#include "../Log.h"

using namespace base;

SoundACM::SoundACM(void){
	m_bOpen = false;
}

SoundACM::~SoundACM(){
	Close();
}


LRESULT SoundACM::Open(WAVEFORMATEX *pWFormat,LPVOID lpSrcBuf,DWORD dwSrcLength){
	Close();

	//	忘れんうちに引数をコピー:p
	m_lpSrcWFormat	= pWFormat;
	m_lpSrcBuf		= lpSrcBuf;
	m_dwSrcLength	= dwSrcLength;

	//	構造体初期化
	::ZeroMemory(&m_destWFormat, sizeof(m_destWFormat));
	::ZeroMemory(&m_acmheader, sizeof(m_acmheader));
	m_hAcm	= NULL;
	m_dwDestLength = 0;

	m_destWFormat.wFormatTag = WAVE_FORMAT_PCM;	//	PCMになって欲しいねん！
	if (acmFormatSuggest(NULL,pWFormat,&m_destWFormat,sizeof(WAVEFORMATEX),ACM_FORMATSUGGESTF_WFORMATTAG)!=0){
		return 1;	//	acm無いんとちゃう？
	}
	if (acmStreamOpen(&m_hAcm,NULL,pWFormat,&m_destWFormat,NULL,NULL,NULL,ACM_STREAMOPENF_NONREALTIME)!=0){
		return 2;	//	acmおかしんとちゃう？
	}
	if (acmStreamSize(m_hAcm,dwSrcLength,&m_dwDestLength,ACM_STREAMSIZEF_SOURCE)!=0){
		return 3;	//	なんでやねんと言いたい（笑）
	}

	if (m_dwDestLength == 0) return 4;	//	なぜじゃー

	m_bOpen = true;	//	Openに成功。これより任務に移る:p
	return 0;
}

LRESULT SoundACM::Open(LPBYTE src,DWORD size){	//	直接MP3のファイルとかをオープン:p
	LPBYTE src_begin = src;
	
	//	フレームヘッダからWAVEFORMATEXのデータを用意する
	
	if (size <= 128) return 1; // そんな小さいわけあらへんがな:p
	
	//  ID3v2タグがついているならば、読み飛ばす
	if ((src[0] == 'I') && (src[1] == 'D') && (src[2] == '3')) {
		DWORD dwID3Size = src[9] + (src[8]<<7) + (src[7]<<14) + (src[6]<<21);
		
		// バージョンチェック
		if(src[3]>=0x04)
		{
			// ID3v2.4.0以降
			if(src[5]&0x10){	// フッタの有無
				dwID3Size+=20; // フッタあり
			}else{
				dwID3Size+=10; // フッタなし
			}
		}else{
			// ID3v2.3.0以前
			dwID3Size+=10; // フッタなし
		}
		
		if (size <= dwID3Size + 128) return 1;
		
		src += dwID3Size;
		size -= dwID3Size;
	}
	
	//	MP3チェック
	if (src[0] !=0xff)		return 1;
	if ((src[1]&0xf8)!=0xf8) return 1;
	
	int	anBitrate[2][3][16] = {
		{
			// MPEG-1
			{ 0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0 },	//	32000Hz(layer1)
			{ 0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,0 },	//	44100Hz(layer2)
			{ 0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,0 },	//	48000Hz(layer3)
		},
		{
				// MPEG-2
			{ 0,32,48,56, 64, 80, 96,112,128,144,160,176,192,224,256,0 },	//	32000Hz(layer1)
			{ 0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0 },	//	44100Hz(layer2)
			{ 0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0 },	//	48000Hz(layer3)
			},
	};
	int anFreq[2][4] = {
		{ 44100,48000,32000,0 },
		{ 22050,24000,16000,0 },
	};
	
	int nLayer		= 4-((src[1] >> 1) & 3);
	if (nLayer == 4) {
		Log::Error( "SoundACM::Open MP3が無効なレイヤ" );
		return 1;
	}
	
	int nMpeg		= ((src[1] & 8) == 0) ? 1 : 0;
	int nBitrate	= anBitrate[nMpeg][nLayer-1][ src[2]>>4 ];
	int nFreq		= anFreq[nMpeg][(src[2] >> 2) & 3];
	int nChannel	= ((src[3] >> 6) == 3) ? 1 : 2;
	int nFrameSize	= 144000 * nBitrate / nFreq;
	
	//	無理矢理MP3のタグを用意する
	::ZeroMemory(&m_WFormat, sizeof(m_WFormat));
	m_WFormat.wfx.cbSize			= MPEGLAYER3_WFX_EXTRA_BYTES;
	m_WFormat.wfx.wFormatTag		= WAVE_FORMAT_MPEGLAYER3;
	m_WFormat.wfx.nChannels			= nChannel;
	m_WFormat.wfx.nSamplesPerSec	= nFreq;
	m_WFormat.wfx.nAvgBytesPerSec	= nBitrate * 1000 / 8;
	m_WFormat.wfx.nBlockAlign		= 1;
	m_WFormat.wfx.wBitsPerSample	= 0;
	m_WFormat.wID					= MPEGLAYER3_ID_MPEG;
	m_WFormat.fdwFlags				= MPEGLAYER3_FLAG_PADDING_OFF;
	m_WFormat.nBlockSize			= nFrameSize;
	m_WFormat.nFramesPerBlock		= 1;
	m_WFormat.nCodecDelay			= 0x0571;
	
	//	後ろにID3タグがついているならば、その分を除外する
	//	size>128である事は保証されている
	if ((src_begin[size-128] == 'T') && (src_begin[size-127] == 'A') && (src_begin[size-126] == 'G')) {
		size -= 128;
	}
	
	return Open((WAVEFORMATEX*)&m_WFormat,(LPVOID)(src+4),size-4);
	
};

WAVEFORMATEX* SoundACM::GetFormat(void) {
	if (m_bOpen) return &m_destWFormat;
	return NULL;//	Openしてへんのに呼ぶな～！
}

DWORD SoundACM::GetSize(void) const {
	if (m_bOpen) return m_dwDestLength;
	return 0;	//	Openしてへんのに呼ぶな～！
}

LRESULT SoundACM::Convert(LPVOID lpDestBuf){
	//	ここでDirectSoundのLockしたメモリポインタを渡す

	if (!m_bOpen) {
		return 1;	//	Openできてへんのに呼ぶなっちゅーに！
	}
	m_acmheader.cbStruct		=	sizeof(m_acmheader);
	m_acmheader.pbSrc			=	(BYTE*)m_lpSrcBuf;
	m_acmheader.cbSrcLength		=	m_dwSrcLength;
	m_acmheader.pbDst			=	(BYTE*)lpDestBuf;		//	ここにコピーしたいねん！
	m_acmheader.cbDstLength		=	m_dwDestLength;
	if (acmStreamPrepareHeader(m_hAcm,&m_acmheader,NULL)!=0) {
		return 2;	//	勘弁して～（笑）
	}

	if (acmStreamConvert(m_hAcm,&m_acmheader,NULL)!=0){
		return 3;	//	ダメじゃん（笑）
	}
	return 0;	//	任務終了
}

LRESULT SoundACM::Close(void) {
	if (m_bOpen) {
		m_bOpen = false;
		acmStreamUnprepareHeader(m_hAcm,&m_acmheader,NULL);
		return acmStreamClose(m_hAcm,NULL);
	}
	return 0;
}

bool	SoundACM::IsOpen(void) const {
	return m_bOpen;
}