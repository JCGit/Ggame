#pragma once

#include <hash_map>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace stdext;


/*
// Ȯ��(DWORD)�� ���� Action�� �ϳ� �����Ѵ�.

RatioSelector<int>	rs();
rs.Add( 1, 20 );
rs.Add( 2, 10 );
rs.Add( 3, 50 );
rs.Add( 4, 30 );

//rs.GenerateRatios();	// ���ص� Select()�� GetTotalRatio()���� �ڵ����� ��.

// m_Ratios�� ����			// Action	Ratio		������ ����
// [19] = 1					// 1		20			0~19
// [29] = 2					// 2		10			20~29
// [79] = 3					// 3		50			30~79
// [109] = 4				// 4		30			80~109

int a;
assert( rs.Select( 0, a ) ); assert( a == 1 );
assert( rs.Select( 19, a ) ); assert( a == 1 );
assert( rs.Select( 20, a ) ); assert( a == 2 );
assert( rs.Select( 21, a ) ); assert( a == 2 );
assert( rs.Select( 81, a ) ); assert( a == 4 );
assert( rs.Select( 109, a ) ); assert( a == 4 );
assert( !rs.Select( 110, a ) );  // 110�� ������ ����Ƿ� ����. a���� �ǹ̾���.
*/

template <class Action>
class RatioSelector
{
	typedef stdext::hash_map<Action, DWORD>		ACTIONS;
	typedef map<DWORD, Action>			RATIOS;
	typedef pair<Action, DWORD>			ACTION_RATIO;

	class ActionRatioCompare 
	{
	public :
		bool			operator () (ACTION_RATIO& l, ACTION_RATIO& r)
		{
			if (l.second < r.second)
			{
				return true;
			}

			return false;
		}
	};

public :
	RatioSelector();
	~RatioSelector() {}

	VOID		Release();

	BOOL		Add(Action action, DWORD ratio);
	BOOL		Remove(Action action);

	VOID		GenerateRatios();

	BOOL		Select(DWORD ratio, Action& selectedAction);
	DWORD		GetTotalRatio();

private :
	bool		m_bGenerated;
	DWORD		m_TotalRatio;
	ACTIONS		m_Actions;
	RATIOS		m_Ratios;
};

template <class Action>
RatioSelector<Action>::RatioSelector() 
: m_bGenerated(true), m_TotalRatio(0)
{
}

template <class Action>
VOID		
RatioSelector<Action>::Release()
{
	m_bGenerated = true;
	m_TotalRatio = 0;
	m_Actions.clear();
	m_Ratios.clear();
}

template <class Action>
BOOL
RatioSelector<Action>::Add(Action action, DWORD ratio)
{

	ASSERT( ratio > 0 );

	ACTIONS::const_iterator iAction = m_Actions.find( action );
	if (iAction != m_Actions.end())
	{
		return FALSE;
	}

	m_Actions[action] = ratio;
	m_bGenerated = false;

	return TRUE;
}


template <class Action>
BOOL
RatioSelector<Action>::Remove(Action action)
{
	ACTIONS::iterator iAction = m_Actions.find( action );
	if (iAction != m_Actions.end())
	{
		m_Actions.erase(iAction);
		m_bGenerated = false;

		return TRUE;
	}

	return FALSE;
}


template <class Action>
VOID
RatioSelector<Action>::GenerateRatios()
{
	m_Ratios.clear();

	// map�� �ִ´�.	
	m_TotalRatio = 0;
	ACTIONS::const_iterator itr = m_Actions.begin();
	for (;itr != m_Actions.end(); itr++)
	{
		const Action&	action = itr->first;
		DWORD			ratio  = itr->second;

		m_TotalRatio += ratio;	

		// �����δ� -1������ action�� ���õǴ� �����̴�.
		// ex) ratio=20�̸�, 0~19����.
		m_Ratios[m_TotalRatio-1] = action;
	}

	m_bGenerated = true;
}


template <class Action>
BOOL
RatioSelector<Action>::Select(DWORD ratio, Action& selectedAction)
{
	if (!m_bGenerated)
	{
		GenerateRatios();
	}

	RATIOS::const_iterator itr = m_Ratios.lower_bound(ratio);

	if (itr==m_Ratios.end())
	{
		return FALSE;
	}

	selectedAction = itr->second;
	return TRUE;
}


template <class Action>
DWORD		
RatioSelector<Action>::GetTotalRatio()		
{ 
	if (!m_bGenerated)
	{
		GenerateRatios();
	}
	
	return m_TotalRatio; 
}

