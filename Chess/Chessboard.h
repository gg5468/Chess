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
private:
	Square squares[8][8];
	Piece pieces[8][8];
	
	std::string fen_notation = "2kr3r/ppp2ppp/2n2n2/2b1p3/2B1P3/2N2N2/PPP2PPP/R1B2RK1 b - - 4 10";
};
