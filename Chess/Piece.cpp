#include "pch.h"
#include "Piece.h"

Piece::Piece() {
    pieceType = PieceType::Uninitialized;
}

Piece::Piece(PieceType pt) {
    pieceType = pt;
}

PieceType Piece::GetPieceType() {
    return pieceType;
}

LPCWSTR Piece::GetPieceSymbol() {
    switch (pieceType) {
    case PieceType::None:        return L"";          // IDS_PIECE_NONE
    case PieceType::WhitePawn:   return L"\u2659";    // IDS_WHITE_PAWN
    case PieceType::WhiteKnight: return L"\u2658";    // IDS_WHITE_KNIGHT
    case PieceType::WhiteBishop: return L"\u2657";    // IDS_WHITE_BISHOP
    case PieceType::WhiteRook:   return L"\u2656";    // IDS_WHITE_ROOK
    case PieceType::WhiteQueen:  return L"\u2655";    // IDS_WHITE_QUEEN
    case PieceType::WhiteKing:   return L"\u2654";    // IDS_WHITE_KING
    case PieceType::BlackPawn:   return L"\u265F";    // IDS_BLACK_PAWN
    case PieceType::BlackKnight: return L"\u265E";    // IDS_BLACK_KNIGHT
    case PieceType::BlackBishop: return L"\u265D";    // IDS_BLACK_BISHOP
    case PieceType::BlackRook:   return L"\u265C";    // IDS_BLACK_ROOK
    case PieceType::BlackQueen:  return L"\u265B";    // IDS_BLACK_QUEEN
    case PieceType::BlackKing:   return L"\u265A";    // IDS_BLACK_KING
    default:                     return L"?";         // IDS_PIECE_UNKNOWN
    }
}

PieceColor Piece::GetPieceColor() {
    switch (pieceType) {
    case PieceType::WhitePawn:
    case PieceType::WhiteKnight:
    case PieceType::WhiteBishop:
    case PieceType::WhiteRook:
    case PieceType::WhiteQueen:
    case PieceType::WhiteKing:
        return PieceColor::White;
    case PieceType::BlackPawn:
    case PieceType::BlackKnight:
    case PieceType::BlackBishop:
    case PieceType::BlackRook:
    case PieceType::BlackQueen:
    case PieceType::BlackKing:
        return PieceColor::Black;
    default:
        return PieceColor::None;
    }
}

bool Piece::IsPawn() { return pieceType == PieceType::WhitePawn || pieceType == PieceType::BlackPawn; }
bool Piece::IsKnight() { return pieceType == PieceType::WhiteKnight || pieceType == PieceType::BlackKnight; }
bool Piece::IsBishop() { return pieceType == PieceType::WhiteBishop || pieceType == PieceType::BlackBishop; }
bool Piece::IsRook() { return pieceType == PieceType::WhiteRook || pieceType == PieceType::BlackRook; }
bool Piece::IsQueen() { return pieceType == PieceType::WhiteQueen || pieceType == PieceType::BlackQueen; }
bool Piece::IsKing() { return pieceType == PieceType::WhiteKing || pieceType == PieceType::BlackKing; }