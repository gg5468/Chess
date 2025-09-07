#pragma once
#include "Player.h"

#include "Chessboard.h"
#include <memory>

class Game
{
public:
	Game(Game&&) noexcept = default;
	Game& operator=(Game&&) noexcept = default;

	Game();
	~Game();
	void Draw(HDC dc, RECT client_rect);
	bool OnLButtonDown(CPoint p);


private:
	std::unique_ptr<Chessboard> chessboard;
	Square* selected_square = nullptr;
	
	Player white = Player("White");
	Player black = Player("Black");
	
	Player* turn = &white;


	//todo: refactor
	Square* piece_in_hand = nullptr;

};

