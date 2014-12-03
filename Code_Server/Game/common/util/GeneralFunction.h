
#pragma once


BOOL EnterLockApplication( HANDLE hLock, TCHAR * pszLockName );
void LeaveLockApplication( HANDLE hLock );