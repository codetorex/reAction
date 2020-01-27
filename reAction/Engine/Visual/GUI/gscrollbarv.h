
//////////////////////////////////////////////////////////////////////////
///  VERTICAL SCROLL BAR
//////////////////////////////////////////////////////////////////////////

GVScrollBar::GVScrollBar()
{
	s_ubut = 8;
	s_dbut = 12;
	s_shft = 0;
	s_tbut = 0;	
	smallchange = 1;
	largechange = 3;
	value = 0;

	bup.pid = this;
	bdwn.pid = this;
	bthm.pid = this;

	// set callbacks
	bup.pClass = this;
	bdwn.pClass = this;
	bthm.pClass = this;

	void (GVScrollBar::*A)(int,int,int)			= &GVScrollBar::bup_MouseDown;
	void (GVScrollBar::*B)(int,int,int,bool)	= &GVScrollBar::bup_MouseUp;
	void (GVScrollBar::*C)(int,int,int)			= &GVScrollBar::bup_MouseMove;
	void (GVScrollBar::*D)()					= &GVScrollBar::bup_MouseOut;

	AttachFunctionPointer(bup.MouseDown0	,A);
	AttachFunctionPointer(bup.MouseUp0		,B);
	AttachFunctionPointer(bup.MouseMove0	,C);
	AttachFunctionPointer(bup.MouseOut0		,D);

	A = &GVScrollBar::bdwn_MouseDown;
	B = &GVScrollBar::bdwn_MouseUp;
	C = &GVScrollBar::bdwn_MouseMove;
	D = &GVScrollBar::bdwn_MouseOut;

	AttachFunctionPointer(bdwn.MouseDown0	,A);
	AttachFunctionPointer(bdwn.MouseUp0		,B);
	AttachFunctionPointer(bdwn.MouseMove0	,C);
	AttachFunctionPointer(bdwn.MouseOut0	,D);


	A = &GVScrollBar::bthm_MouseDown;
	B = &GVScrollBar::bthm_MouseUp;
	C = &GVScrollBar::bthm_MouseMove;
	D = &GVScrollBar::bthm_MouseOut;

	AttachFunctionPointer(bthm.MouseDown0	,A);
	AttachFunctionPointer(bthm.MouseUp0		,B);
	AttachFunctionPointer(bthm.MouseMove0	,C);
	AttachFunctionPointer(bthm.MouseOut0	,D);
}

// Up Arrow Button Events
void GVScrollBar::bup_MouseDown(int x,int y,int button)
{
	s_ubut = 9;
	fdown = GetTickCount();
	updown = true;
	firststage =true;

	value -= smallchange;
	valuechanged();
}

void GVScrollBar::bup_MouseUp(int x,int y,int button,bool inside)
{
	firststage = false;
	secondstage = false;
	/*if (inside)
	{
		value -= smallchange;
		valuechanged();
	}*/
}

void GVScrollBar::bup_MouseMove(int x,int y,int button)
{
	if (focused == &bup)
	{
		sig_wfmup ? s_ubut = 9 : s_ubut = 11;
	}
	else
	{
		if (!sig_wfmup) s_ubut = 11;
	}
}

void GVScrollBar::bup_MouseOut()
{
	s_ubut = 8;
}

// Down Arrow Button Events

void GVScrollBar::bdwn_MouseDown(int x,int y,int button)
{
	s_dbut = 13;
	fdown = GetTickCount();
	updown = false;
	firststage =true;
	value +=smallchange;
	valuechanged();
}

void GVScrollBar::bdwn_MouseUp(int x,int y,int button,bool inside)
{
	firststage = false;
	secondstage = false;
	/*if (inside)
	{
		value +=smallchange;
		valuechanged();
	}*/
}

void GVScrollBar::bdwn_MouseMove(int x,int y,int button)
{
	if (focused == &bdwn)
	{
		sig_wfmup ? s_dbut = 13 : s_dbut = 15;
	}
	else
	{
		if (!sig_wfmup) s_dbut = 15;
	}
}

void GVScrollBar::bdwn_MouseOut()
{
	s_dbut = 12;
}


// Thumb Button Events
void GVScrollBar::bthm_MouseDown(int x,int y,int button)
{
	
	s_tbut = 1;
	bthm.SetRelative();
	//dtx = *mx - bthm.sx;
	dty = *my - bthm.sy;
	moving = true;
}

void GVScrollBar::bthm_MouseUp(int x,int y,int button,bool inside)
{
	moving = false;
	//QuickInfo("bthm pressed");
}

void GVScrollBar::bthm_MouseMove(int x,int y,int button)
{
	if (focused == &bthm)
	{
		sig_wfmup ? s_tbut = 1 : s_tbut = 2;
	}
	else
	{
		if (!sig_wfmup) s_tbut = 2;
	}
}

void GVScrollBar::bthm_MouseOut()
{
	s_tbut = 0;
}

void GVScrollBar::initialize()
{
	w = cskin->vsb_w;
	
	bup.x = 0;
	bup.y = 0;
	bup.w = cskin->vsb_w;
	bup.h = cskin->vsb_h;

	bdwn.x = 0;
	bdwn.y = h-cskin->vsb_h;
	bdwn.w = cskin->vsb_w;
	bdwn.h = cskin->vsb_h;

	bthm.x = 0;
	bthm.y = cskin->vsb_h;
	bthm.w = cskin->vsb_w;

	bup.SetRelative();
	bdwn.SetRelative();
}

void GVScrollBar::valuechanged()
{
	if (value < 0 ) value = 0;	
	if ( value > extradatah ) value = extradatah;

	bthm.y = cskin->vsb_h + (int)((float)value / freq);
	bthm.SetRelative();
}

void GVScrollBar::fixedrefresh()
{
	int old_val;
	float old_freq;
	old_val = value;
	old_freq = freq;
	
	refresh();

	value = Ratio(old_freq,old_val,freq);
	valuechanged();
}

void GVScrollBar::refresh() // when parameters changed call this
{
	
	shafth = h - cskin->vsb_h - cskin->vsb_h;
	bthm.h = ((shafth * svisible) / stotal);

	extradatah = stotal - svisible;
	exh = shafth - bthm.h;

	freq = (float)extradatah / (float)exh;

	bthm.y = cskin->vsb_h + (int)((float)value * freq);

	bthm.SetRelative();
}

void GVScrollBar::update()
{
	SetRelative();
	bthm.SetRelative();
	bup.SetRelative();
	bdwn.SetRelative();
}

void GVScrollBar::doevents()
{	
	bup.DeliverEvents();
	bdwn.DeliverEvents();
	bthm.DeliverEvents();
	if (moving)
	{
		bthm.y = *my - dty - sy;
		value = (int)((float)(bthm.y-cskin->vsb_h) * freq);
		valuechanged();
	}
	if (firststage)
	{
		if (GetTickCount() - fdown > 400) // wait 400 ms for adding numbers
		{
			secondstage = true;
			firststage = false;
		}
	}
	if (secondstage)
	{
		if (GetTickCount() - fdown > 70) // every 70 ms add or decrease numbers
		{
			if (updown)
			{
				value -= smallchange;
				valuechanged();
			}
			else
			{
				value += smallchange;
				valuechanged();
			}
			fdown = GetTickCount();
		}
	}
	DeliverEvents();
	
}

void GVScrollBar::render()
{
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	//glBindTexture(GL_TEXTURE_2D,cskin->button_tex);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	//glDisable(GL_BLEND);
	// the rendering code
	glLoadIdentity();
	glTranslated(sx,sy,0);

	cskin->vss_quad[s_shft].Render(0,cskin->vsb_h,cskin->vsb_w,bdwn.y,false);

	//render up button
	glCallList(cskin->vsb_base+s_ubut);
	

	//s_tbut = bthm.state;
	bthm.SetRelative();
	cskin->vst_quad[s_tbut].Render(&bthm);
	//	cskin->vst_quad[s_tbut].Render(bthm.sx,bthm.sy,bthm.w,bthm.h,true);
	//cskin->vst_quad[s_tbut].Render(bthm.sx,bthm.sy,bthm.w,bthm.h);
	//QuickInfo("%i %i %i %i",bthm.sx,bthm.sy,bthm.w,bthm.h);
	/*glBegin(GL_QUADS);

		switch(s_tbut)
		{
			case 0:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.0f,0.0f,0.33f,1.0f);
				break;
			case 1:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.33f,0.0f,0.66f,1.0f);
				break;
			case 2:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.66f,0.0f,1.0f,1.0f);
				break;
		}
		//DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.0f,0.0f,0.33f,1.0f);
	glEnd();*/

	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	glLoadIdentity();
	glTranslated(bdwn.sx,bdwn.sy,0);
	glCallList(cskin->vsb_base+s_dbut);
	
	

	//glEnable(GL_BLEND);
	//glTranslated(0,ry,0);

	/*glBegin(GL_QUADS);
		DrawQuad(0,0,cskin->cbox_w,cskin->cbox_h,0.083f*state,0,0.083f*(state+1),1.0f);
	glEnd();*/
	/*glPushAttrib(GL_LIST_BIT);
	glListBase(cskin->cbox_base);
	glCallList(state);
	glPopAttrib();*/
//	glCallList(cskin->cbox_base+state);
	// draw label
	/*glColor4f(1.0f,1.0f,1.0f,1.0f);
	cskin->basefont->PrintText(100,100,"%i",bthm.state);*/
}