#include "File.h"
using namespace base;

File::File() {
	_hFile = NULL;
}


File::~File() {
	if( IsOpen() ){
		Close();
	}
}


DWORD File::GetSize() {
	return ::GetFileSize( _hFile, 0 );
}

LPVOID File::GetMemory() {
	if( !IsOpen() ) {
		return NULL;
	}
	return _pMemory;
}

BOOL File::Open( LPCTSTR lpFileName ) {
	_hFile = CreateFile( lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if( _hFile == INVALID_HANDLE_VALUE ) {
		return FALSE;
	}else{
		return Read();
	}
}

BOOL File::Read() {
	DWORD size = this->GetSize() + 1;
	DWORD read_size;
	_pMemory = (LPVOID)::GlobalAlloc(GMEM_FIXED | GMEM_NOCOMPACT, size );
	::ReadFile( _hFile, _pMemory, size, &read_size, 0 );
	if( read_size == 0 ) {
		return FALSE;
	}
	return TRUE;
}



BOOL File::IsOpen() {
	if( _hFile == NULL ) {
		return FALSE;
	}else{
		return TRUE;
	}
}

BOOL File::Close() {
	if( IsOpen() && CloseHandle( _hFile ) ) {
		::GlobalFree( _pMemory );
		return TRUE;
	}
	return FALSE;
}




