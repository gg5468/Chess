#include "pch.h"
#include "Game.h"
#include "Chessboard.h"

Game::Game()
{
	chessboard = new Chessboard();
}
Game::~Game()
{
	chessboard = nullptr;
}

void Game::Draw(HDC dc, RECT client_rect)
{
	chessboard -> DrawBoard(dc, client_rect);
}
