/**
 �̺�Ʈ ���� ����� �����ϴ� Ŭ������ ������
*/

#include "UtilityCommon.h"
#include "Event.h"

namespace util
{

static HANDLE Create(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName); // ������ Ÿ�� ����(���� �Լ��� ��ü ������� ��� ������)

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState)
{
	m_hEvent = Create(lpEventAttributes, bManualReset, bInitialState, NULL);
}

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const _tstring& name)
{
	m_hEvent = Create(lpEventAttributes, bManualReset, bInitialState, name.c_str());
}

void CEvent::Wait() const
{ // �̺�Ʈ�� ��ٸ�
	if(!Wait(INFINITE)) {
	}
}

BOOL CEvent::Wait(DWORD timeoutMillis) const
{ // �̺�Ʈ�� ������ �ð���ŭ ��ٸ�
   BOOL ok;

   DWORD result = ::WaitForSingleObject(m_hEvent, timeoutMillis);

   if(result == WAIT_TIMEOUT) ok = false;
   else if(result == WAIT_OBJECT_0) ok = true;
   else {
	  return false;
   }
   
   return ok;
}

void CEvent::Reset(){ // �̺�Ʈ�� ��Ȱ��ȭ ��
	if(!::ResetEvent(m_hEvent)){
		return;
	};
}

void CEvent::Set()
{ // �̺�Ʈ�� Ȱ��ȭ ��
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
