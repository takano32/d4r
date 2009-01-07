#include "SoundSystem.h"
using namespace base;

#include "../Log.h"


int SoundSystem::__channels = 2;
int SoundSystem::__sample_rate = 22050;
int SoundSystem::__bits_per_sample = 16;

SoundSystem::SoundSystem( HWND hWnd ) {
	_hWnd = hWnd;
	this->_pSoundObject = NULL;							// DirectSoundObject
	this->_pPrimaryBuffer = NULL;						// プライマリバッファ
	InitializeSystem();
}


SoundSystem::~SoundSystem() {
	// パンと周波数を戻す
	if( this->_pPrimaryBuffer!= NULL ){
		_pPrimaryBuffer->SetPan( 0 );			// パンを戻す
	}
	RELEASE( this->_pPrimaryBuffer );			// プライマリバッファの解放
	RELEASE( this->_pSoundObject );				// ダイレクトサウンドオブジェクトの解放
}


LPDIRECTSOUND SoundSystem::GetSoundObject(){
	return _pSoundObject;
}

BOOL SoundSystem::InitializeSound() {
	HRESULT	ret;
	// ダイレクトサウンドオブジェクトの作成（ デフォルトのデバイスを使用 ）
	ret = DirectSoundCreate( NULL, &this->_pSoundObject, NULL );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd, "DirectSound オブジェクトの作成に失敗しました" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeCooperativeLevel() {
	HRESULT	ret;
	// 協調レベルの設定
	ret = this->_pSoundObject->SetCooperativeLevel( _hWnd , DSSCL_PRIORITY );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd, "協調レベル設定に失敗しました" );		
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeSpeaker() {
	HRESULT	ret;
	// スピーカーの設定（ ステレオに設定 ）
	ret = this->_pSoundObject->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE ) );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd,  "スピーカーの設定に失敗しました" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeSoundBuffer() {
	HRESULT	ret;
	// プライマリ・バッファの作成 ------------------------------------
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC	dsbdesc; 
	ZeroMemory( &dsbdesc, sizeof( DSBUFFERDESC ) );		// 初期化
	dsbdesc.dwSize = sizeof( DSBUFFERDESC );			// サイズの設定
	// プライマリ・バッファを指定（ 音量と左右の音量を変更可 ）
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER; 
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// バッファを作成する
	ret = this->_pSoundObject->CreateSoundBuffer( &dsbdesc, &_pPrimaryBuffer, NULL );
	if( ret != DS_OK ) { 
	    // バッファの作成に失敗
		Log::ShowError( _hWnd, "プライマリ・バッファの作成に失敗しました" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeWaveFormat() {
	HRESULT	ret;
	// プライマリ・バッファのWaveフォーマットを設定
	// 優先協調レベル以上の協調レベルが設定されている必要があります．
	WAVEFORMATEX pcmwf; 
	ZeroMemory( &pcmwf, sizeof( WAVEFORMATEX ) );	// 初期化 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM;				// フォーマットの指定
	pcmwf.nChannels = __channels;			// ２チャンネル（ステレオ）	
	pcmwf.nSamplesPerSec = __sample_rate;			// サンプリング・レート　44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = __bits_per_sample;		// 16ビット
	ret = this->_pPrimaryBuffer->SetFormat( &pcmwf );	// セット
	if( ret != DS_OK ){
		Log::ShowError( _hWnd , "プライマリ・バッファのフォーマット設定に失敗しました" );
		return FALSE;
	}

	return TRUE;
}

BOOL SoundSystem::InitializeSystem() {
	if(InitializeSound() && 
		InitializeCooperativeLevel() &&
		InitializeSpeaker() &&
		InitializeSoundBuffer() &&
		InitializeWaveFormat()) {
		return TRUE;
	}else {
		//失敗
		return FALSE;
	}
}



