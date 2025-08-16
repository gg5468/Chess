#include "pch.h"
#include "Chessboard.h"
#include "Square.h"
#include "Piece.h"
#include <iostream>
#include "FEN.h"
#include<string>

Chessboard::Chessboard()
{
	setPieces();
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

	auto white = RGB(205, 170, 125);


	auto black = RGB(101, 67, 33);

	HBRUSH white_brush = CreateSolidBrush(white);
	HBRUSH black_brush = CreateSolidBrush(black);

	HPEN white_pen = CreatePen(PS_SOLID, 0.8, white);
	HPEN black_pen = CreatePen(PS_SOLID, 0.8, black);


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
			RECT r = { width * j, height * i, (width * j) + width, (height * i) + height };

			if ((j + i) % 2 == 0) {
				Squares[i][j] = new Square(r, &Pieces[i][j]);

				Squares[i][j]->DrawSquare(dc, client_rect, white_brush, white_pen);
			}
			else {
				;
				Squares[i][j] = new Square(r, &Pieces[i][j]);

				Squares[i][j]->DrawSquare(dc, client_rect, black_brush, black_pen);
			}
		}
	}

	SelectObject(dc, old_font);

	DeleteObject(white_brush);
	DeleteObject(black_brush);

	DeleteObject(white_pen);
	DeleteObject(black_pen);
}


void Chessboard::setPieces()
{
	auto p = FEN::GetPieces(fen_notation);

	int skip = 0;
	int i = 0;
	int j = 0;

	int c = 0;

	while (c < p.length()) {
		if (skip > 0) {
			Pieces[i][j] = Piece::None;
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
		case 'r': Pieces[i][j] = Piece::BlackRook; break;
		case 'n': Pieces[i][j] = Piece::BlackKnight; break;
		case 'b': Pieces[i][j] = Piece::BlackBishop; break;
		case 'q': Pieces[i][j] = Piece::BlackQueen; break;
		case 'k': Pieces[i][j] = Piece::BlackKing; break;
		case 'p': Pieces[i][j] = Piece::BlackPawn; break;
		case 'R': Pieces[i][j] = Piece::WhiteRook; break;
		case 'N': Pieces[i][j] = Piece::WhiteKnight;; break;
		case 'B': Pieces[i][j] = Piece::WhiteBishop; break;
		case 'Q': Pieces[i][j] = Piece::WhiteQueen; break;
		case 'K': Pieces[i][j] = Piece::WhiteKing; break;
		case 'P': Pieces[i][j] = Piece::WhitePawn; break;
		}
		
		c++;
		j++;

	}

}
