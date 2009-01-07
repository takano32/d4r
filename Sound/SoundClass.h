#pragma once

#include <map>
#include "SoundSystem.h"
#include "SoundBuffer.h"
#include "SoundFadeThread.h"
#include "../Thread.h"
using namespace std;


#define MAX_SOUND 128

//#pragma warning( disable : 4786 )

namespace base {
	class SoundClass{
	private:
		SoundSystem* _pSystem;
		SoundBuffer* _pBuffers[MAX_SOUND];
		int _last_fade_id;
		int _last2_fade_id;
		SoundFadeThread* _pThread;
	public:
		SoundClass( HWND hWnd );
		~SoundClass();
		BOOL Load( int id, BYTE* data, int size );
		BOOL Open( int id, char* file_name );
		BOOL Close( int id );
		BOOL Play( int id, BOOL with_loop );
		BOOL SetPosition( int id, int pos );
		BOOL Stop( int id );
		BOOL PlayWithFade( int id, int fade_msec, BOOL with_loop);
		BOOL BackScene( int fade_msec, BOOL with_loop );
	};
}

