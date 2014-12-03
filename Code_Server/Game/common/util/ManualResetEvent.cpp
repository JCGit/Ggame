#include "UtilityCommon.h"	
#include "ManualResetEvent.h"

namespace util{

CManualResetEvent::CManualResetEvent(BOOL initialState) : CEvent(0, TRUE, initialState){
}

CManualResetEvent::CManualResetEvent(const _tstring& name, BOOL initialState) : CEvent(0, TRUE, initialState, name){
}
   
}//End of Namespace util