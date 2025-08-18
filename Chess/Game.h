#pragma once

#include "Chessboard.h"

class Game
{
public:
	Game();
	~Game();
	void Draw(HDC dc, RECT client_rect);
	bool OnLButtonDown(CPoint p);
private:
	Chessboard* chessboard;
	Square* selected_square = nullptr;
};

