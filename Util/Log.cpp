#include "Log.h"
#include <time.h>
#include <iostream>  // cin, cout, cerr


using namespace base;

ofstream* Log::_out = NULL;

BOOL Log::Open( const char* file_name ) {
	if( _out ) {
		Close();
	}

	_out = new ofstream( file_name, ios::app );
	return _out?TRUE:FALSE;
}

BOOL Log::Close() {
	if( _out ) {
		delete _out;
		_out = NULL;
		return TRUE;
	}
	return FALSE;
}

void Log::Out(Level level, HWND hWnd, va_list pArgs, const char* szFormat, ... ) {
    TCHAR szBuffer[1024];
    const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    const int LASTCHAR = NUMCHARS - 1;


    _vsnprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    

    szBuffer[LASTCHAR] = TEXT('\0');
	
	time_t timer;
	struct tm *t_st;
	char *wday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	char date[256];
	/* Œ»ÝŽž‚ÌŽæ“¾ */
	time(&timer);
	/* Œ»ÝŽž‚ð\‘¢‘Ì‚É•ÏŠ· */
	t_st = localtime(&timer);
	sprintf( date, "%04d-%02d-%02d(%s) %02d:%02d:%02d",
		t_st->tm_year+1900, 
		t_st->tm_mon+1, 
		t_st->tm_mday, 
		wday[t_st->tm_wday], 
		t_st->tm_hour, 
		t_st->tm_min,
		t_st->tm_sec );
	if( _out ) {
		*_out<< date << " [" << ::GetCurrentThreadId() << "]\t" << szBuffer << endl;
	}else{
		cerr << date << " [" << ::GetCurrentThreadId() << "]\t" << szBuffer << endl;
	}
	
	if( __SHOW_ERROR == level ){
		MessageBox( hWnd, szBuffer, TEXT(""), MB_OK | MB_ICONERROR);
	}
}


void Log::Debug( const char* szFormat, ... ) {
    va_list pArgs;
    va_start(pArgs, szFormat);
	Out( __DEBUG, NULL, pArgs, szFormat );
	va_end(pArgs);
}

void Log::Warn( const char* szFormat, ... ) {
	va_list pArgs;
    va_start(pArgs, szFormat );
	Out( __WARN, NULL, pArgs, szFormat );
	va_end(pArgs);
}

void Log::Error( const char* szFormat, ... ) {
	va_list pArgs;
    va_start(pArgs, szFormat );
	Out( __ERROR, NULL, pArgs, szFormat );
	va_end(pArgs);
}

void Log::Fatal( const char* szFormat, ... ) {
	va_list pArgs;
    va_start(pArgs, szFormat );
	Out( __FATAL, NULL, pArgs, szFormat );
	va_end(pArgs);
}


void Log::ShowError( HWND hWnd, const char* szFormat, ... ) {
	va_list pArgs;
    va_start(pArgs, szFormat );
	Out( __SHOW_ERROR, hWnd, pArgs, szFormat );
	va_end(pArgs);
}
