#include "pch.h"
#include "Game.h"
#include "Chessboard.h"

Game::Game()
{
	chessboard = new Chessboard();
}
Game::~Game()
{
	chessboard = nullptr;
}

void Game::Draw(HDC dc, RECT client_rect)
{
	const int labelMargin = 40;

    RECT rectBottomLabels = {
     client_rect.left + labelMargin,
     client_rect.bottom - labelMargin,
     client_rect.right,
     client_rect.bottom
    };

   
    RECT rectLeftLabels = {
        client_rect.left,
        client_rect.bottom - labelMargin - (client_rect.bottom - client_rect.top - labelMargin),
        client_rect.left + labelMargin,
        client_rect.bottom - labelMargin
    };

    
    RECT rectBoard = {
        client_rect.left + labelMargin,
        client_rect.top + labelMargin,
        client_rect.right - labelMargin,
        client_rect.bottom - labelMargin
    };

    RECT rectTopLabels = {
        client_rect.left + labelMargin,       
        client_rect.top,                          
        client_rect.right,                  
        client_rect.top + labelMargin        
    };

    RECT rectRightLabels = {
        client_rect.right - labelMargin,
        client_rect.top + labelMargin,    
        client_rect.right,   
        client_rect.bottom - labelMargin
    };


	chessboard -> DrawBoard(dc, rectBoard);
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