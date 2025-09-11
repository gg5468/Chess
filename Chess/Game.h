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
	
	Game(CChessDlg* parent);
	~Game();
	void Draw(HDC dc, RECT client_rect);
	
	bool OnLButtonDown(CPoint p);

	Square* GetChessboardSquare(int row, int col);
	
	void PromotePawn(Square* square, PieceType newType);

private:
	void switchTurn();

	bool isValidMove(Square& from, Square& to, Square board[8][8]);
	bool findSquarePosition(Square& target, Square board[8][8], int& outX, int& outY);
	bool isInCheck(PieceColor kingColor, Square board[8][8]);
	bool isCheckmate(PieceColor kingColor, Square board[8][8]);
	void copyBoard(Square src[8][8], Square dest[8][8]);
	bool canCastle(Square& kingSquare, Square& rookSquare, Square board[8][8]);

	CChessDlg* parentDlg = nullptr;

	std::unique_ptr<Chessboard> chessboard = std::make_unique<Chessboard>();;
	Square* selected_square = nullptr;
	
	std::unique_ptr<Player> white = std::make_unique<Player>(PieceColor::White);
	std::unique_ptr<Player> black = std::make_unique<Player>(PieceColor::Black);
	Player* currentPlayer = white.get();

	int lastPawnDoubleStepX = -1; 
	int lastPawnDoubleStepY = -1; 

	Square* piece_in_hand = nullptr;

	bool inBounds(int x, int y);
	bool pathClear(int fromX, int fromY, int toX, int toY, Square board[8][8]);
	void checkCheck();

};

