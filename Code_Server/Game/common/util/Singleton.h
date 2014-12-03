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


// ����ʽģʽ���
// 
// ������ 
//������֤һ�������һ��ʵ����
///ͬ���͵Ķ���ʵ��ֻ������һ��
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
