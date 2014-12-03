//-----------------------------------------------------------------------------
// 文件名 : SysTime.h
// 模块	  :	SERVER
// 功能	  : 系统时间管理
// 创建者 ：tao
//-----------------------------------------------------------------------------


#ifndef _SYSTIME_H
#define _SYSTIME_H

#include <time.h>
using namespace std;

struct STLocalTime
{
	char	sec;			///< seconds after minute (0-59)
	char	min;			///< Minutes after hour (0-59)
	char	hour;			///< Hours after midnight (0-23)
	char	month;			///< Month (0-11; 0=january)
	char	monthday;		///< Day of the month (1-31)
	char	weekday;		///< Day of the week (0-6, 6=sunday)
	int		year;			///< current year minus 1900
	int 	yearday;		///< Day of year (0-365)
	bool	isdst;			///< true if daylight savings time is active
};

class SysTime
{
public:
	SysTime(){}
	~SysTime(){}	

	static void GetLocalTime(STLocalTime &lt, time_t long_time = 0)
	{
		struct tm *systime;

		if (long_time == 0)
		{
			time(&long_time);
		}
		// Get time as long integer.
		systime = localtime( &long_time );	// Convert to local time.

		lt.sec      = systime->tm_sec;      
		lt.min      = systime->tm_min;      
		lt.hour     = systime->tm_hour;     
		lt.month    = systime->tm_mon + 1;    
		lt.monthday = systime->tm_mday; 
		lt.weekday  = systime->tm_wday;  
		lt.year     = systime->tm_year + 1900;     
		lt.yearday  = systime->tm_yday;  
		lt.isdst    = bool (systime->tm_isdst);    
	}

	static int GetTime()
	{
		time_t long_time;
		time(&long_time);
		return int(long_time);
	} 

	static bool IsInSameWeek(time_t tDayTime, time_t tAnothDayTime)
	{
		STLocalTime stDay;
		STLocalTime stAnothDay;
		GetLocalTime(stDay, tDayTime);
		GetLocalTime(stAnothDay, tAnothDayTime);

		//周日的0.0.0 与 周四的 23.59.59 之间相差4天 + 23.59.59，采用整除，最终结果还是4 
		if (tAnothDayTime > tDayTime)
		{
			if((tAnothDayTime - tDayTime) / tDayTime <= 6 - stDay.weekday)
				return true;
		}
		else
		{
			if((tDayTime - tAnothDayTime) / tDayTime <= stDay.weekday)
				return true;			
		}
		return false;
	}

	static bool IsInSameDay(time_t tDayTime, time_t tAnothDayTime)
	{
		STLocalTime stDay;
		STLocalTime stAnothDay;
		GetLocalTime(stDay, tDayTime);
		GetLocalTime(stAnothDay, tAnothDayTime);
		
		if (stDay.monthday == stAnothDay.monthday && 
			stDay.month == stAnothDay.month &&
			stDay.year == stAnothDay.year)
		{
			return true;
		}
		else
			return false;
	}

	static bool IsInSameDay(time_t tDayTime, time_t tAnothDayTime, int32 &iDays)
	{
		STLocalTime stDay;
		STLocalTime stAnothDay;
		GetLocalTime(stDay, tDayTime);
		GetLocalTime(stAnothDay, tAnothDayTime);

		if (stDay.year == stAnothDay.year)
		{
			iDays = stDay.yearday - stAnothDay.yearday;
		}

		if (stDay.monthday == stAnothDay.monthday && 
			stDay.month == stAnothDay.month &&
			stDay.year == stAnothDay.year)
		{
			return true;
		}
		else
			return false;
	}

	//得到本周
	static int32 GetCurWeekTime(int32 iCurTime,int32 iWeekDay, int32 iHout, int32 iMin)
	{
		STLocalTime stLocalTime;
		GetLocalTime(stLocalTime,iCurTime);
		//周日 0.0.0.0
		int32 iWeekBeginTime = iCurTime - (stLocalTime.weekday * 24 * 60 * 60 + 
			stLocalTime.hour * 60 * 60 + stLocalTime.min * 60 + stLocalTime.sec);
		
		int32 iWeekTime = iWeekBeginTime + iWeekDay * 24 * 60 * 60 + iHout * 60 * 60
			+ iMin * 60;

		return iWeekTime;
	}

	// 判断是否某周的两段时间之间
	static bool IsInWeekTime(int32 iBeginWeekDay, int32 iBeginHout, int32 iBeginMin, int32 iEndWeekDay, int32 iEndHout, int32 iEndMin )
	{
		if (iBeginHout >= 24 || iBeginHout < 0 || iEndHout >= 24 || iEndHout < 0 || iBeginMin >= 60 || iBeginMin < 0 || iEndMin >= 60 || iEndMin < 0)
		return false;

		int32 iCurTime = SysTime::GetTime();
		int32 iOPTime = GetCurWeekTime(iCurTime,iBeginWeekDay, iBeginHout, iBeginMin);
		int32 iClTime = GetCurWeekTime(iCurTime,iEndWeekDay, iEndHout, iEndMin);

		if (iCurTime < iOPTime || iCurTime > iClTime)
			return false;

		return true;
	}

	// 判断是否某日的两段时间之间
	static bool IsInDayTime(int32 iBeginHout, int32 iEndHout)
	{
		STLocalTime stCurTime;
		SysTime::GetLocalTime(stCurTime);
		if(stCurTime.hour > iEndHout  || stCurTime.hour < iBeginHout)
			return false;

		return true;
	}

	// 判断是否某日的两段时间之间
	static bool IsInDayTime(int32 iBeginHout, int32 iBeginMin, int32 iEndHout, int32 iEndMin)
	{
		STLocalTime stLocalTime;
		GetLocalTime(stLocalTime);
		
		if (iBeginHout >= 24 || iBeginHout < 0 || iEndHout >= 24 || iEndHout < 0 || iBeginMin >= 60 || iBeginMin < 0 || iEndMin >= 60 || iEndMin < 0)
			return false;

		return SysTime::IsInWeekTime(stLocalTime.weekday, iBeginHout, iBeginMin, stLocalTime.weekday, iEndHout, iEndMin);
	}

	// 得到今日某一时间的值
	static int32 GetTadayTime(int32 iHout, int32 iMin, int32 iSec)
	{
		int32 iTime = GetTime();

		STLocalTime stLocalTime;
		GetLocalTime(stLocalTime, iTime);
		
		int32 iSubSecond = stLocalTime.hour * 3600 + stLocalTime.min * 60 + stLocalTime.sec;
		int32 iAddSecond = iHout * 3600 + iMin * 60 + iSec;

		return iTime - iSubSecond + iAddSecond;
	}

	// 各道距离现在的时间差值 秒
	static int32 GetToNowLeftSec(int32 iHout, int32 iMin, int32 iSec)
	{
		int32 iTime = GetTime();

		STLocalTime stLocalTime;
		GetLocalTime(stLocalTime, iTime);

		int32 iSubSecond = stLocalTime.hour * 3600 + stLocalTime.min * 60 + stLocalTime.sec;
		int32 iAddSecond = iHout * 3600 + iMin * 60 + iSec;

		return iAddSecond - iSubSecond;
	}

	
	// 各道距离现在的时间差值 秒
	static int32 GetToWeekLeftSec(int32 iWeekDay, int32 iHout, int32 iMin)
	{
		int32 iTime = GetTime();

		int32 iWeekDayTime = GetCurWeekTime(iTime, iWeekDay, iHout, iMin);

		return iWeekDayTime > iTime ? iWeekDayTime - iTime : 0;
	}

};



#endif
