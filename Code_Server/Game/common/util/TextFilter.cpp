#include "../ProgramCommon/Define.h"
#include "UtilityCommon.h"
#include "CommonDef.h"
#include "SolarFileReader.h"
#include ".\TextFilter.h"


#define	ISHANGUL(ch)	((ch) & 0x80)

//Todo:유니코드 체크로 다국어 지원 할수 있게!!!
bool  ITextFilter::CheckIncludeString(char *szSrc)
{

	for (int n = 0; n < (signed)strlen(szSrc); n++)
	{
		switch (szSrc[n])
		{
		case ' ' ://공백 
		case '\t' ://탭은 안된다.
			return true;
		}

		if(szSrc[n]& 0x80)
		{

			unsigned char	cBuf1=		szSrc[n];
			unsigned char	cBuf2=		szSrc[n+1];
			unsigned short	wBuf=		(unsigned short)cBuf1<<8 | cBuf2;

			if(wBuf >= 0xB0A1  &&  wBuf <= 0xC8FE)	//한글 	
			{
				;
			}
			else if(wBuf >= 0xCAA1  &&  wBuf <= 0xFDFE) //한자 
			{
				return true;
			}
			else if(wBuf >= 0xA1A1  &&  wBuf <= 0xACFE) //2바이트 특수 문자
			{
				return true;
			}
			n++;
		}
		else if(isdigit(szSrc[n]))//숫자면 
		{
			;
		}
		else if(isalpha(szSrc[n]))//영문이면
		{
			;
		}
		else //기타 
		{
			return true;
		}
	}

	return false;
}

//*****************************************************************************
//	문자열에서 white space와 tab 을 지우고 문자만 남긴다.
//
//	Paremeters
//		char *szDest	: trim 시킨 내용을 담을 포인터
//		char *szSrc		: trim 시킬 문자열
//	Return Values
//		void
//	Remark
//		삭제 대상 : ' ' '\t' '\n' '\"'
//*****************************************************************************
void	ITextFilter::Trim(char *szDest, const char *szSrc)
{  
	int i;
	for (int n = 0, i = 0; n < (signed)strlen(szSrc); n++)
	{
		switch (szSrc[n])
		{
		case ' ' :
		case '\t' :
		case '\n' :
		//case '\"' :
			break;

		default :
			szDest[i] = szSrc[n];
			i++;
			break;
		}
	}
	szDest[i] = '\0';
}


//*****************************************************************************
//	문자열에서 특정 길이만큼만 잘라낸 스트링을 만든다.
//
//	Paremeters
//		char *szDest	: 잘라낸 스트링을 담을 포인터
//		char *szSrc		: 잘라낼 문자열
//		int nLength		: 잘라낼 길이
//	Return Values
//		void
//	Remark
//		strncpy + 널문자 넣기 의 기능.
//		만일 szSrc의 길이가 nLength보다 작으면 szSrc의 길이만큼 복사한다.
//*****************************************************************************
void	ITextFilter::GetStringPart(char *szDest, const char *szSrc, int nLength)
{
	if (strlen(szSrc) <= nLength)
		strcpy(szDest, szSrc);
	else
	{
		memcpy(szDest, szSrc, nLength);
		szDest[nLength] = '\0';
	}
}

//*****************************************************************************
//	한 문자열에서 특정 문자열의 위치를 찾아낸다.
//
//	Paremeters
//		char *szStr		: 검색할 문자열 (ex : ABCDEFG)
//		char *szCharSet	: 찾는 대상		(ex : EF)
//	Return Values
//		char*			: szChatSet의 위치 포인터. 없으면 NULL.
//	Remark
//		strstr과 기능과 사용법이 같으나 한글 find의 문제점을 해결
//*****************************************************************************
char*	ITextFilter::FindString(char *szStr, const char *szCharSet)
{
	if (strlen(szStr) < strlen(szCharSet))
		return NULL;

	for (int n = 0; n < strlen(szStr) - strlen(szCharSet) + 1;)
	{
		char *l_szStr = (char*) malloc(strlen(szCharSet) + 1);
		GetStringPart(l_szStr, szStr + n, strlen(szCharSet));
		
		if ( CSTR_EQUAL == CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, l_szStr,_tcslen(l_szStr), szCharSet, _tcslen(szCharSet)) )
		{
			free(l_szStr);
			return (szStr + n);
		}
		free(l_szStr);
		if (ISHANGUL(szStr[n]))
			n += 2;
		else
			n++;
	}
	return NULL;
}


bool ITextFilter::CheckString(char *szSrc)
{
	//일치하는단어를 포함하고 있나 검사 
	return false;
}




bool ITextFilter::IsHaveReplace(char *szSrc,bool binclude) 
{
/*	if(binclude)
	{
		if(CheckIncludeString(szSrc))
		{
			return true;
		}	
	}*/

	return GetReplaceTarget(NULL, szSrc)!=NULL;	
}


//*****************************************************************************
//	문자열의 모든 치환 대상을 치환시킨다.
//
//	Paremeters
//		char *szDest	: 치환이 끝난 문자열을 담을 포인터
//		char *szSrc		: 치환시킬 문자열
//		int nMaxLength	: 치환시킨 후의 내용을 담을 최대 길이
//	Return Values
//		bool			: 하나라도 치환을 했는지 여부
//	Remark
//		치환후 길이가 nMaxLength 보다 길어지면 ''로 치환시킨다.
//		ReplaceWord에서 szDest에 들어갈 문자열이 szSrc의 문자열보다 10배 이상
//		길면 에러가 나게 된다. 주의!! (예 : A를 ABCDEFGHIJK로 치환시키면 에러!)
//		무한 치환이 되지 않도록 주의할 것! (예 : A를 AA로 치환하면 무한 치환한다!)
//*****************************************************************************
bool	ITextFilter::Replace(char *szDest, char *szSrc, int nMaxLength)
{
	if (!IsHaveReplace(szSrc))
	{
		GetStringPart(szDest, szSrc, nMaxLength);
		return false;
	}

	char *l_szStr = (char*) malloc(strlen(szSrc) + 1);
	strcpy(l_szStr, szSrc);

	while (IsHaveReplace(l_szStr))
	{
		char *l_szTargetOffset = NULL, *l_szTargetString = NULL, *l_szReplaceString = NULL;
		int l_nTargetLength = 0;

		//	바꿔야 할 스트링을 l_szTargetString에 저장.
		l_szTargetOffset = GetReplaceTarget(&l_nTargetLength, l_szStr);
		l_szTargetString = (char*) malloc(l_nTargetLength + 1);
		GetStringPart(l_szTargetString, l_szTargetOffset, l_nTargetLength);

		//	바뀐 뒤의 스트링을 l_szReplaceString에 저장.
		l_szReplaceString = (char*) malloc(l_nTargetLength * 10 + 1);
		ReplaceWord(l_szReplaceString, l_szTargetString, l_nTargetLength * 10);

		//	치환했을 경우 nMaxLength를 넘어가지 않는지 검사.
		if (strlen(l_szStr) + strlen(l_szReplaceString) - strlen(l_szTargetString) > nMaxLength)
			strcpy(l_szReplaceString, "");

		//	치환
		strcpy(l_szTargetOffset + strlen(l_szReplaceString), l_szTargetOffset + strlen(l_szTargetString));
		memcpy(l_szTargetOffset, l_szReplaceString, strlen(l_szReplaceString));

		free(l_szTargetString);
		free(l_szReplaceString);
	}
	GetStringPart(szDest, l_szStr, nMaxLength);
	free (l_szStr);
	return true;
}

//*****************************************************************************
//	데이터를 초기화한다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************
void	CNSlangTextFilter::Clear()
{
	m_nSlangNumber = 0;
	m_szaSlangTarget = NULL;
	m_szaSlangReplace = NULL;
}

//*****************************************************************************
//	욕 DATA 파일을 불러온다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************
HRESULT	CNSlangTextFilter::Load(const char *szFName)
{
	FILE *pFile = fopen(szFName, "r");
	if (pFile == NULL)
		return false;

	char l_szStr[1024];
	m_nSlangNumber = 0;
	while (fgets(l_szStr, 1023, pFile) != NULL)
	{
		Trim(l_szStr, l_szStr);
		if (l_szStr[0] != '/'&&l_szStr[1] != '/' && strlen(l_szStr) != 0)
			m_nSlangNumber++;
	}

	m_szaSlangTarget = new char*[m_nSlangNumber];
	m_szaSlangReplace = new char*[m_nSlangNumber];
	for (int n = 0; n < m_nSlangNumber; n++)
	{
		m_szaSlangTarget[n] = new char[SLANG_LENGTH_MAX];
		m_szaSlangReplace[n] = new char[SLANG_LENGTH_MAX];
	}

	fseek(pFile, 0, 0);
	int n = 0;
	while (fgets(l_szStr, 1023, pFile) != NULL)
	{
		Trim(l_szStr, l_szStr);
		if (l_szStr[0] != '/' && l_szStr[1] != '/' &&strlen(l_szStr) != 0)
		{
			if (strstr(l_szStr, ":") == NULL)
			{
				strcpy(m_szaSlangTarget[n], l_szStr);
				strcpy(m_szaSlangReplace[n], "");
			}
			else
			{
				GetStringPart(m_szaSlangTarget[n], l_szStr, strstr(l_szStr, ":") - l_szStr);
				strcpy(m_szaSlangReplace[n], strstr(l_szStr, ":") + 1);
			}
			n++;
		}
	}
	fclose(pFile);
	return true;
}

//*****************************************************************************
//	욕 DATA 파일을 팩파일에서 불러온다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************

HRESULT	CNSlangTextFilter::LoadFromPack(const char *szFName)
{
	util::SolarFileReader sr;
	if( !sr.OpenFileFromPack( "Data\\Resource.wpk", (char *)szFName, SEPERATOR_ONLY_NEWLINE ) )
		return FALSE;

	m_nSlangNumber		= 0;

	char pToken[SLANG_LENGTH_MAX];

	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType(); 
		if ( TOKEN_END == ettType )
			break;

		strncpy( pToken, sr.GetTokenString(), SLANG_LENGTH_MAX );
		++m_nSlangNumber;
	}

	m_szaSlangTarget	= new char*[m_nSlangNumber];
	m_szaSlangReplace	= new char*[m_nSlangNumber];
	for (int n = 0; n < m_nSlangNumber; n++)
	{
		m_szaSlangTarget[n]		= new char[SLANG_LENGTH_MAX];
		m_szaSlangReplace[n]	= new char[SLANG_LENGTH_MAX];
	}

	sr.Reset();

	for(int n = 0; n < m_nSlangNumber; ++n )
	{
		sr.GetNextTokenType( TOKEN_STRING );
		strncpy( pToken, sr.GetTokenString(), SLANG_LENGTH_MAX );

		strcpy( m_szaSlangTarget[n], pToken );
	}

	sr.CloseFile();

	return TRUE;
}

//*****************************************************************************
//	LOAD에서 부른 data들을 파괴시킨다.
//
//	Paremeters
//		void
//	Return Values
//		void
//	Remark
//*****************************************************************************
HRESULT	CNSlangTextFilter::Destroy()
{
	if (!IsInit())
		return false;

	m_xInit = false;
	for (int n = 0; n < m_nSlangNumber; n++)
	{
		SAFE_DELETE(m_szaSlangTarget[n]);
		SAFE_DELETE(m_szaSlangReplace[n]);
	}
	SAFE_DELETE_ARRAY(m_szaSlangTarget);
	SAFE_DELETE_ARRAY(m_szaSlangReplace);
	return true;
}

//*****************************************************************************
//	소스에서 치환할 대상을 찾아낸다.
//
//	Paremeters
//		int *nLength		: 치환 대상의 길이를 받을 포인터
//		const char *szSrc	: 치환 대상을 찾을 문자열
//	Return Values
//		char*				: 치환 대상의 위치
//	Remark
//		치환 대상이 없으면 nLength에는 0이 들어가고 NULL이 반환된다.
//*****************************************************************************
char*	CNSlangTextFilter::GetReplaceTarget(int *nLength, char *szSrc)
{
	if (!IsInit())
		return NULL;

	char *l_szStr = (char*) malloc(strlen(szSrc) + 1);
	Trim(l_szStr, szSrc);
	for (int n = 0; n < m_nSlangNumber; n++)
	{
		if (FindString(l_szStr, m_szaSlangTarget[n]))
		{
			int l_nTargetPos = FindString(l_szStr, m_szaSlangTarget[n]) - l_szStr;
			int l_nOffsetSpaceNumber = 0, l_nTargetSpaceNumber = 0;
			int i;int l_nOffset;
			for (int i = 0, l_nOffset = 0; ; i++)
			{
				if (szSrc[i] == ' ' || szSrc[i] == '\t' || szSrc[i] == '\n')
					l_nOffsetSpaceNumber++;
				else
				{
					if (l_nOffset >= l_nTargetPos)
						break;
					l_nOffset++;
				}
			}

			for (l_nOffset = 0; l_nOffset < strlen(m_szaSlangTarget[n]); i++)
			{
				if (szSrc[i] == ' ' || szSrc[i] == '\t' || szSrc[i] == '\n')
					l_nTargetSpaceNumber++;
				else
					l_nOffset++;
			}
			free(l_szStr);
			if (nLength != NULL)
				*nLength = strlen(m_szaSlangTarget[n]) + l_nTargetSpaceNumber;
			return (szSrc + l_nTargetPos + l_nOffsetSpaceNumber);
		}
	}
	free(l_szStr);
	return NULL;
}

//*****************************************************************************
//	특정 문자열을 무엇으로 치환해야 하는지 찾아낸다.
//
//	Paremeters
//		char *szDest		: 치환한 뒤의 문자열을 저장할 포인터
//		const char *szSrc	: 치환할 문자열
//		int nMaxLength		: szDest 버퍼의 최대 크기
//	Return Values
//		void				:
//	Remark
//		치환한 문자열이 없으면 ""이 들어간다.
//*****************************************************************************
void	CNSlangTextFilter::ReplaceWord(char *szDest, const char *szSrc, int nMaxLength)
{
	if (!IsInit())
		return;

	char *l_szStr = (char*) malloc(strlen(szSrc) + 1);
	Trim(l_szStr, szSrc);

	for (int n = 0; n < m_nSlangNumber; n++)
	{
		if (strcmp(l_szStr, m_szaSlangTarget[n]) == 0)
		{
			GetStringPart(szDest, m_szaSlangReplace[n], nMaxLength);
			free(l_szStr);
			return;
		}
	}
	strcpy(szDest, "");
	free(l_szStr);
}
