

#ifndef __EVENT_H__
#define __EVENT_H__

#pragma once

#include <windows.h>
#include "tstring.h"

namespace util{

class CEvent{ // �̺�Ʈ ���� ����� �����ϴ� Ŭ����
private:
	HANDLE m_hEvent; // �̺�Ʈ ��ü

public:
	CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState);
	CEvent(LPSECURITY_ATTRIBUTES lpSecurityAttributes, BOOL manualReset, BOOL initialState, const _tstring& name);
	CEvent(const CEvent& rhs);
    CEvent& operator=(const CEvent& rhs);
	virtual ~CEvent(void);
	
    HANDLE GetEvent() const;
	void Wait() const;
    BOOL Wait(DWORD timeoutMillis) const;
    void Reset();
    void Set();
    void Pulse();
};

}//End of namespace : util

#endif
