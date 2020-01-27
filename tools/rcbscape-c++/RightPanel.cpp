// RightPanel.cpp : implementation file
//

#include "stdafx.h"
#include "rcbscape.h"
#include "RightPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RightPanel dialog


RightPanel::RightPanel(CWnd* pParent /*=NULL*/)
	: CDialog(RightPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(RightPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RightPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RightPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RightPanel, CDialog)
	//{{AFX_MSG_MAP(RightPanel)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RightPanel message handlers
