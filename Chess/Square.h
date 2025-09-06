#pragma once
#include "Piece.h"

enum class SquareState {
	Uninitialized,
	Initialized
};

class Square
{
public:
	SquareState state;

	Square(Piece* p);
	Square() : piece(PieceType::None), state(SquareState::Uninitialized) {}
	
	void DrawSquare(HDC dc, RECT client_rect, HBRUSH brush, HPEN hPen);
	void SetSelected(bool sel) { selected = sel; }

	Piece GetPiece() { return piece; }
	Piece SetPiece(Piece p) { piece = p; return piece; }

	RECT GetRect() { return rect; }
	RECT SetRect(RECT& r) { rect = r; return rect; }
private:
	Piece piece;
	RECT rect;
	
	void drawSelected(HDC dc, RECT r, HPEN hPen);
	bool selected = false; 
};

