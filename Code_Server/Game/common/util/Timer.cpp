#include <process.h>
#include "Timer.h"

CTimer::	CTimer():m_hThreadHandle(INVALID_HANDLE_VALUE)
{
	m_bEnd = false;
	m_bActive = false;
	delay = 1;
}

CTimer::~CTimer()
{
	SetActive(false);

	EndThread();
	if ( INVALID_HANDLE_VALUE != m_hThreadHandle )
	{
		WaitForSingleObject(m_hThreadHandle, INFINITE);
	}
}


bool CTimer::StartThread()
{
	m_hThreadHandle=(HANDLE)_beginthreadex(0,0,RunPtr,this,0,&m_ThreadID);

	if(m_hThreadHandle !=0)
		return true;
	return false;
}

void CTimer::Run()
{
	while(!m_bEnd)
	{
		if (!m_bActive)
		{
			Sleep(delay);
			continue;
		}		
		__try
		{
			Running();
			Sleep(delay);
		}
		__except( EXCEPTION_EXECUTE_HANDLER ) 
		{
			EndThread();
		}
	}
}


unsigned int __stdcall  CTimer::RunPtr(PVOID  pvoid)
{
	CTimer* p = (CTimer*)pvoid;
	p->Run();
	_endthreadex(0);
	return 0;
}



