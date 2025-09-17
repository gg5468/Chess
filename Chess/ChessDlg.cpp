#include "pch.h"
#include "framework.h"
#include "Chess.h"
#include "ChessDlg.h"
#include "afxdialogex.h"
#include "Game.h"
#include "PromotionDialog.h"


CChessDlg::CChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHESS_DIALOG, pParent),
	game(*this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CChessDlg::DrawGame()
{
	CRect client_rect;
	GetClientRect(&client_rect);
	CPaintDC dc(this);

	game.Draw(dc, client_rect);
}

void CChessDlg::RedrawGame()
{
	CRect client_rect;
	GetClientRect(&client_rect);
	CPaintDC dc(this);

	game.Redraw(dc, client_rect);
	Invalidate();
}

void CChessDlg::PromotePawn(int x, int y)
{
	Square* sq = game.GetChessboardSquare(x,y);
	PieceType oldType = sq->GetPiece().GetPieceType();
	PieceColor color = (oldType <= PieceType::WhiteKing) ? PieceColor::White : PieceColor::Black;

	PromotionDialog dlg(this);

	if (dlg.DoModal() == IDOK)
	{
		PieceType newType;
		switch (dlg.choice)
		{
		case 0: newType = (color == PieceColor::White) ? PieceType::WhiteQueen : PieceType::BlackQueen; break;
		case 1: newType = (color == PieceColor::White) ? PieceType::WhiteRook : PieceType::BlackRook; break;
		case 2: newType = (color == PieceColor::White) ? PieceType::WhiteBishop : PieceType::BlackBishop; break;
		case 3: newType = (color == PieceColor::White) ? PieceType::WhiteKnight : PieceType::BlackKnight; break;
		default: newType = oldType; break;
		}

		game.PromotePawn(sq, newType);
		
		Invalidate(); 
	}
}



// CChessDlg message handlers

BOOL CChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	

	ShowWindow(SW_SHOWNORMAL);


	return TRUE; 
}

BOOL CChessDlg::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CChessDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(&rect);

	int dimensions = 8;

	int height = rect.bottom / dimensions;
	int width = rect.right / dimensions;

	int boardWidth = rect.right - rect.left;
	int boardHeight = rect.bottom - rect.top;

	int squareWidth = boardWidth / dimensions;
	int squareHeight = boardHeight / dimensions;


	for (int i = 0; i < dimensions; ++i) {
		for (int j = 0; j < dimensions; ++j) {

			Square* sq = game.GetChessboardSquare(i,j);

			RECT r = {
				rect.left + j * squareWidth,
				rect.top + i * squareHeight,
				rect.left + (j + 1) * squareWidth,
				rect.top + (i + 1) * squareHeight
			};

			sq->SetRect(r);
		}
	}

	Invalidate();
}

void CChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	try {
		game.OnLButtonDown(point);
	}
	catch (const std::exception& e) {
		AfxMessageBox((CString)e.what(), MB_ICONERROR);
		return;
	}
	Invalidate();
}


void CChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawGame();
	}
}


HCURSOR CChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

