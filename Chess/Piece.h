#pragma once

#include <string>

enum class Piece {
    None,
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing,
};

inline LPCWSTR GetPieceSymbol(Piece piece) {
    switch (piece) {
	case Piece::None:        return L"";
    case Piece::WhitePawn:   return L"\u2659";
    case Piece::WhiteKnight: return L"\u2658";
    case Piece::WhiteBishop: return L"\u2657";
    case Piece::WhiteRook:   return L"\u2656";
    case Piece::WhiteQueen:  return L"\u2655";
    case Piece::WhiteKing:   return L"\u2654";
    case Piece::BlackPawn:   return L"\u265F";
    case Piece::BlackKnight: return L"\u265E";
    case Piece::BlackBishop: return L"\u265D";
    case Piece::BlackRook:   return L"\u265C";
    case Piece::BlackQueen:  return L"\u265B";
    case Piece::BlackKing:   return L"\u265A";
    default: return L"?";
    }
}
