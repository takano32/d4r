

#pragma once

#include <windows.h>


namespace base{
	class File{
	private:
		HANDLE _hFile;
		LPVOID _pMemory;
	public:
		File();
		~File();
		DWORD GetSize();
		LPVOID GetMemory();
		BOOL Open( LPCTSTR lpFileName );
		BOOL Read();
		BOOL Close();
		BOOL IsOpen();
		
	};
}