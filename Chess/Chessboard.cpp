#include "pch.h"
#include "Chessboard.h"
#include "Square.h"
#include <iostream>

Chessboard::Chessboard()
{
	// Constructor implementation
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
	

	int fontPointSize = height / 2.5;
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
				Squares[i][j] = new Square(r);
              
				Squares[i][j]->DrawSquare(dc, client_rect, white_brush, white_pen);
			}
			else {
				RECT rect = { j,i,j + 1,i + 1 };
				Squares[i][j] = new Square(r);

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

