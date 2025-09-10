#pragma once
#include "Player.h"
#include "Piece.h"
#include "Chessboard.h"
#include <memory>

class CChessDlg; // forward declaration

class Game
{
public:
	Game(Game&&) noexcept = default;
	Game& operator=(Game&&) noexcept = default;
	
	Game();
	~Game();
	void Draw(HDC dc, RECT client_rect);
	bool CanCastle(Square& kingSquare, Square& rookSquare, Square board[8][8]);
	bool OnLButtonDown(CPoint p);
	bool Game::IsValidMove(Square& from, Square& to, Square board[8][8]);
	bool Game::findSquarePosition(Square& target, Square board[8][8], int& outX, int& outY);

	bool IsInCheck(PieceColor kingColor, Square board[8][8]);

	bool IsCheckmate(PieceColor kingColor, Square board[8][8]);

	void CopyBoard(Square src[8][8], Square dest[8][8]);
	CChessDlg* parentDlg = nullptr;

private:
	std::unique_ptr<Chessboard> chessboard;
	Square* selected_square = nullptr;
	
	std::unique_ptr<Player> white = std::make_unique<Player>(PieceColor::White);
	std::unique_ptr<Player> black = std::make_unique<Player>(PieceColor::Black);
	Player* currentPlayer = white.get();

	int lastPawnDoubleStepX; // X of last double-step pawn
	int lastPawnDoubleStepY; // Y of last double-step pawn

	Square* piece_in_hand = nullptr;

};

