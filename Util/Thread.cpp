#include "Thread.h"

using namespace base;

//
// CriticalSection
//
CriticalSection::CriticalSection()
{
	InitializeCriticalSection( &m_section );
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection( &m_section );
}

void CriticalSection::enter()
{
	EnterCriticalSection( &m_section );
}

void CriticalSection::leave()
{
	LeaveCriticalSection( &m_section );
}


#include <process.h>

//
// Thread
//
Thread::Thread()
:m_requested_stop(false)
,m_thread_handle(0)
{
}

Thread::~Thread()
{
	if( m_thread_handle ){
		stop();
	}
}

void Thread::processStatic(void *instance)
{
	Thread* thread = (Thread*)instance;
	thread->processOriginal();
}

void Thread::processOriginal()
{
	run();
	m_thread_handle = 0;
}


bool Thread::start()
{
	if( m_thread_handle ) return false;
	
	m_requested_stop = false;
	m_thread_handle = _beginthread( Thread::processStatic, 0, (void*)this );
	if( m_thread_handle == (unsigned long)-1 ){
		m_thread_handle = 0;
		return false;
	}
	return true;
}

void Thread::stop()
{
	if( !m_thread_handle ) return;
	
	m_requested_stop = true;
	WaitForSingleObject( (HANDLE)m_thread_handle, INFINITE );
	m_thread_handle = 0;
}

void Thread::setPriority(Priority priority)
{
	if( !m_thread_handle ) return;
	
	switch( priority )
	{
	case Priority::PRIORITY_HIGHEST:
		SetThreadPriority( (HANDLE)m_thread_handle, THREAD_PRIORITY_TIME_CRITICAL );
		break;
	case Priority::PRIORITY_NORMAL:
		SetThreadPriority( (HANDLE)m_thread_handle, THREAD_PRIORITY_NORMAL );
		break;
	}
}

