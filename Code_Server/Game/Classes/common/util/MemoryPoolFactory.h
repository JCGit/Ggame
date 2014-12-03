#ifndef __MEMORYPOOLFACTORY_H__
#define __MEMORYPOOLFACTORY_H__
// MemoryPoolFactory.h: interface for the CMemoryPoolFactory class.
// Memory Pool Management Class version 1.01
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-1-10
// 
//-------------------------------------------------------------------
// History.
// 2004-3-31	:	MemoryPoolTempl class ver 0.1
// 2004-6-18	:	MemoryPoolTempl class ver 0.2
// 2004-6-21    :	Error Check Byte added ver 0.21
// 2004-7-26    :	MemoryPool Framework ver1.0
// 2005-1-10    :	SafeMemoryPoolFactory class added 1.01
// 2005-3-28    :	set default value as dwPoolExtendSize = dwPoolBasicSize/2+1
// Be Done
//		- MemoryPool template class of inherited-method and factory-method
//		- Multi-thread safety
//		- Buffer overflow write check
// To do
//		- Analysis of amount used for band
//		- alloc() for various data type
//-------------------------------------------------------------------
// example)
//
//	1. Thread-NonSafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    CMemoryPoolFactory<Money1> * pool = new CMemoryPoolFactory<Money1>;
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
//	2. Thread-SafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    SafeMemoryPoolFactory<Money1> * pool = new SafeMemoryPoolFactory<Money1>; //!!!!!!!! different thing !!!!!!!
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
// error situation)
// If (m_pFreeBand of m_pFreeBand->pNext) is NULL in Free() function,
// =>possibility
// 1. Free()을 Alloc()호출한 횟수이상을 호출했을 때 
// 2. 또는 Free(node)의 node가 중복되게 Free되었을 때
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilityCommon.h"
#include "MemBand.h"
#include <stdio.h>


namespace util
{

	

template <class Type>
class CMemoryPoolFactory
{
public:
	CMemoryPoolFactory()
		:	m_pBandHead( NULL ),
			m_pFreeBand( NULL ),
			m_dwPoolExtendSize( 0 )
	{
	}
	virtual ~CMemoryPoolFactory()
	{
		Release();
	}
	BOOL Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
	{
		if( 0 == dwPoolExtendSize )
			m_dwPoolExtendSize = dwPoolBasicSize/2+1;
		else
			m_dwPoolExtendSize = dwPoolExtendSize;

		if( NULL == m_pBandHead )
		{
			CMemTypeBand<Type>::AllocBand( m_pBandHead, dwPoolBasicSize );
			m_pFreeBand = m_pBandHead;
			if( !m_pFreeBand ) 
				return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	void Release()
	{
		if( m_pBandHead )
		{
			CMemTypeBand<Type>::FreeBand( m_pBandHead );
			//m_pBandHead->FreeBand();
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}
	}
	
	inline Type * Alloc()
	{
		Type * pn = m_pFreeBand->AlloObject();
		if(pn == NULL)
		{
			if( !m_pFreeBand->pPrev )
			{
				CMemTypeBand<Type>::AllocBand( m_pBandHead, m_dwPoolExtendSize );
				m_pFreeBand = m_pBandHead;
				pn = m_pFreeBand->AlloObject();
			}
			else
			{
				m_pFreeBand = m_pFreeBand->pPrev;
				pn = m_pFreeBand->AlloObject();
			}
		}
		ASSERT( !IsBadReadPtr( pn, sizeof(Type) ) );
		ASSERT( !IsBadWritePtr( pn, sizeof(Type) ) );

		return pn;
	}
	inline void Free(Type * pNode)
	{
		ASSERT( !IsBadReadPtr( pNode, sizeof(Type) ) );
		ASSERT( !IsBadWritePtr( pNode, sizeof(Type) ) );

		if(!m_pFreeBand->FreeObject(pNode))
		{
			if( !m_pFreeBand->pNext )
			{
				//printf( "반환을 많이 했다.\n" );
				return;
			}

			m_pFreeBand = m_pFreeBand->pNext;


			if(!m_pFreeBand->FreeObject(pNode))	//< 여기서 뻑 날경우 ( m_pFreeBand == NULL ), 개수 이상의 pNode를 반환하여 생김
			{
			}
		}
	}

	inline DWORD GetPoolBasicSize()
	{
		return m_pBandHead->GetMaxNumberOfObjects();
	}

	inline DWORD GetPoolExtendSize()
	{
		return m_dwPoolExtendSize;
	}

	inline DWORD GetNumberOfBands()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> *	pHead = m_pBandHead;
		while( pHead )
		{
			++dwTotalNum;
			pHead = pHead->pNext;
		}
		return dwTotalNum;
	}

	inline DWORD GetAvailableNumberOfTypes()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> * pHead = m_pBandHead;
		while(pHead)
		{
			dwTotalNum += pHead->GetAvailableNumberOfObjects();
			pHead = pHead->pNext;
		}
		return dwTotalNum;
	}
protected:

	CMemTypeBand<Type> *	m_pBandHead;
	CMemTypeBand<Type> *	m_pFreeBand;
	DWORD					m_dwPoolExtendSize;
};



} /// namespace util



#endif //__MEMORYPOOLFACTORY_H__