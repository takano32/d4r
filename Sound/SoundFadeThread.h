
#pragma once

#include <queue>

#include "SoundBuffer.h"
#include "SoundFadeRequest.h"
#include "../Thread.h"

using namespace std;
using namespace base;

namespace base {
	class SoundFadeThread : public Thread {
	private:
		//void (*_process)(SoundBuffer* );
		queue<SoundFadeRequest> _request_queue;
	public:
		SoundFadeThread();
		~SoundFadeThread();
		void run();
		void Fade( SoundBuffer* from, SoundBuffer* to, int fade_msec, BOOL with_loop );
	};
}

