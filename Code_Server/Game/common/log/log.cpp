#include "log.h"
#include <windows.h>
#include "direct.h"

#define	LOG_MAX_LEN		1024*1024

Log::Log()
{

}
Log::Log(char* szFilePath) 
{
	ZeroMemory(m_szFilePath,sizeof(m_szFilePath));
	strcpy(m_szFilePath,szFilePath);
	char szCmp[2] = {0},szPath[256] = {0};
	int i=1;
	for (;i<strlen(szFilePath);i++)
	{
		memcpy(szCmp,szFilePath+i,1);
		if (strcmp(szCmp,"\\") == 0)
		{
			memcpy(szPath,szFilePath,i);
			_mkdir(szPath);			
		}
	}
	if (i != strlen(szFilePath))
		_mkdir(m_szFilePath);
	GenerateFile();
}
void        Log::IniLog(char* szFilePath)
{
	ZeroMemory(m_szFilePath,sizeof(m_szFilePath));
	strcpy(m_szFilePath,szFilePath);
	char szCmp[2] = {0},szPath[256] = {0};
	int i=1;
	for (;i<strlen(szFilePath);i++)
	{
		memcpy(szCmp,szFilePath+i,1);
		if (strcmp(szCmp,"\\") == 0)
		{
			memcpy(szPath,szFilePath,i);
			_mkdir(szPath);			
		}
	}
	if (i != strlen(szFilePath))
		_mkdir(m_szFilePath);
	GenerateFile();

}
void Log::GenerateFile()
{
	static char szName[100]={0},szTime[32]={0};
	strcpy(szName,m_szFilePath);

	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	sprintf(szTime,"%4d%02d%02d%02d%02d%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond);
	strcpy(szName+strlen(m_szFilePath),szTime);
	strcpy(szName+strlen(m_szFilePath)+strlen(szTime),".log");

	m_stream.open(szName,ios::app);
}

void Log::Write(char* logline)
{
	m_stream << logline << endl;
	if (m_stream.tellp() > LOG_MAX_LEN)
	{
		m_stream.close();
		GenerateFile();
	}
}

Log::~Log()
{
	m_stream.close();
}
