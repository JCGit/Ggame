#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

//=======================================================================================================================
/// IThreadInterface ��������� ���ϰ� �����ϱ� ���� �޴��� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- IThreadInterface ��������� �߰� AddThread() �ؼ� �����ؼ� �����Ѵ�.
			- BatchStartThread() ��������� �ϰ� ResumeThread()�Ѵ�.
			- BatchStopThread() ��������� �ϰ� EndTread()�Ѵ�.
	@history
			- 2004.11.11 : utility library�� �߰� & util namespace�� ���
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