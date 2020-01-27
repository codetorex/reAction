
#include "StdAfx.h"
#include "vgui.h"

using namespace RTools;

int *mx,*my;
void *held;
GuiSkin* DefaultSkin;

GListBox* dbox;

GDesktop mDesktop;

/* Rules of Fight Club

1. mouse down olan obje, mouse up olana kadar deismez
2. keydown olan obje belli bi thresoldda sürekli keydown olur
3. keyup olan obje keydowndan farklý bi obje olamaz zor zaten

*/

//////////////////////////////////////////////////////////////////////////
/// Event Functions
//////////////////////////////////////////////////////////////////////////

ObjectHandle* focused; // mouse downed object, waitng for mouse up, or keyboard caching object

dword	sig_event;
int		sig_char;
char	sig_mouse;
int		sig_x,sig_y;
bool	sig_wfmup; // wait for mouse up
int		sig_delta;
bool	sig_cbox;

int		sig_scissor_x;
int		sig_scissor_y;
int		sig_scissor_w;
int		sig_scissor_h;
bool	sig_scissor=false;

void EnableEventScissor(int x,int y,int w,int h)
{
	sig_scissor_x = x;
	sig_scissor_y = y;
	sig_scissor_w = w;
	sig_scissor_h = h;
	sig_scissor = true;
}

void KeyDownHandler(int keycode)
{
	sig_char = keycode;
	sig_event = GEVENT_KEYDOWN;
	//focused->doevents();
	if (focused != 0)
	{
		focused->DeliverEvents();
	}
	sig_event = 0;
}

void KeyUpHandler(int keycode)
{
	sig_char = keycode;
	sig_event = GEVENT_KEYUP;
	//focused->doevents();
	if (focused!=0)
	{
		focused->DeliverEvents();
	}
	sig_event = 0;
}

void KeyPressHandler(int keycode)
{
	sig_char = keycode;
	sig_event = GEVENT_KEYPRESS;
	//focused->doevents();
	if (focused!=0)
	{
		focused->DeliverEvents();
	}
	sig_event = 0;
}

void MouseDownHandler(int x,int y,char button)
{
	sig_x = x;
	sig_y = y;
	sig_event = GEVENT_MOUSEDOWN;
	//sig_char = button;
	sig_mouse = button;
	sig_wfmup = true;
	if (focused != 0)
	{
		if (sig_cbox)
		{
			if (focused->IsMouseInside())
			{
				focused->doevents();
				
			}
			else
			{
				//focused->visible = false;
			}
		}
	}
}

void MouseUpHandler(int x,int y,char button)
{
	sig_x = x;
	sig_y = y;
	sig_event = GEVENT_MOUSEUP;
	//sig_char = button;
	sig_mouse = button;
	if (focused != 0)
	{
		focused->doevents();
	}
	sig_event = 0;
	sig_wfmup = false;
}

void MouseWheelHandler(int x,int y,int delta)
{
	sig_x = x;
	sig_y = y;
	sig_event = GEVENT_MOUSEWHEEL;
	sig_delta = delta;
	if (focused!=0)
	{
		if (sig_cbox)
		{
			if (focused->IsMouseInside())
			{
				focused->doevents();
				
			}
			else
			{
				//focused->visible = false;
			}
		}
	}
}

void SetFocused(ObjectHandle *foc)
{
	focused = foc;
}

long GetFocused()
{
	return (long)focused;
}

void SetDebugBox(GListBox* abow)
{
	dbox = abow;
}

#include "objectcontainer.h"

#include "objecthandle.h"

#include "guitools.h"

#include "vfont.h"

#include "guiquad.h"
#include "guiskin.h"

#include "gwindow.h"
#include "gbutton.h"
#include "gcheckbox.h"
#include "gscrollbarv.h"
#include "gscrollbarh.h"
#include "glistbox.h" 
#include "gcombobox.h"
#include "gimage.h"
#include "gtextbox.h"
#include "gprogressbar.h"
#include "gmenu.h"
#include "gmenuitem.h"
#include "gmenubar.h"
#include "gmenubaritem.h"
#include "gimagelist.h"
#include "gdesktop.h"
#include "gtoolbar.h"
#include "gtoolbarbutton.h"
#include "gpanel.h"
#include "gtreeview.h"
#include "gtreenode.h"
#include "ggroupbox.h"
#include "gsplitcontainer.h"
#include "glistview.h"
#include "glistitem.h"
#include "gcolumnheader.h"
#include "gscrollbararea.h"
#include "gsliderh.h"

/* SELAM */
