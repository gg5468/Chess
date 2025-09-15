#pragma once
#include "string"
#include "Piece.h"

class Player
{
public:
	Player(PieceColor color);
	PieceColor GetColor();
	bool operator==(const Player& other) const;
private:
	PieceColor color;
};

