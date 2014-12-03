#include "UtilityCommon.h"
#include "GeneralFunction.h"


BOOL EnterLockApplication( HANDLE hLock, TCHAR * pszLockName )
{
	hLock = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, pszLockName);
	if(hLock != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		CloseHandle(hLock); 
		hLock = NULL;
		return FALSE;
	}

	return TRUE;
}

void LeaveLockApplication( HANDLE hLock )
{
	if(hLock)
	{
		CloseHandle(hLock);
		hLock = NULL;
	}
}