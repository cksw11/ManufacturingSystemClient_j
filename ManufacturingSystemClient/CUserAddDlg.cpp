// CUserAddDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ManufacturingSystemClient.h"
#include "afxdialogex.h"
#include "CUserAddDlg.h"


// CUserAddDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserAddDlg, CDialogEx)

CUserAddDlg::CUserAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_AddUser, pParent)
	, m_strName(_T(""))
	, m_strPassword(_T(""))
	, m_strEmail(_T(""))
	, m_strID(_T(""))
{

}

CUserAddDlg::~CUserAddDlg()
{
}

void CUserAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
}


BEGIN_MESSAGE_MAP(CUserAddDlg, CDialogEx)
END_MESSAGE_MAP()


// CUserAddDlg 메시지 처리기
