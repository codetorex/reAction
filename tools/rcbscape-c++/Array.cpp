#include "StdAfx.h"
#include "array.h"
#include "resource.h"
#include "../../reAction/rcabinet.h"
#include "../../reAction/rtools.h"

using namespace RTools;

CString GetItemPath(CTreeCtrl* kr, HTREEITEM tzrt,HTREEITEM rt)
{
	HTREEITEM ups;
	ups = tzrt;
	CString* depth;
	CString final;
	final = "";
	int ta = 0;
	long itmdpt;

	itmdpt = GetItemDepth(kr,tzrt,rt);
	if ( itmdpt > 0)
	{
		depth = new CString [itmdpt];


		

		while (! (ups==rt) )
		{
			depth[ta] = kr->GetItemText(ups);
			//tr.AddElement(kr->GetItemText(ups));
			/*final = final + kr->GetItemText(ups);
			final = final + "/";*/
			ups = kr->GetParentItem(ups);
			ta++;
			
		};

		for (int i = ta-1;i>=0;i--)
		{
			final = final + depth[i] + "/";
		}
		delete [] depth;
	}
	else
	{
		final = "";
	}
	//MessageBox(0,final,"Burak",MB_OK);
	return final;
}

long GetItemDepth(CTreeCtrl* kr, HTREEITEM tzrt,HTREEITEM rt)
{
	HTREEITEM ups;
	ups = tzrt;
	int ta = 0;

	while (! (ups==rt) )
	{
		ups = kr->GetParentItem(ups);
		ta++;
		
	};
	return ta;
}

HTREEITEM IsItemExist(CTreeCtrl* kr,HTREEITEM rt,char* nick)
{
	if (kr->ItemHasChildren(rt))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = kr->GetChildItem(rt);

		while (hChildItem != NULL)
		{
			hNextItem = kr->GetNextItem(hChildItem,TVGN_NEXT);
			if (strcmp(kr->GetItemText(hChildItem),nick) == 0)
			{
				return hChildItem;
			}
			
			hChildItem=hNextItem;
		}
	}
	return 0;
}

void CreatePath(CTreeCtrl* kr,HTREEITEM rt,char* path)
{
	char temp[256];
	char* sbs[128];
	long dept=0;
	int i;

	_asm
	{
		mov esi,path			// source = path
		lea edi,temp			// destination = temp
		xor edx,edx				// edx = 0 (depth of path)

		lea ecx,sbs				// ecx = sbs
		mov dword ptr [ecx],edi	// sbs[0] = edi (first dir name)
		inc edx					// incrase depth 


lop:
		mov al,byte ptr [esi]	// read 1 byte from source
		cmp al,0x2F				// is this "/"
		je slash				// then jump to 'slash'
		cmp al,0x00				// is this 0x00, end of string
		je ende					// jump to end

		mov byte ptr [edi],al	// write 1 byte to destination
		inc esi					// incrase source
		inc edi					// incrase destination
		jmp lop					// jump to lop for loop

slash:							// this is slash
		mov byte ptr [edi],0x00	// make it 0 in destination
		inc esi					// incrase source
		inc edi					// incrase destination

		mov eax,edx				// load depth to eax
		shl eax,2				// multiply depth with 4 and calculate next address
		lea ecx,sbs				// load sbs's address to ecx
		add eax,ecx				// add this address to next address

		mov dword ptr [eax],edi	// put current destination byte's address to sbs[depth]

		inc edx					// incrase depth
		jmp lop					// continue to loop
ende:							// end of loop
		mov byte ptr [edi],0x00	// put 0 to here and finish the string
		mov dept,edx			// put dept variable, depth for later use

	}
	
	HTREEITEM hChildItem;
	HTREEITEM gta;
	hChildItem = rt;


	for (i = 0;i<dept;i++)
	{
		gta = IsItemExist(kr,hChildItem,sbs[i]);
		if ( gta > 0 )
		{
			hChildItem = gta;
		}
		else
		{
			hChildItem = kr->InsertItem(sbs[i],1,1,hChildItem);
		}
	
	}

}

void LoadTree(CTreeCtrl* kr,HTREEITEM rt,RCabinet* rcb)
{
	int i;
	FILE* pak;
	RCBSubFile ss;
	char tmp[256];
	char* fnm;
	long a,b;

	pak = fopen(rcb->currentfile,"rb");
	
	fseek(pak,22,SEEK_SET);

	long pnext;

	for ( i = 0;i < rcb->myheader.filecount;i++)
	{
		rcb->ReadSubHeader(&ss,pak);
		a = strlen(ss.filename);
		fnm = GetFileName(ss.filename,a);
		b = strlen(fnm);

		memcpy(tmp,ss.filename,a-b);
		tmp[a-b-1] = 0;

		CreatePath(kr,rt,tmp);
		fseek(pak,ss.pnextfile,SEEK_SET);
	
	}
}


void LoadDir(CListView* kr,RCabinet* rcb,char* dir)
{
	int i;
	FILE* pak;
	RCBSubFile ss;
	char tmp[256];
	char* fnm;
	long a,b;

	pak = fopen(rcb->currentfile,"rb");
	
	fseek(pak,22,SEEK_SET);

	long pnext;

	for ( i = 0;i < rcb->myheader.filecount;i++)
	{
		rcb->ReadSubHeader(&ss,pak);
		a = strlen(ss.filename);
		fnm = GetFileName(ss.filename,a);
		b = strlen(fnm);

		memcpy(tmp,ss.filename,(a-b));
		tmp[a-b] = 0;

//			GetDocument()->rig->GetListCtrl().InsertItem(0,GetTreeCtrl().GetItemText(hChildItem),1);
		if (strcmp(dir,tmp) == 0)
		{
			kr->GetListCtrl().InsertItem(1,fnm,2);
		}
		
		//CreatePath(kr,rt,tmp);
		fseek(pak,ss.pnextfile,SEEK_SET);
	
	}	
}
