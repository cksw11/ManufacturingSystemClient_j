#pragma once
#include <afxdialogex.h>

class CUserSelectDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CUserSelectDlg)

public:
    CUserSelectDlg(CWnd* pParent = nullptr);   // ǥ�� ������
    virtual ~CUserSelectDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_UserSelect };       // ��ȭ���� ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����
    DECLARE_MESSAGE_MAP()

public:
    // ��ư �̺�Ʈ �Լ� ����
    // ��ư �̺�Ʈ �Լ� ����
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
