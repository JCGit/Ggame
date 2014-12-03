//-----------------------------------------------------------------------------
// FileName   : common.cpp
// Module	  :	ZoneServer
// Function	  : common
// source code created by peng
// 2011-12-17
//-----------------------------------------------------------------------------
#include <math.h>
#include "common.h"


//-- 计算字符串的长度
int memlen(const char *str)
{
	if(!str)  return 0;

	const char *eos = str;

	while(*eos++);

	return((int)(eos - str));
}


///@ 根据参数和等级，基础值，公式计算
int FormularParam::FormularCalculate( int level , int baseValue )
{
	int retValue = 0 ;
	switch( formularType )
	{
	case Formular_BaseIncrease:
		retValue = int(iParam1 + ( level - 1 ) * iParam2);
		break;
	case Formular_PerBaseIncrease:
		retValue = int( ( iParam1 + ( level -1 ) * iParam2 ) * 0.01 * baseValue );
		break;
	case Formular_PowerIncrease:
		retValue = int(( level + iParam1 ) * ( level + iParam2 ));
		break;
	case Formular_Multi:
		retValue = int( level * baseValue * iParam1 * iParam2 ) ;
		break;
	case Formular_BasePower:
		retValue = int( iParam1 + pow( iParam2 , float(level))  ) ;
		break;
	case Formular_paraPower:
		retValue = int( iParam1* pow(iParam2,level)  ) ;
		break;
	default:
		break;
	}

	return retValue ;
}