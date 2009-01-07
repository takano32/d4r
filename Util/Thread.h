#pragma once

// 2003/12/23
// (C) Keigo Machinaga

#include <windows.h>

namespace base {
	// CriticalSection�N���X
	// enter()�ŃN���e�B�J���Z�N�V�����ɓ���Aleave()�Ŕ�����B
	// leave���Ȃ��Ńf�X�g���N�g�������̋����͕s���B
	class CriticalSection
	{
	public:
		CriticalSection();
		virtual ~CriticalSection();
		void enter();
		void leave();
	private:
		CRITICAL_SECTION m_section;
	};
	
	// Thread�N���X
	// �C���X�^���X���쐬���Astart���Ăяo����process()���ʃX���b�h�Ŏ��s����܂��B
	// ����Thread�N���X���p�����āAprocess()��ړI�ɉ����ċL�q���܂��B
	// �i�����o���\�b�h��������̂ŁAprocess()��p�ɃN���X�����Ɨǂ��j
	// process()���I������Ǝ����I�ɃX���b�h�͏I�����܂��B
	// �O������I�����߂𑗂肽���Ƃ���static���\�b�h��stop()���Ăяo���܂��B
	// �����requestedStop()��true�ɂȂ�̂ŁAprocess()���I�����ĉ������B
	// stop()��process()���I������̂�҂��܂��B
	// �f�X�g���N�^���Ă΂ꂽ�Ƃ��A�X���b�h���I�����Ă��Ȃ���ΏI����҂��܂��B
	class Thread
	{
	public:
		Thread();
		virtual ~Thread();
		// ���s���ł���Ύ��s����B
		virtual bool start();
		// �������ꂽ�X���b�h���s�������B
		virtual void run()=0;
		// ��~�v�����A��~����܂ő҂B
		virtual void stop();
		
		// setPriority�Ŏg���D��x�̎��
		enum Priority{ PRIORITY_NORMAL, PRIORITY_HIGHEST };
		// ���s���̂ݗL��
		void setPriority(Priority priotiry);
		
	protected:
		// ��~�v�������Ă��邩�ǂ����B
		bool requestedStop(){ return m_requested_stop; }
		
	protected:
		void processOriginal();
		static void processStatic(void *instance);
		bool m_requested_stop;
		unsigned long m_thread_handle;
	};
}



