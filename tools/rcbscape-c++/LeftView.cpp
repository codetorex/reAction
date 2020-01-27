// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "rcbscape.h"

#include "rcbscapeDoc.h"
#include "LeftView.h"
#include "Array.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_COMMAND(ID_NEWFOLDER, OnNewfolder)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_COMMAND(ID_PARENTFOLDER, OnParentfolder)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	
	

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CRcbscapeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{	


	GetDocument()->lft = this;

	GetTreeCtrl().SetImageList(&GetDocument()->imglist,TVSIL_NORMAL);

	
	//HTREEITEM rt;
	GetTreeCtrl().ModifyStyle(0,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS);
	CTreeView::OnInitialUpdate();
	GetTreeCtrl().DeleteAllItems();
	rt = GetTreeCtrl().InsertItem("/",0,0);

	if (GetDocument()->isnew == false)
	{
		LoadTree(&GetTreeCtrl(),rt,&GetDocument()->thefile);
	}

	
	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}


CRcbscapeDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRcbscapeDoc)));
	return (CRcbscapeDoc*)m_pDocument;
}
#endif //_DEBUG

void CLeftView::OnNewfolder() 
{
	// TODO: Add your command handler code here
	HTREEITEM adem,kr;
	adem = GetTreeCtrl().GetSelectedItem();
	kr = GetTreeCtrl().InsertItem("New Folder",1,1,adem);
	
	
	GetTreeCtrl().Expand(adem,TVE_EXPAND);
	GetTreeCtrl().RedrawWindow();	
	GetTreeCtrl().EditLabel(kr);

	


}

void CLeftView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	if ( pTVDispInfo->item.pszText != 0 && 
		 !(strchr(pTVDispInfo->item.pszText,'/') > 0) &&
		 (strlen(pTVDispInfo->item.pszText)> 0)
		)
	{
		GetTreeCtrl().SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
		GetTreeCtrl().SelectItem(pTVDispInfo->item.hItem);
	}
	*pResult = 0;
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//GetItemPath (&GetTreeCtrl(),pNMTreeView->itemNew.hItem,rt);
	HTREEITEM hmyItem;
	hmyItem = pNMTreeView->itemNew.hItem;

	GetItemDepth(&GetTreeCtrl(),hmyItem,rt);
	GetDocument()->rig->GetListCtrl().DeleteAllItems();

	GetDocument()->ld = hmyItem;

	GetDocument()->cpath = GetItemPath(&GetTreeCtrl(),hmyItem,rt); // when path changes put it to temporary space
	
	if (GetTreeCtrl().ItemHasChildren(hmyItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetTreeCtrl().GetChildItem(hmyItem);

		while (hChildItem != NULL)
		{
			hNextItem = GetTreeCtrl().GetNextItem(hChildItem,TVGN_NEXT);
			GetDocument()->rig->GetListCtrl().InsertItem(0,GetTreeCtrl().GetItemText(hChildItem),1);
			hChildItem=hNextItem;
		}
	}
	LoadDir(GetDocument()->rig,&GetDocument()->thefile,GetDocument()->cpath.GetBuffer(256));
	//GetTreeCtrl().GetChildItem(pNMTreeView->itemNew.hItem)

	*pResult = 0;
}

void CLeftView::OnParentfolder() 
{
	// TODO: Add your command handler code here
	HTREEITEM slc,prt;
	slc = GetTreeCtrl().GetSelectedItem();
	prt = GetTreeCtrl().GetParentItem(slc);

	if ( prt != NULL)
	{
		GetTreeCtrl().SelectItem(prt);
	}
	
	
}
