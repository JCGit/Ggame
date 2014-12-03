#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

//=======================================================================================================================
/// IThreadInterface 스레드들을 편리하게 관리하기 위한 메니져 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- IThreadInterface 스레드들을 추가 AddThread() 해서 통합해서 관리한다.
			- BatchStartThread() 스래드들을 일괄 ResumeThread()한다.
			- BatchStopThread() 스래드들을 일괄 EndTread()한다.
	@history
			- 2004.11.11 : utility library에 추가 & util namespace에 등록
*/
//=======================================================================================================================
#pragma once


#include "Linkedlist.h"
using namespace util;


namespace util 
{


class IThreadInterface;

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	VOID								AddThread( IThreadInterface * pThread, BOOL bSuspend = FALSE );
	inline VOID							SetAutoRelease( BOOL bVal ) { m_bAutoRelease = bVal; }
	VOID								BatchStopThread();
	VOID								BatchStartThread();

private:
	VOID								_release();

private:
	CLinkedlist<IThreadInterface *>		* m_pThreadList;
	BOOL								m_bAutoRelease;
};

}



#endif // __THREADPOOL_H__