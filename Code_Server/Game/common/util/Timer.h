#ifndef _TIME_H_
#define _TIME_H_

#include <Windows.h>
#include <time.h>
#include <WINNT.H>
#include <stdio.h>
//#include <winbase.h>

////�ӿڣ���Ҫ�õ���ʱ������ʵ�ָýӿ��е�RUN����
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
	static unsigned int __stdcall	RunPtr(PVOID pvoid);							//�̻߳ص�
	void							Run();											//����ʱ
	bool							StartThread();									//�����߳�
	inline void						EndThread(){ m_bEnd = true; };					//�����߳�
	inline void						SetActive(bool bActive){ m_bActive = bActive; };//����״̬
	inline bool						IsAcitve(){return m_bActive;};					//�Ƿ񼤻�
	virtual							void Running() =0;	
public:
	int delay;
protected:
	
	HANDLE m_hThreadHandle;										//�߳̾��
	unsigned int m_ThreadID;									//�߳�ID
	bool m_bActive;												//�Ƿ񼤻�
	volatile bool m_bEnd;										//���н���

};

#endif


