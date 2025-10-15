// ManufacturingSystemClientView.h: CManufacturingSystemClientView 클래스의 인터페이스
//

#pragma once
#include <afxcmn.h>


class CManufacturingSystemClientView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CManufacturingSystemClientView() noexcept;
	DECLARE_DYNCREATE(CManufacturingSystemClientView)

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUFACTURINGSYSTEMCLIENT_FORM };
#endif

	// 특성입니다.
public:
	CManufacturingSystemClientDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

	// 구현입니다.
public:
	virtual ~CManufacturingSystemClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// UI 컨트롤 멤버 변수
	CTreeCtrl m_ctrlDateTree;
	CListCtrl m_ctrlListALine;
	CListCtrl m_ctrlListBLine;
	CListCtrl m_ctrlBasketList;

	// 헬퍼 함수
	CString GetFullPath(HTREEITEM hItem);
	void UpdateDataView(HTREEITEM hSelectedItem);
	void SetProductViewColumns();
	void SetSummaryViewColumns(CString strHeader);

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBnClickedButtonAddAll();
	afx_msg void OnBnClickedButtonAddSelected();
	afx_msg void OnBnClickedButtonViewData();
	afx_msg void OnBnClickedButtonSendMail();
	afx_msg void OnTvnSelchangedDateTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonDeleteSelected(); // 삭제 버튼 핸들러
	afx_msg void OnLvnKeydownBasketList(NMHDR* pNMHDR, LRESULT* pResult); // 키보드 입력 핸들러
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUserSelect();
	afx_msg void OnBnClickedButtonFactoryControl();
};

#ifndef _DEBUG  // ManufacturingSystemClientView.cpp의 디버그 버전
inline CManufacturingSystemClientDoc* CManufacturingSystemClientView::GetDocument() const
{
	return reinterpret_cast<CManufacturingSystemClientDoc*>(m_pDocument);
}
#endif
