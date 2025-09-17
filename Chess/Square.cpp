#include "pch.h"
#include "Square.h"
#include "Piece.h"
#include <stdexcept>

Square& Square::operator=(const Square& other)
{
    if (this == &other)
        return *this;

    piece = other.piece;
    rect = other.rect;
    selected = other.selected;

    return *this;
}

void Square::DrawSquare(HDC dc, HBRUSH brush, HPEN hPen)
{
	FillRect(dc, &rect, brush);
    SetRect(rect);
	DrawText(dc, piece.GetPieceSymbol(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (selected) {
		drawSelected(dc, rect ,hPen);
	}
}

void Square::drawSelected(HDC dc, RECT r, HPEN hPen) {
    HPEN hOldPen = (HPEN)SelectObject(dc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(dc, GetStockObject(NULL_BRUSH));

    Rectangle(dc, r.left, r.top, r.right, r.bottom);

    SelectObject(dc, hOldBrush);
    SelectObject(dc, hOldPen);
}
