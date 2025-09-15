#include "pch.h"
#include "Player.h"
#include "string"

Player::Player(PieceColor c)
{
	color = c;
}

PieceColor Player::GetColor()
{
	return color;
}

bool Player::operator==(const Player& other) const {
	return this->color == other.color;
}