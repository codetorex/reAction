/*

	reAction Exporter
	Started at 20.06.2007

	Burak YILDIRIM


*/
#include "stdafx.h"
#include "exporter.h"


class MyExporterClassDesc : public ClassDesc2 
{
public:
    int             IsPublic()              { return TRUE; }
    void *          Create(BOOL loading)    { return new MyExporter; }
    const TCHAR *   ClassName()             { return GetString(IDS_CLASSNAME); }
    SClass_ID       SuperClassID()          { return SCENE_EXPORT_CLASS_ID; }
    Class_ID        ClassID()               { return MYEXP_CLASSID; }
    const TCHAR*    Category()              { return _T("");  }
    const TCHAR*    InternalName()          { return _T("RMDExporter"); }
    HINSTANCE       HInstance()             { return g_hInstance; }
};

static MyExporterClassDesc g_ExportCD;
ClassDesc* GetSceneExportDesc() { return &g_ExportCD; }
MyExporter::MyExporter(): m_exportSelected(false), m_suppressPrompts(false), m_ip(NULL), m_expip(NULL), m_fileStream(NULL){}
int MyExporter::ExtCount() {return 1; }
const TCHAR * MyExporter::Ext(int n){switch(n) {case 0:return (_T(MYEXP_EXT));}return _T("");}
const TCHAR * MyExporter::LongDesc(){return GetString(IDS_LONGDESC);}
const TCHAR * MyExporter::ShortDesc(){return GetString(IDS_SHORTDESC);}
const TCHAR * MyExporter::AuthorName(){return GetString(IDS_AUTHOR);}
const TCHAR * MyExporter::CopyrightMessage(){return GetString(IDS_COPYRIGHT);}
const TCHAR * MyExporter::OtherMessage1(){return GetString(IDS_OTHER1);}
const TCHAR * MyExporter::OtherMessage2() {return GetString(IDS_OTHER2); }
unsigned int MyExporter::Version(){return MYEXP_VER;}
static BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){MyExporter * myexp = (MyExporter*)GetWindowLong(hWnd,GWL_USERDATA);if (!myexp && msg != WM_INITDIALOG)return FALSE;switch (msg) {case WM_INITDIALOG: myexp = (MyExporter*)lParam;SetWindowLong(hWnd,GWL_USERDATA,lParam);break;case WM_DESTROY:break;case WM_COMMAND:switch (LOWORD(wParam)) {case IDOK:EndDialog(hWnd,1);break;}break;default:return FALSE;}return TRUE;}
void MyExporter::ShowAbout(HWND hWnd){DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_ABOUT),hWnd,AboutDlgProc,(LPARAM)this);}
BOOL MyExporter::SupportsOptions(int ext, DWORD options){if (options & SCENE_EXPORT_SELECTED) return TRUE;else return FALSE;}

//MyExporter* wthis;

static INT_PTR CALLBACK DebugDlgProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
	MyExporter *expo= (MyExporter*)GetWindowLongPtr(hWnd,GWLP_USERDATA); 
	switch (msg) 
	{
		case WM_INITDIALOG:
			expo = (MyExporter*)lParam;
			SetWindowLongPtr(hWnd,GWLP_USERDATA,lParam); 
			CenterWindow(hWnd, GetParent(hWnd));
			expo->DebugDlgs = &hWnd;
			
			expo->StartExporting();

			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
				case IDOK:
					EndDialog(hWnd, 1);
					break;
			}
			break;

			default:
				return FALSE;
	}
	return TRUE;
}  

static INT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
	MyExporter *expo = (MyExporter*)GetWindowLongPtr(hWnd,GWLP_USERDATA); 
	switch (msg) 
	{
		case WM_INITDIALOG:
			expo = (MyExporter*)lParam;
			SetWindowLongPtr(hWnd,GWLP_USERDATA,lParam); 
			CenterWindow(hWnd, GetParent(hWnd));
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
				case IDOK:
					if ( IsDlgButtonChecked(hWnd,IDC_CMESH) == 1 )		expo->exportmode |= EXPORT_MESH;
					if ( IsDlgButtonChecked(hWnd,IDC_CLIGHT) == 1 )		expo->exportmode |= EXPORT_LIGHT;
					if ( IsDlgButtonChecked(hWnd,IDC_CCAMERA) == 1 )	expo->exportmode |= EXPORT_CAMERA;
					if ( IsDlgButtonChecked(hWnd,IDC_CKEYFRAME) == 1 )	expo->exportmode |= EXPORT_KEYFRAME;
					if ( IsDlgButtonChecked(hWnd,IDC_CPHYSIQUE) == 1 )	expo->exportmode |= EXPORT_PHYSIQUE;
					if ( IsDlgButtonChecked(hWnd,IDC_CSKELETAL) == 1 )	expo->exportmode |= EXPORT_SKELETAL;
					if ( IsDlgButtonChecked(hWnd,IDC_MATERIALS) == 1 )	expo->exportmode |= EXPORT_MATERIALS;
					if ( IsDlgButtonChecked(hWnd,IDC_CCANIM) == 1 )		expo->exportmode |= EXPORT_CANIM;
					if ( IsDlgButtonChecked(hWnd,IDC_CENTITY) == 1 )	expo->exportmode |= EXPORT_ENTITY;
					if ( IsDlgButtonChecked(hWnd,IDC_CTANGENT) == 1 )	expo->exportmode |= EXPORT_TANGENT;

					EndDialog(hWnd, 1);
					break;

				case IDCANCEL:

					EndDialog(hWnd, 0);
					break;

				case IDC_BMOD:
					CheckDlgButton(hWnd,IDC_CMESH,		1);
					CheckDlgButton(hWnd,IDC_CLIGHT,		0);
					CheckDlgButton(hWnd,IDC_CCAMERA,	0);
					CheckDlgButton(hWnd,IDC_CKEYFRAME,	0);
					CheckDlgButton(hWnd,IDC_CPHYSIQUE,	1);
					CheckDlgButton(hWnd,IDC_CSKELETAL,	0);
					CheckDlgButton(hWnd,IDC_MATERIALS,	1);
					CheckDlgButton(hWnd,IDC_CCANIM,		0);
					CheckDlgButton(hWnd,IDC_CENTITY,	0);
					break;

				case IDC_BMAP:
					CheckDlgButton(hWnd,IDC_CMESH,		1);
					CheckDlgButton(hWnd,IDC_CLIGHT,		1);
					CheckDlgButton(hWnd,IDC_CCAMERA,	1);
					CheckDlgButton(hWnd,IDC_CKEYFRAME,	0);
					CheckDlgButton(hWnd,IDC_CPHYSIQUE,	0);
					CheckDlgButton(hWnd,IDC_CSKELETAL,	0);
					CheckDlgButton(hWnd,IDC_MATERIALS,	1);
					CheckDlgButton(hWnd,IDC_CCANIM,		0);
					CheckDlgButton(hWnd,IDC_CENTITY,	1);
					break;

				case IDC_BSCENE:
					CheckDlgButton(hWnd,IDC_CMESH,		1);
					CheckDlgButton(hWnd,IDC_CLIGHT,		1);
					CheckDlgButton(hWnd,IDC_CCAMERA,	1);
					CheckDlgButton(hWnd,IDC_CKEYFRAME,	0);
					CheckDlgButton(hWnd,IDC_CPHYSIQUE,	0);
					CheckDlgButton(hWnd,IDC_CSKELETAL,	0);
					CheckDlgButton(hWnd,IDC_MATERIALS,	1);
					CheckDlgButton(hWnd,IDC_CCANIM,		1);
					CheckDlgButton(hWnd,IDC_CENTITY,	1);
					break;

				case IDC_BSKELETAL:
					CheckDlgButton(hWnd,IDC_CMESH,		0);
					CheckDlgButton(hWnd,IDC_CLIGHT,		0);
					CheckDlgButton(hWnd,IDC_CCAMERA,	0);
					CheckDlgButton(hWnd,IDC_CKEYFRAME,	0);
					CheckDlgButton(hWnd,IDC_CPHYSIQUE,	0);
					CheckDlgButton(hWnd,IDC_CSKELETAL,	1);
					CheckDlgButton(hWnd,IDC_MATERIALS,	0);
					CheckDlgButton(hWnd,IDC_CCANIM,		0);
					CheckDlgButton(hWnd,IDC_CENTITY,	0);
					break;
			}
			break;

			default:
				return FALSE;
	}
	return TRUE;
}  

int MyExporter::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
    TCHAR buf[256];

    // cache export session stuff for utility methods
    m_exportSelected = (options & SCENE_EXPORT_SELECTED);
    m_suppressPrompts = !!suppressPrompts;
    m_ip = i;
    m_expip = ei;
	exportmode = 0;

	if (!DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_EXPORT),m_ip->GetMAXHWnd(), ExportDlgProc, (long)this)) 
	{
		return 1;
	}

    // open file
    fopen_s(&m_fileStream,name, ("wb"));
    if (!m_fileStream) 
	{
        ::sprintf_s(buf, GetString(IDS_OPENFILEERR), name);
        ::MessageBox(i->GetMAXHWnd(), buf, GetString(IDS_EXPCAPTION), MB_OK | MB_ICONSTOP);
        return FALSE; // fail
    }
	
	if (DEBUGMODE == 1 || DEBUGMODE == 3)
	{			
		DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_DEBUG),m_ip->GetMAXHWnd(), DebugDlgProc, (long)this);
	}
	else
	{
		StartExporting();
	}

    return TRUE;
}

MyExporter::~MyExporter()
{

}
