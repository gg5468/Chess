#include "pch.h"
#include "framework.h"
#include "Chess.h"
#include "ChessDlg.h"
#include "afxdialogex.h"
#include "Game.h"
#include "PromotionDialog.h"


CChessDlg::CChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHESS_DIALOG, pParent)
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
	ON_MESSAGE(WM_USER_PROMOTE_PAWN, &CChessDlg::OnPromotePawn)
	ON_MESSAGE(WM_USER_REDRAW_GAME, &CChessDlg::OnRedrawGame)
END_MESSAGE_MAP()

void CChessDlg::DrawGame()
{
	CRect client_rect;
	GetClientRect(&client_rect);
	CPaintDC dc(this);

	game.Draw(dc, client_rect);
}

LRESULT CChessDlg::OnRedrawGame(WPARAM wParam, LPARAM lParam)
{
	game = Game(this);
	DrawGame();
	return 0;
}

LRESULT CChessDlg::OnPromotePawn(WPARAM wParam, LPARAM lParam)
{
	int x = static_cast<int>(wParam); 
	int y = static_cast<int>(lParam); 

	Square* sq = game.GetChessboardSquare(x,y);
	PieceType oldType = sq->GetPiece().GetPieceType();
	PieceColor color = (oldType <= PieceType::WhiteKing) ? PieceColor::White : PieceColor::Black;

	std::unique_ptr<PromotionDialog> dlg = std::make_unique<PromotionDialog>(this);

	if (dlg->DoModal() == IDOK)
	{
		PieceType newType;
		switch (dlg->choice)
		{
		case 0: newType = (color == PieceColor::White) ? PieceType::WhiteQueen : PieceType::BlackQueen; break;
		case 1: newType = (color == PieceColor::White) ? PieceType::WhiteRook : PieceType::BlackRook; break;
		case 2: newType = (color == PieceColor::White) ? PieceType::WhiteBishop : PieceType::BlackBishop; break;
		case 3: newType = (color == PieceColor::White) ? PieceType::WhiteKnight : PieceType::BlackKnight; break;
		default: newType = oldType; break;
		}

		sq->SetPiece(Piece(newType));
		Invalidate(); 
	}

	return 0;
}



// CChessDlg message handlers

BOOL CChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOWNORMAL);

	// TODO: Add extra initialization here
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

BOOL CChessDlg::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CChessDlg::OnSize(UINT nType, int cx, int cy)
{
	Invalidate();
}

void CChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	game.OnLButtonDown(point);
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



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

