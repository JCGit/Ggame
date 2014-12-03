// Singleton.h: interface for the Singleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <assert.h>
#include <windows.h>


// 单步式模式设计
// 
// 　功能 
//　　保证一个类仅有一个实例。
///同类型的对象实例只可能有一个
// 
// 
// 
// maik.m
// 



namespace util
{
template <typename T>
class Singleton
{
public:
	~Singleton()
	{
		
	}

	static T * Instance()
	{
		if (0 == ms_pInstance)
		{
			ms_pInstance = new T;
		}

		return ms_pInstance;
	}

	static VOID DestroyInstance()
	{
		if (ms_pInstance) {		
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

private:

	static T *		ms_pInstance;
};


template< class T > T * Singleton<T>::ms_pInstance = 0;
}



#endif // __SINGLETON_H__
