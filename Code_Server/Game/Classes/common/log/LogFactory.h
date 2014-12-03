#ifndef _LOGFACTORY_H
#define _LOGFACTORY_H
//////////////////////////////////////////////////////////////////////////
//
//
//  ÃÏ¡˙OL logFactory œµÕ≥
//////////////////////////////////////////////////////////////////////////
//#include <fstream>
#include "log.h"
//#include "../../Utility/MemoryPoolFactory.h"
#include "../../common/def/MsgDef.h"
#include "../../Utility/Singleton.h"
using namespace util;

class Log;


class LogFactory : public Singleton<LogFactory>
{
public:
	LogFactory();
	~LogFactory();

	bool								        AllocLog(char * szServerName);
	VOID										FreeLog( Log * plog );
	VOID                                        Release();
	Log *                                       GetLogSys(eLogSysType elog)
	{
		if (elog >=LOG_ACTOR_TYPE && elog< LOG_MAX_TYPE )
		{
			return m_pLogPool[elog];
		}

		return NULL;
	}

private:
	Log		*m_pLogPool[LOG_MAX_TYPE];
};

#define sgLogFactory LogFactory::Instance()
#endif // __SERVER_SESSION_FACTORY_H__
