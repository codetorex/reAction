// rcbscapeDoc.h : interface of the CRcbscapeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBSCAPEDOC_H__7EDA4641_7F02_4226_8BB8_FBD93E3047BB__INCLUDED_)
#define AFX_RCBSCAPEDOC_H__7EDA4641_7F02_4226_8BB8_FBD93E3047BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../reAction/rcabinet.h"
#include "LeftView.h"
#include "rcbscapeView.h"

class CRcbscapeDoc : public CDocument
{
protected: // create from serialization only
	CRcbscapeDoc();
	DECLARE_DYNCREATE(CRcbscapeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcbscapeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	FILE* fileptr;
	bool isnew;
	RCabinet thefile;
	CString cpath;
	HTREEITEM ld;
	

	COLORREF bkclr;

	CLeftView* lft;
	CRcbscapeView* rig;
	CImageList imglist;
	CImageList imglist_big;
	CStatusBar*	stbar;

	virtual ~CRcbscapeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRcbscapeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CRcbscapeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCBSCAPEDOC_H__7EDA4641_7F02_4226_8BB8_FBD93E3047BB__INCLUDED_)
