#include "pch.h"
#include "Chessboard.h"
#include "Square.h"
#include "Piece.h"
#include <iostream>
#include "fen.h"
#include<string>

Chessboard::Chessboard()
{
	SetPieces();
}

Chessboard::~Chessboard()
{
	// Destructor implementation
}

void Chessboard::DrawBoard(HDC dc, RECT client_rect)
{

	SetBkMode(dc, TRANSPARENT);

	int dimensions = 8;

	int height = client_rect.bottom / dimensions;
	int width = client_rect.right / dimensions;

	int boardWidth = client_rect.right - client_rect.left;
	int boardHeight = client_rect.bottom - client_rect.top;

	int squareWidth = boardWidth / dimensions;
	int squareHeight = boardHeight / dimensions;


	auto white = RGB(205, 170, 125);


	auto black = RGB(101, 67, 33);

	HBRUSH white_brush = CreateSolidBrush(white);
	HBRUSH black_brush = CreateSolidBrush(black);

	HPEN selected_pen = CreatePen(PS_SOLID, 3, RGB(123,526,233));

	int fontPointSize = (height + width) / 6;
	int logicalHeight = -MulDiv(fontPointSize, GetDeviceCaps(dc, LOGPIXELSY), 72);
	CFont m_dynamicFont;
	m_dynamicFont.CreateFont(
		logicalHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Segoe UI Symbol"
	);

	auto old_font = SelectObject(dc, m_dynamicFont);

	for (int i = 0; i < dimensions; ++i) {
		for (int j = 0; j < dimensions; ++j) {
			RECT r = {
				client_rect.left + j * squareWidth,
				client_rect.top + i * squareHeight,
				client_rect.left + (j + 1) * squareWidth,
				client_rect.top + (i + 1) * squareHeight
			};

			Square* sq = &squares[i][j];
			if (sq -> state == SquareState::Uninitialized) {
				sq -> state = SquareState::Initialized;
				sq -> SetPiece(pieces[i][j]);
			}

			Square square = squares[i][j];

			if ((j + i) % 2 == 0) {
				sq -> DrawSquare(dc, r, white_brush, selected_pen);
			}
			else {

				sq -> DrawSquare(dc, r, black_brush, selected_pen);
			}
		}
	}

	SelectObject(dc, old_font);

	DeleteObject(white_brush);
	DeleteObject(black_brush);

	DeleteObject(selected_pen);
}


void Chessboard::SetPieces()
{
	auto p = fen::GetPieces(fen_notation);

	int skip = 0;
	int i = 0;
	int j = 0;

	int c = 0;

	while (c < p.length() + 1) {
		if (skip > 0) {
			pieces[i][j] = Piece(PieceType::None);
			skip--;
			j++;

			continue;
		}
		if (std::isdigit(p[c])) {
			skip = p[c] - '0';

			c++;
			continue;
		}
		if (p[c] == '/') {
			i++;
			j = 0;
			c++;
			continue;
		}

		switch (p[c]) {
			case 'r': pieces[i][j] = Piece(PieceType::BlackRook); break;
			case 'n': pieces[i][j] = Piece(PieceType::BlackKnight); break;
			case 'b': pieces[i][j] = Piece(PieceType::BlackBishop); break;
			case 'q': pieces[i][j] = Piece(PieceType::BlackQueen); break;
			case 'k': pieces[i][j] = Piece(PieceType::BlackKing); break;
			case 'p': pieces[i][j] = Piece(PieceType::BlackPawn); break;
			case 'R': pieces[i][j] = Piece(PieceType::WhiteRook); break;
			case 'N': pieces[i][j] = Piece(PieceType::WhiteKnight);; break;
			case 'B': pieces[i][j] = Piece(PieceType::WhiteBishop); break;
			case 'Q': pieces[i][j] = Piece(PieceType::WhiteQueen); break;
			case 'K': pieces[i][j] = Piece(PieceType::WhiteKing); break;
			case 'P': pieces[i][j] = Piece(PieceType::WhitePawn); break;
		}
		
		c++;
		j++;

	}

}

Square* Chessboard::OnLButtonDown(CPoint point) {
	return FindSquareWithPoint(point);
}

Square* Chessboard::FindSquareWithPoint(CPoint point) {
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			Square* sq = &squares[row][col];

			if (PtInRect(&sq->GetRect(), point)) {
				return sq;
			}

		}
	}

	return nullptr;
}