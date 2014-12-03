#ifndef _LOG_H
#define _LOG_H
//////////////////////////////////////////////////////////////////////////
//
//
//  ÃÏ¡˙OL log œµÕ≥
//////////////////////////////////////////////////////////////////////////
#include <fstream>

using namespace std;

class Log 
{
public:
	Log();
	Log(char* szFilePath);
	~Log();
public:
	void		Write(char* logline);
	void        IniLog(char* szFilePath);

protected:
	void		GenerateFile();
private:	
	ofstream	m_stream;
	char		m_szFilePath[256];
};
#endif

