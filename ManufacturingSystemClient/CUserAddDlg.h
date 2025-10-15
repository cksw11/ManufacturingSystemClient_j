#pragma once
#include "afxdialogex.h"
#include "CUserSelectDlg.h"

// CUserAddDlg 대화 상자

class CUserAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserAddDlg)

public:
	CUserAddDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserAddDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddUser };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strPassword;
	CString m_strEmail;
	CString m_strID;
};
