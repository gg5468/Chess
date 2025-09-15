#include "pch.h"
#include "PromotionDialog.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(PromotionDialog, CDialogEx)

BEGIN_MESSAGE_MAP(PromotionDialog, CDialogEx)
END_MESSAGE_MAP()

PromotionDialog::PromotionDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PROMOTION_DIALOG, pParent), m_radioChoice(0), choice(0)
{
}

BOOL PromotionDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_radioChoice = 0;
    return TRUE;
}

void PromotionDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Radio(pDX, IDC_RADIO_QUEEN, m_radioChoice);
}

void PromotionDialog::OnOK()
{
    UpdateData(TRUE);
    choice = m_radioChoice;
    CDialogEx::OnOK();    
}
