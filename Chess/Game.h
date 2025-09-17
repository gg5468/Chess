#pragma once
#include "Player.h"
#include "Piece.h"
#include "Chessboard.h"
#include <memory>
#include <vector>

class CChessDlg; // forward declaration

class Game
{
public:
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;
    
    Game(CChessDlg& parent) : parentDlg(parent) {};
    void Draw(HDC dc, RECT client_rect);

    void Redraw(HDC dc, RECT client_rect);
    
    bool OnLButtonDown(CPoint p);

    Square* GetChessboardSquare(int row, int col);
    
    void PromotePawn(Square* square, PieceType newType);

private:
    void switchTurn();

    bool isValidMove(Square& from, Square& to, const std::vector<std::vector<Square>>& board);
    bool findSquarePosition(Square& target, const std::vector<std::vector<Square>>& board, int& outX, int& outY);
    bool isInCheck(PieceColor kingColor, const std::vector<std::vector<Square>>& board);
    bool isCheckmate(PieceColor kingColor, const std::vector<std::vector<Square>>& board);
    bool canCastle(const Square& kingSquare, const Square& rookSquare, const std::vector<std::vector<Square>>& board);

    CChessDlg& parentDlg;

    Chessboard chessboard;
    Square* selected_square = nullptr;
    
    Player white = Player(PieceColor::White);
    Player black = Player(PieceColor::Black);
    Player* currentPlayer = &white;

    int lastPawnDoubleStepX = -1; 
    int lastPawnDoubleStepY = -1; 

    Square* piece_in_hand = nullptr;

    bool inBounds(int x, int y);
    bool pathClear(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Square>>& board);
    void checkCheck();
};

