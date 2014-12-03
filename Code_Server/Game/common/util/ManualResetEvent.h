
#ifndef __MANUAL_RESET_EVENT_H__
#define __MANUAL_RESET_EVENT_H__

#pragma once

#include "Event.h"

namespace util{

class CManualResetEvent : public CEvent{
public:
	explicit CManualResetEvent(BOOL initialState = false);
    explicit CManualResetEvent(const _tstring& name, BOOL initialState = false);

private:
   CManualResetEvent(const CManualResetEvent& rhs);
   CManualResetEvent &operator=(const CManualResetEvent& rhs);
};

}//End of Namespace util

#endif