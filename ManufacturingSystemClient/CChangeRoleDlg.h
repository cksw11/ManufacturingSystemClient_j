#pragma once
#include "afxdialogex.h"
#include "afxmenubutton.h"

// CChangeRoleDlg 대화 상자

class CChangeRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeRoleDlg)
public:
	CChangeRoleDlg(CWnd* pParent = nullptr);
	virtual ~CChangeRoleDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGE_ROLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog(); // 다이얼로그 초기화 함수 선언

protected:
	DECLARE_MESSAGE_MAP()

public:
	// 컨트롤 변수 및 데이터 저장을 위한 변수들
//	CMFCMenuButton m_btnRole;
	CString m_strCurrentRole;  // 이전 창에서 전달받을 현재 역할
	CString m_strSelectedRole; // 사용자가 선택한 새 역할

	// 메뉴 항목 클릭 시 호출될 함수들
	
	afx_msg void OnRoleAdmin();
	afx_msg void OnRoleStaff();
	afx_msg void OnRoleViewer();
	int m_nRoleType;
};
