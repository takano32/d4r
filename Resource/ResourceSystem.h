#pragma once
#include <windows.h>
#include <time.h>
#include "unzip32.h"
#include <string>
using namespace std;

namespace base{
	class ResourceSystem{
		HINSTANCE _hLib;
		
		typedef int (WINAPI *typeUnZipExtractMem)(const HWND, LPCSTR ,
			LPBYTE ,const DWORD, time_t*,
			LPWORD, LPDWORD);
		typedef int (WINAPI *typeUnZipFindFirst)(HARC,
			LPCSTR ,LPINDIVIDUALINFO);
		typedef HARC (WINAPI *typeUnZipOpenArchive)(const HWND, LPCSTR,
			const DWORD);
		typedef int (WINAPI *typeUnZipCloseArchive)(HARC);

		typeUnZipExtractMem _fpUnZipExtractMem;
		typeUnZipFindFirst _fpUnZipFindFirst;
		typeUnZipOpenArchive _fpUnZipOpenArchive;
		typeUnZipCloseArchive _fpUnZipCloseArchive;
		
	private:
		BOOL LoadLibrary();
		BOOL InitializeFunction();
		bool _isArchive;
		string _path;

		string AddPath(const char* filename);
	public:
		ResourceSystem(const char *path);
		~ResourceSystem();

		DWORD GetFileSize(LPCSTR szFileName);
		BOOL ReadIn(LPCSTR szFileName,
			LPBYTE szBuffer, const DWORD dwSize, LPDWORD lpdwWriteSize);
	};
}
