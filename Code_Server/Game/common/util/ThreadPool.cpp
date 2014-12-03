#include "UtilityCommon.h"
#include ".\ThreadPool.h"
#include ".\IThreadInterface.h"

namespace util 
{

ThreadPool::ThreadPool()
	:	m_pThreadList	( NULL ),
		m_bAutoRelease	( TRUE )
{
	m_pThreadList = new CLinkedlist<IThreadInterface *>;
}

ThreadPool::~ThreadPool()
{
	_release();

	if( m_pThreadList )
	{
		delete m_pThreadList;
		m_pThreadList = NULL;
	}
}

VOID ThreadPool::_release()
{
	BatchStopThread();

	INT		nThreads = m_pThreadList->GetCount();
	HANDLE	* phThreads = new HANDLE[nThreads];

	int i = 0;
	IThreadInterface * pThread = NULL;
	POS pos = m_pThreadList->GetFirstPos();
	while(pos)
	{
		pThread = m_pThreadList->GetNextPos(pos);
		phThreads[i++] = pThread->GetHandle();
	}

	WaitForMultipleObjects( nThreads, phThreads, TRUE, INFINITE );

	delete phThreads;

	if( m_bAutoRelease )
	{
		BOOL bInfiniteLoop = TRUE;
		while ( bInfiniteLoop )
		{
			pThread = m_pThreadList->DeleteHead();

			if (pThread == NULL)
			{
				break;
			}

			SAFE_DELETE(pThread);
		}
	}
	else
	{
		m_pThreadList->DeleteAll();
	}
}

VOID ThreadPool::AddThread( IThreadInterface * pThread, BOOL bSuspend )
{
	m_pThreadList->AddTail( pThread );
	pThread->StartThread( bSuspend );
}

VOID ThreadPool::BatchStopThread()
{
	IThreadInterface * pThread = NULL;
	POS pos = m_pThreadList->GetFirstPos();
	while(pos)
	{
		pThread = m_pThreadList->GetNextPos(pos);
		pThread->EndThread();
	}
}

VOID ThreadPool::BatchStartThread()
{
	IThreadInterface * pThread = NULL;
	POS pos = m_pThreadList->GetFirstPos();
	while(pos)
	{
		pThread = m_pThreadList->GetNextPos(pos);
		pThread->ResumeThread();
	}
}
}