#pragma once
#include "afxdialogex.h"
#include "afxcmn.h"

// CFactoryControlDlg 대화 상자

class CFactoryControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFactoryControlDlg)

public:
	CFactoryControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFactoryControlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACTORY_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	BOOL UpdateDeviceStateInDB(CString strDeviceName, int nState);

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderFan;
	CSliderCtrl m_sliderConveyor;
	//CStatic m_staticFanStatus;  // 상태 표시용 텍스트 변수

	BOOL m_bFanOn;
	BOOL m_bConveyorOn;

	// 슬라이더를 움직였을 때 호출될 함수
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	
};
