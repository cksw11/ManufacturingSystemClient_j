// CAdminAuthDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ManufacturingSystemClient.h"
#include "afxdialogex.h"
#include "CAdminAuthDlg.h"


// CAdminAuthDlg 대화 상자

IMPLEMENT_DYNAMIC(CAdminAuthDlg, CDialogEx)

CAdminAuthDlg::CAdminAuthDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_AUTH, pParent)
	, m_strPassword(_T(""))
{

}

CAdminAuthDlg::~CAdminAuthDlg()
{
}

void CAdminAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CAdminAuthDlg, CDialogEx)
END_MESSAGE_MAP()


// CAdminAuthDlg 메시지 처리기
