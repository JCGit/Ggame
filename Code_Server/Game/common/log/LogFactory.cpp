#include "LogFactory.h"
#include "../../Utility/UtilityCommon.h"

LogFactory::LogFactory()
{
	for (int i = 0;i<LOG_MAX_TYPE ;i++)
	{
		 m_pLogPool[i] =NULL;
	}
}
LogFactory::~LogFactory()
{
	for (int i = 0;i<LOG_MAX_TYPE ;i++)
	{
		SAFE_DELETE( m_pLogPool[i] );
	}

}
bool LogFactory::AllocLog(char * szServerName)
{
	if (!szServerName)
	{
		return false ;
	}

	std::string szPatch1(szServerName);
	std::string szPatch2(szServerName);
	std::string szPatch3(szServerName);
	std::string szPatch4(szServerName);
	std::string szPatch5(szServerName);
	std::string szPatch6(szServerName);
	std::string szPatch7(szServerName);
	std::string szPatch8(szServerName);

	m_pLogPool[LOG_ACTOR_TYPE] = new Log;
	if (m_pLogPool[LOG_ACTOR_TYPE])
	{
		szPatch1+="Actor\\";
		m_pLogPool[LOG_ACTOR_TYPE]->IniLog((char *)szPatch1.c_str());
	}
	m_pLogPool[LOG_ITEM_TYPE] = new Log;
	if (m_pLogPool[LOG_ITEM_TYPE])
	{
		szPatch2+="Item\\";
		m_pLogPool[LOG_ITEM_TYPE]->IniLog((char *)szPatch2.c_str());
	}

	m_pLogPool[LOG_SKILL_TYPE] = new Log;
	if (m_pLogPool[LOG_SKILL_TYPE])
	{
		szPatch3+="Skill\\";
		m_pLogPool[LOG_SKILL_TYPE]->IniLog((char *)szPatch3.c_str());
	}
	m_pLogPool[LOG_EXP_TYPE] = new Log;
	if (m_pLogPool[LOG_EXP_TYPE])
	{
		szPatch4+="Exp\\";
		m_pLogPool[LOG_EXP_TYPE]->IniLog((char *)szPatch4.c_str());
	}
	m_pLogPool[LOG_SESSINE_TYPE] = new Log;
	if (m_pLogPool[LOG_SESSINE_TYPE])
	{
		szPatch5+="Sessine\\";
		m_pLogPool[LOG_SESSINE_TYPE]->IniLog((char *)szPatch5.c_str());
	}

	m_pLogPool[LOG_STRENGTH] = new Log;
	if (m_pLogPool[LOG_STRENGTH])
	{
		szPatch6+="Strength\\";
		m_pLogPool[LOG_STRENGTH]->IniLog((char *)szPatch6.c_str());
	}

	m_pLogPool[LOG_MONEY_JINBO] = new Log;
	if (m_pLogPool[LOG_MONEY_JINBO])
	{
		szPatch7+="Money\\";
		m_pLogPool[LOG_MONEY_JINBO]->IniLog((char *)szPatch7.c_str());
	}

	m_pLogPool[LOG_YUANBO] = new Log;
	if (m_pLogPool[LOG_YUANBO])
	{
		szPatch8+="YuanBo\\";
		m_pLogPool[LOG_YUANBO]->IniLog((char *)szPatch8.c_str());
	}

// 	if (pItemLog)
// 	{
// 		szPatch+="\\Item";
// 		pItemLog->IniLog((char *)szPatch.c_str());
// 	}
// 	if (pItemLog)
// 	{
// 		szPatch+="\\Item";
// 		pItemLog->IniLog((char *)szPatch.c_str());
// 	}

	return true;
}
void LogFactory::FreeLog( Log * plog )
{
	//pLogPool->Free(plog);
}
void LogFactory::Release()
{
  
}