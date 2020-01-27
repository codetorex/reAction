// rcbscapeDoc.cpp : implementation of the CRcbscapeDoc class
//

#include "stdafx.h"
#include "rcbscape.h"

#include "rcbscapeDoc.h"
#include "../../reAction/rcabinet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeDoc

IMPLEMENT_DYNCREATE(CRcbscapeDoc, CDocument)

BEGIN_MESSAGE_MAP(CRcbscapeDoc, CDocument)
	//{{AFX_MSG_MAP(CRcbscapeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRcbscapeDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CRcbscapeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IRcbscape to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {455A5C08-8430-40F1-918A-C5A022C1C9AF}
static const IID IID_IRcbscape =
{ 0x455a5c08, 0x8430, 0x40f1, { 0x91, 0x8a, 0xc5, 0xa0, 0x22, 0xc1, 0xc9, 0xaf } };

BEGIN_INTERFACE_MAP(CRcbscapeDoc, CDocument)
	INTERFACE_PART(CRcbscapeDoc, IID_IRcbscape, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeDoc construction/destruction

CRcbscapeDoc::CRcbscapeDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();
		CWinApp* pApp = AfxGetApp();

		imglist.Create(16,16,ILC_COLORDDB | ILC_MASK ,4,4);
		imglist.SetBkColor(GetSysColor(COLOR_WINDOW)); // TODO: GET SYSTEM COLOR
		imglist.Add(pApp->LoadIcon(IDR_CABTYPE));
		imglist.Add(pApp->LoadIcon(IDI_FOLDER));
		imglist.Add(pApp->LoadIcon(IDI_DOCUMENT));
		

		imglist_big.Create(32,32,ILC_COLORDDB | ILC_MASK , 4,4);
		imglist_big.SetBkColor(GetSysColor(COLOR_WINDOW));
		imglist_big.Add(pApp->LoadIcon(IDR_CABTYPE));
		imglist_big.Add(pApp->LoadIcon(IDI_FOLDER));
		imglist_big.Add(pApp->LoadIcon(IDI_DOCUMENT));

		
	AfxOleLockApp();
}

CRcbscapeDoc::~CRcbscapeDoc()
{
	AfxOleUnlockApp();
}

BOOL CRcbscapeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	isnew = TRUE;
	//memset(&thefile.myheader.signature,4,"RCB\0");
	memcpy(thefile.myheader.signature,"RCB\0",4);
	thefile.myheader.version[0] = 0;
	thefile.myheader.version[1] = 1;
	thefile.myheader.id = 0;
	thefile.myheader.filesize = sizeof(thefile.myheader);
	thefile.myheader.filecount = 0;
	thefile.myheader.flags = 0;

	fileptr = fopen("temp.rcb","w+b");
/*	struct RCBHeader // rcb main header
{
	char			signature[4];	// Signature of rcb file
	long			id;				// identification number for file
	unsigned char	version[2];		// version of file
	long			filesize;		// filesize of rcb
	long			flags;			// flags for file system
	long			filecount;		// file count of rcb
};*/

	thefile.WriteHeader(&thefile.myheader,fileptr);

	fclose(fileptr);
	

	thefile.currentfile = "temp.rcb\0";
	
	
	//&thefile.myheader.signature = "RCB";
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRcbscapeDoc serialization

void CRcbscapeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeDoc diagnostics

#ifdef _DEBUG
void CRcbscapeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRcbscapeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRcbscapeDoc commands




BOOL CRcbscapeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	char* cp;
	isnew = false;
	
	cp = new char [strlen(lpszPathName)];
	memcpy(cp,lpszPathName,strlen(lpszPathName)+1);
	thefile.LoadRCB(cp);

	// NOW WE MUST LOAD TREE
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

