#include "pch.h"
#include "Chessboard.h"
#include "Square.h"
#include "Piece.h"
#include <iostream>
#include "fen.h"
#include <string>
#include <vector>

Chessboard::Chessboard()
    : squares(8, std::vector<Square>(8))
{
    SetPieces();
}

void Chessboard::DrawBoard(HDC dc, const RECT& client_rect)
{
    SetBkMode(dc, TRANSPARENT);

    int dimensions = 8;

    int height = client_rect.bottom / dimensions;
    int width = client_rect.right / dimensions;

    int fontPointSize = (height + width) / 6;
    int logicalHeight = -MulDiv(fontPointSize, GetDeviceCaps(dc, LOGPIXELSY), 72);

    auto white = RGB(205, 170, 125);
    auto black = RGB(101, 67, 33);

    HBRUSH white_brush = CreateSolidBrush(white);
    HBRUSH black_brush = CreateSolidBrush(black);

    HPEN selected_pen = CreatePen(PS_SOLID, 3, RGB(0,200,4));

    CFont m_dynamicFont;
    m_dynamicFont.CreateFont(
        logicalHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI Symbol"
    );

    auto old_font = SelectObject(dc, m_dynamicFont);

    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {

            Square* sq = &squares[i][j];

            if ((j + i) % 2 == 0) {
                sq->DrawSquare(dc, white_brush, selected_pen);
            }
            else {
                sq->DrawSquare(dc, black_brush, selected_pen);
            }
        }
    }

    SelectObject(dc, old_font);

    DeleteObject(white_brush);
    DeleteObject(black_brush);

    DeleteObject(selected_pen);
}

void Chessboard::SetPieces()
{
    auto p = fen::GetPieces(fen_notation);

    int skip = 0;
    int i = 0;
    int j = 0;

    int c = 0;

    while (c < p.length() + 1) {
        if (skip > 0) {
            squares[i][j].SetPiece(PieceType::None);
            skip--;
            j++;

            continue;
        }
        if (std::isdigit(p[c])) {
            skip = p[c] - '0';

            c++;
            continue;
        }
        if (p[c] == '/') {
            i++;
            j = 0;
            c++;
            continue;
        }

        switch (p[c]) {
            case 'r': squares[i][j].SetPiece(Piece(PieceType::BlackRook)); break;
            case 'n': squares[i][j].SetPiece(Piece(PieceType::BlackKnight)); break;
            case 'b': squares[i][j].SetPiece(Piece(PieceType::BlackBishop)); break;
            case 'q': squares[i][j].SetPiece(Piece(PieceType::BlackQueen)); break;
            case 'k': squares[i][j].SetPiece(Piece(PieceType::BlackKing)); break;
            case 'p': squares[i][j].SetPiece(Piece(PieceType::BlackPawn)); break;
            case 'R': squares[i][j].SetPiece(Piece(PieceType::WhiteRook)); break;
            case 'N': squares[i][j].SetPiece(Piece(PieceType::WhiteKnight)); break;
            case 'B': squares[i][j].SetPiece(Piece(PieceType::WhiteBishop)); break;
            case 'Q': squares[i][j].SetPiece(Piece(PieceType::WhiteQueen)); break;
            case 'K': squares[i][j].SetPiece(Piece(PieceType::WhiteKing)); break;
            case 'P': squares[i][j].SetPiece(Piece(PieceType::WhitePawn)); break;
        }

        c++;
        j++;
    }
}

Square* Chessboard::GetSquare(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw std::out_of_range("GetSquare: indices out of range");
    }
    return &squares[row][col];
}

Square* Chessboard::OnLButtonDown(const CPoint& point) {
    return FindSquareWithPoint(point);
}

Square* Chessboard::FindSquareWithPoint(const CPoint& point) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Square* sq = &squares[row][col];

            if (PtInRect(&sq->GetRect(), point)) {
                return sq;
            }
        }
    }
    return nullptr;
}

void Chessboard::Reset() {
    for (auto& row : squares) {
        for (auto& s : row) {
            s.SetPiece(PieceType::None);
        }
    }

    SetPieces();
}