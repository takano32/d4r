#include "SoundBuffer.h"

#include "../Log.h"
#include "SoundACM.h"
#include <math.h>

using namespace base;

SoundBuffer::SoundBuffer( SoundSystem* pSystem ) {
	_pSoundObject = pSystem->GetSoundObject();
	_pFormat = new WAVEFORMATEX();
	_pFile = new File();
	_pSoundACM = new SoundACM();
}

SoundBuffer::~SoundBuffer() {
	Close();
	delete _pFile;
	delete _pSoundACM;
	delete _pFormat;
	_pFormat = NULL;
}

int SoundBuffer::GetVolume() {
	long tmp;
	HRESULT ret;
	ret = _pBuffer->GetVolume( &tmp );
	double volume = (double)tmp;
	volume /= -100;
	volume = 100 - 50 * log10(volume);
	if( ret == DS_OK ) {
		return (int)volume;
	}
	return -1;
}

BOOL SoundBuffer::SetVolume( int volume ) {
	double value = pow(100, 1.0 - (double)volume / 100.0);
	value *= -100;

	HRESULT hr = _pBuffer->SetVolume( (long)value );
	if( hr != DS_OK ) {
		return FALSE;
	}
	return TRUE;
}



BOOL SoundBuffer::Close() {
	_pFile->Close();
	_pSoundACM->Close();

	// 開放
	if( _pBuffer != NULL ){
		if( _pBuffer->Release() != DS_OK ){
			MessageBox( GetActiveWindow(), "バッファの開放に失敗しました", "ERROR", MB_OK );
			return FALSE;		// 失敗
		}
		_pBuffer = NULL;
	}
	
	// 成功
	return TRUE;
}


BOOL SoundBuffer::Load( BYTE* data, int size ) {
	MMIOINFO mmio;
	ZeroMemory(&mmio,sizeof(mmio));
	mmio.pchBuffer = (LPSTR)data;
	mmio.fccIOProc = FOURCC_MEM;
	mmio.cchBuffer = size;


	_hMmio = mmioOpen(NULL,&mmio,MMIO_READ);
	if( _hMmio == NULL ){
		Log::Error( "MultiMediaIO::MultiMediaIO メモリの読み込みに失敗しました" );
		return FALSE;
	}
	_mmckParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if( mmioDescend( _hMmio, &_mmckParent, NULL, MMIO_FINDRIFF ) ){
		mmioClose( _hMmio, 0 );
		// MP3である可能性が高い．
		if( _pSoundACM->Open((BYTE*)data, (DWORD)size ) == 0 ) {
			return ReadWithACM();
		}
		return FALSE;
	}

	_mmckChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( mmioDescend( _hMmio, &_mmckChild, &_mmckParent, MMIO_FINDCHUNK ) ){
		mmioClose( _hMmio, 0 );
		return FALSE;
	}


	// WAVEファイルからWAVEフォーマットを読み取ります
	if( mmioRead( _hMmio, (char *)_pFormat, sizeof( WAVEFORMATEX ) ) != sizeof( WAVEFORMATEX ) ){
		// fmtチャンクを読み取ることができません
		mmioClose( _hMmio, 0 );		// 閉じる
		return FALSE;				// 失敗
	}
	
	// PCMフォーマットのWAVEしか扱えません
	if( _pFormat->wFormatTag != WAVE_FORMAT_PCM ){
		// WAVEファイルはPCMフォーマットではありません
		mmioClose( _hMmio, 0 );		// 閉じる
		return FALSE;				// 失敗
	}
	
	// アセンド
	if( mmioAscend( _hMmio, &_mmckChild, 0 ) ){
		// アセンドできません
		mmioClose( _hMmio, 0 );		// 閉じる
		return FALSE;				// 失敗
	}
	
	// dataチャンクへのディセンド
	_mmckChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( mmioDescend( _hMmio, &_mmckChild, &_mmckParent, MMIO_FINDCHUNK ) ){
		// WAVEファイルにdataチャンクがありません
		mmioClose( _hMmio, 0 );		// 閉じる
		return FALSE;				// 失敗
	}
		
	return Read();
}


BOOL SoundBuffer::Open( char* file_name ) {
	_pFile->Open( file_name );
	return Load( (BYTE*)_pFile->GetMemory(), _pFile->GetSize() );
}



BOOL SoundBuffer::ReadWithACM() {
	//_lengthと_pFormatの設定
	if (_pSoundACM->IsOpen()) {	//	既にMP3ファイルをオープンしている
		_length	= _pSoundACM->GetSize();	//	acmから変換後のサイズを得る
		_pFormat	= _pSoundACM->GetFormat();	//	acmから変換後のフォーマットを得る
	}

		// ファイルを閉じる
	mmioClose( _hMmio, 0 );
	
	if( Create() == FALSE ) {
		return FALSE;
	}
		  
	// バッファに書きこむ
	if( FillWithACM() == FALSE ){
		return FALSE;				// 失敗
	}
	return TRUE;
}



BOOL SoundBuffer::FillWithACM() {
	// DirectSoundバッファをLock
	BYTE* lpDSBuffData;
	LRESULT hr;
	hr = _pBuffer->Lock(0, _length, (void**)&lpDSBuffData,
		&_length, NULL, 0, 0);
		// これは、実は、失敗することは多々有るのだ:p

	if (hr==DSERR_BUFFERLOST){
		_pBuffer->Restore(); // これでオッケ!（笑）
		hr = _pBuffer->Lock(0, _length, (void**)&lpDSBuffData,
			&_length, NULL, 0, 0);
		// んで、もっかいリトライするの！！
	}

	if (hr!=DS_OK) {
		// これでダメなら、メモリ足りんのちゃう？
		Log::Error("Sound::FillWithACM()サウンドのLock()に失敗！");
		return 8;
	}

	if (_pSoundACM->IsOpen()) {
		//	acmを使うならば、acmにLockしたバッファポインタを渡して
		//	直接そこに変換してもらう。（なんでみんなそうせーへんの？）
		if (_pSoundACM->Convert(lpDSBuffData)!=0) {
			Log::Error("Sound::Acmでの変換に失敗");
			return 9;
		};
	} else {
		// WaveDataをDirectSoundバッファに転送
		//CopyMemory(lpDSBuffData,(LPVOID)pWaveData,dwDataLen);
		return FALSE;
	}

	// DirectSoundバッファのUnlock...
	if (_pBuffer->Unlock(lpDSBuffData, _length, NULL, 0)
		!= DS_OK) {
		// こんなんふつー、失敗するかぁ...どないせーちゅーんじゃ
		Log::Error("Sound::LoadのUnlock()に失敗！");
		return 10;
	}

	return TRUE;
}



BOOL SoundBuffer::Read( ) {
	// WAVE データ読み込みようの変数を作成する
	BYTE* data = new BYTE[ _mmckChild.cksize];		// 
	_length = _mmckChild.cksize;					// 音の長さの保存
	if( data == NULL ){					// 領域作成失敗
		return FALSE;				// 失敗
	}
	// wave データを読み取ります
	if( ( DWORD)mmioRead( _hMmio, ( char*)data, _mmckChild.cksize ) != _mmckChild.cksize ){
		// dataチャンクを読み取ることができません
		mmioClose( _hMmio, 0 );		// 閉じる
		return FALSE;				// 失敗
	}
	
	// ファイルを閉じる
	mmioClose( _hMmio, 0 );
	
	if( Create() == FALSE ) {
		return FALSE;
	}
		  
	// バッファに書きこむ
	if( Fill( data ) == FALSE ){
		return FALSE;				// 失敗
	}

	delete[] data;
		  
	return TRUE;
}




BOOL SoundBuffer::Fill( BYTE* data ) {
	LPVOID	write1;
	DWORD	length1;
	LPVOID	write2;
	DWORD	length2;
	HRESULT hr;
	
	// バッファを書きこむためにロックをする（ 一度失敗してももう一度行う ）
	hr = _pBuffer->Lock( 0, _length , &write1, &length1, &write2, &length2, 0 );
	if( hr == DSERR_BUFFERLOST){
		_pBuffer->Restore();
		hr = _pBuffer->Lock( 0, _length, &write1, &length1, &write2, &length2, 0 );
	}
	// ２度目ロック失敗ならば失敗にする
	if( hr != DS_OK ){
		return FALSE;
	}
	
	// 書きこむ write1 に書きこみ先のアドレスが入っている
	CopyMemory( write1, data, length1 );
	
	// 一度目で書きこめなかったら２度目を書き込む
	if( write2 != NULL ){
		CopyMemory( write2, data + length1, length2 );
	}
	
	// ロック解除
	hr = _pBuffer->Unlock( write1, length1, write2, length2 );
	if( hr != DS_OK ){
		return FALSE;
	}
	
	// 成功
	return TRUE;
}


BOOL SoundBuffer::Create( ) {
	DSBUFFERDESC	dsbdesc;
	HRESULT			hr;

	
	// 構造体の初期化
	ZeroMemory( &dsbdesc, sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize			= sizeof( DSBUFFERDESC );
	dsbdesc.dwBufferBytes	= _length;
	dsbdesc.lpwfxFormat 	= _pFormat;
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS; 

	// バッファの作成
	hr = _pSoundObject->CreateSoundBuffer( &dsbdesc, &_pBuffer, NULL );
	if( hr != DS_OK ){
		return FALSE;
	}

	SetPosition( 0 );
	SetVolume( VOLUME_MAX );
	
	// 成功
	return TRUE;
}



BOOL SoundBuffer::SetPosition( int position ) {
	HRESULT hr = _pBuffer->SetCurrentPosition( position );
	if( hr != DS_OK ) {
		return FALSE;
	}
	return TRUE;

}

BOOL SoundBuffer::Play( BOOL with_loop ) {
	static	long	loop_states[] = { 0, DSBPLAY_LOOPING, };	// ループ状態用
	HRESULT hr;													// 結果

	// 再生
	hr = _pBuffer->Play( 0, 0, loop_states[ with_loop] );
	if( hr != DS_OK ){
		_pBuffer->Restore();
	}
	if( FAILED(hr) ) {
		return FALSE;
	}else{
		return TRUE;
	}
}



BOOL SoundBuffer::Stop()
{
	HRESULT hr = _pBuffer->Stop();
	if( hr != DS_OK ) {
		return FALSE;
	}
	return TRUE;
}


BOOL SoundBuffer::FadeIn( int fade_msec, BOOL with_loop) {
	if( SetVolume( VOLUME_MIN ) == FALSE ) return FALSE;

	double increase = (double)(VOLUME_MAX - GetVolume());
	increase /= fade_msec;
	increase *= FADE_INTERVAL;
	double volume = (double)GetVolume();

	Play( with_loop );
	
	while( volume + increase < VOLUME_MAX ) {
		volume += increase;
		if( SetVolume( (int)volume ) == FALSE ) return FALSE;
		Sleep( FADE_INTERVAL );
	}
	
	return SetVolume( VOLUME_MAX );
}

BOOL SoundBuffer::FadeOut( int fade_msec ) {
	double decrease = (double)(GetVolume() - VOLUME_MIN);
	decrease /= fade_msec;
	decrease *= FADE_INTERVAL;
	double volume = (double)GetVolume();
	
	while( VOLUME_MIN < volume - decrease ) {
		volume -= decrease;
		if( SetVolume( (int)volume ) == FALSE ) return FALSE;
		Sleep( FADE_INTERVAL );
	}
	SetVolume( VOLUME_MIN );
	return Stop();
}

