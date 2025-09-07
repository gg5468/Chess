#include "pch.h"
#include "Game.h"
#include "Chessboard.h"

Game::Game()
{
	chessboard = std::make_unique<Chessboard>();
}
Game::~Game()
{
}

void Game::Draw(HDC dc, RECT client_rect)
{
	const int labelMargin = 40;

	chessboard -> DrawBoard(dc, client_rect);
}

bool Game::OnLButtonDown(CPoint point)
{
    
    selected_square = chessboard->OnLButtonDown(point);
	if (!selected_square) {
		return false;
	}

    if (piece_in_hand) {
        selected_square->SetPiece(piece_in_hand -> GetPiece());
	    piece_in_hand -> SetPiece(Piece(PieceType::None));
	    piece_in_hand = nullptr;
    }
	else {
        if (selected_square->GetPiece().GetPieceType() == PieceType::None) {
            return false;
        }
        piece_in_hand = selected_square;
       
	}

	return true;
}