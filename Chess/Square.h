#pragma once
class Square
{
public:
	RECT rect = {};

	Square(RECT r);
	void DrawSquare(HDC dc, RECT client_rect, HBRUSH brush, HPEN hPen);
};

