#pragma once

#include "Chessboard.h"

class Game
{
public:
	Game();
	~Game();
	void Draw(HDC dc, RECT client_rect);
private:
	Chessboard chessboard;
};

