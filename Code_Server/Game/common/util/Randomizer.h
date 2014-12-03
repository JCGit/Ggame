#pragma once

#include <vector>
#include <algorithm>

typedef std::vector<int>		RANDOM_VEC;
typedef RANDOM_VEC::iterator	RANDOM_VEC_ITER;

// ����ȭ�� ���� �����
// ��÷�� Create �ѹ� �ϰ� Rand �Լ� ����ϸ� ��!
class Randomizer
{
public:
	Randomizer() { m_iRandMax = 0; m_iGetCount = 0; }
	~Randomizer() { Destroy; }

	void Create( int iRandMax )
	{
		m_iRandMax = iRandMax;
		for( int i = 1; i <= iRandMax; ++i )
		{
			m_vecRandom.push_back( i );
		}
		RandomShuffule();
	}

	void Destroy()
	{
		m_vecRandom.clear();
	}

	// 1 ~ RandMax ������ ������ ��
	inline int Rand()
	{
		int retVal = m_vecRandom[m_iGetCount];
		if( ++m_iGetCount >= m_iRandMax ) {	m_iGetCount = 0; RandomShuffule(); }
		return retVal;
	}

	// min ~ max ������ ������ ��
	inline int Rand( int min, int max )
	{
		return ( Rand() % ( max + 1 - min ) ) + min;
	}

private:
	inline void RandomShuffule()
	{
		random_shuffle( m_vecRandom.begin(), m_vecRandom.end() );
	}

	RANDOM_VEC		m_vecRandom;
	int				m_iRandMax;
	int				m_iGetCount;
};