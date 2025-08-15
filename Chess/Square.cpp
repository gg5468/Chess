#include "pch.h"
#include "Square.h"

Square::Square(RECT r)
{
    rect = r;
}

void Square::DrawSquare(HDC dc,RECT client_rect, HBRUSH brush, HPEN hPen)
{

	FillRect(dc, &rect, brush);
	

	DrawTextW(dc, L"\u2654", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    HPEN    hOldPen = (HPEN)SelectObject(dc, hPen);

    MoveToEx(dc, rect.left, rect.top, nullptr);
    LineTo(dc, rect.right, rect.top);

    MoveToEx(dc, rect.right, rect.top, nullptr);
    LineTo(dc, rect.right, rect.bottom);

    MoveToEx(dc, rect.right, rect.bottom, nullptr);
    LineTo(dc, rect.left, rect.bottom);

    MoveToEx(dc, rect.left, rect.bottom, nullptr);
    LineTo(dc, rect.left, rect.top);

    SelectObject(dc, hOldPen);
}
