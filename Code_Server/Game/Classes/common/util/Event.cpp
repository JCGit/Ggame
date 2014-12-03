/**
 이벤트 관련 기능을 제공하는 클래스를 정의함
*/

#include "UtilityCommon.h"
#include "Event.h"

namespace util
{

static HANDLE Create(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName); // 프로토 타입 선언(정적 함수로 객체 선언없이 사용 가능함)

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState)
{
	m_hEvent = Create(lpEventAttributes, bManualReset, bInitialState, NULL);
}

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const _tstring& name)
{
	m_hEvent = Create(lpEventAttributes, bManualReset, bInitialState, name.c_str());
}

void CEvent::Wait() const
{ // 이벤트를 기다림
	if(!Wait(INFINITE)) {
	}
}

BOOL CEvent::Wait(DWORD timeoutMillis) const
{ // 이벤트를 지정된 시간만큼 기다림
   BOOL ok;

   DWORD result = ::WaitForSingleObject(m_hEvent, timeoutMillis);

   if(result == WAIT_TIMEOUT) ok = false;
   else if(result == WAIT_OBJECT_0) ok = true;
   else {
	  return false;
   }
   
   return ok;
}

void CEvent::Reset(){ // 이벤트를 비활성화 함
	if(!::ResetEvent(m_hEvent)){
		return;
	};
}

void CEvent::Set()
{ // 이벤트를 활성화 함
	if(!::SetEvent(m_hEvent)){
		return;
	}
}

void CEvent::Pulse()
{
	if(!::PulseEvent(m_hEvent)){
		return;
	};
}

CEvent::~CEvent(void)
{
}

HANDLE CEvent::GetEvent() const
{
	return m_hEvent;
}

static HANDLE Create(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName)
{
   HANDLE hEvent = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);

   if(hEvent == NULL){
	  return NULL;
   }

   return hEvent;
}

}//End of namespace : util
