#include "../ProgramCommon/Define.h"
#include "UtilityCommon.h"
#include "CommonDef.h"
#include "SolarFileReader.h"
#include ".\TextFilter.h"


#define	ISHANGUL(ch)	((ch) & 0x80)

//Todo:�����ڵ� üũ�� �ٱ��� ���� �Ҽ� �ְ�!!!
bool  ITextFilter::CheckIncludeString(char *szSrc)
{

	for (int n = 0; n < (signed)strlen(szSrc); n++)
	{
		switch (szSrc[n])
		{
		case ' ' ://���� 
		case '\t' ://���� �ȵȴ�.
			return true;
		}

		if(szSrc[n]& 0x80)
		{

			unsigned char	cBuf1=		szSrc[n];
			unsigned char	cBuf2=		szSrc[n+1];
			unsigned short	wBuf=		(unsigned short)cBuf1<<8 | cBuf2;

			if(wBuf >= 0xB0A1  &&  wBuf <= 0xC8FE)	//�ѱ� 	
			{
				;
			}
			else if(wBuf >= 0xCAA1  &&  wBuf <= 0xFDFE) //���� 
			{
				return true;
			}
			else if(wBuf >= 0xA1A1  &&  wBuf <= 0xACFE) //2����Ʈ Ư�� ����
			{
				return true;
			}
			n++;
		}
		else if(isdigit(szSrc[n]))//���ڸ� 
		{
			;
		}
		else if(isalpha(szSrc[n]))//�����̸�
		{
			;
		}
		else //��Ÿ 
		{
			return true;
		}
	}

	return false;
}

//*****************************************************************************
//	���ڿ����� white space�� tab �� ����� ���ڸ� �����.
//
//	Paremeters
//		char *szDest	: trim ��Ų ������ ���� ������
//		char *szSrc		: trim ��ų ���ڿ�
//	Return Values
//		void
//	Remark
//		���� ��� : ' ' '\t' '\n' '\"'
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
//	���ڿ����� Ư�� ���̸�ŭ�� �߶� ��Ʈ���� �����.
//
//	Paremeters
//		char *szDest	: �߶� ��Ʈ���� ���� ������
//		char *szSrc		: �߶� ���ڿ�
//		int nLength		: �߶� ����
//	Return Values
//		void
//	Remark
//		strncpy + �ι��� �ֱ� �� ���.
//		���� szSrc�� ���̰� nLength���� ������ szSrc�� ���̸�ŭ �����Ѵ�.
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
//	�� ���ڿ����� Ư�� ���ڿ��� ��ġ�� ã�Ƴ���.
//
//	Paremeters
//		char *szStr		: �˻��� ���ڿ� (ex : ABCDEFG)
//		char *szCharSet	: ã�� ���		(ex : EF)
//	Return Values
//		char*			: szChatSet�� ��ġ ������. ������ NULL.
//	Remark
//		strstr�� ��ɰ� ������ ������ �ѱ� find�� �������� �ذ�
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
	//��ġ�ϴ´ܾ �����ϰ� �ֳ� �˻� 
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
//	���ڿ��� ��� ġȯ ����� ġȯ��Ų��.
//
//	Paremeters
//		char *szDest	: ġȯ�� ���� ���ڿ��� ���� ������
//		char *szSrc		: ġȯ��ų ���ڿ�
//		int nMaxLength	: ġȯ��Ų ���� ������ ���� �ִ� ����
//	Return Values
//		bool			: �ϳ��� ġȯ�� �ߴ��� ����
//	Remark
//		ġȯ�� ���̰� nMaxLength ���� ������� ''�� ġȯ��Ų��.
//		ReplaceWord���� szDest�� �� ���ڿ��� szSrc�� ���ڿ����� 10�� �̻�
//		��� ������ ���� �ȴ�. ����!! (�� : A�� ABCDEFGHIJK�� ġȯ��Ű�� ����!)
//		���� ġȯ�� ���� �ʵ��� ������ ��! (�� : A�� AA�� ġȯ�ϸ� ���� ġȯ�Ѵ�!)
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

		//	�ٲ�� �� ��Ʈ���� l_szTargetString�� ����.
		l_szTargetOffset = GetReplaceTarget(&l_nTargetLength, l_szStr);
		l_szTargetString = (char*) malloc(l_nTargetLength + 1);
		GetStringPart(l_szTargetString, l_szTargetOffset, l_nTargetLength);

		//	�ٲ� ���� ��Ʈ���� l_szReplaceString�� ����.
		l_szReplaceString = (char*) malloc(l_nTargetLength * 10 + 1);
		ReplaceWord(l_szReplaceString, l_szTargetString, l_nTargetLength * 10);

		//	ġȯ���� ��� nMaxLength�� �Ѿ�� �ʴ��� �˻�.
		if (strlen(l_szStr) + strlen(l_szReplaceString) - strlen(l_szTargetString) > nMaxLength)
			strcpy(l_szReplaceString, "");

		//	ġȯ
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
//	�����͸� �ʱ�ȭ�Ѵ�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
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
//	�� DATA ������ �ҷ��´�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
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
//	�� DATA ������ �����Ͽ��� �ҷ��´�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
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
//	LOAD���� �θ� data���� �ı���Ų��.
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
//	�ҽ����� ġȯ�� ����� ã�Ƴ���.
//
//	Paremeters
//		int *nLength		: ġȯ ����� ���̸� ���� ������
//		const char *szSrc	: ġȯ ����� ã�� ���ڿ�
//	Return Values
//		char*				: ġȯ ����� ��ġ
//	Remark
//		ġȯ ����� ������ nLength���� 0�� ���� NULL�� ��ȯ�ȴ�.
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
//	Ư�� ���ڿ��� �������� ġȯ�ؾ� �ϴ��� ã�Ƴ���.
//
//	Paremeters
//		char *szDest		: ġȯ�� ���� ���ڿ��� ������ ������
//		const char *szSrc	: ġȯ�� ���ڿ�
//		int nMaxLength		: szDest ������ �ִ� ũ��
//	Return Values
//		void				:
//	Remark
//		ġȯ�� ���ڿ��� ������ ""�� ����.
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
