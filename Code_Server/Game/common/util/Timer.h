#ifndef _TIME_H_
#define _TIME_H_

#include <Windows.h>
#include <time.h>
#include <WINNT.H>
#include <stdio.h>
//#include <winbase.h>

////接口，需要用到定时器必须实现该接口中的RUN方法
//class ITimer{
//public:    
//		   virtual void Run();
//};

class CTimer  
{
public: 
	CTimer();
	~CTimer();
	

public:
	static unsigned int __stdcall	RunPtr(PVOID pvoid);							//线程回调
	void							Run();											//运行时
	bool							StartThread();									//开启线程
	inline void						EndThread(){ m_bEnd = true; };					//结束线程
	inline void						SetActive(bool bActive){ m_bActive = bActive; };//激活状态
	inline bool						IsAcitve(){return m_bActive;};					//是否激活
	virtual							void Running() =0;	
public:
	int delay;
protected:
	
	HANDLE m_hThreadHandle;										//线程句柄
	unsigned int m_ThreadID;									//线程ID
	bool m_bActive;												//是否激活
	volatile bool m_bEnd;										//运行结束

};

#endif


