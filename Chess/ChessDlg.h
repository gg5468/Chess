
// ChessDlg.h : header file
//

#pragma once
#include "Game.h"

// CChessDlg dialog
class CChessDlg : public CDialogEx
{
// Construction
public:
	CChessDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
public:
	void RedrawGame();
	void PromotePawn(int x, int y);
protected:
	HICON m_hIcon;
	Game game = Game(this);
	void DrawGame();



	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg
	BOOL OnEraseBkgnd(CDC* pDC);
	void OnSize(UINT nType, int cx, int cy);
	void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
};
