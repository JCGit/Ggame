#pragma once

#include "cqueue.h"
#include "../../Utility/CriticalSection.h"

class CWHQueue : public CQueue< BYTE >, public CCriticalSection
{
public:
	virtual ~CWHQueue()
	{
		ClearAll();
	}

	bool PushQ( BYTE *lpbtQ )
	{
		Lock();
		bool bRet = Enqueue( lpbtQ );
		Unlock();

		return bRet;
	}

	BYTE * PopQ()
	{
		Lock();
		BYTE *pData = Dequeue();
		Unlock();

		return pData;
	}
};