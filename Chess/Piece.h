#pragma once

#include <string>
#include "memory.h"

enum class PieceType {
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
private:
	PieceType pieceType;
   
};




