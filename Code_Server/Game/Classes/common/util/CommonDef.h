#pragma once

#define __DISABLE_COPY(className)					\
		private :									\
		className(const className&);				\
		void	operator = (const className&)

#define	__UNUSED(variable)		variable

#define __INFINITE_LOOP(name)	BOOL b##name##Loop = TRUE;	while (b##name##Loop)


#define INOUT
	
//=============================================================================================================================
// SafeDeleter
//=============================================================================================================================

#ifdef SAFE_DELETE
	#undef SAFE_DELETE
#endif	

#ifdef SAFE_DELETE_ARRAY
	#undef SAFE_DELETE_ARRAY
#endif

#define SAFE_DELETE( p)				{ if ( p) { delete ( p); ( p) = NULL; } }
#define SAFE_DELETE_ARRAY( p)		{ if ( p) { delete[] ( p); ( p) = NULL; } }
#define SAFE_RELEASE( p)			{ if ( p) { ( p)->Release(); ( p) = NULL; } }
#define	SAFE_RELEASENDELETE(p)				if (p) { (p)->Release(); delete (p); (p) = NULL; }

class SafeDeleter
{
public :
	template <class T>
	void	operator () (T*& ptr) const
	{
		SAFE_DELETE( ptr );
	}
};

template <class T> 
struct Param2Class
{
	typedef T ClassType;
};


//=============================================================================================================================
// PROPERTY
//=============================================================================================================================
#define __PROPERTY( DataType, name )											\
		public :																\
			void		Set##name( DataType value )		{ m_##name = value; }	\
			DataType	Get##name() const				{ return m_##name; }	\
		protected :																\
			DataType	m_##name;

#define __BOOL_PROPERTY( name )													\
		public :																\
			void	Set##name( bool bValue = true )		{ m_##name = bValue; }	\
			bool	Is##name() const					{ return m_##name; }	\
		protected :																\
			bool	m_##name;


#define __PTR_PROPERTY( DataType, name )										\
		public :																\
			void		Set##name( DataType* value )	{ m_p##name = value; }	\
			const DataType*	Get##name() const			{ return m_p##name; }	\
			DataType*	Get##name()						{ return m_p##name; }	\
		protected :																\
			DataType*	m_p##name;

#define __ARRAY_PROPERTY( DataType, num, name )										\
		public :																	\
			void		Set##name( WORD idx, DataType value )	{ m_p##name[idx] = value; }	\
			DataType	Get##name( WORD idx )			{ return m_p##name[idx]; }	\
			DataType*	Get##name##Ptr()			{ return m_p##name; }			\
		protected :																	\
			DataType	m_p##name[num];

#if defined( _UNICODE ) || defined( UNICODE )
/*
/////////////////////////////////////////////////////////////////////////////////////
// 유니코드용 함수들
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					wcscpy
	#define TSTRNCPY				wcsncpy
	#define TATOL					_wtol
	#define TSPRINTF				swprintf
	#define TSTRLEN					wcslen	
#else

#if defined( _MBCS )
/////////////////////////////////////////////////////////////////////////////////////
// MBCS
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					_mbscpy
	#define TSTRNCPY				_mbsncpy
	#define TATOL					atol
	#define TSPRINTF				sprintf
	#define TSTRLEN					_mbslen
#else
/////////////////////////////////////////////////////////////////////////////////////
// SBCS
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					strcpy
	#define TSTRNCPY				strncpy
	#define TATOL					atol
	#define TSPRINTF				sprintf
	#define TSTRLEN					strlen
#endif
*/
#endif

// peng
//#define _countof(array) (sizeof(array)/sizeof(array[0]))
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifdef _DEBUG

	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	inline void StartMemoryLeakCheck(void)		{ _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); }
	#define BreakPointOnMemoryLeak( iMemory)	_CrtSetBreakAlloc( iMemory)

	#include <assert.h>

	#define ASSERT(f)					assert(f)
	#define ASSERTMSG(f,m)				if(!(f))		\
										{				\
											printf(m);	\
										}

/*
	#define ASSERT(f)					__noop
	#define ASSERTMSG(f,m)				__noop
*/

	#define DISPMSG						printf
	#define DEBUG_CODE(c)				c

#else


	#define StartMemoryLeakCheck()		__noop
	#define BreakPointOnMemoryLeak		__noop
	#define ASSERT(f)					__noop
	#define ASSERTMSG(f,m)				__noop
	#define DISPMSG						printf
	#define DEBUG_CODE(c)				c

#endif