#include "pch.h"
#include "ManufacturingSystemClient.h"
#include "afxdialogex.h"
#include "CChangeRoleDlg.h"
#include "resource.h" // 소문자로 변경

// CChangeRoleDlg 대화 상자

IMPLEMENT_DYNAMIC(CChangeRoleDlg, CDialogEx)

CChangeRoleDlg::CChangeRoleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGE_ROLE, pParent)
	, m_nRoleType(0) // m_nRoleType 변수 초기화
{

}

CChangeRoleDlg::~CChangeRoleDlg()
{
}

void CChangeRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 라디오 버튼 그룹과 정수형 변수 m_nRoleType을 연결합니다.
	DDX_Radio(pDX, IDC_RADIO_ADMIN, m_nRoleType);
}

// 메시지 맵은 비워둡니다.
BEGIN_MESSAGE_MAP(CChangeRoleDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeRoleDlg 메시지 처리기

// OnInitDialog는 특별한 초기화 작업이 없으므로 비워둡니다.
BOOL CChangeRoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}