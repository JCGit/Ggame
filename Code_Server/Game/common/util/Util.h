#ifndef _UTIL_H
#define _UTIL_H
//#include <iostream> 　　
//#include <string> 

#include "../def/define.h"
#include <math.h>
#include <vector>
#include "../log/log.h"
#include "../log/LogFactory.h"
#include <ATLComTime.h>


// const int GAME_MAP_CELL_WIDTH = 64;
// const double GAME_MAP_CELL_HEIGHT = 32*(::tan(22.5*PAI/180))*2;
const int GAME_MAP_CELL_WIDTH = 32;

const int GAME_MAP_CELL_HEIGHT = 32;
//#define __max(a,b)  (((a) > (b)) ? (a) : (b)) maik.m  直接用系统的
//#define __min(a,b)  (((a) < (b)) ? (a) : (b))

class Point{
public:
	Point()
	{
		x = 0 ;
		y = 0 ;
	}
	Point(int i, int j)
	{
		x = i ;
		y = j ;
	}
	int x;
	int y;
	static float Distance(const Point* a , const Point* b)
	{
		int power = (a->x - b->x) * (a->x - b->x);
		power += (a->y - b->y) * (a->y - b->y);
		return sqrt(float(power));
	}
	static float Distance(int ax , int ay ,  int bx , int by)
	{
		int power = (ax - bx) * (ax - bx);
		power += (ay - by) * (ay - by);
		return sqrt(float(power));
	}
};

std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);

class Util
{

public:
	Util();
	~Util();
	enum DIRECTION
	{
		DIR_NULL        = 0 ,
		DIR_DOWN		= 1 ,
		DIR_LEFT_DOWN	= 2 ,
		DIR_LEFT		= 3 ,
		DIR_LEFT_UP		= 4 ,
		DIR_UP			= 5 ,
		DIR_RIGHT_UP	= 6 ,
		DIR_RIGHT		= 7 ,
		DIR_RIGHT_DOWN  = 8 ,
		DIR_MUM         = 9 ,
	}; 
public:
	//需要cocos2d坐标高度
	static void	GetPixelPoint(int tx, int ty , Point* pt,int iMapHeight = 0);
    static void	GetCellPoint(int px,int py , Point* pt,int iMapHeight = 0);
	static Util::DIRECTION GetDirection(const Point* pt , int  dir = 1);
	static bool		GetRand(int value);
	static int		Rand();//随即函数
	static int		GetRandomNum(int startValue , int  endValue);//获取随即函数
	// static const int GAME_MAP_CELL_WIDTH; //= 64;//地图单元格宽
	// static const int GAME_MAP_CELL_HEIGHT; // = 32*Math.tan(22.5*Math.PI/180)*2;//地图单元格高

	//初始化长度
	static void		InitMsgLen();

	static void		Split(char str[],std::vector<char*>& arr,char* split);
	static int		StringToInt(const string& str);//字符串转化成数字
public:
	static char *	GetCurTimeString();
	static void 	CurvertStringToTime(char szTime[20],COleDateTime &time);
public:
	//打印info 不写log 只有debug下显示
	static void		PrintfString(char szStr[]);
	static void		PrintfString(const char *_Format,...);
	//错误info 严重错误调用 写入log
	static void		PrintfError(const char *_Format,...);
	//警告info 严重错误调用 写入log
	static void		PrintfWarn(const char *_Format,...);
	//正确info 写入log
	static void		PrintfOk(const char *_Format,...);
	static void		PrintfEnter();
	//日志文件
	static void		InitLog(Log *pLog);
	//写入log文件
	static void		ToInfoLog(eLogSysType eLog,const char *_Format,...);

	static void		ScriptPrintfError(const char str[]);

	static void STRING_TOLOWER(std::string& str)
	{
		for(size_t i = 0; i < str.length(); ++i)
			str[i] = (char)tolower(str[i]);
	};

	static void STRING_TOUPPER(std::string& str)
	{
		for(size_t i = 0; i < str.length(); ++i)
			str[i] = (char)toupper(str[i]);
	};
	
	static int EMClampI(int val, int low, int high)
	{
		return __max(__min(val, high), low);
	}

	static float EMClampF(float val, float low, float high)
	{
		return __max(__min(val, high), low);
	}
    
	//UTF-8 转为 GB2312
	static int UTF8ToGB2312(char *out,const char* str);

	///@ 获得当前朝向的相邻网络
	static void GetDirCell(int32 nDir, int32 curCellX , int32 curCellY , int32& nX, int32& nY);


	// 随机选择多个元素
	static void MultiRandomSelect(vector<int32>* const srcVec , vector<int32>* const notSelectVec   ,vector<int32>* resultVec , int32 selectCount );

	//产生iMin--iMax之前iCount个随机数，通过iArray返回
	static bool Random(int32 iMin, int32 iMax, int32 iCount, int32* iArray);
	/**
	 * //根据逻辑坐标得到直角坐标
	 * @param logic 网格坐标点
	 * @param row 当前地图有多少行
	 * @return 
	 * 
	 */		
   static Point Util::getDirectPoint( Point logic, int row);

   static int32 Get_GmCmd_Level(int32 iCmdId);

};

//extern int	g_nMsgLen[20][2000];




#endif