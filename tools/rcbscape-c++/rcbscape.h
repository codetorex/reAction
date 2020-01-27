// rcbscape.h : main header file for the RCBSCAPE application
//

#if !defined(AFX_RCBSCAPE_H__29F3986A_1958_40F1_8E08_F803D46D8940__INCLUDED_)
#define AFX_RCBSCAPE_H__29F3986A_1958_40F1_8E08_F803D46D8940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeApp:
// See rcbscape.cpp for the implementation of this class
//

class CRcbscapeApp : public CWinApp
{
public:
	CRcbscapeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcbscapeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CRcbscapeApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCBSCAPE_H__29F3986A_1958_40F1_8E08_F803D46D8940__INCLUDED_)
