#ifndef __OBJ_KEY_GENERATOR_H__
#define __OBJ_KEY_GENERATOR_H__

#pragma once

#include <deque>
#include "CriticalSection.h"

using namespace std;

namespace util {

	/**
	Object∂‘œÛ Key_Id 
	*/
	class CObjKeyGenerator
	{
	private:
		CCriticalSection		m_cs;
		deque<DWORD>			m_dequeKey;
		DWORD 					m_dwKeyStart;
		DWORD					m_dwKeyEnd;

	public:
		CObjKeyGenerator(){}
		CObjKeyGenerator(DWORD dwKeyStart, DWORD dwKeyEnd);
		~CObjKeyGenerator(VOID);

		VOID					Create( DWORD dwKeyStart, DWORD dwKeyEnd );
		DWORD					GetKey();
		BOOL					ReserveKey(DWORD key);
		VOID					RestoreKey(DWORD key);
		BOOL					IsEmpty();

		BOOL					IsExistKey(DWORD dwKey);

		size_t					GetSize();

	};

}// End of Namespace Util

#endif //__OBJ_KEY_GENERATOR_H__
