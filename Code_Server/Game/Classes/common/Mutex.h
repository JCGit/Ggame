#ifndef _DB_MUTEX_H
#define _DB_MUTEX_H

#include <windows.h>

//�߳�������
class _CCriticalSectionImp
{
public:
    virtual ~_CCriticalSectionImp(){};

//ȷ������ֻ����Ϊ����ʹ��
protected:
	_CCriticalSectionImp(){} ;

protected:
    CRITICAL_SECTION m_csAccess;
};


//���Զ��߳��� (��Ҫ��������Ҫ������ CSemiAutoIntLock, Ȼ��ʹ�� CSALocker ����m_csAccess;)
class CSALocker
{
public:
    CSALocker(CRITICAL_SECTION& csAccess)
    {
        m_pcsAccess = &csAccess;
        EnterCriticalSection(m_pcsAccess);
    }
    virtual ~CSALocker()
    {
        LeaveCriticalSection(m_pcsAccess);
    }

private:
    LPCRITICAL_SECTION m_pcsAccess;

private:
    CSALocker(){}
    CSALocker(const CSALocker&){}
    CSALocker& operator=(const CSALocker&){return *this;}
};

class CSemiAutoIntLock : public _CCriticalSectionImp
{
public:
    CSemiAutoIntLock() {InitializeCriticalSection(&m_csAccess);}
    virtual ~CSemiAutoIntLock() {DeleteCriticalSection(&m_csAccess);}

protected:
    friend CSALocker;
};


//�Զ��߳��� (��Ҫ����ʱֻ������һ��ʵ���������������������ڶ������ṩ�̰߳�ȫ)
class CAutoIntLock : public _CCriticalSectionImp
{
public:
    CAutoIntLock()
    { 
        InitializeCriticalSection(&m_csAccess);
        EnterCriticalSection(&m_csAccess);
    }
    virtual ~CAutoIntLock()
    {
        LeaveCriticalSection(&m_csAccess);
        DeleteCriticalSection(&m_csAccess); 
    }
 
private:
    CAutoIntLock(const CAutoIntLock&)
    {
        InitializeCriticalSection(&m_csAccess);
        EnterCriticalSection(&m_csAccess);
    }
    CAutoIntLock operator = (const CAutoIntLock&) { return *this; }

};


//�ֶ��߳��� (��Ҫ����/����ʱ��Ҫ�ֶ�����Lock��Unlock)
class CIntLock : public _CCriticalSectionImp
{
public:
	CIntLock(){ InitializeCriticalSection(&m_csAccess); }
    virtual ~CIntLock(){ DeleteCriticalSection(&m_csAccess); }

public:
    void Lock(){ EnterCriticalSection(&m_csAccess); }
    void Unlock(){ LeaveCriticalSection(&m_csAccess); }

public:
    CIntLock(const CIntLock&){ InitializeCriticalSection(&m_csAccess); } 
    CIntLock operator = (const CIntLock&) { return *this; }
};

#endif