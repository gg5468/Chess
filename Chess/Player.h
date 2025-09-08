#pragma once
#include "string"
#include "Piece.h"

class Player
{
public:
	Player(PieceColor color);
	PieceColor GetColor();
private:
	PieceColor color;
};

