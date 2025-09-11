#pragma once

#include <string>
#include "memory.h"

enum class PieceType {
	Uninitialized,
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

enum class PieceColor {
    None,
    White,
    Black
};

class Piece {
public:
	Piece();
	Piece(PieceType pieceType);
	LPCWSTR GetPieceSymbol();
	PieceType GetPieceType();
	PieceColor GetPieceColor();

    bool GetHasMoved() { return hasMoved; }
    void SetHasMoved(bool moved) { hasMoved = moved; }

    bool IsPawn();
    bool IsKnight();
    bool IsBishop();
    bool IsRook();
    bool IsQueen();
    bool IsKing();

private:
    bool hasMoved = false;
    PieceType pieceType = PieceType::Uninitialized;
   
};




