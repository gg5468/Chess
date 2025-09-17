#pragma once
#include "Piece.h"

enum class SquareState{
	Uninitialized,
	Initialized
};

class Square
{
public:          
	Square& operator=(const Square& other);
	
	void DrawSquare(HDC dc, HBRUSH brush, HPEN hPen);
	void SetSelected(bool sel) { selected = sel; }

	Piece GetPiece() const { return piece; };
	Piece SetPiece(Piece p) { piece = p; return piece; }

	RECT GetRect() { return rect; }
	RECT SetRect(RECT& r) { rect = r; return rect; }
private:
	Piece piece = PieceType::Uninitialized;
	RECT rect = { 0,0,0,0 };
	SquareState state = SquareState::Uninitialized;
	
	void drawSelected(HDC dc, RECT r, HPEN hPen);
	bool selected = false; 
};

