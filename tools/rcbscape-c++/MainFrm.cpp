// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "rcbscape.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "crig.h"

#include "rcbscapeView.h"
#include "TrueColorToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}*/

	/*if ( !m_wndTrueToolBar.CreateEx(this) || !m_wndTrueToolBar.LoadTrueColorToolBar(16,IDR_MAINFRAME) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}*/

	//m_wndTrueToolBar.SetBorders(1, 1, 1, 1);
	if (!m_wndTrueToolBar.Create(this) ||
		!m_wndTrueToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndTrueToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	
	m_wndTrueToolBar.LoadTrueColorToolBar(16,IDB_TOOLBAR);

	/*if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}*/
	
	/*if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndTrueToolBar) /*||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}*/
	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	/*int strPartDim[4] = { 80,160,240,-1};

	m_wndStatusBar.GetStatusBarCtrl().SetParts(4,strPartDim);
	
	m_wndStatusBar.GetStatusBarCtrl().SetText("BURAK",1,0);*/

	// TODO: Remove this if you don't want tool tips
	m_wndTrueToolBar.SetBarStyle(m_wndTrueToolBar.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndTrueToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndTrueToolBar);
	//GetRightPane()->GetDocument()->stbar = &m_wndStatusBar;

/*	CRcbscapeDoc* wh;

	wh = GetRightPane()->GetDocument()->lft = this;*/
	



	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 3))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRcbscapeView), CSize(500, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(crig), CSize(100, 100), pContext )
	   )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CRcbscapeView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CRcbscapeView* pView = DYNAMIC_DOWNCAST(CRcbscapeView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CRcbscapeView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CRcbscapeView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CRcbscapeView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}

void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	long wNumFilesDropped = DragQueryFile(hDropInfo, -1, NULL, 0);
	CString firstFile="";
	char* pth;

	for ( long x = 0 ; x < wNumFilesDropped ; x++)
	{
		WORD wPathnameSize = DragQueryFile(hDropInfo, x, NULL, 0)+1;
		pth = new char[wPathnameSize];
		DragQueryFile(hDropInfo, x, pth, wPathnameSize);
		GetRightPane()->GetDocument()->thefile.AddFile(pth,GetRightPane()->GetDocument()->cpath.GetBuffer(512),0,0);
	}

	DragFinish(hDropInfo);
	SetFocus();
	SetRedraw(TRUE);
	RedrawWindow(NULL,NULL);
	//CFrameWnd::OnDropFiles(hDropInfo);
}
