// crig.cpp : implementation file
//

#include "stdafx.h"
#include "rcbscape.h"
#include "crig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// crig

IMPLEMENT_DYNCREATE(crig, CFormView)

crig::crig()
	: CFormView(crig::IDD)
{
	//{{AFX_DATA_INIT(crig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

crig::~crig()
{
}

void crig::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(crig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(crig, CFormView)
	//{{AFX_MSG_MAP(crig)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// crig diagnostics

#ifdef _DEBUG
void crig::AssertValid() const
{
	CFormView::AssertValid();
}

void crig::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// crig message handlers
