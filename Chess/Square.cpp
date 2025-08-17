#include "pch.h"
#include "Square.h"
#include "Piece.h"

Square::Square(Piece* p)
{
	state = SquareState::Uninitialized;
    piece = *p;
}

void Square::DrawSquare(HDC dc,RECT r, HBRUSH brush, HPEN hPen)
{
	FillRect(dc, &r, brush);
    setRect(r);
	DrawTextW(dc, GetPieceSymbol(piece), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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