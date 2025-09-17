#pragma once
#include "Square.h"
#include <string>
#include <vector>

class Chessboard
{
public:
    Chessboard();

    void DrawBoard(HDC dc, const RECT& client_rect);

    void SetPieces();
    void Chessboard::Reset();
    Square* OnLButtonDown(const CPoint& point);
    Square* FindSquareWithPoint(const CPoint& point);

    Square* GetSquare(int row, int col);

    const std::vector<std::vector<Square>>& GetSquares() const { return squares; }

private:
    std::vector<std::vector<Square>> squares;

    const std::wstring fen_notation = L"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};
