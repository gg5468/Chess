#pragma once
#include "Piece.h"
class Square
{
public:
	RECT rect = {};
	Piece piece;
	Square(RECT r, Piece* p);
	void DrawSquare(HDC dc, RECT client_rect, HBRUSH brush, HPEN hPen);
};

