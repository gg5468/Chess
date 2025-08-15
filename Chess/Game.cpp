#include "pch.h"
#include "Game.h"
#include "Chessboard.h"

Game::Game()
{
	chessboard = Chessboard();
}
Game::~Game()
{
	// Destructor implementation
}

void Game::Draw(HDC dc, RECT client_rect)
{
	chessboard.DrawBoard(dc, client_rect);
}
