#pragma once

// 2003/12/23
// (C) Keigo Machinaga

#include <windows.h>

namespace base {
	// CriticalSectionクラス
	// enter()でクリティカルセクションに入り、leave()で抜ける。
	// leaveしないでデストラクトした時の挙動は不明。
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
	
	// Threadクラス
	// インスタンスを作成し、startを呼び出すとprocess()が別スレッドで実行されます。
	// このThreadクラスを継承して、process()を目的に応じて記述します。
	// （メンバメソッドが増えるので、process()専用にクラスを作ると良い）
	// process()が終了すると自動的にスレッドは終了します。
	// 外部から終了命令を送りたいときはstaticメソッドのstop()を呼び出します。
	// するとrequestedStop()がtrueになるので、process()を終了して下さい。
	// stop()はprocess()が終了するのを待ちます。
	// デストラクタが呼ばれたとき、スレッドが終了していなければ終了を待ちます。
	class Thread
	{
	public:
		Thread();
		virtual ~Thread();
		// 実行中であれば失敗する。
		virtual bool start();
		// 生成されたスレッドが行う処理。
		virtual void run()=0;
		// 停止要求し、停止するまで待つ。
		virtual void stop();
		
		// setPriorityで使う優先度の種類
		enum Priority{ PRIORITY_NORMAL, PRIORITY_HIGHEST };
		// 実行中のみ有効
		void setPriority(Priority priotiry);
		
	protected:
		// 停止要求が来ているかどうか。
		bool requestedStop(){ return m_requested_stop; }
		
	protected:
		void processOriginal();
		static void processStatic(void *instance);
		bool m_requested_stop;
		unsigned long m_thread_handle;
	};
}



