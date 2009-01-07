#include "SoundFadeThread.h"

using namespace base;

SoundFadeThread::SoundFadeThread() {
}

SoundFadeThread::~SoundFadeThread() {
}

void SoundFadeThread::run() {
	while( !m_requested_stop ) {
		if( _request_queue.size() != 0 ) {
			SoundFadeRequest request = _request_queue.front();
			_request_queue.pop();
			request.Fade();
		}
		Sleep(500);
	}
}


void SoundFadeThread::Fade( SoundBuffer* from, SoundBuffer* to, int fade_msec, BOOL with_loop ) {
	_request_queue.push( SoundFadeRequest(from, to, fade_msec, with_loop ) );
}

