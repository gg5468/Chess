#pragma once
#include "Square.h"



class Chessboard
{
public:
	Chessboard();
	~Chessboard();

	void DrawBoard(HDC dc,RECT client_rect);

private:
	Square* Squares[8][8];
};

