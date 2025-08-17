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
	bool OnLButtonDown(CPoint point);
private:
	Square squares[8][8];
	Piece pieces[8][8];

	std::string fen_notation = "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4";
};