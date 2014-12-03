#include "UtilityCommon.h"
#include "ObjKeyGenerator.h"
//#include <vector>
#include <algorithm>
//#include <atlcomcli.h>

namespace util {

	CObjKeyGenerator::CObjKeyGenerator( DWORD dwKeyStart, DWORD dwKeyEnd )
	{
		Create(dwKeyStart, dwKeyEnd);

	}
	VOID CObjKeyGenerator::Create( DWORD dwKeyStart, DWORD dwKeyEnd )
	{
		for( DWORD i = dwKeyStart; i <= dwKeyEnd; ++i )
		{
			m_dequeKey.push_back( i );
		}
	}

	CObjKeyGenerator::~CObjKeyGenerator(VOID)
	{
		m_dequeKey.clear();
	}

	DWORD CObjKeyGenerator::GetKey()
	{
		DWORD key;

		if (IsEmpty() == TRUE) { 
			return NULL;
		}

		m_cs.Lock();

		key = *m_dequeKey.begin();
		m_dequeKey.pop_front();

		m_cs.Unlock();

		return key;
	}

	BOOL CObjKeyGenerator::ReserveKey(DWORD key)
	{
		BOOL bReserved = FALSE;

		if (IsEmpty() == TRUE) { 
			return FALSE;
		}

		m_cs.Lock();

		deque<DWORD>::iterator itr = m_dequeKey.begin();
		while (itr != m_dequeKey.end())
		{
			if ( *itr == key )
			{
				bReserved = TRUE;
				m_dequeKey.erase(itr);
				break;
			}
		}

		return bReserved;
	}


	BOOL CObjKeyGenerator::IsEmpty()
	{
		return m_dequeKey.empty() == TRUE; 
	}	

	BOOL CObjKeyGenerator::IsExistKey(DWORD dwKey)
	{
		deque<DWORD>::iterator itr = m_dequeKey.begin();

		for (; itr != m_dequeKey.end(); itr++)
		{
			if (*itr == dwKey)
			{
				return TRUE;
			}
		}
		
		return FALSE;
	}

	VOID CObjKeyGenerator::RestoreKey(DWORD key)
	{		 
		CSyncCriticalSection sync_cs(m_cs);
		m_dequeKey.push_back(key);
	}

	size_t CObjKeyGenerator::GetSize()
	{
		CSyncCriticalSection sync_cs(m_cs);
		return m_dequeKey.size();
	}

} // End of Namespace Util

