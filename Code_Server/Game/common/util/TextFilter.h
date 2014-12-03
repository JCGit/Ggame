#ifndef	__TEXT_FILTER__
#define	__TEXT_FILTER__

//------------------------------------------------------------------------------
/**
@brief

//	�ؽ�Ʈ ���͸��� ���� Ŭ����.
//	���͸� ����� �ִ����� �˻��ϴ� �Լ� (IsHaveReplace)��
//	������ ���͸��� �ϴ� �Լ� (Replace) �ΰ��� �̷���� �ִ�.
//	�� ���͸� ���� ���� ITextFilter�� ����� ��
//	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc);
//	virtual	void	ReplaceWord(char *szDest, const char *szSrc, int nMaxLength);
//	�ΰ� �Լ��� override�����ν� �����ϰ� �ȴ�.

@author
	- ���� 

@since
	- 2005.8.12
@remarks

*/

#define	SLANG_LENGTH_MAX	32


class IAbstractInterface
{
public:
	bool		IsInit() {return m_xInit;	}
	
protected:
	bool		m_xInit;
};

class	ITextFilter : public IAbstractInterface
{
public :
					ITextFilter() {Initialize();	}
	virtual			~ITextFilter() {Release();	}

	virtual	HRESULT	Create() = 0;
	virtual	HRESULT	Destroy() = 0;
	virtual	HRESULT	Save(const char *szFName) = 0;
	virtual	HRESULT	Load(const char *szFName) = 0;
	virtual	HRESULT	LoadFromPack(const char *szFName) = 0;


	bool			Replace(char *szDest, char *szSrc, int nMaxLength);
	bool            CheckIncludeString(char *szSrc);//

	bool            CheckString(char *szSrc);
	bool			IsHaveReplace(char *szSrc,bool binclude=true);
	
protected :
	void			Clear() {}
	void			Initialize() {m_xInit = false;	}
	void			Release() {}

	static	char*	FindString(char *szStr, const char *szCharSet);
	static	void	GetStringPart(char *szDest, const char *szSrc, int nLength);
	static	void	Trim(char *szDest, const char *szSrc);

	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc) = 0;
	virtual	void	ReplaceWord(char *szDest, const char *szSrc, int nMaxLength) = 0;

protected :
	char			**m_szaSlangTarget, **m_szaSlangReplace;
	int				m_nSlangNumber;

protected :

};

class	CNSlangTextFilter : public ITextFilter
{

public :
	CNSlangTextFilter() {Clear();	Create();}
	virtual			~CNSlangTextFilter() {Destroy();	}

	virtual	HRESULT	Create() {m_xInit = true;	return true;	}
	virtual	HRESULT	Destroy();
	virtual	HRESULT	Save(const char *szFName) {szFName;return false;	}
	virtual	HRESULT	Load(const char *szFName);
	virtual	HRESULT	LoadFromPack(const char *szFName);

protected :
	void			Clear();
	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc);
	virtual	void	ReplaceWord(char *szDest, const char *szSrc, int nMaxLength);
};


#endif