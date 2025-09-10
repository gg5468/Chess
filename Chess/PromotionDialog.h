#pragma once
#include "afxwin.h"

class PromotionDialog : public CDialogEx
{
    DECLARE_DYNAMIC(PromotionDialog)

public:
    PromotionDialog(CWnd* pParent = nullptr);   // standard constructor
    virtual ~PromotionDialog();

    int choice;          // final user selection 0..3

protected:
    int m_radioChoice;   // internal mapping for DDX_Radio

    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};
