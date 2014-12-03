//-----------------------------------------------------------------------------
// FileName   : common.h
// Module	  :	ZoneServer
// Function	  : common
// source code created by peng
// 2011-12-17
//-----------------------------------------------------------------------------

#ifndef COMMON_H 
#define COMMON_H

// 将一些通用的函数放在这里
#define ZERO_CONST   (0.0001f)  

int memlen(const char *str);

struct FormularParam
{
	///@ 公式类型
	enum FormularType
	{
		Formular_BaseIncrease     = 0 ,  // iParam1+（skillLvl-1）*iParam2
		Formular_PerBaseIncrease  = 1 ,  // iParam1%+（skillLvl-1）*iParam2%
		Formular_PowerIncrease    = 2 ,  // (skillLvl+iParam1)*(skillLvl+iParam2)
        Formular_Multi            = 3 ,  // skillLvl*ChaLvl*iParam1*iParam2
		Formular_BasePower        = 4 ,  // iParam1+ power(iParam2 , skillLvl)
		Formular_paraPower        = 5 ,  // iParam1* math.pow(iParam2,level)
	};

	FormularType        formularType;
	float               iParam1;
	float               iParam2;
	int   FormularCalculate( int level = 0 , int baseValue = 0 );
};

inline bool EqualZero(float fValue)
{
	if( fValue < 0 ) fValue = (-fValue) ;
	return ( fValue <  ZERO_CONST );
}


#endif