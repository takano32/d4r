#include "SoundFadeRequest.h"
using namespace base;

SoundFadeRequest::SoundFadeRequest( SoundBuffer* from, SoundBuffer* to, int fade_msec, BOOL with_loop ){
	_with_loop = with_loop;
	_from_buffer = from;
	_to_buffer = to;
	_fade_msec = fade_msec;
}


void SoundFadeRequest::Fade() {
	if( _from_buffer ) {
		_from_buffer->FadeOut( _fade_msec );
	}
	if( _to_buffer ) {
		_to_buffer->FadeIn( _fade_msec, _with_loop );
	}
}



