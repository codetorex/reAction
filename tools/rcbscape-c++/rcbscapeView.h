// rcbscapeView.h : interface of the CRcbscapeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBSCAPEVIEW_H__4D5F8C24_5869_464A_8164_50BB0FD1F0FB__INCLUDED_)
#define AFX_RCBSCAPEVIEW_H__4D5F8C24_5869_464A_8164_50BB0FD1F0FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRcbscapeView : public CListView
{
protected: // create from serialization only
	CRcbscapeView();
	DECLARE_DYNCREATE(CRcbscapeView)

// Attributes
public:
	CRcbscapeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcbscapeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRcbscapeView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRcbscapeView)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemdblclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in rcbscapeView.cpp
inline CRcbscapeDoc* CRcbscapeView::GetDocument()
   { return (CRcbscapeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCBSCAPEVIEW_H__4D5F8C24_5869_464A_8164_50BB0FD1F0FB__INCLUDED_)
