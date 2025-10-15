#pragma once
#include <afxdialogex.h>

class CUserSelectDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CUserSelectDlg)

public:
    CUserSelectDlg(CWnd* pParent = nullptr);   // 표준 생성자
    virtual ~CUserSelectDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_UserSelect };       // 대화상자 ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원
    DECLARE_MESSAGE_MAP()

public:
    // 버튼 이벤트 함수 선언
    // 버튼 이벤트 함수 선언
    CListCtrl m_userList;
    afx_msg void OnBnClickedButtonUserSelect();
    afx_msg void OnBnClickedButtonAddUser();
    afx_msg void OnBnClickedButtonDeleteUser();
    afx_msg void OnBnClickedButtonRollChange();
    CString m_strSearchKeyword;
    afx_msg void OnClickedButtonSearch();
    afx_msg void OnClickedButtonShowAll();
    void LoadUserList(CString strWhereClause = _T(""));
};
