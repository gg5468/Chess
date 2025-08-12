
// Chess.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChessApp:
// See Chess.cpp for the implementation of this class
//

class CChessApp : public CWinApp
{
public:
	CChessApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CChessApp theApp;
