#include "SoundSystem.h"
using namespace base;

#include "../Log.h"


int SoundSystem::__channels = 2;
int SoundSystem::__sample_rate = 22050;
int SoundSystem::__bits_per_sample = 16;

SoundSystem::SoundSystem( HWND hWnd ) {
	_hWnd = hWnd;
	this->_pSoundObject = NULL;							// DirectSoundObject
	this->_pPrimaryBuffer = NULL;						// �v���C�}���o�b�t�@
	InitializeSystem();
}


SoundSystem::~SoundSystem() {
	// �p���Ǝ��g����߂�
	if( this->_pPrimaryBuffer!= NULL ){
		_pPrimaryBuffer->SetPan( 0 );			// �p����߂�
	}
	RELEASE( this->_pPrimaryBuffer );			// �v���C�}���o�b�t�@�̉��
	RELEASE( this->_pSoundObject );				// �_�C���N�g�T�E���h�I�u�W�F�N�g�̉��
}


LPDIRECTSOUND SoundSystem::GetSoundObject(){
	return _pSoundObject;
}

BOOL SoundSystem::InitializeSound() {
	HRESULT	ret;
	// �_�C���N�g�T�E���h�I�u�W�F�N�g�̍쐬�i �f�t�H���g�̃f�o�C�X���g�p �j
	ret = DirectSoundCreate( NULL, &this->_pSoundObject, NULL );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd, "DirectSound �I�u�W�F�N�g�̍쐬�Ɏ��s���܂���" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeCooperativeLevel() {
	HRESULT	ret;
	// �������x���̐ݒ�
	ret = this->_pSoundObject->SetCooperativeLevel( _hWnd , DSSCL_PRIORITY );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd, "�������x���ݒ�Ɏ��s���܂���" );		
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeSpeaker() {
	HRESULT	ret;
	// �X�s�[�J�[�̐ݒ�i �X�e���I�ɐݒ� �j
	ret = this->_pSoundObject->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE ) );
	if( ret != DS_OK ){
		Log::ShowError( _hWnd,  "�X�s�[�J�[�̐ݒ�Ɏ��s���܂���" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeSoundBuffer() {
	HRESULT	ret;
	// �v���C�}���E�o�b�t�@�̍쐬 ------------------------------------
	// DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC	dsbdesc; 
	ZeroMemory( &dsbdesc, sizeof( DSBUFFERDESC ) );		// ������
	dsbdesc.dwSize = sizeof( DSBUFFERDESC );			// �T�C�Y�̐ݒ�
	// �v���C�}���E�o�b�t�@���w��i ���ʂƍ��E�̉��ʂ�ύX�� �j
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER; 
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// �o�b�t�@���쐬����
	ret = this->_pSoundObject->CreateSoundBuffer( &dsbdesc, &_pPrimaryBuffer, NULL );
	if( ret != DS_OK ) { 
	    // �o�b�t�@�̍쐬�Ɏ��s
		Log::ShowError( _hWnd, "�v���C�}���E�o�b�t�@�̍쐬�Ɏ��s���܂���" );
		return FALSE;
	}
	return TRUE;
}

BOOL SoundSystem::InitializeWaveFormat() {
	HRESULT	ret;
	// �v���C�}���E�o�b�t�@��Wave�t�H�[�}�b�g��ݒ�
	// �D�拦�����x���ȏ�̋������x�����ݒ肳��Ă���K�v������܂��D
	WAVEFORMATEX pcmwf; 
	ZeroMemory( &pcmwf, sizeof( WAVEFORMATEX ) );	// ������ 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM;				// �t�H�[�}�b�g�̎w��
	pcmwf.nChannels = __channels;			// �Q�`�����l���i�X�e���I�j	
	pcmwf.nSamplesPerSec = __sample_rate;			// �T���v�����O�E���[�g�@44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = __bits_per_sample;		// 16�r�b�g
	ret = this->_pPrimaryBuffer->SetFormat( &pcmwf );	// �Z�b�g
	if( ret != DS_OK ){
		Log::ShowError( _hWnd , "�v���C�}���E�o�b�t�@�̃t�H�[�}�b�g�ݒ�Ɏ��s���܂���" );
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
		//���s
		return FALSE;
	}
}



