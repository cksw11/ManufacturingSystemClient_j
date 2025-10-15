// CFactoryControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ManufacturingSystemClient.h"
#include "afxdialogex.h"
#include "CFactoryControlDlg.h"
#include <afxdb.h>

// CFactoryControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CFactoryControlDlg, CDialogEx)

CFactoryControlDlg::CFactoryControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACTORY_CONTROL, pParent)
{

}

CFactoryControlDlg::~CFactoryControlDlg()
{
}

void CFactoryControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_FAN, m_sliderFan);
	// 컨베이어 벨트 슬라이더 변수를 연결합니다.
	DDX_Control(pDX, IDC_SLIDER_CONVEYOR, m_sliderConveyor);
}


BEGIN_MESSAGE_MAP(CFactoryControlDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

BOOL CFactoryControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// --- DB에서 장치의 현재 상태를 읽어옵니다 ---
	CDatabase db;
	CRecordset rs(&db);
	try
	{
		CString sDsn;
		sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
		db.OpenEx(sDsn, CDatabase::noOdbcDialog);

		// device_control 테이블에서 모든 장치의 이름과 상태를 조회합니다.
		rs.Open(CRecordset::forwardOnly, _T("SELECT device_name, device_state FROM device_control"), CRecordset::readOnly);

		// 결과를 한 줄씩 읽습니다.
		while (!rs.IsEOF())
		{
			CString deviceName, deviceState;
			rs.GetFieldValue(_T("device_name"), deviceName);
			rs.GetFieldValue(_T("device_state"), deviceState);
			int nState = _ttoi(deviceState); // 문자열 상태를 정수로 변환

			if (deviceName.CompareNoCase(_T("fan")) == 0)
			{
				m_bFanOn = (nState == 1); // 상태가 1이면 TRUE, 아니면 FALSE
			}
			else if (deviceName.CompareNoCase(_T("conveyor")) == 0)
			{
				m_bConveyorOn = (nState == 1);
			}
			rs.MoveNext();
		}
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("초기 상태 로드 중 DB 오류가 발생했습니다:\n") + e->m_strError);
		e->Delete();
		// 오류 발생 시 기본값(OFF)으로 설정
		m_bFanOn = FALSE;
		m_bConveyorOn = FALSE;
	}
	if (rs.IsOpen()) rs.Close();
	if (db.IsOpen()) db.Close();

	// --- 읽어온 상태에 맞춰 슬라이더 초기화 ---
	m_sliderFan.SetRange(0, 1);
	m_sliderFan.SetPos(m_bFanOn ? 1 : 0); // 상태가 ON이면 1, OFF이면 0으로 위치 설정

	m_sliderConveyor.SetRange(0, 1);
	m_sliderConveyor.SetPos(m_bConveyorOn ? 1 : 0);

	return TRUE;
}

BOOL CFactoryControlDlg::UpdateDeviceStateInDB(CString strDeviceName, int nState)
{
	CDatabase db;
	try
	{
		CString sDsn;
		sDsn.Format(_T("DRIVER={MySQL ODBC 9.4 Unicode Driver};SERVER=192.168.0.62;PORT=3306;DATABASE=projectserver;USER=admin1;PASSWORD=Moble;OPTION=3;"));
		db.OpenEx(sDsn, CDatabase::noOdbcDialog);

		// UPDATE 쿼리문
		CString strSQL;
		strSQL.Format(_T("UPDATE device_control SET device_state = %d WHERE device_name = '%s'"), nState, strDeviceName);

		db.ExecuteSQL(strSQL);

		if (db.IsOpen()) db.Close();
		return TRUE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("DB 업데이트 중 오류가 발생했습니다:\n") + e->m_strError);
		e->Delete();
		if (db.IsOpen()) db.Close();
		return FALSE;
	}
}

void CFactoryControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 팬 슬라이더가 움직였을 경우
	if (pScrollBar->GetSafeHwnd() == m_sliderFan.GetSafeHwnd())
	{
		int newState = m_sliderFan.GetPos();
		m_bFanOn = (newState == 1);

		// DB에 'fan'의 상태를 업데이트
		UpdateDeviceStateInDB(_T("fan"), newState);

		GetDlgItem(IDC_STATIC_FAN_LIGHT)->Invalidate(); // 상태등 다시 그리기 요청
	}
	// 컨베이어 슬라이더가 움직였을 경우
	else if (pScrollBar->GetSafeHwnd() == m_sliderConveyor.GetSafeHwnd())
	{
		int newState = m_sliderConveyor.GetPos();
		m_bConveyorOn = (newState == 1);

		// DB에 'conveyor'의 상태를 업데이트
		UpdateDeviceStateInDB(_T("conveyor"), newState);

		GetDlgItem(IDC_STATIC_CONVEYOR_LIGHT)->Invalidate(); // 상태등 다시 그리기 요청
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
// CFactoryControlDlg 메시지 처리기

void CFactoryControlDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	CBrush brush;

	switch (nIDCtl)
	{
	case IDC_STATIC_FAN_LIGHT: // 팬 상태등을 그릴 차례라면
		if (m_bFanOn) // 팬이 켜져있으면 초록색
			brush.CreateSolidBrush(RGB(0, 255, 0));
		else // 꺼져있으면 빨간색
			brush.CreateSolidBrush(RGB(255, 0, 0));
		break;

	case IDC_STATIC_CONVEYOR_LIGHT: // 컨베이어 상태등을 그릴 차례라면
		if (m_bConveyorOn)
			brush.CreateSolidBrush(RGB(0, 255, 0)); // 초록색
		else
			brush.CreateSolidBrush(RGB(255, 0, 0)); // 빨간색
		break;

	default:
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		return;
	}

	// 선택한 색으로 원을 그립니다.
	pDC->SelectObject(&brush);
	pDC->Ellipse(&rect);
}

