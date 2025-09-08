#include "pch.h"
#include "Game.h"
#include "Chessboard.h"
#include <cmath>

Game::Game()
{
    chessboard = std::make_unique<Chessboard>();
}

Game::~Game()
{
}

void Game::Draw(HDC dc, RECT client_rect)
{
    const int labelMargin = 40;
    chessboard->DrawBoard(dc, client_rect);
}

bool Game::OnLButtonDown(CPoint point)
{
    selected_square = chessboard->OnLButtonDown(point);

    if (!selected_square) {
        return false; 
    }

    if (piece_in_hand) {
        if (!IsValidMove(*piece_in_hand, *selected_square, chessboard->squares)) {
            return false; 
        }

        selected_square->SetPiece(piece_in_hand->GetPiece());
        piece_in_hand->SetPiece(Piece(PieceType::None));
        piece_in_hand = nullptr;

        currentPlayer = (currentPlayer == white.get()) ? black.get() : white.get();

        return true;
    }
    else {
        if (selected_square->GetPiece().GetPieceType() == PieceType::None) {
            return false; 
        }
        if (selected_square->GetPiece().GetPieceColor() == currentPlayer->GetColor()) {
            piece_in_hand = selected_square;
            return true;
        }
    }

    return false;
}

bool Game::IsValidMove(Square& from, Square& to, Square board[8][8])
{
    int fromX, fromY, toX, toY;
    if (!findSquarePosition(from, board, fromX, fromY) ||
        !findSquarePosition(to, board, toX, toY)) {
        return false;
    }

    if (fromX == toX && fromY == toY) return false; // same square

    Piece movingPiece = from.GetPiece();
    if (movingPiece.GetPieceType() == PieceType::None) return false; // no piece to move

    int dx = toX - fromX;
    int dy = toY - fromY;
    int adx = std::abs(dx);
    int ady = std::abs(dy);

    if (!(dx == 0 || dy == 0 || adx == ady)) return false;

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = fromX + stepX;
    int y = fromY + stepY;

    // check path until destination
    while (x != toX || y != toY) {
        if (board[y][x].GetPiece().GetPieceType() != PieceType::None) {
            return false; // blocked
        }
        x += stepX;
        y += stepY;
    }

    Piece targetPiece = to.GetPiece();
    if (targetPiece.GetPieceType() == PieceType::None) return true; // empty square ok

    return targetPiece.GetPieceColor() != movingPiece.GetPieceColor(); // capture enemy
}

bool Game::findSquarePosition(Square& target, Square board[8][8], int& outX, int& outY)
{
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (&board[y][x] == &target) { 
                outX = x;
                outY = y;
                return true;
            }
        }
    }
    return false;
}
