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
    DDX_Control(pDX, IDC_USERLIST, m_userList); // 리스트 컨트롤 연결
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
    // TODO: 1. 리스트 컨트롤 멤버 변수(m_userList)를 추가해야 합니다.
    // 클래스 뷰 -> CUserSelectDlg 우클릭 -> 클래스 마법사 -> 멤버 변수 탭
    // -> IDC_USERLIST 선택 -> 변수 추가 -> 이름: m_userList, 범주: Control, 변수 형식: CListCtrl
    // 아래 코드는 m_userList 라는 이름으로 변수를 추가했다고 가정합니다.

    // TODO: 2. 실제 테이블의 컬럼 이름으로 바꿔주세요.
    const int numColumns = 5; // users 테이블의 컬럼 개수
    CString columnNames[numColumns] = { _T("user_id"), _T("username"), _T("email")/*, _T("password_hash")*/, _T("role"), _T("status") }; // users 테이블의 실제 컬럼 이름들

    // 리스트 컨트롤 초기화 및 컬럼 헤더 설정
    m_userList.DeleteAllItems(); // 기존 목록 삭제
    while (m_userList.DeleteColumn(0)); // 기존 컬럼 삭제

    // 리스트 컨트롤에 Full Row Select 및 그리드 라인 스타일 적용
    m_userList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // 컬럼 헤더 추가
    m_userList.InsertColumn(0, _T("아이디"), LVCFMT_LEFT, 100);
    m_userList.InsertColumn(1, _T("이름"), LVCFMT_LEFT, 150);
    m_userList.InsertColumn(2, _T("이메일"), LVCFMT_LEFT, 200);
    //m_userList.InsertColumn(3, _T("비밀번호"), LVCFMT_LEFT, 200);
    m_userList.InsertColumn(3, _T("권한"), LVCFMT_LEFT, 200);
    m_userList.InsertColumn(4, _T("상태"), LVCFMT_LEFT, 200);
    // ... 컬럼 개수만큼 추가 ...

    // --- DB 연결 ---
    CString DB_HOST = _T("localhost");
    CString DB_NAME = _T("projectserver"); // 데이터베이스 이름 확인!
    CString DB_USER = _T("root");
    CString DB_PASS = _T("Moble"); // 실제 비밀번호

    CString sDsn;
    sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"),
        DB_HOST, DB_NAME, DB_USER, DB_PASS);

    CDatabase db;
    CRecordset rs(&db);

    try
    {
        // DB 열기
        db.OpenEx(sDsn, CDatabase::noOdbcDialog);

        // TODO: 3. 실제 테이블 이름으로 바꿔주세요.
        CString strSQL = _T("SELECT * FROM users");

        // 쿼리 실행
        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        if (rs.IsBOF() && rs.IsEOF())
        {
            AfxMessageBox(_T("데이터가 없습니다."));
            rs.Close();
            db.Close();
            return;
        }

        // --- 리스트 컨트롤에 데이터 추가 ---
        int nItem;
        for (nItem = 0; !rs.IsEOF(); nItem++)
        {
            m_userList.InsertItem(nItem, _T("")); // 먼저 빈 행을 추가

            for (int i = 0; i < numColumns; i++)
            {
                CString fieldValue;
                rs.GetFieldValue(columnNames[i], fieldValue); // 필드 이름으로 값 가져오기
                m_userList.SetItemText(nItem, i, fieldValue); // 해당 행, 열에 데이터 설정
            }
            rs.MoveNext(); // 다음 레코드로 이동
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
        AfxMessageBox(_T("오류 발생: ") + e->m_strError);
        e->Delete();
    }

    // 리소스 정리
    if (rs.IsOpen())
        rs.Close();
    if (db.IsOpen())
        db.Close();
}


void CUserSelectDlg::OnBnClickedButtonAddUser()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CUserAddDlg dlg;

    // 사용자가 '확인' 버튼을 눌렀을 경우에만 아래 코드 실행
    if (dlg.DoModal() == IDOK)
    {
        // 2. 다이얼로그에서 사용자가 입력한 값들을 변수로 가져오기
        CString strName = dlg.m_strName;
        CString strID = dlg.m_strID;
        CString strEmail = dlg.m_strEmail;
        CString strPassword = dlg.m_strPassword;

        // 3. 입력값 유효성 검사 (하나라도 비어 있으면 중단)
        if (strName.IsEmpty() || strEmail.IsEmpty() || strPassword.IsEmpty())
        {
            AfxMessageBox(_T("모든 항목을 입력해주세요."));
            return;
        }

        // --- DB 연결 및 INSERT 쿼리 실행 ---
        CDatabase db;
        try
        {
            // DB 연결 문자열 (본인 환경에 맞게 확인)
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 4. SQL Injection 방지를 위한 작은 따옴표 처리
            strName.Replace(_T("'"), _T("''"));
            strEmail.Replace(_T("'"), _T("''"));
            strPassword.Replace(_T("'"), _T("''"));

            // 5. 실행할 INSERT SQL 쿼리문 작성
            // users 테이블과 컬럼 이름(id, name, email, password, role, status)은 실제 DB와 동일해야 합니다.
            CString strSQL;
            strSQL.Format(_T("INSERT INTO users (username, email, password_hash, role, status) VALUES ('%s', '%s', '%s', 'viewer', 'active')"),
                strName, strEmail, strPassword);

            // 6. SQL 쿼리 실행
            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("사용자가 성공적으로 추가되었습니다."));

            // 7. 메인 리스트 새로고침
            // 기존의 사용자 조회 함수를 다시 호출하여 변경사항을 바로 반영
            OnBnClickedButtonUserSelect();
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("데이터베이스 오류가 발생했습니다:\n") + e->m_strError);
            e->Delete();
        }

        // DB 연결이 열려있으면 닫기
        if (db.IsOpen())
            db.Close();
    }
}

void CUserSelectDlg::OnBnClickedButtonDeleteUser()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    POSITION pos = m_userList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(_T("삭제할 사용자를 먼저 선택해주세요."));
        return;
    }

    // 선택된 항목의 인덱스를 가져옵니다.
    int nSelectedItem = m_userList.GetNextSelectedItem(pos);

    // 2. 선택된 항목의 '아이디' 값을 가져옵니다. (0번째 컬럼)
    // 이 ID를 기준으로 DB에서 해당 유저를 찾습니다.
    CString strUserID = m_userList.GetItemText(nSelectedItem, 0);

    // 3. 사용자에게 삭제 여부를 다시 한번 확인합니다.
    CString strMessage;
    strMessage.Format(_T("사용자를 정말로 삭제하시겠습니까? 이 작업은 되돌릴 수 없습니다."));

    // 사용자가 '예(Y)'를 눌렀을 경우에만 삭제 로직을 실행합니다.
    if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        // --- DB에서 삭제 실행 ---
        CDatabase db;
        try
        {
            // DB 연결
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 4. SQL Injection 방지를 위한 작은 따옴표 처리
            strUserID.Replace(_T("'"), _T("''"));

            // 5. ID를 기준으로 삭제하는 DELETE 쿼리문 작성 (WHERE 절이 매우 중요!)
            CString strSQL;
            strSQL.Format(_T("DELETE FROM users WHERE user_id = '%s'"), strUserID);

            // 6. SQL 실행
            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("사용자가 성공적으로 삭제되었습니다."));

            // 7. 리스트 새로고침
            OnBnClickedButtonUserSelect(); // 조회 함수를 다시 호출하여 화면에 변경사항 반영
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("DB 오류: ") + e->m_strError);
            e->Delete();
        }

        if (db.IsOpen())
            db.Close();
    }
}

void CUserSelectDlg::OnBnClickedButtonRollChange()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    POSITION pos = m_userList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(_T("권한을 변경할 사용자를 먼저 선택해주세요."));
        return;
    }
    int nSelectedItem = m_userList.GetNextSelectedItem(pos);

    // 2. 선택된 사용자의 ID와 현재 Role 가져오기
    CString strUserID = m_userList.GetItemText(nSelectedItem, 0);      // 0번 컬럼: ID
    CString strCurrentRole = m_userList.GetItemText(nSelectedItem, 4); // 4번 컬럼: Role (순서는 본인 리스트에 맞게)

    // 3. 역할 변경 다이얼로그 생성
    CChangeRoleDlg roleDlg;

    // 4. 다이얼로그가 열리기 전, 현재 역할에 맞는 라디오 버튼이 미리 선택되도록 초기값 설정
    if (strCurrentRole.CompareNoCase(_T("admin")) == 0) {
        roleDlg.m_nRoleType = 0;
    }
    else if (strCurrentRole.CompareNoCase(_T("staff")) == 0) {
        roleDlg.m_nRoleType = 1;
    }
    else { // viewer
        roleDlg.m_nRoleType = 2;
    }

    // 5. 다이얼로그 실행. 사용자가 '확인'을 누르면 아래 코드 진행
    if (roleDlg.DoModal() == IDOK)
    {
        // 6. 사용자가 선택한 새 역할을 문자열로 변환 (0=admin, 1=staff, 2=viewer)
        CString strNewRole;
        if (roleDlg.m_nRoleType == 0) strNewRole = _T("admin");
        else if (roleDlg.m_nRoleType == 1) strNewRole = _T("staff");
        else strNewRole = _T("viewer");

        // 역할이 변경되지 않았다면 아무것도 하지 않음
        if (strNewRole.CompareNoCase(strCurrentRole) == 0) {
            return;
        }

        // ==========================================================
        // ▼▼▼ 관리자 인원 제한 기능 추가 ▼▼▼
        // ==========================================================
        // 만약 새로 부여할 역할이 'admin'이라면, DB의 현재 admin 수를 확인합니다.
        if (strNewRole.CompareNoCase(_T("admin")) == 0)
        {
            CDatabase db_check;
            CRecordset rs_check(&db_check);
            try
            {
                CString sDsn;
                sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
                db_check.OpenEx(sDsn, CDatabase::noOdbcDialog);

                // 현재 admin 역할을 가진 사용자의 수를 세는 쿼리
                CString strSQL_Count = _T("SELECT COUNT(*) FROM users WHERE role = 'admin'");

                rs_check.Open(CRecordset::forwardOnly, strSQL_Count, CRecordset::readOnly);

                // 쿼리 결과가 있는지 확인
                if (!rs_check.IsEOF())
                {
                    CString strCount;
                    rs_check.GetFieldValue((short)0, strCount);
                    int nAdminCount = _ttoi(strCount);

                    // admin 수가 3명 이상이면 경고 메시지를 띄우고 작업을 중단합니다.
                    if (nAdminCount >= 3)
                    {
                        AfxMessageBox(_T("관리자(admin)는 최대 3명까지만 지정할 수 있습니다."), MB_ICONWARNING);
                        rs_check.Close();
                        db_check.Close();
                        return; // 함수를 여기서 종료
                    }
                }
                rs_check.Close();
                db_check.Close();
            }
            catch (CDBException* e)
            {
                AfxMessageBox(_T("관리자 인원 확인 중 DB 오류가 발생했습니다:\n") + e->m_strError);
                e->Delete();
                return; // 오류 발생 시에도 작업을 중단합니다.
            }
        }

        // --- DB에 UPDATE 쿼리 실행 ---
        CDatabase db;
        try
        {
            CString sDsn;
            sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
            db.OpenEx(sDsn, CDatabase::noOdbcDialog);

            // 7. UPDATE 쿼리 작성 (WHERE 절이 매우 중요!)
            CString strSQL;
            // ?? 중요: WHERE 절의 컬럼 이름이 `id`인지 `user_id`인지 DB에서 꼭 확인하세요!
            strSQL.Format(_T("UPDATE users SET role = '%s' WHERE user_id = '%s'"), strNewRole, strUserID);

            db.ExecuteSQL(strSQL);
            AfxMessageBox(_T("사용자 권한이 성공적으로 변경되었습니다."));

            // 8. 리스트 새로고침
            OnBnClickedButtonUserSelect(); // 이 함수 이름이 조회 함수가 맞는지 확인하세요.
        }
        catch (CDBException* e)
        {
            AfxMessageBox(_T("DB 오류: ") + e->m_strError);
            e->Delete();
        }
        if (db.IsOpen()) db.Close();
    }
}
// CUserSelectDlg.cpp

// ... (다른 함수들 생략) ...

// ▼▼▼ 이 함수 구현부 전체를 복사해서 파일에 추가합니다. ▼▼▼
void CUserSelectDlg::LoadUserList(CString strWhereClause)
{
    // 리스트 컨트롤 깜빡임 방지를 위해 업데이트를 잠시 중단합니다.
    m_userList.SetRedraw(FALSE);
    // 기존 리스트 내용을 모두 삭제합니다.
    m_userList.DeleteAllItems();

    // --- DB 연결 및 데이터 조회 ---
    CDatabase db;
    CRecordset rs(&db);
    try
    {
        // DB 연결
        CString sDsn;
        sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
        db.OpenEx(sDsn, CDatabase::noOdbcDialog);

        // SQL 쿼리문 작성 (인자로 받은 WHERE 절을 뒤에 붙여줍니다)
        CString strSQL = _T("SELECT user_id, username, email, password_hash, role, status FROM users ") + strWhereClause;

        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        // CRecordset의 필드 정보를 가져옵니다.
        short nFields = rs.GetODBCFieldCount();

        // 리스트 컨트롤에 데이터 추가
        for (int nItem = 0; !rs.IsEOF(); nItem++)
        {
            // 첫 번째 컬럼에 아이템을 추가합니다.
            CString fieldValue;
            rs.GetFieldValue((short)0, fieldValue);
            m_userList.InsertItem(nItem, fieldValue);

            // 나머지 컬럼들을 채웁니다.
            for (short i = 1; i < nFields; i++)
            {
                rs.GetFieldValue(i, fieldValue);
                m_userList.SetItemText(nItem, i, fieldValue);
            }
            rs.MoveNext();
        }

        // 컬럼 폭을 내용에 맞게 자동 조절
        for (int i = 0; i < m_userList.GetHeaderCtrl()->GetItemCount(); i++)
        {
            m_userList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        }
    }
    catch (CDBException* e)
    {
        AfxMessageBox(_T("DB 오류: ") + e->m_strError);
        e->Delete();
    }

    // 리소스 정리
    if (rs.IsOpen()) rs.Close();
    if (db.IsOpen()) db.Close();

    // 리스트 업데이트를 재개하고 화면에 최종 결과를 표시합니다.
    m_userList.SetRedraw(TRUE);
}

void CUserSelectDlg::OnClickedButtonSearch()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // 1. 에디트 컨트롤의 값을 m_strSearchKeyword 멤버 변수로 가져옵니다.
    UpdateData(TRUE);

    // 검색어가 비어있는지 확인합니다.
    if (m_strSearchKeyword.IsEmpty())
    {
        AfxMessageBox(_T("검색어를 입력해주세요."));
        return;
    }

    // SQL Injection 기본 방지를 위해 작은 따옴표를 처리합니다.
    CString strKeyword = m_strSearchKeyword;
    strKeyword.Replace(_T("'"), _T("''"));

    // 2. 'name' 컬럼에서 LIKE 검색을 하는 WHERE 절 문자열을 만듭니다.
    CString strWhereClause;
    strWhereClause.Format(_T("WHERE username LIKE '%%%s%%'"), strKeyword);

    // 3. 생성된 WHERE 절을 인자로 넘겨 사용자 목록을 새로고침합니다.
    LoadUserList(strWhereClause);
}

void CUserSelectDlg::OnClickedButtonShowAll()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // 1. 검색어 멤버 변수를 비웁니다.
    m_strSearchKeyword.Empty();

    // 2. 비워진 변수 값을 에디트 컨트롤에 반영하여 검색창을 깨끗하게 합니다.
    UpdateData(FALSE);

    // 3. WHERE 절 없이(빈 문자열) LoadUserList를 호출하여 전체 목록을 불러옵니다.
    LoadUserList(_T(""));
}
