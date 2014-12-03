


#include <windows.h>
#include <string.h> 
#include <stdio.h>
#include<stdlib.h>
#include "Util.h"
#include "../Mutex.h"
#include "../../common/def/GMMsgDef.h"

//#include "../def/MsgDef.h"
//#include "../def/BattleMsgDef.h"
//#include "../def/BuffMsgDef.h"
//#include "../def/BusinessMsgDef.h"
//#include "../def/EquipMsgDef.h"
//#include "../def/FamilyMsgDef.h"
//#include "../def/FriendSysMsgDef.h"
//#include "../def/ItemMsgDef.h"
//#include "../def/MissionMsgDef.h"
//#include "../def/NpcMsgDef.h"
//#include "../def/PetMsgDef.h"
//#include "../def/SkillMsgDef.h"
//#include "../def/ChatMsgDef.h"
//#include "../def/MonMsgDef.h"

//输出句柄
HANDLE stdout_handle, stderr_handle;
//多线程锁
CIntLock	g_IntLock;

//输出时间
char		g_szTime[50]={0,};
//输出缓存
char		g_szBuff[1024]={0,};
//int			g_nMsgLen[20][2000] = {0,};
//日志实例父类
Log*		g_pLog=NULL;


Util::	Util()
{
	
}

Util::~Util()
{

}
void Util::InitLog(Log *pLog)
{
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	g_pLog = pLog;

}
void Util::InitMsgLen()
{
}

// void Util::GetPixelPoint(int tx, int ty , Point* pt)
// {
//     int tileWidth  = GAME_MAP_CELL_WIDTH ;
// 	int tileHeight = GAME_MAP_CELL_HEIGHT;
// 
// 	//偶数行tile中心
// 	int tileCenter= (tx * tileWidth) + tileWidth/2;
// 	// x象素  如果为奇数行加半个宽
// 	int xPixel = tileCenter + (ty&1) * tileWidth/2;
// 	// y象素
// 	int yPixel = (ty + 1) * tileHeight/2;
// 
// 	pt->x = xPixel;
// 	pt->y = yPixel;
// }
void Util::GetPixelPoint(int tx, int ty , Point* pt ,int iMapHeight)
{
	int tileWidth  = GAME_MAP_CELL_WIDTH ;
	int tileHeight = GAME_MAP_CELL_HEIGHT;
	if (iMapHeight == 0)
	{
		iMapHeight = 94;
	}

	int mapSize = iMapHeight ;//temp
	
	int x =( tx * tileWidth  + tileWidth * 0.5);
	int y =( mapSize * tileHeight - ty * tileHeight - tileHeight * 0.5);


	pt->x = x;
	pt->y = y;

}
//根据屏幕象素坐标取得网格的坐标
void  Util::GetCellPoint(int px,int py , Point* pt,int iMapHeight)
{
	int tileWidth  = GAME_MAP_CELL_WIDTH ;
	int tileHeight = GAME_MAP_CELL_HEIGHT;
	if (iMapHeight == 0)
	{
		iMapHeight = 94;
	}

	int mapSize = iMapHeight ;//temp


	int x = px / tileWidth;
	int y = ((mapSize * tileHeight) - py) / tileHeight;


	pt->x = x;
 	pt->y = y;
}
//根据屏幕象素坐标取得网格的坐标
// void  Util::GetCellPoint(int px,int py , Point* pt)
// {
// 
// 	int tileWidth  = GAME_MAP_CELL_WIDTH ;
// 	int tileHeight = GAME_MAP_CELL_HEIGHT;
// 
// 	int xtile = 0;	//网格的x坐标
// 	int ytile = 0;	//网格的y坐标
// 
// 	int cx, cy, rx, ry;
// 	cx = (int)((px / tileWidth) * tileWidth + tileWidth/2);	//计算出当前X所在的以tileWidth为宽的矩形的中心的X坐标
// 	cy = (int)((py / tileHeight) * tileHeight + tileHeight/2);//计算出当前Y所在的以tileHeight为高的矩形的中心的Y坐标
// 
// 	rx = (px - cx) * tileHeight/2;
// 	ry = (py - cy) * tileWidth/2;
// 
// 	if (::abs(rx)+::abs(ry) <= tileWidth * tileHeight/4)
// 	{
// 		//xtile = int(pixelPoint.x / tileWidth) * 2;
// 		xtile = (int)(px / tileWidth);
// 		ytile = (int)((py / tileHeight) * 2);
// 	}
// 	else
// 	{
// 		px = px - tileWidth/2;
// 		//xtile = int(pixelPoint.x / tileWidth) * 2 + 1;
// 		xtile =( int)(px / tileWidth) + 1;
// 
// 		py = py - tileHeight/2;
// 		ytile = (int)((py / tileHeight) * 2) + 1;
// 	}
// 
// 	pt->x = xtile - (ytile&1);
// 	pt->y = ytile;
// }

//随机
bool Util::GetRand(int value){
	int a = rand()%WEIGHT;
	if(a<=value){
		return true;
	}
	return false;

}


int Util::Rand()
{
	return rand()%WEIGHT;
}
//获取随即函数范围
int Util::GetRandomNum(int startValue ,int  endValue)
{	
	if (endValue==0)
	{
		return 0;
	} 
	else
	{
		return startValue+rand() % endValue;
	}

}

char *Util::GetCurTimeString()
{
	memset(g_szTime,0,sizeof(g_szTime));
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	sprintf(g_szTime,"[%4d-%02d-%02d %02d:%02d:%02d] ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond/*,sys.wMilliseconds*/);

	return g_szTime;
}

void Util::CurvertStringToTime(char szTime[20],COleDateTime &time)
{
	int nyear = 0;
	int nmonth = 0;
	int ndate = 0;
	int nhour = 0;
	int nminutes = 0;
	int nsecond = 0;
	try
	{
		sscanf(szTime, "%04d-%02d-%02d %02d:%d02:%02d", &nyear, &nmonth, &ndate,&nhour,&nminutes,&nsecond);
	}
	catch(...){}
	
	time = COleDateTime(nyear,nmonth,ndate,nhour,nminutes,nsecond);
}


void Util::PrintfString(char szStr[])
{
#ifdef _DEBUG
	if (strlen(szStr) == 0)	return;
	g_IntLock.Lock();	

	printf(GetCurTimeString());
	printf(szStr);
	PrintfEnter();

	g_IntLock.Unlock();
#endif // _DEBUG
	
}
void Util::PrintfError(const char *_Format,...)
{
	g_IntLock.Lock();	


	SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);

	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

	va_list arg_ptr;
	va_start(arg_ptr, _Format);
	_vsnprintf(p, sizeof(g_szBuff)-nLen-1, _Format, arg_ptr);

	printf(g_szBuff);
	PrintfEnter();

	SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#if! def _DEBUG

	if (g_pLog != NULL)
	{
		g_pLog->Write(g_szBuff);
	}
#endif // _DEBUG


	g_IntLock.Unlock();	
}

void Util::ScriptPrintfError(const char str[])
{
	g_IntLock.Lock();	


	SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);

	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

    strncpy( p , str  , strlen(str) );
	printf(g_szBuff);
	PrintfEnter();

	SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#if! def _DEBUG

	if (g_pLog != NULL)
	{
		g_pLog->Write(g_szBuff);
	}
#endif // _DEBUG


	g_IntLock.Unlock();	
}

void Util::PrintfOk(const char *_Format,...)
{
	g_IntLock.Lock();	

    SetConsoleTextAttribute(stdout_handle, /* FOREGROUND_BLUE|*/FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

	va_list arg_ptr;
	va_start(arg_ptr, _Format);
	_vsnprintf(p, sizeof(g_szBuff)-nLen-1, _Format, arg_ptr);

	printf(g_szBuff);
	PrintfEnter();
	SetConsoleTextAttribute(stdout_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#if!def _DEBUG
	if (g_pLog != NULL)
	{
		g_pLog->Write(g_szBuff);
	}
#endif // _DEBUG

	g_IntLock.Unlock();	
}
void Util::PrintfWarn(const char *_Format,...)
{
	g_IntLock.Lock();	

    SetConsoleTextAttribute(stdout_handle,  FOREGROUND_RED|FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

	va_list arg_ptr;
	va_start(arg_ptr, _Format);
	_vsnprintf(p, sizeof(g_szBuff)-nLen-1, _Format, arg_ptr);

	printf(g_szBuff);
	PrintfEnter();
	SetConsoleTextAttribute(stdout_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	//if (g_pLog != NULL)
	//{
	//	g_pLog->Write(g_szBuff);
	//}

	g_IntLock.Unlock();	
}
void Util::PrintfString(const char *_Format,...)
{
#ifdef _DEBUG
	g_IntLock.Lock();	

	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

	va_list arg_ptr;
	va_start(arg_ptr, _Format);
	_vsnprintf(p, sizeof(g_szBuff)-nLen-1, _Format, arg_ptr);

	printf(g_szBuff);
	PrintfEnter();

	//if (g_pLog != NULL)
	//{
	//	g_pLog->Write(g_szBuff);
	//}

	
	g_IntLock.Unlock();	
#endif // _DEBUG
}
void Util::ToInfoLog(eLogSysType eLog,const char *_Format,...)
{
	g_IntLock.Lock();	

	SetConsoleTextAttribute(stdout_handle, FOREGROUND_GREEN| FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	ZeroMemory(g_szBuff,sizeof(g_szBuff));
	//时间
	char *p = g_szBuff;
	strcpy(p,GetCurTimeString());
	int nLen = strlen(p);
	p += nLen;

	va_list arg_ptr;
	va_start(arg_ptr, _Format);
	_vsnprintf(p, sizeof(g_szBuff)-nLen-1, _Format, arg_ptr);

#ifdef _DEBUG
	printf(g_szBuff);
	PrintfEnter();
#endif // _DEBUG

#if !def _DEBUG
	Log *pLog = sgLogFactory->GetLogSys(eLog);
	if (pLog)
	{
		pLog->Write(g_szBuff);
	}
#endif // _DEBUG
	SetConsoleTextAttribute(stdout_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	g_IntLock.Unlock();	

}

void Util::PrintfEnter()
{
	printf("\r\n");
}
//拆分成数组
void	Util::Split(char str[],std::vector<char*>& arr,char* split)
{
	char * p; 
	p = strtok (str,split); 
	while(p!=NULL) 
	{ 
		arr.push_back(p);
		p = strtok(NULL,split); 
	}
}
//字符串转化成数字
int Util::StringToInt(const string& str)
{
	/*std::istringstream iss(str);
	int num;
	iss >> num;
	return num; */   
	return 0;
}

Util::DIRECTION Util::GetDirection(const Point* pt , int dir )
{
	float angle = atan2(float(pt->x),float(pt->y))*180/PAI;
	Util::DIRECTION direction;
	if( pt->x== 0 && pt->y == 0 )
	{
		if(dir == 0)
			direction = Util::DIR_DOWN;
		else
			direction = (Util::DIRECTION)dir;
	}
	else
	{
		angle = angle*(-1);
		if( angle>-22.5 && angle<=22.5 )
		{
			direction = Util::DIR_DOWN ;
		}
		else if( angle>22.5 && angle<=67.5 )
		{
			direction = Util::DIR_LEFT_DOWN ;
		}
		else if( angle>67.5 && angle<=112.5 )
		{
			direction = Util::DIR_LEFT ;
		}
		else if( angle>112.5 && angle<=157.5 )
		{
			direction = Util::DIR_LEFT_UP ;
		}
		else if(( angle>=157.5 && angle<=180 ) || ( angle>=-180 && angle<-157.5 ))
		{
			direction = Util::DIR_UP ;
		}
		else if( angle>-157.5 && angle<=-112.5 )
		{
			direction = Util::DIR_RIGHT_UP ;
		}
		else if( angle>-112.5 && angle<=-67.5 )
		{
			direction = Util::DIR_RIGHT ;
		}
		else if( angle>-67.5 && angle<=-22.5 )
		{
			direction = Util::DIR_RIGHT_DOWN ;
		}
	}
	return direction ;
}

int Util::UTF8ToGB2312(char *out,const char* str)
{
	WCHAR *strSrc;
	TCHAR *szRes;
	int len;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	len = (i+1)*sizeof(CHAR);
	memcpy(out,szRes,len);
	out[len+1] ='\0';

	delete []strSrc;
	delete []szRes;

	return len;
}

//GetDirCell

void Util::GetDirCell(int32 nDir, int32 curCellX , int32 curCellY , int32& nX, int32& nY)
{
	int32 offset = (curCellY % 2 == 1) ? 1 : 0 ;
	switch (nDir)
	{
	case Util::DIR_UP:
		nX = curCellX;
		nY = curCellY - 2;
		break;
	case Util::DIR_RIGHT_UP:
		nX = curCellX + offset;
		nY = curCellY - 1;
		break;
	case Util::DIR_RIGHT:
		nX = curCellX + 1;
		nY = curCellY;
		break;
	case Util::DIR_RIGHT_DOWN:
		nX = curCellX + offset;
		nY = curCellY + 1;
		break;
	case Util::DIR_DOWN:
		nX = curCellX;
		nY = curCellY + 2;
		break;
	case Util::DIR_LEFT_DOWN:
		nX = curCellX - 1 + offset;
		nY = curCellY + 1;
		break;
	case Util::DIR_LEFT:
		nX = curCellX - 1;
		nY = curCellY;
		break;
	case Util::DIR_LEFT_UP:
		nX = curCellX - 1 + offset;
		nY = curCellY - 1;
		break;
	}
}

void Util::MultiRandomSelect(vector<int32>* const srcVec , vector<int32>* const notSelectVec   ,vector<int32>* resultVec , int32 selectCount )
{
   if( NULL == srcVec || NULL == resultVec || selectCount <= 0 || srcVec->size() <= 0 ) return ;

   vector<int32> tempSrcVec;

   tempSrcVec.assign( srcVec->begin() , srcVec->end() ) ;

   // 剔除不需要的元素
   if( NULL != notSelectVec && notSelectVec->size() > 0 )
   {
       vector<int32>::iterator  itr = tempSrcVec.begin() ;
	   for ( ; itr != tempSrcVec.end() ; )
	   {
		   bool bRmov = false  ;
		   for (int i = 0 ; i < notSelectVec->size() ; i++ )
		   {
			   if( (*itr) == (*notSelectVec)[i] )
			   {
				   itr = tempSrcVec.erase( itr );
				   bRmov = true ;
				   break;
			   }
		   }

		   if( !bRmov )
			   itr++ ;
	   }
   }

   // 在剩下的元素里面随机选择
   while( selectCount <  tempSrcVec.size() )
   {
	   int32 rmvIndex = GetRandomNum(0 , tempSrcVec.size() );
	   tempSrcVec.erase( tempSrcVec.begin() + rmvIndex );
   }

   resultVec->assign( tempSrcVec.begin() , tempSrcVec.end() );

}

bool Util::Random(int32 iMin, int32 iMax,const int32 iCount, int32* iArray)
{
	int32 iNum = iMax - iMin + 1;
	if (iNum <= 0 || iNum <= iCount) return false;

	int32 *iSrcArray = new int32[iNum];
	for(int32 i = 0; i < iNum; i++)
	{
		iSrcArray[i] = i + iMin;
	}

	int32 iRandNum = 0;

	int32 iEnd = iNum - 1;
	
	for (int i = 0; i < iCount; i++)
	{
		iRandNum = rand() % iNum;
		iArray[i] = iSrcArray[iRandNum];
		iSrcArray[iRandNum] = iSrcArray[iEnd];
		iNum--;
		iEnd--;
	}
	
	delete []iSrcArray;

	return true;
}

/**
 * //根据逻辑坐标得到直角坐标
 * @param logic 网格坐标点
 * @param row  当前地图有多少行
 * @return 
 * 
 */		
Point Util::getDirectPoint( Point logic, int row)
{
	Point dPoint  ;//直角坐标点

	float tempValue = .0f;
	if(logic.y & 1)
	{
		tempValue = ( logic.x - logic.y / 2.0f ) + 1 + (row-1)/2.0f ;
		dPoint.x = int32 ( floor( tempValue ) );
	}
	else
	{
		tempValue = ( logic.x - logic.y / 2.0f ) + ceil( float((row-1)/2.0f) ) ;
		dPoint.x = int32( tempValue );
	}
	tempValue = floor( float(( logic.y / 2.0f ) + logic.x + ( logic.y & 1 )) );
	dPoint.y = int32( tempValue ) ;return Point( dPoint.x , dPoint.y );
}




vector<string> StrSplit(const string &src, const string &sep)
{
	vector<string> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != string::npos) {
			if (s.length()) r.push_back(s);
			s = "";
		} else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}

int32 Util::Get_GmCmd_Level(int32 iCmdId)
{
	if (iCmdId <= MSG_GM_ERROR ||iCmdId >= MSG_GM_LELEL4)
	{
		return int32(MSG_GM_ERROR);
	}

	if (iCmdId > MSG_GM_ERROR && iCmdId < MSG_GM_LELEL1)
	{
		return int32(GM_LVL1);
	}
	else if (iCmdId > MSG_GM_LELEL1 && iCmdId < MSG_GM_LELEL2)
	{
		return int32(GM_LVL2);
	}
	else if (iCmdId > MSG_GM_LELEL2 && iCmdId < MSG_GM_LELEL3)
	{
		return int32(GM_LVL3);
	}
	else if (iCmdId > MSG_GM_LELEL3 && iCmdId < MSG_GM_LELEL4)
	{
		return int32(GM_LVL4);
	}

	return 0;
}

