// ManufacturingSystemClientView.cpp: CManufacturingSystemClientView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ManufacturingSystemClient.h"
#endif

#include "ManufacturingSystemClientDoc.h"
#include "ManufacturingSystemClientView.h"
#include "resource.h" 
#include "CUserSelectDlg.h"
#include "CAdminAuthDlg.h"
#include "CFactoryControlDlg.h"
#include <afxdb.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CManufacturingSystemClientView

IMPLEMENT_DYNCREATE(CManufacturingSystemClientView, CFormView)

BEGIN_MESSAGE_MAP(CManufacturingSystemClientView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CManufacturingSystemClientView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ALL, &CManufacturingSystemClientView::OnBnClickedButtonAddAll)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SELECTED, &CManufacturingSystemClientView::OnBnClickedButtonAddSelected)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_DATA, &CManufacturingSystemClientView::OnBnClickedButtonViewData)
	ON_BN_CLICKED(IDC_BUTTON_SEND_MAIL, &CManufacturingSystemClientView::OnBnClickedButtonSendMail)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DATE_TREE, &CManufacturingSystemClientView::OnTvnSelchangedDateTree)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SELECTED, &CManufacturingSystemClientView::OnBnClickedButtonDeleteSelected)
	ON_NOTIFY(LVN_KEYDOWN, IDC_BASKET_LIST, &CManufacturingSystemClientView::OnLvnKeydownBasketList)
	ON_BN_CLICKED(IDC_BUTTON_USER_SELECT, &CManufacturingSystemClientView::OnBnClickedButtonUserSelect)
	ON_BN_CLICKED(IDC_BUTTON_FACTORY_CONTROL, &CManufacturingSystemClientView::OnBnClickedButtonFactoryControl)
END_MESSAGE_MAP()

// CManufacturingSystemClientView 생성/소멸

CManufacturingSystemClientView::CManufacturingSystemClientView() noexcept
	: CFormView(IDD_MANUFACTURINGSYSTEMCLIENT_FORM)
{
}

CManufacturingSystemClientView::~CManufacturingSystemClientView()
{
}

void CManufacturingSystemClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_TREE, m_ctrlDateTree);
	DDX_Control(pDX, IDC_LIST_A_LINE, m_ctrlListALine);
	DDX_Control(pDX, IDC_LIST_B_LINE, m_ctrlListBLine);
	DDX_Control(pDX, IDC_BASKET_LIST, m_ctrlBasketList);
}

BOOL CManufacturingSystemClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CManufacturingSystemClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// --- UI 컨트롤 초기화 코드 ---

	// 1. 기간 선택 UI (Tree Control) 초기화 (2024년 추가)
	HTREEITEM h2024 = m_ctrlDateTree.InsertItem(_T("2024년"));
	HTREEITEM h2024_8 = m_ctrlDateTree.InsertItem(_T("8월"), h2024);
	m_ctrlDateTree.InsertItem(_T("15일"), h2024_8);
	m_ctrlDateTree.InsertItem(_T("16일"), h2024_8);

	HTREEITEM h2025 = m_ctrlDateTree.InsertItem(_T("2025년"));
	HTREEITEM h2025_9 = m_ctrlDateTree.InsertItem(_T("9월"), h2025);
	m_ctrlDateTree.InsertItem(_T("1일"), h2025_9);
	m_ctrlDateTree.InsertItem(_T("2일"), h2025_9);
	m_ctrlDateTree.InsertItem(_T("3일"), h2025_9);

	HTREEITEM h2025_10 = m_ctrlDateTree.InsertItem(_T("10월"), h2025);
	m_ctrlDateTree.InsertItem(_T("1일"), h2025_10);
	m_ctrlDateTree.InsertItem(_T("2일"), h2025_10);

	m_ctrlDateTree.Expand(h2024, TVE_EXPAND);
	m_ctrlDateTree.Expand(h2025, TVE_EXPAND);

	// 2. 리스트 컨트롤 기본 스타일 설정
	m_ctrlListALine.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_ctrlListBLine.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_ctrlBasketList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 3. 전송 바구니 UI 컬럼 초기화
	m_ctrlBasketList.InsertColumn(0, _T("요청 항목"), LVCFMT_LEFT, 320);

	// 4. 초기 화면은 제품별 뷰로 설정
	SetProductViewColumns();
}

// 리스트 컨트롤 컬럼을 "제품명" 기준으로 변경하는 함수
void CManufacturingSystemClientView::SetProductViewColumns()
{
	// 기존 컬럼 모두 삭제
	while (m_ctrlListALine.DeleteColumn(0));
	while (m_ctrlListBLine.DeleteColumn(0));

	// A라인 컬럼 설정
	m_ctrlListALine.InsertColumn(0, _T("제품명"), LVCFMT_CENTER, 100);
	m_ctrlListALine.InsertColumn(1, _T("생산량"), LVCFMT_RIGHT, 50);
	m_ctrlListALine.InsertColumn(2, _T("불량"), LVCFMT_RIGHT, 50);
	m_ctrlListALine.InsertColumn(3, _T("합계"), LVCFMT_RIGHT, 50);
	m_ctrlListALine.InsertColumn(4, _T("폐기"), LVCFMT_RIGHT, 50);

	// B라인 컬럼 설정
	m_ctrlListBLine.InsertColumn(0, _T("제품명"), LVCFMT_CENTER, 100);
	m_ctrlListBLine.InsertColumn(1, _T("생산량"), LVCFMT_RIGHT, 50);
	m_ctrlListBLine.InsertColumn(2, _T("불량"), LVCFMT_RIGHT, 50);
	m_ctrlListBLine.InsertColumn(3, _T("합계"), LVCFMT_RIGHT, 50);
	m_ctrlListBLine.InsertColumn(4, _T("폐기"), LVCFMT_RIGHT, 50);
}

// 리스트 컨트롤 컬럼을 "월" 또는 "일" 기준으로 변경하는 함수
void CManufacturingSystemClientView::SetSummaryViewColumns(CString strHeader)
{
	// 기존 컬럼 모두 삭제
	while (m_ctrlListALine.DeleteColumn(0));
	while (m_ctrlListBLine.DeleteColumn(0));

	// A라인 컬럼 설정
	m_ctrlListALine.InsertColumn(0, strHeader, LVCFMT_CENTER, 100);
	m_ctrlListALine.InsertColumn(1, _T("총생산량"), LVCFMT_RIGHT, 80);
	m_ctrlListALine.InsertColumn(2, _T("총불량"), LVCFMT_RIGHT, 80);
	m_ctrlListALine.InsertColumn(3, _T("총폐기"), LVCFMT_RIGHT, 80);

	// B라인 컬럼 설정
	m_ctrlListBLine.InsertColumn(0, strHeader, LVCFMT_CENTER, 100);
	m_ctrlListBLine.InsertColumn(1, _T("총생산량"), LVCFMT_RIGHT, 80);
	m_ctrlListBLine.InsertColumn(2, _T("총불량"), LVCFMT_RIGHT, 80);
	m_ctrlListBLine.InsertColumn(3, _T("총폐기"), LVCFMT_RIGHT, 80);
}


void CManufacturingSystemClientView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CManufacturingSystemClientView::OnContextMenu(CWnd* pWnd, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CManufacturingSystemClientView 진단
#ifdef _DEBUG
void CManufacturingSystemClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CManufacturingSystemClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CManufacturingSystemClientDoc* CManufacturingSystemClientView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CManufacturingSystemClientDoc)));
	return (CManufacturingSystemClientDoc*)m_pDocument;
}
#endif //_DEBUG

// CManufacturingSystemClientView 메시지 처리기
void CManufacturingSystemClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFormView::OnFilePrintPreview();
#endif
}

void CManufacturingSystemClientView::OnTvnSelchangedDateTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	m_ctrlListALine.DeleteAllItems();
	m_ctrlListBLine.DeleteAllItems();

	HTREEITEM hSelectedItem = m_ctrlDateTree.GetSelectedItem();
	if (hSelectedItem == NULL) return;

	UpdateDataView(hSelectedItem);
}

void CManufacturingSystemClientView::UpdateDataView(HTREEITEM hSelectedItem)
{
	HTREEITEM hParent = m_ctrlDateTree.GetParentItem(hSelectedItem);
	HTREEITEM hGrandParent = (hParent != NULL) ? m_ctrlDateTree.GetParentItem(hParent) : NULL;

	int nYear, nMonth, nDay;
	CString strValue;

	if (hGrandParent != NULL) // 일(Day) 노드 선택
	{
		SetProductViewColumns();
		nYear = _ttoi(m_ctrlDateTree.GetItemText(hGrandParent));
		nMonth = _ttoi(m_ctrlDateTree.GetItemText(hParent));
		nDay = _ttoi(m_ctrlDateTree.GetItemText(hSelectedItem));

		// A라인 데이터
		int nIndex = m_ctrlListALine.InsertItem(0, _T("CPU"));
		strValue.Format(_T("%d"), 100 + nDay + nMonth); m_ctrlListALine.SetItemText(nIndex, 1, strValue);
		strValue.Format(_T("%d"), 5 + nDay); m_ctrlListALine.SetItemText(nIndex, 2, strValue);
		strValue.Format(_T("%d"), 105 + nDay * 2 + nMonth); m_ctrlListALine.SetItemText(nIndex, 3, strValue);
		strValue.Format(_T("%d"), nDay); m_ctrlListALine.SetItemText(nIndex, 4, strValue);

		nIndex = m_ctrlListALine.InsertItem(1, _T("GPU"));
		strValue.Format(_T("%d"), 80 - nDay + nMonth); m_ctrlListALine.SetItemText(nIndex, 1, strValue);
		strValue.Format(_T("%d"), 3); m_ctrlListALine.SetItemText(nIndex, 2, strValue);
		strValue.Format(_T("%d"), 83 - nDay + nMonth); m_ctrlListALine.SetItemText(nIndex, 3, strValue);
		strValue.Format(_T("%d"), 1); m_ctrlListALine.SetItemText(nIndex, 4, strValue);

		// B라인 데이터
		nIndex = m_ctrlListBLine.InsertItem(0, _T("RAM"));
		strValue.Format(_T("%d"), 200 + nDay); m_ctrlListBLine.SetItemText(nIndex, 1, strValue);
		strValue.Format(_T("%d"), 10 + nDay); m_ctrlListBLine.SetItemText(nIndex, 2, strValue);
		strValue.Format(_T("%d"), 210 + nDay * 2); m_ctrlListBLine.SetItemText(nIndex, 3, strValue);
		strValue.Format(_T("%d"), nDay * 2); m_ctrlListBLine.SetItemText(nIndex, 4, strValue);
	}
	else if (hParent != NULL) // 월(Month) 노드 선택
	{
		SetSummaryViewColumns(_T("일"));
		nYear = _ttoi(m_ctrlDateTree.GetItemText(hParent));
		nMonth = _ttoi(m_ctrlDateTree.GetItemText(hSelectedItem));

		for (int day = 1; day <= 30; day++) // 30일까지만 있다고 가정
		{
			// A라인
			strValue.Format(_T("%d일"), day);
			int nIndex = m_ctrlListALine.InsertItem(day - 1, strValue);
			strValue.Format(_T("%d"), (100 + day + nMonth) + (80 - day + nMonth)); m_ctrlListALine.SetItemText(nIndex, 1, strValue); // 총생산
			strValue.Format(_T("%d"), (5 + day) + 3); m_ctrlListALine.SetItemText(nIndex, 2, strValue); // 총불량
			strValue.Format(_T("%d"), day + 1); m_ctrlListALine.SetItemText(nIndex, 3, strValue); // 총폐기

			// B라인
			nIndex = m_ctrlListBLine.InsertItem(day - 1, strValue);
			strValue.Format(_T("%d"), 200 + day); m_ctrlListBLine.SetItemText(nIndex, 1, strValue); // 총생산
			strValue.Format(_T("%d"), 10 + day); m_ctrlListBLine.SetItemText(nIndex, 2, strValue); // 총불량
			strValue.Format(_T("%d"), day * 2); m_ctrlListBLine.SetItemText(nIndex, 3, strValue); // 총폐기
		}
	}
	else // 년(Year) 노드 선택
	{
		SetSummaryViewColumns(_T("월"));
		nYear = _ttoi(m_ctrlDateTree.GetItemText(hSelectedItem));

		for (int month = 1; month <= 12; month++)
		{
			// A라인
			strValue.Format(_T("%d월"), month);
			int nIndex = m_ctrlListALine.InsertItem(month - 1, strValue);
			strValue.Format(_T("%d"), (180 + month * 2) * 30); m_ctrlListALine.SetItemText(nIndex, 1, strValue); // 총생산
			strValue.Format(_T("%d"), (8 + month) * 30); m_ctrlListALine.SetItemText(nIndex, 2, strValue); // 총불량
			strValue.Format(_T("%d"), (month + 1) * 30); m_ctrlListALine.SetItemText(nIndex, 3, strValue); // 총폐기

			// B라인
			nIndex = m_ctrlListBLine.InsertItem(month - 1, strValue);
			strValue.Format(_T("%d"), (200 + month) * 30); m_ctrlListBLine.SetItemText(nIndex, 1, strValue); // 총생산
			strValue.Format(_T("%d"), (10 + month) * 30); m_ctrlListBLine.SetItemText(nIndex, 2, strValue); // 총불량
			strValue.Format(_T("%d"), (month * 2) * 30); m_ctrlListBLine.SetItemText(nIndex, 3, strValue); // 총폐기
		}
	}
}

void CManufacturingSystemClientView::OnBnClickedButtonAddAll()
{
	HTREEITEM hSelectedItem = m_ctrlDateTree.GetSelectedItem();
	if (hSelectedItem == NULL)
	{
		AfxMessageBox(_T("먼저 기간을 선택해주세요."));
		return;
	}
	CString strItemText = GetFullPath(hSelectedItem);
	CString strBasketItem;
	strBasketItem.Format(_T("[%s] 전체 데이터"), strItemText);
	m_ctrlBasketList.InsertItem(0, strBasketItem);
}

void CManufacturingSystemClientView::OnBnClickedButtonAddSelected()
{
	HTREEITEM hSelectedItem = m_ctrlDateTree.GetSelectedItem();
	if (hSelectedItem == NULL)
	{
		AfxMessageBox(_T("먼저 기간을 선택해주세요."));
		return;
	}
	CString strDateText = GetFullPath(hSelectedItem);

	for (int i = 0; i < m_ctrlListALine.GetItemCount(); i++)
	{
		if (m_ctrlListALine.GetCheck(i))
		{
			CString strProductText = m_ctrlListALine.GetItemText(i, 0);
			CString strBasketItem;
			strBasketItem.Format(_T("[%s] A-Line %s 데이터"), strDateText, strProductText);
			m_ctrlBasketList.InsertItem(0, strBasketItem);
		}
	}

	for (int i = 0; i < m_ctrlListBLine.GetItemCount(); i++)
	{
		if (m_ctrlListBLine.GetCheck(i))
		{
			CString strProductText = m_ctrlListBLine.GetItemText(i, 0);
			CString strBasketItem;
			strBasketItem.Format(_T("[%s] B-Line %s 데이터"), strDateText, strProductText);
			m_ctrlBasketList.InsertItem(0, strBasketItem);
		}
	}
}

void CManufacturingSystemClientView::OnBnClickedButtonViewData()
{
	int nItemCount = m_ctrlBasketList.GetItemCount();
	if (nItemCount == 0)
	{
		AfxMessageBox(_T("조회할 항목을 먼저 바구니에 담아주세요."));
		return;
	}
	CString strMessage = _T("다음 항목들의 데이터를 서버에 요청합니다:\n\n");
	for (int i = 0; i < nItemCount; i++)
	{
		strMessage += m_ctrlBasketList.GetItemText(i, 0) + _T("\n");
	}
	AfxMessageBox(strMessage);
	m_ctrlBasketList.DeleteAllItems();
}

void CManufacturingSystemClientView::OnBnClickedButtonSendMail()
{
	int nItemCount = m_ctrlBasketList.GetItemCount();
	if (nItemCount == 0)
	{
		AfxMessageBox(_T("전송할 항목을 먼저 바구니에 담아주세요."));
		return;
	}
	AfxMessageBox(_T("바구니에 담긴 항목들을 메일로 전송합니다."));
	m_ctrlBasketList.DeleteAllItems();
}

CString CManufacturingSystemClientView::GetFullPath(HTREEITEM hItem)
{
	CString strPath = _T("");
	CString strTemp = _T("");
	if (hItem == NULL) return strPath;
	while (hItem != NULL)
	{
		strTemp = m_ctrlDateTree.GetItemText(hItem);
		if (strPath.IsEmpty())
		{
			strPath = strTemp;
		}
		else
		{
			strPath = strTemp + _T(" ") + strPath;
		}
		hItem = m_ctrlDateTree.GetParentItem(hItem);
	}
	return strPath;
}

// '선택 항목 삭제' 버튼 클릭 시
void CManufacturingSystemClientView::OnBnClickedButtonDeleteSelected()
{
	// POSITION pos = m_ctrlBasketList.GetFirstSelectedItemPosition();
	// if (pos != NULL)
	// {
	// 	int nItem = m_ctrlBasketList.GetNextSelectedItem(pos);
	// 	m_ctrlBasketList.DeleteItem(nItem);
	// }
	for (int i = m_ctrlBasketList.GetItemCount() - 1; i >= 0; i--)
	{
		if (m_ctrlBasketList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_ctrlBasketList.DeleteItem(i);
		}
	}
}

// '전송 바구니'에서 키보드 입력 시
void CManufacturingSystemClientView::OnLvnKeydownBasketList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		OnBnClickedButtonDeleteSelected(); // 삭제 버튼 함수를 그대로 호출
	}
	*pResult = 0;
}

//여기서부터 민준 코드
void CManufacturingSystemClientView::OnBnClickedButtonUserSelect()
{
	CAdminAuthDlg authDlg;
	if (authDlg.DoModal() == IDOK)
	{
		// 2. 사용자가 입력한 비밀번호를 가져옵니다. (ID 가져오는 코드 삭제)
		CString strPassword = authDlg.m_strPassword;

		// 3. 입력값 유효성 검사 (비밀번호만 확인)
		if (strPassword.IsEmpty())
		{
			AfxMessageBox(_T("비밀번호를 입력해주세요."));
			return;
		}

		// --- DB에서 해당 정보와 일치하는 관리자 계정이 있는지 확인 ---
		CDatabase db;
		CRecordset rs(&db);
		try
		{
			// DB 연결
			CString sDsn;
			sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
			db.OpenEx(sDsn, CDatabase::noOdbcDialog);

			// SQL Injection 방지를 위한 따옴표 처리
			strPassword.Replace(_T("'"), _T("''"));

			// 4. 비밀번호가 일치하고 role이 'admin'인 사용자가 있는지 확인하는 SELECT 쿼리 (WHERE 조건 변경)
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM users WHERE password_hash = '%s' AND role = 'admin'"),
				strPassword);

			rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

			// 5. 쿼리 결과 확인
			if (rs.IsEOF())
			{
				// 결과가 없으면 인증 실패
				AfxMessageBox(_T("비밀번호가 잘못되었거나 관리자 계정이 아닙니다."), MB_ICONERROR);
			}
			else
			{
				// 결과가 있으면 인증 성공!
				AfxMessageBox(_T("관리자 인증에 성공했습니다."));

				// 사용자 관리 다이얼로그(CUserSelectDlg)를 엽니다.
				CUserSelectDlg userSelectDlg;
				userSelectDlg.DoModal();
			}
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("DB 오류: ") + e->m_strError);
			e->Delete();
		}

		// 리소스 정리
		if (rs.IsOpen())
			rs.Close();
		if (db.IsOpen())
			db.Close();
	}
}

void CManufacturingSystemClientView::OnBnClickedButtonFactoryControl()
{
	CAdminAuthDlg authDlg;
	if (authDlg.DoModal() == IDOK)
	{
		// 2. 사용자가 입력한 비밀번호를 가져옵니다. (ID 가져오는 코드 삭제)
		CString strPassword = authDlg.m_strPassword;

		// 3. 입력값 유효성 검사 (비밀번호만 확인)
		if (strPassword.IsEmpty())
		{
			AfxMessageBox(_T("비밀번호를 입력해주세요."));
			return;
		}

		// --- DB에서 해당 정보와 일치하는 관리자 계정이 있는지 확인 ---
		CDatabase db;
		CRecordset rs(&db);
		try
		{
			// DB 연결
			CString sDsn;
			sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
			db.OpenEx(sDsn, CDatabase::noOdbcDialog);

			// SQL Injection 방지를 위한 따옴표 처리
			strPassword.Replace(_T("'"), _T("''"));

			// 4. 비밀번호가 일치하고 role이 'admin'인 사용자가 있는지 확인하는 SELECT 쿼리 (WHERE 조건 변경)
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM users WHERE password_hash = '%s' AND role = 'admin'"),
				strPassword);

			rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

			// 5. 쿼리 결과 확인
			if (rs.IsEOF())
			{
				// 결과가 없으면 인증 실패
				AfxMessageBox(_T("비밀번호가 잘못되었거나 관리자 계정이 아닙니다."), MB_ICONERROR);
			}
			else
			{
				// 결과가 있으면 인증 성공!
				AfxMessageBox(_T("관리자 인증에 성공했습니다."));

				// 사용자 관리 다이얼로그(CUserSelectDlg)를 엽니다.
				CFactoryControlDlg factoryDlg;
				factoryDlg.DoModal();
			}
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("DB 오류: ") + e->m_strError);
			e->Delete();
		}

		// 리소스 정리
		if (rs.IsOpen())
			rs.Close();
		if (db.IsOpen())
			db.Close();
	}
}
