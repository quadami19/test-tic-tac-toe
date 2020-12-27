#pragma once
#include <vector>
#include <cassert>
#include <queue>
#include "vec2d.hpp"

namespace ttt {

struct Field;

namespace ai {

typedef std::vector<long> PrMap;
typedef std::vector<char> VisMap;
typedef std::vector<Vector2D> PrQueue;

enum { PER_FREE_PRIORITY = 1, PER_SYMBOL_PRIORITY = 3 };

struct Engine
{
	private:

		PrMap m_priorityMap;
		VisMap m_visitedMap;
		PrQueue m_prQueue;
		Field& m_fieldRef;

	public:

		Engine(Field& _field);
		Engine(const Engine&) = delete;

		void resolve(char _aiSymbol, char _plSymbol, int _winCondition);
		bool isEmpty() { return m_prQueue.empty(); }
		void clear();
		Vector2D popPos();

	private:

		void fillOccupied();
		void enqueueAll();
};

struct PriorityCalculator
{
	private:
		PrMap& m_priorityMapRef;
		VisMap& m_visitedMapRef;
		Field& m_fieldRef;
		int m_winCondition;
		int m_multipl = 1;
		char m_symbol;

	public:
		PriorityCalculator(PrMap& _pm, VisMap& _vm, Field& _f) :
		    m_priorityMapRef(_pm),
		    m_visitedMapRef(_vm),
		    m_fieldRef(_f)
		{}

		void calculate(int _winCondit, int _symbol, int _priorMult);

	private:

		void processSymbols();
		void processLine(Vector2D _v, Vector2D _dirV);
		void cellsCalc(Vector2D& _v, Vector2D _dirV, int& _numAiSymb, int& _numFree);
		void priorityLine(Vector2D _begV, Vector2D _endV, int _szSymb, int _szFree);
		void incPriority(Vector2D _v, long _inc);
};

}
}
