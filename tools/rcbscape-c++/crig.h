#if !defined(AFX_CRIG_H__676B27DD_23E8_432D_A3D7_17FC665E8EC5__INCLUDED_)
#define AFX_CRIG_H__676B27DD_23E8_432D_A3D7_17FC665E8EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// crig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// crig form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class crig : public CFormView
{
protected:
	crig();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(crig)

// Form Data
public:
	//{{AFX_DATA(crig)
	enum { IDD = IDR_MAINFRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(crig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~crig();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(crig)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRIG_H__676B27DD_23E8_432D_A3D7_17FC665E8EC5__INCLUDED_)
