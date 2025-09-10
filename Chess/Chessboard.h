#pragma once
#include "Square.h"
#include <string>


class Chessboard
{
public:
	Chessboard();
	~Chessboard();

	void DrawBoard(HDC dc, RECT client_rect);
	
	void SetPieces();
	Square* OnLButtonDown(CPoint point);
	Square* FindSquareWithPoint(CPoint point);

	Square& GetSquare(int row, int col);
private:
	Square squares[8][8];
	Piece pieces[8][8];
	
	std::string fen_notation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};
