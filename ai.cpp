#include "ai.hpp"
#include "field.hpp"
#include <iostream>

namespace ttt {
namespace ai {

Engine::Engine(Field& _field) :
    m_fieldRef(_field)
{
	m_priorityMap.resize(m_fieldRef.size()*m_fieldRef.size());
	m_visitedMap.resize(m_fieldRef.size()*m_fieldRef.size());
}

void Engine::resolve(char _aiSymbol, char _plSymbol, int _winCondition)
{
	PriorityCalculator prCalc(m_priorityMap, m_visitedMap, m_fieldRef);

	clear();
	prCalc.calculate(_winCondition, _aiSymbol, 3);
	prCalc.calculate(_winCondition, _plSymbol, 2);

	fillOccupied();
	enqueueAll();
}

void Engine::clear()
{
	std::fill(m_priorityMap.begin(), m_priorityMap.end(), 0l);
	m_prQueue.clear();
}

Vector2D Engine::popPos()
{
	size_t pos = std::rand() % m_prQueue.size();
	Vector2D vec = m_prQueue[pos];
	m_prQueue.erase(m_prQueue.begin() + pos);
	return vec;
}

void Engine::fillOccupied()
{
	Vector2D iter(0, 0);
	size_t iterVal = 0;

	while (iterVal < m_priorityMap.size())
	{
		if (m_fieldRef.isOccupied(iter)) m_priorityMap[iterVal] = -1;

		iterVal++;
		iter = m_fieldRef.toVector2D(iterVal);

	}
}

void Engine::enqueueAll()
{
	long maxPriority = 0;

	for(size_t i = 0; i < m_priorityMap.size(); i++)
	{
		if (m_priorityMap[i] > maxPriority)
		{
			m_prQueue.clear();
			m_prQueue.push_back(m_fieldRef.toVector2D(i));
			maxPriority = m_priorityMap[i];
		}
		else if (m_priorityMap[i] == maxPriority)
		{
			m_prQueue.push_back(m_fieldRef.toVector2D(i));
		}
	}
}

void PriorityCalculator::calculate(int _winCondit, int _symbol, int _priorMult)
{
	m_winCondition = _winCondit;
	m_symbol = _symbol;
	m_multipl = _priorMult;

	processSymbols();
}

void PriorityCalculator::processSymbols()
{
	Vector2D p;

	while (!m_fieldRef.isOutside(p))
	{
		if (m_fieldRef.isEqual(p, m_symbol))
		{
			processLine(p, Vector2D (-1, -1));
			processLine(p, Vector2D (-1,  0));
			processLine(p, Vector2D (-1,  1));
			processLine(p, Vector2D ( 0,  1));
		}
		auto pos = m_fieldRef.fromVector2D(p) + 1;
		p = m_fieldRef.toVector2D(pos);
	}
}

void PriorityCalculator::processLine(Vector2D _v, Vector2D _dirV)
{
	int numFree = 0;
	int numAiSymb = 1;
	Vector2D v (_v);
	Vector2D dirV(_dirV);

	cellsCalc(v, dirV, numAiSymb, numFree);
	v -= dirV;

	_dirV = -dirV;
	cellsCalc(_v, _dirV, numAiSymb, numFree);

	priorityLine(v, _v, numAiSymb, numFree);
}

void PriorityCalculator::cellsCalc(Vector2D& _v, Vector2D _dirV, int& _numAiSymb, int& _numFree)
{
	while(true)
	{
		_v += _dirV;
		bool b1 = m_fieldRef.isOutside(_v);
		bool b2;
		if (!b1) b2 = m_fieldRef.isOpposite(_v, m_symbol);
		if (m_fieldRef.isOutside(_v) ||
		    m_fieldRef.isOpposite(_v, m_symbol) ||
		    _numAiSymb + _numFree == m_winCondition)
		{
			break;
		}
		else if (m_fieldRef.isEqual(_v, m_symbol))
		{
			_numAiSymb++;
		}
		else
		{
			_numFree++;
		}
	}
}

void PriorityCalculator::priorityLine(Vector2D _begV, Vector2D _endV, int _szSymb, int _szFree)
{
	auto diffV = _endV - _begV;
	auto incrV = diffV.increment();

	if (diffV.maxAbs() >= m_winCondition)
	{
		int totPriority = _szFree * PER_FREE_PRIORITY + _szSymb * PER_SYMBOL_PRIORITY;
		for (; _begV != _endV; _begV += incrV)
		{
			if (!m_fieldRef.isOccupied(_begV)) incPriority(_begV, totPriority);
		}
	}
}

void PriorityCalculator::incPriority(Vector2D _v, long _inc)
{
	m_priorityMapRef[_v.y * m_fieldRef.size() + _v.x] += _inc;
}

}
}
