#pragma once

namespace ttt {

struct Vector2D {
		int x = 0;
		int y = 0;

		Vector2D(int _x = 0, int _y = 0) : x(_x), y(_y) {}

		Vector2D operator-(Vector2D _p)
		{ return Vector2D(x - _p.x, y - _p.y); }

		Vector2D operator+(Vector2D _p)
		{ return Vector2D(x + _p.x, y + _p.y); }

		Vector2D& operator-()
		{ x = -x; y = -y; return *this; }

		Vector2D& operator-=(Vector2D _p)
		{ x -= _p.x; y -= _p.y; return *this; }

		Vector2D& operator+=(Vector2D _p)
		{ x += _p.x; y += _p.y; return *this; }

		bool operator== (Vector2D _p)
		{ return x == _p.x && y == _p.y; }

		bool operator!= (Vector2D _p)
		{ return !operator==(_p); }

		Vector2D increment()
		{
			Vector2D ret;
			ret.x = x ? (x < 0 ? -1 : 1) : 0;
			ret.y = y ? (y < 0 ? -1 : 1) : 0;
			return ret;
		}

		int maxAbs()
		{
			int absX = x < 0 ? -x : x;
			int absY = y < 0 ? -y : y;
			return absX > absY ? absX : absY;
		}
};

}
