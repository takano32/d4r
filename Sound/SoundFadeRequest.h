#pragma once

#include "SoundBuffer.h"

namespace base{
	class SoundFadeRequest{
	private:
		SoundBuffer* _from_buffer;
		SoundBuffer* _to_buffer;
		int _fade_msec;
		BOOL _with_loop;
	public:
		SoundFadeRequest( SoundBuffer* from, SoundBuffer* to, int fade_msec, BOOL with_loop );
		void Fade();
	};
}