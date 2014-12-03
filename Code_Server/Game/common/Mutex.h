#ifndef _DB_MUTEX_H
#define _DB_MUTEX_H

#include <windows.h>

//线程锁基类
class _CCriticalSectionImp
{
public:
    virtual ~_CCriticalSectionImp(){};

//确保此类只能作为基类使用
protected:
	_CCriticalSectionImp(){} ;

protected:
    CRITICAL_SECTION m_csAccess;
};


//半自动线程锁 (需要锁定先需要派生自 CSemiAutoIntLock, 然后使用 CSALocker 锁定m_csAccess;)
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


//自动线程锁 (需要锁定时只需生成一个实例即可在它的生命周期内对数据提供线程安全)
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


//手动线程锁 (需要锁定/解锁时需要手动调用Lock和Unlock)
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