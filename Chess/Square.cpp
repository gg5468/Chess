#include "pch.h"
#include "Square.h"
#include "Piece.h"
#include <stdexcept>

Square::Square(Piece* p)
{
    piece = SetPiece(*p);
}

Square::Square()
{
}

Square& Square::operator=(const Square& other)
{
    if (this == &other)
        return *this;

    piece = other.piece;
    rect = other.rect;
    selected = other.selected;

    return *this;
}

void Square::DrawSquare(HDC dc,RECT r, HBRUSH brush, HPEN hPen)
{
	FillRect(dc, &r, brush);
    SetRect(r);
	DrawTextW(dc, piece.GetPieceSymbol(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (selected) {
		drawSelected(dc, r ,hPen);
	}
}

void Square::drawSelected(HDC dc, RECT r, HPEN hPen) {
    HPEN    hOldPen = (HPEN)SelectObject(dc, hPen);

    MoveToEx(dc, r.left, r.top, nullptr);
    LineTo(dc, r.right - 1, r.top);


    MoveToEx(dc, r.right - 1, r.top, nullptr);
    LineTo(dc, r.right - 1, r.bottom - 1);


    MoveToEx(dc, r.right - 1, r.bottom - 1, nullptr);
    LineTo(dc, r.left, r.bottom - 1);


    MoveToEx(dc, r.left, r.bottom - 1, nullptr);
    LineTo(dc, r.left, r.top);

    SelectObject(dc, hOldPen);
}