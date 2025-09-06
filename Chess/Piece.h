#pragma once

#include <string>
#include "PieceType.h"

class Piece {
public:
	Piece();
	Piece(PieceType pieceType);
	LPCWSTR GetPieceSymbol();
	PieceType GetPieceType();
private:
	PieceType pieceType;
};



