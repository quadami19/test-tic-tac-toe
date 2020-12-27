#pragma once
#include <iostream>
#include <cassert>
#include "vec2d.hpp"

namespace ttt {

struct Field
{
	private:
		int   m_size;
		int   m_occup = 0;
		char* m_buffer;
		char  m_emptySymbol = '_';

		Field(const Field&) = delete;

	public:
		Field(int _size, char _emptySymbol)
		{
			m_size = _size;
			m_buffer = new char [m_size*m_size];
			m_emptySymbol = _emptySymbol;

			for (int i = 0; i < m_size*m_size; i++)
				m_buffer[i] = m_emptySymbol;
		}

		~Field()
		{
			delete [] m_buffer;
		}

		char cell(Vector2D _p) const
		{
			assert(!isOutside(_p));
			return m_buffer[_p.y*m_size + _p.x];
		}
		bool put(char _symbol, Vector2D _p)
		{
			char& c = m_buffer[_p.y*m_size + _p.x];

			if (c == m_emptySymbol)
			{
				m_buffer[_p.y*m_size + _p.x] = _symbol;
				++m_occup;
				return true;
			}
			return false;

		}
		int size() const { return m_size; }

		bool isOccupied(Vector2D _p) const { return cell(_p) != m_emptySymbol; }
		bool isFilled() const { return m_size*m_size == m_occup; }
		bool isEqual(Vector2D _p, char _symbol) const { return cell(_p) == _symbol; }
		bool isOutside(Vector2D _p) const { return _p.x < 0 || _p.x >= m_size || _p.y < 0 || _p.y >= m_size; }
		bool isOpposite(Vector2D _p, char _symbol) const
		{ auto c = cell(_p); return _symbol != c && c != m_emptySymbol; }

		bool findSequence(char _symb, int _num)
		{
			Vector2D vec;
			bool result = false;

			while (!isOutside(vec))
			{
				if (isEqual(vec, _symb))
				{
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D(-1, -1));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D(-1,  0));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D(-1,  1));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D( 0,  1));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D( 1,  1));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D( 1,  0));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D( 1, -1));
					result = result || isDirContainsSequence(_symb, _num, vec, Vector2D( 0, -1));
				}
				if (result) return true;

				vec = toVector2D(fromVector2D(vec) + 1);
			}
			return false;
		}

		bool isDirContainsSequence(char _symb, int _num, Vector2D _pos, Vector2D _dir)
		{
			int counter = 0;

			for (; !isOutside(_pos) && isEqual(_pos, _symb); _pos += _dir, counter++) {}

			return counter >= _num;
		}


		Vector2D toVector2D(size_t _pos)
		{ return Vector2D(_pos % m_size, _pos / m_size); }

		size_t fromVector2D(Vector2D _vec)
		{ return _vec.y * m_size + _vec.x; }

};

}
