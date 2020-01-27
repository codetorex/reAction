

// the main handler

GTreeview::GTreeview()
{
	starty = 6;
	//basenode.starty = 16; 
	basenode.parent = this;
	basenode.pid = this;
	basenode.level = 0;

	classID = GTREEVIEW_CLASSID;
	showln = true;
	showpm = true;
	showpic = true;
	snode = &basenode;
	onSelectionChanged = 0;
	scr.pid = this;
	scr.smallchange = 16;
	scr.largechange = 32;

	basenode.carescissor = true;
	basenode.defcarescissor = true;

	pClass = this;
	void (GTreeview::*A)(int,int,int)		= &GTreeview::MouseWheel;
	AttachFunctionPointer(MouseWheel0,A);
}

void GTreeview::MouseWheel(int x,int y, int delta)
{
	if (reqh > h)
	{
		scr.value -= delta / 8; //120 is WHEEL_DELTA but...
		scr.valuechanged();
	}
}

void GTreeview::refresh()
{
	//viewmax = (int)(ceil(h / cskin->basefont->fsize)+1);
	scr.w = cskin->vsb_w;
	scr.x = w - scr.w - cskin->sunkedge_quad[0].sc_h[1]+1;
	scr.y = cskin->sunkedge_quad[0].sc_h[2]-1;
	scr.h = h - cskin->sunkedge_quad[0].sc_h[3]-1;
	//scr.pid = this;
	scr.cskin = cskin;
	scr.stotal = 1;

	scr.svisible = h;
	scr.initialize();
	scr.refresh();

//	shaftlen = h - cskin->vsb_h - cskin->vsb_h;

	SetRelative();
	scr.SetRelative();
	//scr.calcthings();
}


void GTreeview::update()
{
	SetRelative();
	basenode.SetRelative();
	scr.update();
}

void GTreeview::doevents()
{
	if (reqh > h)
	{
		scr.doevents();
	}
	
	EnableEventScissor(sx,sy+3,w,h-6);
		basenode.doevents();
	sig_scissor = false;

	DeliverEvents();
}

void GTreeview::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	cskin->sunkedge_quad[0].Render(this);

	mEngine.EScissor(sx,sy+3,w,h-6); // makasla babos
	basenode.LoadHandle(5,5-scr.value,w-10,16,NULL);
	basenode.pid = this;
	basenode.SetRelative();
	basenode.starty = 16;
	basenode.render(); // after the render fetch how much pixels are used
	
	int newreqh = basenode.starty + 10;
	if (reqh != newreqh)
	{
		reqh = newreqh; // some of them is collapsed or expanded
		if ( reqh > h) // if it is bigger than our space update the scrollbar
		{
			scr.stotal = reqh;
			scr.initialize();
			scr.refresh();
		}
	}

	if (reqh > h)
	{
		scr.render();
	}
	else
	{
		scr.value = 0;
	}
	
	mEngine.DScissor();
	//glDisable(GL_SCISSOR_TEST);
}