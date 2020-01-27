
//////////////////////////////////////////////////////////////////////////
///  VERTICAL SCROLL BAR
//////////////////////////////////////////////////////////////////////////

GHScrollBar::GHScrollBar()
{
	s_ubut = 0;
	s_dbut = 4;
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

	void (GHScrollBar::*A)(int,int,int)			= &GHScrollBar::bup_MouseDown;
	void (GHScrollBar::*B)(int,int,int,bool)	= &GHScrollBar::bup_MouseUp;
	void (GHScrollBar::*C)(int,int,int)			= &GHScrollBar::bup_MouseMove;
	void (GHScrollBar::*D)()					= &GHScrollBar::bup_MouseOut;

	AttachFunctionPointer(bup.MouseDown0	,A);
	AttachFunctionPointer(bup.MouseUp0		,B);
	AttachFunctionPointer(bup.MouseMove0	,C);
	AttachFunctionPointer(bup.MouseOut0		,D);

	A = &GHScrollBar::bdwn_MouseDown;
	B = &GHScrollBar::bdwn_MouseUp;
	C = &GHScrollBar::bdwn_MouseMove;
	D = &GHScrollBar::bdwn_MouseOut;

	AttachFunctionPointer(bdwn.MouseDown0	,A);
	AttachFunctionPointer(bdwn.MouseUp0		,B);
	AttachFunctionPointer(bdwn.MouseMove0	,C);
	AttachFunctionPointer(bdwn.MouseOut0	,D);


	A = &GHScrollBar::bthm_MouseDown;
	B = &GHScrollBar::bthm_MouseUp;
	C = &GHScrollBar::bthm_MouseMove;
	D = &GHScrollBar::bthm_MouseOut;

	AttachFunctionPointer(bthm.MouseDown0	,A);
	AttachFunctionPointer(bthm.MouseUp0		,B);
	AttachFunctionPointer(bthm.MouseMove0	,C);
	AttachFunctionPointer(bthm.MouseOut0	,D);
}

// Up Arrow Button Events
void GHScrollBar::bup_MouseDown(int x,int y,int button)
{
	s_ubut = 1;
	fdown = GetTickCount();
	updown = true;
	firststage =true;

	value -= smallchange;
	valuechanged();
}

void GHScrollBar::bup_MouseUp(int x,int y,int button,bool inside)
{
	firststage = false;
	secondstage = false;
	/*if (inside)
	{
		value -= smallchange;
		valuechanged();
	}*/
}

void GHScrollBar::bup_MouseMove(int x,int y,int button)
{
	if (focused == &bup)
	{
		sig_wfmup ? s_ubut = 1 : s_ubut = 3;
	}
	else
	{
		if (!sig_wfmup) s_ubut = 3;
	}
}

void GHScrollBar::bup_MouseOut()
{
	s_ubut = 0;
}

// Down Arrow Button Events

void GHScrollBar::bdwn_MouseDown(int x,int y,int button)
{
	s_dbut = 5;
	fdown = GetTickCount();
	updown = false;
	firststage =true;
	value +=smallchange;
	valuechanged();
}

void GHScrollBar::bdwn_MouseUp(int x,int y,int button,bool inside)
{
	firststage = false;
	secondstage = false;
	/*if (inside)
	{
		value +=smallchange;
		valuechanged();
	}*/
}

void GHScrollBar::bdwn_MouseMove(int x,int y,int button)
{
	if (focused == &bdwn)
	{
		sig_wfmup ? s_dbut = 5 : s_dbut = 7;
	}
	else
	{
		if (!sig_wfmup) s_dbut = 7;
	}
}

void GHScrollBar::bdwn_MouseOut()
{
	s_dbut = 4;
}


// Thumb Button Events
void GHScrollBar::bthm_MouseDown(int x,int y,int button)
{
	
	s_tbut = 1;
	bthm.SetRelative();
	dtx = *mx - bthm.sx;
	//dty = *my - bthm.sy;
	moving = true;
}

void GHScrollBar::bthm_MouseUp(int x,int y,int button,bool inside)
{
	moving = false;
	//QuickInfo("bthm pressed");
}

void GHScrollBar::bthm_MouseMove(int x,int y,int button)
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

void GHScrollBar::bthm_MouseOut()
{
	s_tbut = 0;
}

void GHScrollBar::initialize()
{
	h = cskin->vsb_w;
	
	bup.x = 0;
	bup.y = 0;
	bup.w = cskin->vsb_w;
	bup.h = cskin->vsb_h;

	bdwn.x = w-cskin->vsb_w;
	bdwn.y = 0;
	bdwn.w = cskin->vsb_w;
	bdwn.h = cskin->vsb_h;

	bthm.y = 0;
	bthm.x = cskin->vsb_w;
	bthm.h = cskin->vsb_h;

	bup.SetRelative();
	bdwn.SetRelative();
}

void GHScrollBar::valuechanged()
{
	if (value < 0 ) value = 0;	
	if ( value > extradatah ) value = extradatah;

	bthm.x = cskin->vsb_h + (int)((float)value / freq);
	bthm.SetRelative();
}

void GHScrollBar::fixedrefresh()
{
	int old_val;
	float old_freq;
	old_val = value;
	old_freq = freq;
	
	refresh();

	value = Ratio(old_freq,old_val,freq);
	valuechanged();
}

void GHScrollBar::refresh() // when parameters changed call this
{
	
	shafth = w - cskin->vsb_h - cskin->vsb_h;
	bthm.w = ((shafth * svisible) / stotal);

	extradatah = stotal - svisible;
	exh = shafth - bthm.w;

	freq = (float)extradatah / (float)exh;

	bthm.x = cskin->vsb_h + (int)((float)value * freq);

	bthm.SetRelative();
}

void GHScrollBar::update()
{
	SetRelative();
	bthm.SetRelative();
	bup.SetRelative();
	bdwn.SetRelative();
}

void GHScrollBar::doevents()
{	
	bup.DeliverEvents();
	bdwn.DeliverEvents();
	bthm.DeliverEvents();
	if (moving)
	{
		bthm.x = *mx - dtx - sx;
		value = (int)((float)(bthm.x-cskin->vsb_h) * freq);
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

void GHScrollBar::render()
{
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	//glBindTexture(GL_TEXTURE_2D,cskin->button_tex);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	//cskin->hst_quad[0].Render(this);
	//glDisable(GL_BLEND);
	// the rendering code
	glLoadIdentity();
	glTranslated(sx,sy,0);

	cskin->hss_quad[s_shft].Render(cskin->vsb_w,0,bdwn.x,h,false);
	//cskin->hss_quad->Render(this);

	//render up button
	glCallList(cskin->vsb_base+s_ubut);



	

	//s_tbut = bthm.state;
	bthm.SetRelative();
	cskin->hst_quad[s_tbut].Render(&bthm);
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