#ifndef _UTIL_H
#define _UTIL_H
//#include <iostream> ����
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
//#define __max(a,b)  (((a) > (b)) ? (a) : (b)) maik.m  ֱ����ϵͳ��
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
	//��Ҫcocos2d����߶�
	static void	GetPixelPoint(int tx, int ty , Point* pt,int iMapHeight = 0);
    static void	GetCellPoint(int px,int py , Point* pt,int iMapHeight = 0);
	static Util::DIRECTION GetDirection(const Point* pt , int  dir = 1);
	static bool		GetRand(int value);
	static int		Rand();//�漴����
	static int		GetRandomNum(int startValue , int  endValue);//��ȡ�漴����
	// static const int GAME_MAP_CELL_WIDTH; //= 64;//��ͼ��Ԫ���
	// static const int GAME_MAP_CELL_HEIGHT; // = 32*Math.tan(22.5*Math.PI/180)*2;//��ͼ��Ԫ���

	//��ʼ������
	static void		InitMsgLen();

	static void		Split(char str[],std::vector<char*>& arr,char* split);
	static int		StringToInt(const string& str);//�ַ���ת��������
public:
	static char *	GetCurTimeString();
	static void 	CurvertStringToTime(char szTime[20],COleDateTime &time);
public:
	//��ӡinfo ��дlog ֻ��debug����ʾ
	static void		PrintfString(char szStr[]);
	static void		PrintfString(const char *_Format,...);
	//����info ���ش������ д��log
	static void		PrintfError(const char *_Format,...);
	//����info ���ش������ д��log
	static void		PrintfWarn(const char *_Format,...);
	//��ȷinfo д��log
	static void		PrintfOk(const char *_Format,...);
	static void		PrintfEnter();
	//��־�ļ�
	static void		InitLog(Log *pLog);
	//д��log�ļ�
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
    
	//UTF-8 תΪ GB2312
	static int UTF8ToGB2312(char *out,const char* str);

	///@ ��õ�ǰ�������������
	static void GetDirCell(int32 nDir, int32 curCellX , int32 curCellY , int32& nX, int32& nY);


	// ���ѡ����Ԫ��
	static void MultiRandomSelect(vector<int32>* const srcVec , vector<int32>* const notSelectVec   ,vector<int32>* resultVec , int32 selectCount );

	//����iMin--iMax֮ǰiCount���������ͨ��iArray����
	static bool Random(int32 iMin, int32 iMax, int32 iCount, int32* iArray);
	/**
	 * //�����߼�����õ�ֱ������
	 * @param logic ���������
	 * @param row ��ǰ��ͼ�ж�����
	 * @return 
	 * 
	 */		
   static Point Util::getDirectPoint( Point logic, int row);

   static int32 Get_GmCmd_Level(int32 iCmdId);

};

//extern int	g_nMsgLen[20][2000];




#endif