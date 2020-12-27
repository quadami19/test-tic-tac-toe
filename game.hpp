#pragma once
#include "ai.hpp"

#include<iostream>
namespace ttt {
namespace game {

enum { NUM_PLAYERS = 2 };
enum { DEFAULT_FIELD_SIZE = 3 };
enum { SYMBOL_O = 'O', SYMBOL_X = 'X', SYMBOL_NULL = '-', DEFAULT_EMPTY_SYMBOL = '_' };
enum { FIRST_PLAYER_SYMBOL = SYMBOL_X };
struct Engine
{
	private:

		Field m_field;
		ai::Engine m_ai;

		const char m_plSymbols[NUM_PLAYERS] = { SYMBOL_X, SYMBOL_O };
		bool m_isPlayerAi[NUM_PLAYERS] = { true };

		int m_winCondition;
		int m_currPlayer = 0;
		int m_winner = -1;
		bool m_isGameOver = false;

		Vector2D m_playerTurnBuff;

	public:
		Engine(int _fieldSize = DEFAULT_FIELD_SIZE, int _winCond = DEFAULT_FIELD_SIZE) :
		    m_field(_fieldSize, DEFAULT_EMPTY_SYMBOL),
		    m_ai(m_field),
		    m_winCondition(_winCond)
		{
			assert(_winCond <= m_field.size());
		}

		void start()
		{
			m_currPlayer = 0;
			m_winner = -1;
			m_isGameOver = false;
		}

		bool turn()
		{
			if (isCurrPlayerAi()) aiTurn();
			else plTurn();

			m_currPlayer = (m_currPlayer + 1) % 2;

			return true;
		}

		bool place(Vector2D _pos)
		{
			if(isCurrPlayerAi() ||
			   m_field.isOutside(_pos) ||
			   m_field.isOccupied(_pos)) return false;

			m_playerTurnBuff = _pos;
			return true;
		}

		void setAiPlayer(char _symbol, bool _isAi)
		{
			int index = symbolToNum(_symbol);
			m_isPlayerAi[index] = _isAi;
		}

		void setAiPlayer(int _player, bool _isAi)
		{
			m_isPlayerAi[_player] = _isAi;
		}

		int symbolToNum(char _symbol)
		{
			switch (_symbol) {
				case SYMBOL_X: return 0;
				case SYMBOL_O: return 1;
			}
		}

		int currPlayerNum() const { return m_currPlayer; }
		bool isCurrPlayerAi() const { return m_isPlayerAi[m_currPlayer]; }
		const Field& field() const { return m_field; }

		bool isGameOver() const { return m_isGameOver || m_field.isFilled(); }
		int winner() const { return m_winner; }

	private:

		void aiTurn()
		{
			m_ai.resolve(m_plSymbols[m_currPlayer], m_plSymbols[(m_currPlayer+1)%2], m_winCondition);
			Vector2D vec = m_ai.popPos();
			m_field.put(m_plSymbols[m_currPlayer], vec);
			calcWin();
		}

		void plTurn()
		{
			m_field.put(m_plSymbols[m_currPlayer], m_playerTurnBuff);
			calcWin();
		}

		bool calcWin()
		{
			if (m_field.findSequence(m_plSymbols[m_currPlayer], m_winCondition))
			{
				m_winner = m_currPlayer;
				m_isGameOver = true;

				return true;
			}
			return false;
		}

		void procPlOrder()
		{
			//if (m_plSymbols)
		}
};

}
}
