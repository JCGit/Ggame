#include "UtilityCommon.h"
#include ".\solardump.h"
#include <time.h>
#include "SolarAssert.h"

typedef BOOL (WINAPI * fnMINIDUMPWRITEDUMP)(
    HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
    );

MINIDUMP_TYPE	SolarDump::s_eDumpType			= MiniDumpNormal;
TCHAR			SolarDump::s_pszDumpFileDir[]	= {0,};
TCHAR			SolarDump::s_pszFilePrefix[]	= {0,};

SolarDump::SolarDump( TCHAR * pszDir, MINIDUMP_TYPE eDumpType )
{	
	//ASSERT( !IsBadReadPtr( pszDir, sizeof(pszDir) ) );

	lstrcpyn( s_pszDumpFileDir, pszDir, MAX_PATH );

	TCHAR szFilename[MAX_PATH];
    GetModuleFileName( NULL, szFilename, MAX_PATH );

	TCHAR * slash = strrchr( szFilename, '\\' );
    TCHAR * dot = strrchr( slash+1, '.' );
    ::lstrcpyn( s_pszFilePrefix, slash+1, (int)(dot - slash) );
	

	s_eDumpType = eDumpType;
	
	::SetUnhandledExceptionFilter( ExceptionFilter );
}

SolarDump::~SolarDump()
{
	
}


LONG WINAPI SolarDump::ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
	LONG rt = EXCEPTION_CONTINUE_SEARCH;

	if( pExceptionInfo == NULL )
	{
		static BOOL first = FALSE;
		if( first ) return rt;
		first = TRUE;

		__try 
		{
			RaiseException( EXCEPTION_BREAKPOINT, 0, 0, NULL );
		} 
		__except( ExceptionFilter( GetExceptionInformation() ), EXCEPTION_CONTINUE_EXECUTION ) 
		{
		}
		return rt;
	}
	// 현재 경로에서 dll로드
	TCHAR pszDllPath[MAX_PATH];
	wsprintf( pszDllPath, "%s\\DBGHELP.DLL" );
	HANDLE hDLL = ::LoadLibrary( pszDllPath );
	if( !hDLL ) hDLL = ::LoadLibrary( "DBGHELP.DLL" );
	if( NULL == hDLL )
    {
		SASSERT(NULL, "DBGHELP.DLL을 로드할 수 없습니다." );
        return rt;
    }

	fnMINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (fnMINIDUMPWRITEDUMP)GetProcAddress( (HMODULE)hDLL, "MiniDumpWriteDump" );
	if( NULL == pfnMiniDumpWriteDump )
    {
		SASSERT(NULL, "오래된 버전의 DBGHELP.DLL입니다. 최신버전으로 업데이트 하시기 바랍니다." );
        return rt;
    }

	struct tm * today;
	time_t		ltime;
	time( &ltime );
	today = localtime( &ltime );

	char szDumpPath[MAX_PATH*2];
	wsprintf( szDumpPath, 
		"%s\\%s%02d-%02d-%02d=%02dh%02dm%02ds.dmp", 
		s_pszDumpFileDir, 
		s_pszFilePrefix, 
		today->tm_year+1900, 
		today->tm_mon+1, 
		today->tm_mday, 
		today->tm_hour, 
		today->tm_min, 
		today->tm_sec );

    HANDLE hFile = CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( INVALID_HANDLE_VALUE == hFile )
    {
		SASSERT(NULL, VSTR( "[%s]파일을 열 수 없습니다.", szDumpPath ) );
        return rt;
    }

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId				= GetCurrentThreadId();
	eInfo.ExceptionPointers		= pExceptionInfo;
	eInfo.ClientPointers		= FALSE;

	BOOL bResult = pfnMiniDumpWriteDump( GetCurrentProcess(), 
		GetCurrentProcessId(), 
		hFile, 
		s_eDumpType, 
		(pExceptionInfo?&eInfo:NULL), 
		NULL, NULL);

    if( !bResult )
    {
		SASSERT(NULL, "MiniDumpWriteDump() 호출이 실패했습니다." );
		CloseHandle(hFile);
		return ( rt = EXCEPTION_EXECUTE_HANDLER );
    }

    CloseHandle( hFile );

	return rt;
}