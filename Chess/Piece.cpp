#include "pch.h"
#include "Piece.h"

Piece::Piece() {
    pieceType = PieceType::None;
}

Piece::Piece(PieceType pt) {
    pieceType = pt;
}

PieceType Piece::GetPieceType() {
    return pieceType;
}

LPCWSTR Piece::GetPieceSymbol() {
    switch (pieceType) {
        case PieceType::None:        return L"";
        case PieceType::WhitePawn:   return L"\u2659";
        case PieceType::WhiteKnight: return L"\u2658";
        case PieceType::WhiteBishop: return L"\u2657";
        case PieceType::WhiteRook:   return L"\u2656";
        case PieceType::WhiteQueen:  return L"\u2655";
        case PieceType::WhiteKing:   return L"\u2654";
        case PieceType::BlackPawn:   return L"\u265F";
        case PieceType::BlackKnight: return L"\u265E";
        case PieceType::BlackBishop: return L"\u265D";
        case PieceType::BlackRook:   return L"\u265C";
        case PieceType::BlackQueen:  return L"\u265B";
        case PieceType::BlackKing:   return L"\u265A";
        default: return L"?";
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

