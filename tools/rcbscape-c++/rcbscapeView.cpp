// rcbscapeView.cpp : implementation of the CRcbscapeView class
//

#include "stdafx.h"
#include "rcbscape.h"

#include "rcbscapeDoc.h"
#include "rcbscapeView.h"
#include "Array.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView

IMPLEMENT_DYNCREATE(CRcbscapeView, CListView)

BEGIN_MESSAGE_MAP(CRcbscapeView, CListView)
	//{{AFX_MSG_MAP(CRcbscapeView)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(HDN_ITEMDBLCLICK, OnItemdblclick)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView construction/destruction

CRcbscapeView::CRcbscapeView()
{
	// TODO: add construction code here

}

CRcbscapeView::~CRcbscapeView()
{
}

BOOL CRcbscapeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView drawing

void CRcbscapeView::OnDraw(CDC* pDC)
{
	CRcbscapeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
//	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
	
}

void CRcbscapeView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

//	GetListCtrl().DeleteColumn(0);


	GetDocument()->rig = this;
	GetListCtrl().SetImageList(&GetDocument()->imglist_big,LVSIL_NORMAL);
	GetListCtrl().SetImageList(&GetDocument()->imglist,LVSIL_SMALL);

	GetListCtrl().DeleteAllItems();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView printing

BOOL CRcbscapeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRcbscapeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRcbscapeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView diagnostics

#ifdef _DEBUG
void CRcbscapeView::AssertValid() const
{
	CListView::AssertValid();
}

void CRcbscapeView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CRcbscapeDoc* CRcbscapeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRcbscapeDoc)));
	return (CRcbscapeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeView message handlers
void CRcbscapeView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CRcbscapeView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default


	//CListView::OnDropFilolsun es(hDropInfo);
}

int CRcbscapeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
		GetListCtrl().InsertColumn(0,"File Name",LVCFMT_LEFT,230);
	GetListCtrl().InsertColumn(1,"Size",LVCFMT_RIGHT,100);
	GetListCtrl().InsertColumn(2,"Type",LVCFMT_LEFT,90);
	GetListCtrl().InsertColumn(3,"Version",LVCFMT_LEFT,60);

	// TODO: Add your specialized creation code here
	
	return 0;
}


void CRcbscapeView::OnItemdblclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	//phdn->pitem->pszText
	HTREEITEM a;
	MessageBox("bur");
	a = IsItemExist( &GetDocument()->lft->GetTreeCtrl() ,GetDocument()->ld,phdn->pitem->pszText);
	if (a > 0)
	{
		GetDocument()->lft->GetTreeCtrl().SelectItem(a);
	}
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
