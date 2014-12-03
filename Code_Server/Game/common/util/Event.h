

#ifndef __EVENT_H__
#define __EVENT_H__

#pragma once

#include <windows.h>
#include "tstring.h"

namespace util{

class CEvent{ // 이벤트 관련 기능을 제공하는 클래스
private:
	HANDLE m_hEvent; // 이벤트 객체

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
