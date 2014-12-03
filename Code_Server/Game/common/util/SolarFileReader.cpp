#include "UtilityCommon.h"
#include "wzFilePacker.h"
#include <windows.h>
#include <assert.h>
#include ".\solarfilereader.h"


namespace util
{

SolarFileReader::SolarFileReader()
	:	m_pszBuffer ( NULL ),
		m_dTokenNumber ( 0.0 ),
		m_dwBufferSize ( 0 ),
		m_dwBufferPos ( 0 ),
		m_Mode ( SEPERATOR_ERROR )
{
}

SolarFileReader::~SolarFileReader()
{
	assert( m_pszBuffer == NULL );
}
VOID SolarFileReader::_clear()
{
	m_dTokenNumber = 0.0;
	m_pszTokenString[0] = NULL;
}
VOID SolarFileReader::CloseFile()
{
	CloseBuffer();
}
VOID SolarFileReader::CloseBuffer()
{
	if ( m_pszBuffer )
	{
		delete [] m_pszBuffer;
		m_pszBuffer = NULL;
	}
	m_dwBufferSize	= 0;
	m_dwBufferPos	= 0;
}
VOID SolarFileReader::Reset()
{
	m_dwBufferPos	= 0;
	_clear();
}
BOOL SolarFileReader::OpenBuffer( BYTE * pBuffer, DWORD dwBufferSize, eSEPERATOR_MODE mode )
{
	assert( NULL == m_pszBuffer );
	m_dwBufferSize	= dwBufferSize;
	m_pszBuffer = new char [dwBufferSize];
	memcpy( m_pszBuffer, pBuffer, dwBufferSize );

	// 맴버 초기화
	m_Mode = mode;
	m_dwBufferPos = 0;

	return TRUE;
}

BOOL SolarFileReader::OpenFile( char * pszFileName, eSEPERATOR_MODE mode )
{
	assert( NULL == m_pszBuffer );

	m_Mode = mode;

	HANDLE hFile = ::CreateFile( pszFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if ( INVALID_HANDLE_VALUE == hFile )
	{
		return FALSE;
	}

	m_dwBufferSize = GetFileSize( hFile, NULL );
	if ( m_dwBufferSize < 0 )
		return FALSE;
	
	m_pszBuffer		= new char [m_dwBufferSize];
	m_dwBufferPos	= 0;

	DWORD dwNumber	= 0;
	BOOL rt = ::ReadFile( hFile, m_pszBuffer, m_dwBufferSize, &dwNumber, NULL );
	if( !rt ) return FALSE;

	::CloseHandle( hFile );

	return TRUE;
}

BOOL SolarFileReader::OpenFileFromPack( char * pszPackFileName, char * pszFileName, eSEPERATOR_MODE mode )
{
	assert( NULL == m_pszBuffer );

	m_Mode = mode;

	CPackFile obPackFile;

	if( !obPackFile.Open( pszPackFileName ) )
		return FALSE;

	HANDLE hFile = obPackFile.OpenFile( pszFileName, PACKFILE_OPEN_READONLY );
	if ( FALSE == hFile )
		return FALSE;

	if( !obPackFile.GetFileSize( hFile, &m_dwBufferSize ) )
		return FALSE;

	if ( m_dwBufferSize <= 0 )
		return FALSE;
	
	m_pszBuffer		= new char [m_dwBufferSize];
	m_dwBufferPos	= 0;

	if( !obPackFile.ReadFile( hFile, m_pszBuffer, m_dwBufferSize ) )
		return FALSE;

	obPackFile.CloseFile( hFile );
	obPackFile.Close();

	return TRUE;
}

char SolarFileReader::_getchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	if( *pPos < m_dwBufferSize )
		return m_pszBuffer[(*pPos)++];
	else
	{
		++(*pPos);
		return TOKEN_END;
	}
}
char SolarFileReader::_getcurchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	return (char)( *pPos < m_dwBufferSize ? m_pszBuffer[*pPos] : TOKEN_END );
}

VOID SolarFileReader::_rewind( int i, DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	assert( *pPos - i >= 0 );
	*pPos -= i;
}

int SolarFileReader::_isspace( char c )
{
	if( c == TOKEN_SPACE || c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN || c == TOKEN_TAB )
		return 1;
	return 0;
}

int SolarFileReader::_isnewline( char c )
{
	if( c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN )
		return 1;
	return 0;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace( eTOKEN_TYPE desireTokenType )
{
	// 주석 처리 : 건너뛰기
	eTOKEN_TYPE getTokenType = TOKEN_ERROR;
	char c = 0, nc = 0;
	
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar();
		nc = _getcurchar();

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar() ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar();// * 무시위해
			while ( ( c = _getchar() ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar() != TOKEN_SLASH );
			c = _getchar();// * 무시위해
			c = _getchar();// / 무시위해
		}

		// 파일 종료 체크
		if( c == TOKEN_END ) 
			return TOKEN_END;

		if( 0 == _isspace( c ) )
			break;
	}

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
		// 숫자
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':	case '+':
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.' || c == 'f') )
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			m_dTokenNumber	= atof( m_pszTokenString );
			getTokenType	= TOKEN_NUMBER;
		}	
		break;
	default: //< 문자열
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"원하는 토큰 타입과 다릅니다. : 데이터 오류일 가능성이 큽니다" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	 SolarFileReader::_getNextTokenType_quotationForStr( eTOKEN_TYPE desireTokenType )
{
	// 주석 처리 : 건너뛰기
	eTOKEN_TYPE getTokenType = TOKEN_ERROR;
	char c = 0, nc = 0;
	
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar();
		nc = _getcurchar();

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar() ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar();// * 무시위해
			while ( ( c = _getchar() ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar() != TOKEN_SLASH );
			c = _getchar();// * 무시위해
			c = _getchar();// / 무시위해
		}

		// 파일 종료 체크
		if( c == TOKEN_END ) 
			return TOKEN_END;

		if( 0 == _isspace( c ) )
			break;
	}

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	// 숫자
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':	case '+':
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.' || c == 'f') )
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			m_dTokenNumber	= atof( m_pszTokenString );
			getTokenType	= TOKEN_NUMBER;
		}	
		break;
	case '"': //< 문자열
		{
			while( ( c = _getchar() ) != TOKEN_END && ( c != '"' ) )
			{
				if( c == '\\') c = _getchar();
				*(pbp++) = c;
			}
			*pbp = NULL;
			getTokenType	= TOKEN_STRING;
		}
		break;
	default: //< 형식에 맞지 않는 문자들
		{
			*(pbp++) = c;
			*pbp = NULL;
			getTokenType	= TOKEN_EXTRA;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"원하는 토큰 타입과 다릅니다. : 데이터 오류일 가능성이 큽니다" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_newline( eTOKEN_TYPE desireTokenType )
{
	// 주석 처리 : 건너뛰기
	eTOKEN_TYPE getTokenType = TOKEN_ERROR;
	char c = 0, nc = 0;
	
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar();
		nc = _getcurchar();

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar() ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar();// * 무시위해
			while ( ( c = _getchar() ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar() != TOKEN_SLASH );
			c = _getchar();// * 무시위해
			c = _getchar();// / 무시위해
		}

		// 파일 종료 체크
		if( c == TOKEN_END ) 
			return TOKEN_END;

		if( 0 == _isspace( c ) )
			break;
	}

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	default: // 숫자, 문자, 공백에 관계없이 무조건 new line 만을 분리자로 인식
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isnewline( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"원하는 토큰 타입과 다릅니다. : 데이터 오류일 가능성이 큽니다" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace_onlystring( eTOKEN_TYPE desireTokenType )
{
	// 주석 처리 : 건너뛰기
	eTOKEN_TYPE getTokenType = TOKEN_ERROR;
	char c = 0, nc = 0;
	
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar();
		nc = _getcurchar();

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar() ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar();// * 무시위해
			while ( ( c = _getchar() ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar() != TOKEN_SLASH );
			c = _getchar();// * 무시위해
			c = _getchar();// / 무시위해
		}

		// 파일 종료 체크
		if( c == TOKEN_END ) 
			return TOKEN_END;

		if( 0 == _isspace( c ) )
			break;
	}

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	default: // 오로지 문자열로만 인식
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"원하는 토큰 타입과 다릅니다. : 데이터 오류일 가능성이 큽니다" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE SolarFileReader::GetNextTokenType( eTOKEN_TYPE desireTokenType )
{
	_clear();

	switch( m_Mode )
	{
	case SEPERATOR_WHITESPACE:
		return _getNextTokenType_whitespace( desireTokenType );
	case SEPERATOR_QUOTATIONMARKFORSTR:
		return _getNextTokenType_quotationForStr( desireTokenType );
	case SEPERATOR_ONLY_NEWLINE:
		return _getNextTokenType_newline( desireTokenType );
	case SEPERATOR_WHITESPACE_ONLY_STRING :
		return _getNextTokenType_whitespace_onlystring( desireTokenType );
	}
	return TOKEN_ERROR;
}

BOOL SolarFileReader::IsEOF()
{
	DWORD m_CurPos = m_dwBufferPos;
	char c = 0, nc = 0;
	
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar( &m_CurPos );
		nc = _getcurchar( &m_CurPos );

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar( &m_CurPos ) ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar( &m_CurPos );// * 무시위해
			while ( ( c = _getchar( &m_CurPos ) ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar( &m_CurPos ) != TOKEN_SLASH );
			c = _getchar( &m_CurPos );// * 무시위해
			c = _getchar( &m_CurPos );// / 무시위해
		}

		// 파일 종료 체크
		if( c == TOKEN_END ) 
			return TRUE;

		if( 0 == _isspace( c ) )
			break;
	}

	return FALSE;
}

char * SolarFileReader::GetNextString( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenString();
}

double SolarFileReader::GetNextNumber( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenNumber();
}


}


//=====================================================================================================

BaseParser::BaseParser()
{
	ZeroMemory( m_pszPackFileName, MAX_PATH );
}

BaseParser::~BaseParser()
{
}

void BaseParser::SetPackFileName( char * pszPackFileName )
{
	if( pszPackFileName )
		strcpy( m_pszPackFileName, pszPackFileName );
	else
		ZeroMemory( m_pszPackFileName, MAX_PATH );
}

BOOL BaseParser::OpenFile( util::SolarFileReader & sr, char * pszFileName,  eSEPERATOR_MODE mode )
{
	BOOL bResult = FALSE;

	// pack
	if( m_pszPackFileName[0] )
	{
		bResult = sr.OpenFileFromPack( m_pszPackFileName, pszFileName, mode );
	}
	// unpack
	else
	{
		bResult = sr.OpenFile( pszFileName, mode );
	}

	if( !bResult )
	{
		 ASSERT( !"File Open Error" );
	}

	return bResult;
}
