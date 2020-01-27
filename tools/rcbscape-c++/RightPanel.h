#if !defined(AFX_RIGHTPANEL_H__720D9AF5_81B6_4A98_9271_5E81BAC0B5A9__INCLUDED_)
#define AFX_RIGHTPANEL_H__720D9AF5_81B6_4A98_9271_5E81BAC0B5A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RightPanel dialog

class RightPanel : public CDialog
{
// Construction
public:
	RightPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RightPanel)
	enum { IDD = IDR_MAINFRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RightPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RightPanel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTPANEL_H__720D9AF5_81B6_4A98_9271_5E81BAC0B5A9__INCLUDED_)
