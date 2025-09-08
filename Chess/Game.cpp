#include "pch.h"
#include "Game.h"
#include "Chessboard.h"
#include <cmath>
#include <algorithm>

// ----------------- Helpers for PieceType <-> Color -----------------

static inline PieceColor GetColorFromType(PieceType pt) {
    switch (pt) {
    case PieceType::WhitePawn: case PieceType::WhiteKnight: case PieceType::WhiteBishop:
    case PieceType::WhiteRook: case PieceType::WhiteQueen: case PieceType::WhiteKing:
        return PieceColor::White;
    case PieceType::BlackPawn: case PieceType::BlackKnight: case PieceType::BlackBishop:
    case PieceType::BlackRook: case PieceType::BlackQueen: case PieceType::BlackKing:
        return PieceColor::Black;
    default:
        return PieceColor::None;
    }
}

static inline bool IsPawn(PieceType pt) { return pt == PieceType::WhitePawn || pt == PieceType::BlackPawn; }
static inline bool IsKnight(PieceType pt) { return pt == PieceType::WhiteKnight || pt == PieceType::BlackKnight; }
static inline bool IsBishop(PieceType pt) { return pt == PieceType::WhiteBishop || pt == PieceType::BlackBishop; }
static inline bool IsRook(PieceType pt) { return pt == PieceType::WhiteRook || pt == PieceType::BlackRook; }
static inline bool IsQueen(PieceType pt) { return pt == PieceType::WhiteQueen || pt == PieceType::BlackQueen; }
static inline bool IsKing(PieceType pt) { return pt == PieceType::WhiteKing || pt == PieceType::BlackKing; }

// ----------------- Game -----------------

Game::Game() {
    chessboard = std::make_unique<Chessboard>();
    lastPawnDoubleStepX = -1;
    lastPawnDoubleStepY = -1;
}

Game::~Game() {}

void Game::Draw(HDC dc, RECT client_rect) {
    chessboard->DrawBoard(dc, client_rect);
}

// ----------------- Player Input -----------------

bool Game::OnLButtonDown(CPoint point) {
    selected_square = chessboard->OnLButtonDown(point);
    if (!selected_square) return false;

    if (piece_in_hand) {
        if (!IsValidMove(*piece_in_hand, *selected_square, chessboard->squares)) {
            piece_in_hand = nullptr; // reset so player can try another move
            return false;
        }

        // --- simulate move ---
        Square copy[8][8];
        CopyBoard(chessboard->squares, copy);

        int fromX, fromY, toX, toY;
        if (!findSquarePosition(*piece_in_hand, chessboard->squares, fromX, fromY) ||
            !findSquarePosition(*selected_square, chessboard->squares, toX, toY)) {
            piece_in_hand = nullptr;
            return false;
        }

        copy[toY][toX].SetPiece(copy[fromY][fromX].GetPiece());
        copy[fromY][fromX].SetPiece(Piece(PieceType::None));

        PieceColor moverColor = GetColorFromType(piece_in_hand->GetPiece().GetPieceType());

        // illegal if king would still be in check
        if (IsInCheck(moverColor, copy)) {
            piece_in_hand = nullptr;
            return false;
        }

        // --- handle en passant capture ---
        PieceType movedType = piece_in_hand->GetPiece().GetPieceType();
        int dx = toX - fromX;
        int dy = toY - fromY;
        PieceType targetType = selected_square->GetPiece().GetPieceType();

        if (IsPawn(movedType) && targetType == PieceType::None && std::abs(dx) == 1 && dy == ((moverColor == PieceColor::White) ? -1 : 1)) {
            int captureRow = toY + ((moverColor == PieceColor::White) ? 1 : -1);
            chessboard->squares[captureRow][toX].SetPiece(Piece(PieceType::None));
        }

        // --- commit move ---
        selected_square->SetPiece(piece_in_hand->GetPiece());
        piece_in_hand->SetPiece(Piece(PieceType::None));
        piece_in_hand = nullptr;

        // track en passant possibility
        if (IsPawn(movedType) && std::abs(dy) == 2) {
            lastPawnDoubleStepX = toX;
            lastPawnDoubleStepY = toY;
        }
        else {
            lastPawnDoubleStepX = -1;
            lastPawnDoubleStepY = -1;
        }

        // switch turn
        currentPlayer = (currentPlayer == white.get()) ? black.get() : white.get();

        // check opponent status
        PieceColor opponentColor = currentPlayer->GetColor();
        if (IsInCheck(opponentColor, chessboard->squares)) {
            if (IsCheckmate(opponentColor, chessboard->squares)) {
                MessageBox(NULL, L"Checkmate!", L"Game Over", MB_OK);
            }
            else {
                MessageBox(NULL, L"Check!", L"Warning", MB_OK);
            }
        }

        return true;
    }
    else {
        if (selected_square->GetPiece().GetPieceType() == PieceType::None) return false;
        if (GetColorFromType(selected_square->GetPiece().GetPieceType()) == currentPlayer->GetColor()) {
            piece_in_hand = selected_square;
            return true;
        }
    }

    return false;
}

// ----------------- Movement Rules -----------------

static inline bool InBounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

static bool PathClear(int fromX, int fromY, int toX, int toY, Square board[8][8]) {
    int dx = toX - fromX;
    int dy = toY - fromY;
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = fromX + stepX;
    int y = fromY + stepY;

    while (x != toX || y != toY) {
        if (board[y][x].GetPiece().GetPieceType() != PieceType::None) return false;
        x += stepX;
        y += stepY;
    }
    return true;
}

bool Game::IsValidMove(Square& from, Square& to, Square board[8][8]) {
    int fromX, fromY, toX, toY;
    if (!findSquarePosition(from, board, fromX, fromY) ||
        !findSquarePosition(to, board, toX, toY))
        return false;

    if (fromX == toX && fromY == toY) return false;

    PieceType movingType = from.GetPiece().GetPieceType();
    PieceColor moverColor = GetColorFromType(movingType);
    if (movingType == PieceType::None || moverColor == PieceColor::None) return false;

    PieceType targetType = to.GetPiece().GetPieceType();
    PieceColor targetColor = GetColorFromType(targetType);
    if (targetType != PieceType::None && targetColor == moverColor) return false;

    int dx = toX - fromX;
    int dy = toY - fromY;
    int adx = std::abs(dx);
    int ady = std::abs(dy);

    // --- Movement rules ---
    if (IsKing(movingType)) {
        return max(adx, ady) == 1;
    }
    if (IsQueen(movingType)) {
        return (dx == 0 || dy == 0 || adx == ady) && PathClear(fromX, fromY, toX, toY, board);
    }
    if (IsRook(movingType)) {
        return (dx == 0 || dy == 0) && PathClear(fromX, fromY, toX, toY, board);
    }
    if (IsBishop(movingType)) {
        return (adx == ady) && PathClear(fromX, fromY, toX, toY, board);
    }
    if (IsKnight(movingType)) {
        return (adx == 1 && ady == 2) || (adx == 2 && ady == 1);
    }
    if (IsPawn(movingType)) {
        int dir = (moverColor == PieceColor::White) ? -1 : +1;
        int startRow = (moverColor == PieceColor::White) ? 6 : 1;

        // capture
        if (dy == dir && std::abs(dx) == 1 && targetType != PieceType::None && targetColor != moverColor)
            return true;

        // forward
        if (dx == 0 && dy == dir && targetType == PieceType::None)
            return true;

        // double forward
        if (dx == 0 && dy == 2 * dir && fromY == startRow &&
            board[fromY + dir][fromX].GetPiece().GetPieceType() == PieceType::None &&
            targetType == PieceType::None)
            return true;

        // --- en passant ---
        int enPassantRow = (moverColor == PieceColor::White) ? 3 : 4;
        if (fromY == enPassantRow && dy == dir && std::abs(dx) == 1 && targetType == PieceType::None) {
            if (toX == lastPawnDoubleStepX && fromY == lastPawnDoubleStepY) {
                return true;
            }
        }

        return false;
    }

    return false;
}

// ----------------- Position / Check / Checkmate -----------------

bool Game::findSquarePosition(Square& target, Square board[8][8], int& outX, int& outY) {
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

bool Game::IsInCheck(PieceColor kingColor, Square board[8][8]) {
    Square* kingSquare = nullptr;
    for (int y = 0; y < 8 && !kingSquare; ++y) {
        for (int x = 0; x < 8; ++x) {
            PieceType pt = board[y][x].GetPiece().GetPieceType();
            if ((kingColor == PieceColor::White && pt == PieceType::WhiteKing) ||
                (kingColor == PieceColor::Black && pt == PieceType::BlackKing)) {
                kingSquare = &board[y][x];
                break;
            }
        }
    }
    if (!kingSquare) return false;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            PieceType pt = board[y][x].GetPiece().GetPieceType();
            if (pt != PieceType::None && GetColorFromType(pt) != kingColor) {
                if (IsValidMove(board[y][x], *kingSquare, board)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Game::IsCheckmate(PieceColor kingColor, Square board[8][8]) {
    if (!IsInCheck(kingColor, board)) return false;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            PieceType pt = board[y][x].GetPiece().GetPieceType();
            if (pt == PieceType::None || GetColorFromType(pt) != kingColor) continue;

            for (int ty = 0; ty < 8; ++ty) {
                for (int tx = 0; tx < 8; ++tx) {
                    if (x == tx && y == ty) continue;
                    if (IsValidMove(board[y][x], board[ty][tx], board)) {
                        Square copy[8][8];
                        CopyBoard(board, copy);
                        copy[ty][tx].SetPiece(copy[y][x].GetPiece());
                        copy[y][x].SetPiece(Piece(PieceType::None));
                        if (!IsInCheck(kingColor, copy)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Game::CopyBoard(Square src[8][8], Square dest[8][8]) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            dest[y][x].SetPiece(src[y][x].GetPiece());
            dest[y][x].SetRect(src[y][x].GetRect());
            dest[y][x].state = src[y][x].state;
        }
    }
}
