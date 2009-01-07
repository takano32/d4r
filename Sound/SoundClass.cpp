#include "SoundClass.h"
#include <ruby.h>
using namespace base;

SoundClass::SoundClass( HWND hWnd ) {
	_pSystem = new SoundSystem( hWnd );
	memset( _pBuffers, 0, sizeof(SoundBuffer*) * MAX_SOUND );
	_last_fade_id = 0;
	_last2_fade_id = 0;
	_pThread = new SoundFadeThread();
	_pThread->start();
}

SoundClass::~SoundClass() {
	delete _pSystem;
	_pThread->stop();
	delete _pThread;
}

BOOL SoundClass::Load( int id, BYTE* data, int size ) {
	SoundBuffer** buffer = &_pBuffers[id];
	if( *buffer ) {
		rb_raise(rb_eFatal, "Sound#load this id is already exists: %d", id);
		return Qnil;
		//delete *buffer;
		//*buffer = NULL;
	}
	*buffer = new SoundBuffer( _pSystem );
	return (*buffer)->Load( data, size );
}

BOOL SoundClass::Open( int id, char* file_name ) {
	Close( id );
	_pBuffers[id] = new SoundBuffer( _pSystem );
	return _pBuffers[id]->Open( file_name );
}

BOOL SoundClass::Close( int id ) {
	if( _pBuffers[id] ) {
		_pBuffers[id]->Close();
		delete _pBuffers[id];
		_pBuffers[id] = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL SoundClass::SetPosition( int id, int pos ) {
	return _pBuffers[id]->SetPosition( pos );
}

BOOL SoundClass::Play( int id, BOOL with_loop ) {
	return _pBuffers[id]->Play( with_loop );
}

BOOL SoundClass::Stop( int id ) {
	return _pBuffers[id]->Stop();
}


BOOL SoundClass::PlayWithFade( int id, int fade_msec, BOOL with_loop ) {
	_pThread->Fade( _pBuffers[_last_fade_id], _pBuffers[id], fade_msec, with_loop );
	_last2_fade_id = _last_fade_id;
	_last_fade_id = id;
	return TRUE;
}

BOOL SoundClass::BackScene(  int fade_msec, BOOL with_loop) {
	_pThread->Fade( _pBuffers[_last_fade_id], _pBuffers[_last2_fade_id], fade_msec, with_loop );
	int tmp = _last_fade_id;
	_last_fade_id = _last2_fade_id;
	_last2_fade_id = tmp;
	return TRUE;
}

