#include "pch.h"
#include "CUserSelectDlg.h"
#include "framework.h"
#include "afxdialogex.h"
#include "resource.h"
#include <afxdb.h>
#include "CUserAddDlg.h"
#include "CChangeRoleDlg.h"

IMPLEMENT_DYNAMIC(CUserSelectDlg, CDialogEx)

CUserSelectDlg::CUserSelectDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_UserSelect, pParent)
    , m_strSearchKeyword(_T(""))
{
}

CUserSelectDlg::~CUserSelectDlg()
{
}

void CUserSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearchKeyword);
    DDX_Control(pDX, IDC_USERLIST, m_userList); // ����Ʈ ��Ʈ�� ����
}

BEGIN_MESSAGE_MAP(CUserSelectDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_USER_SELECT, &CUserSelectDlg::OnBnClickedButtonUserSelect)
    ON_BN_CLICKED(IDC_BUTTON_ADD_USER, &CUserSelectDlg::OnBnClickedButtonAddUser)
    ON_BN_CLICKED(IDC_BUTTON_DELETE_USER, &CUserSelectDlg::OnBnClickedButtonDeleteUser)
    ON_BN_CLICKED(IDC_BUTTON_ROLL_CHANGE, &CUserSelectDlg::OnBnClickedButtonRollChange)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUserSelectDlg::OnClickedButtonSearch)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_ALL, &CUserSelectDlg::OnClickedButtonShowAll)
END_MESSAGE_MAP()
void CUserSelectDlg::OnBnClickedButtonUserSelect()
{
    // TODO: 1. ����Ʈ ��Ʈ�� ��� ����(m_userList)�� �߰��ؾ� �մϴ�.
    // Ŭ���� �� -> CUserSelectDlg ��Ŭ�� -> Ŭ���� ������ -> ��� ���� ��
    // -> IDC_USERLIST ���� -> ���� �߰� -> �̸�: m_userList, ����: Control, ���� ����: CListCtrl
    // �Ʒ� �ڵ�� m_userList ��� �̸����� ������ �߰��ߴٰ� �����մϴ�.

    // TODO: 2. ���� ���̺��� �÷� �̸����� �ٲ��ּ���.
    const int numColumns = 5; // users ���̺��� �÷� ����
    CString columnNames[numColumns] = { _T("user_id"), _T("username"), _T("email")/*, _T("password_hash")*/, _T("role"), _T("status") }; // users ���̺��� ���� �÷� �̸���

    // ����Ʈ ��Ʈ�� �ʱ�ȭ �� �÷� ��� ����
    m_userList.DeleteAllItems(); // ���� ��� ����
    while (m_userList.DeleteColumn(0)); // ���� �÷� ����

    // ����Ʈ ��Ʈ�ѿ� Full Row Select �� �׸��� ���� ��Ÿ�� ����
    m_userList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // �÷� ��� �߰�
    m_userList.InsertColumn(0, _T("���̵�"), LVCFMT_LEFT, 100);
    m_userList.InsertColumn(1, _T("�̸�"), LVCFMT_LEFT, 150);
    m_userList.InsertColumn(2, _T("�̸���"), LVCFMT_LEFT, 200);
    //m_userList.InsertColumn(3, _T("��й�ȣ"), LVCFMT_LEFT, 200);
    m_userList.InsertColumn(3, _T("����"), LVCFMT_LEFT, 200);
    m_userList.InsertColumn(4, _T("����"), LVCFMT_LEFT, 200);
    // ... �÷� ������ŭ �߰� ...

    // --- DB ���� ---
    CString DB_HOST = _T("localhost");
    CString DB_NAME = _T("projectserver"); // �����ͺ��̽� �̸� Ȯ��!
    CString DB_USER = _T("root");
    CString DB_PASS = _T("Moble"); // ���� ��й�ȣ

    CString sDsn;
    sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"),
        DB_HOST, DB_NAME, DB_USER, DB_PASS);

    CDatabase db;
    CRecordset rs(&db);

    try
    {
        // DB ����
        db.OpenEx(sDsn, CDatabase::noOdbcDialog);

        // TODO: 3. ���� ���̺� �̸����� �ٲ��ּ���.
        CString strSQL = _T("SELECT * FROM users");

        // ���� ����
        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        if (rs.IsBOF() && rs.IsEOF())
        {
            AfxMessageBox(_T("�����Ͱ� �����ϴ�."));
            rs.Close();
            db.Close();
            return;
        }

        // --- ����Ʈ ��Ʈ�ѿ� ������ �߰� ---
        int nItem;
        for (nItem = 0; !rs.IsEOF(); nItem++)
        {
            m_userList.InsertItem(nItem, _T("")); // ���� �� ���� �߰�

            for (int i = 0; i < numColumns; i++)
            {
                CString fieldValue;
                rs.GetFieldValue(columnNames[i], fieldValue); // �ʵ� �̸����� �� ��������
                m_userList.SetItemText(nItem, i, fieldValue); // �ش� ��, ���� ������ ����
            }
            rs.MoveNext(); // ���� ���ڵ�� �̵�
        }
        m_userList.SetColumnWidth(0, 50);
        m_userList.SetColumnWidth(1, LVSCW_AUTOSIZE);
        m_userList.SetColumnWidth(2, LVSCW_AUTOSIZE);
        m_userList.SetColumnWidth(3, LVSCW_AUTOSIZE);
        m_userList.SetColumnWidth(4, LVSCW_AUTOSIZE);
       // m_userList.SetColumnWidth(5, LVSCW_AUTOSIZE);
    }
    catch (CDBException* e)
    {
        AfxMessageBox(_T("���� �߻�: ") + e->m_strError);
        e->Delete();
    }

    // ���ҽ� ����
    if (rs.IsOpen())
        rs.Close();
    if (db.IsOpen())
        db.Close();
}


void CUserSelectDlg::OnBnClickedButtonAddUser()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CUserAddDlg dlg;

    // ����ڰ� 'Ȯ��' ��ư�� ������ ��쿡�� �Ʒ� �ڵ� ����
    if (dlg.DoModal() == IDOK)
    {
        // 2. ���̾�α׿��� ����ڰ� �Է��� ������ ������ ��������
        CString strName = dlg.m_strName;
        CString strID = dlg.m_strID;
        CString strEmail = dlg.m_strEmail;
        CString strPassword = dlg.m_strPassword;

        // 3. �Է°� ��ȿ�� �˻� (�ϳ��� ��� ������ �ߴ�)
        if (strName.IsEmpty() || strEmail.IsEmpty() || strPassword.IsEmpty())
        {
            AfxMessageBox(_T("��� �׸��� �Է����ּ���."));
            return;
        }

        // --- DB ���� �� INSERT ���� ���� ---
        CDatabase db;
        try
        {
            // DB ���� ���ڿ� (���� ȯ�濡 �°� Ȯ��)
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 4. SQL Injection ������ ���� ���� ����ǥ ó��
            strName.Replace(_T("'"), _T("''"));
            strEmail.Replace(_T("'"), _T("''"));
            strPassword.Replace(_T("'"), _T("''"));

            // 5. ������ INSERT SQL ������ �ۼ�
            // users ���̺�� �÷� �̸�(id, name, email, password, role, status)�� ���� DB�� �����ؾ� �մϴ�.
            CString strSQL;
            strSQL.Format(_T("INSERT INTO users (username, email, password_hash, role, status) VALUES ('%s', '%s', '%s', 'viewer', 'active')"),
                strName, strEmail, strPassword);

            // 6. SQL ���� ����
            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("����ڰ� ���������� �߰��Ǿ����ϴ�."));

            // 7. ���� ����Ʈ ���ΰ�ħ
            // ������ ����� ��ȸ �Լ��� �ٽ� ȣ���Ͽ� ��������� �ٷ� �ݿ�
            OnBnClickedButtonUserSelect();
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("�����ͺ��̽� ������ �߻��߽��ϴ�:\n") + e->m_strError);
            e->Delete();
        }

        // DB ������ ���������� �ݱ�
        if (db.IsOpen())
            db.Close();
    }
}

void CUserSelectDlg::OnBnClickedButtonDeleteUser()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    POSITION pos = m_userList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(_T("������ ����ڸ� ���� �������ּ���."));
        return;
    }

    // ���õ� �׸��� �ε����� �����ɴϴ�.
    int nSelectedItem = m_userList.GetNextSelectedItem(pos);

    // 2. ���õ� �׸��� '���̵�' ���� �����ɴϴ�. (0��° �÷�)
    // �� ID�� �������� DB���� �ش� ������ ã���ϴ�.
    CString strUserID = m_userList.GetItemText(nSelectedItem, 0);

    // 3. ����ڿ��� ���� ���θ� �ٽ� �ѹ� Ȯ���մϴ�.
    CString strMessage;
    strMessage.Format(_T("����ڸ� ������ �����Ͻðڽ��ϱ�? �� �۾��� �ǵ��� �� �����ϴ�."));

    // ����ڰ� '��(Y)'�� ������ ��쿡�� ���� ������ �����մϴ�.
    if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        // --- DB���� ���� ���� ---
        CDatabase db;
        try
        {
            // DB ����
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 4. SQL Injection ������ ���� ���� ����ǥ ó��
            strUserID.Replace(_T("'"), _T("''"));

            // 5. ID�� �������� �����ϴ� DELETE ������ �ۼ� (WHERE ���� �ſ� �߿�!)
            CString strSQL;
            strSQL.Format(_T("DELETE FROM users WHERE user_id = '%s'"), strUserID);

            // 6. SQL ����
            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("����ڰ� ���������� �����Ǿ����ϴ�."));

            // 7. ����Ʈ ���ΰ�ħ
            OnBnClickedButtonUserSelect(); // ��ȸ �Լ��� �ٽ� ȣ���Ͽ� ȭ�鿡 ������� �ݿ�
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("DB ����: ") + e->m_strError);
            e->Delete();
        }

        if (db.IsOpen())
            db.Close();
    }
}

void CUserSelectDlg::OnBnClickedButtonRollChange()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    POSITION pos = m_userList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(_T("������ ������ ����ڸ� ���� �������ּ���."));
        return;
    }
    int nSelectedItem = m_userList.GetNextSelectedItem(pos);

    // 2. ���õ� ������� ID�� ���� Role ��������
    CString strUserID = m_userList.GetItemText(nSelectedItem, 0);      // 0�� �÷�: ID
    CString strCurrentRole = m_userList.GetItemText(nSelectedItem, 4); // 4�� �÷�: Role (������ ���� ����Ʈ�� �°�)

    // 3. ���� ���� ���̾�α� ����
    CChangeRoleDlg roleDlg;

    // 4. ���̾�αװ� ������ ��, ���� ���ҿ� �´� ���� ��ư�� �̸� ���õǵ��� �ʱⰪ ����
    if (strCurrentRole.CompareNoCase(_T("admin")) == 0) {
        roleDlg.m_nRoleType = 0;
    }
    else if (strCurrentRole.CompareNoCase(_T("staff")) == 0) {
        roleDlg.m_nRoleType = 1;
    }
    else { // viewer
        roleDlg.m_nRoleType = 2;
    }

    // 5. ���̾�α� ����. ����ڰ� 'Ȯ��'�� ������ �Ʒ� �ڵ� ����
    if (roleDlg.DoModal() == IDOK)
    {
        // 6. ����ڰ� ������ �� ������ ���ڿ��� ��ȯ (0=admin, 1=staff, 2=viewer)
        CString strNewRole;
        if (roleDlg.m_nRoleType == 0) strNewRole = _T("admin");
        else if (roleDlg.m_nRoleType == 1) strNewRole = _T("staff");
        else strNewRole = _T("viewer");

        // ������ ������� �ʾҴٸ� �ƹ��͵� ���� ����
        if (strNewRole.CompareNoCase(strCurrentRole) == 0) {
            return;
        }

        // ==========================================================
        // ���� ������ �ο� ���� ��� �߰� ����
        // ==========================================================
        // ���� ���� �ο��� ������ 'admin'�̶��, DB�� ���� admin ���� Ȯ���մϴ�.
        if (strNewRole.CompareNoCase(_T("admin")) == 0)
        {
            CDatabase db_check;
            CRecordset rs_check(&db_check);
            try
            {
                CString sDsn;
                sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
                db_check.OpenEx(sDsn, CDatabase::noOdbcDialog);

                // ���� admin ������ ���� ������� ���� ���� ����
                CString strSQL_Count = _T("SELECT COUNT(*) FROM users WHERE role = 'admin'");

                rs_check.Open(CRecordset::forwardOnly, strSQL_Count, CRecordset::readOnly);

                // ���� ����� �ִ��� Ȯ��
                if (!rs_check.IsEOF())
                {
                    CString strCount;
                    rs_check.GetFieldValue((short)0, strCount);
                    int nAdminCount = _ttoi(strCount);

                    // admin ���� 3�� �̻��̸� ��� �޽����� ���� �۾��� �ߴ��մϴ�.
                    if (nAdminCount >= 3)
                    {
                        AfxMessageBox(_T("������(admin)�� �ִ� 3������� ������ �� �ֽ��ϴ�."), MB_ICONWARNING);
                        rs_check.Close();
                        db_check.Close();
                        return; // �Լ��� ���⼭ ����
                    }
                }
                rs_check.Close();
                db_check.Close();
            }
            catch (CDBException* e)
            {
                AfxMessageBox(_T("������ �ο� Ȯ�� �� DB ������ �߻��߽��ϴ�:\n") + e->m_strError);
                e->Delete();
                return; // ���� �߻� �ÿ��� �۾��� �ߴ��մϴ�.
            }
        }

        // --- DB�� UPDATE ���� ���� ---
        CDatabase db;
        try
        {
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 7. UPDATE ���� �ۼ� (WHERE ���� �ſ� �߿�!)
            CString strSQL;
            // ?? �߿�: WHERE ���� �÷� �̸��� `id`���� `user_id`���� DB���� �� Ȯ���ϼ���!
            strSQL.Format(_T("UPDATE users SET role = '%s' WHERE user_id = '%s'"), strNewRole, strUserID);

            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("����� ������ ���������� ����Ǿ����ϴ�."));

            // 8. ����Ʈ ���ΰ�ħ
            OnBnClickedButtonUserSelect(); // �� �Լ� �̸��� ��ȸ �Լ��� �´��� Ȯ���ϼ���.
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("DB ����: ") + e->m_strError);
            e->Delete();
        }
        if (db.IsOpen()) db.Close();
    }
}
// CUserSelectDlg.cpp

// ... (�ٸ� �Լ��� ����) ...

// ���� �� �Լ� ������ ��ü�� �����ؼ� ���Ͽ� �߰��մϴ�. ����
void CUserSelectDlg::LoadUserList(CString strWhereClause)
{
    // ����Ʈ ��Ʈ�� ������ ������ ���� ������Ʈ�� ��� �ߴ��մϴ�.
    m_userList.SetRedraw(FALSE);
    // ���� ����Ʈ ������ ��� �����մϴ�.
    m_userList.DeleteAllItems();

    // --- DB ���� �� ������ ��ȸ ---
    CDatabase db;
    CRecordset rs(&db);
    try
    {
        // DB ����
        CString sDsn;
        sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
        db.OpenEx(sDsn, CDatabase::noOdbcDialog);

        // SQL ������ �ۼ� (���ڷ� ���� WHERE ���� �ڿ� �ٿ��ݴϴ�)
        CString strSQL = _T("SELECT user_id, username, email, password_hash, role, status FROM users ") + strWhereClause;

        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        // CRecordset�� �ʵ� ������ �����ɴϴ�.
        short nFields = rs.GetODBCFieldCount();

        // ����Ʈ ��Ʈ�ѿ� ������ �߰�
        for (int nItem = 0; !rs.IsEOF(); nItem++)
        {
            // ù ��° �÷��� �������� �߰��մϴ�.
            CString fieldValue;
            rs.GetFieldValue((short)0, fieldValue);
            m_userList.InsertItem(nItem, fieldValue);

            // ������ �÷����� ä��ϴ�.
            for (short i = 1; i < nFields; i++)
            {
                rs.GetFieldValue(i, fieldValue);
                m_userList.SetItemText(nItem, i, fieldValue);
            }
            rs.MoveNext();
        }

        // �÷� ���� ���뿡 �°� �ڵ� ����
        for (int i = 0; i < m_userList.GetHeaderCtrl()->GetItemCount(); i++)
        {
            m_userList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        }
    }
    catch (CDBException* e)
    {
        AfxMessageBox(_T("DB ����: ") + e->m_strError);
        e->Delete();
    }

    // ���ҽ� ����
    if (rs.IsOpen()) rs.Close();
    if (db.IsOpen()) db.Close();

    // ����Ʈ ������Ʈ�� �簳�ϰ� ȭ�鿡 ���� ����� ǥ���մϴ�.
    m_userList.SetRedraw(TRUE);
}

void CUserSelectDlg::OnClickedButtonSearch()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // 1. ����Ʈ ��Ʈ���� ���� m_strSearchKeyword ��� ������ �����ɴϴ�.
    UpdateData(TRUE);

    // �˻�� ����ִ��� Ȯ���մϴ�.
    if (m_strSearchKeyword.IsEmpty())
    {
        AfxMessageBox(_T("�˻�� �Է����ּ���."));
        return;
    }

    // SQL Injection �⺻ ������ ���� ���� ����ǥ�� ó���մϴ�.
    CString strKeyword = m_strSearchKeyword;
    strKeyword.Replace(_T("'"), _T("''"));

    // 2. 'name' �÷����� LIKE �˻��� �ϴ� WHERE �� ���ڿ��� ����ϴ�.
    CString strWhereClause;
    strWhereClause.Format(_T("WHERE username LIKE '%%%s%%'"), strKeyword);

    // 3. ������ WHERE ���� ���ڷ� �Ѱ� ����� ����� ���ΰ�ħ�մϴ�.
    LoadUserList(strWhereClause);
}

void CUserSelectDlg::OnClickedButtonShowAll()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // 1. �˻��� ��� ������ ���ϴ�.
    m_strSearchKeyword.Empty();

    // 2. ����� ���� ���� ����Ʈ ��Ʈ�ѿ� �ݿ��Ͽ� �˻�â�� �����ϰ� �մϴ�.
    UpdateData(FALSE);

    // 3. WHERE �� ����(�� ���ڿ�) LoadUserList�� ȣ���Ͽ� ��ü ����� �ҷ��ɴϴ�.
    LoadUserList(_T(""));
}
