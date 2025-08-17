#pragma once
#include "Piece.h"

enum class SquareState {
	Uninitialized,
	Initialized
};

class Square
{
public:
	RECT rect;
	Piece piece;
	SquareState state;

	Square(Piece* p);
	Square() : piece(Piece::None), state(SquareState::Uninitialized) {}
	void DrawSquare(HDC dc, RECT client_rect, HBRUSH brush, HPEN hPen);

	void SetSelected(bool sel) { selected = sel; }
private:
	
	void drawSelected(HDC dc, RECT r, HPEN hPen);
	bool selected = false; 
	void setRect(RECT& r) {rect = r;}
};

