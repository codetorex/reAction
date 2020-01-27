
GComboBox::GComboBox()
{
	s_udbut = 0;
	text = 0;
	state = 0;

	cbox_fc[0] = 0.0f;
	cbox_fc[1] = 0.0f;
	cbox_fc[2] = 0.0f;
	cbox_fc[3] = 1.0f;

	lst.visible = false;
	lst.pid = this;
	lst.pEventClass = this;
	
	pClass = this;
	ddbut.pid = this;
	ddbut.pClass = this;

	
	void (GComboBox::*A)(int,int,int)		= &GComboBox::ddbut_MouseDown;
	void (GComboBox::*B)(int,int,int,bool)	= &GComboBox::ddbut_MouseUp;
	void (GComboBox::*C)(int,int,int)		= &GComboBox::ddbut_MouseMove;
	void (GComboBox::*D)()					= &GComboBox::ddbut_MouseOut;
	void (GComboBox::*E)(int)				= &GComboBox::lst_onSelectionChanged;
	void (GComboBox::*F)()					= &GComboBox::lst_onLostFocus;


	B = &GComboBox::MouseUp;
	AttachFunctionPointer(ddbut.MouseDown0			,A);
	AttachFunctionPointer(ddbut.MouseUp0			,B);
	AttachFunctionPointer(ddbut.MouseMove0			,C);
	AttachFunctionPointer(ddbut.MouseOut0			,D);
	AttachFunctionPointer(lst.onSelectionChanged	,E);
	AttachFunctionPointer(lst.onLostFocus			,F);
	AttachFunctionPointer(MouseUp0					,B);
}

void GComboBox::MouseUp(int x,int y,int button,bool inside)
{
	ddbut_MouseUp(x,y,button,inside);
}

void GComboBox::ddbut_MouseDown(int x,int y,int button)
{
	s_udbut = 1;
}

void GComboBox::ddbut_MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		// open list box here
		lst.visible = !lst.visible;
		lst.scr.value = lst.ListIndex;
		lst.scr.valuechanged();
		if ( lst.visible == true) lst.SetFocus();
		sig_cbox = lst.visible;
	}
}

void GComboBox::ddbut_MouseMove(int x,int y,int button)
{
	if (focused == &ddbut)
	{
		sig_wfmup ? s_udbut = 1 : s_udbut = 3;
	}
	else
	{
		if (!sig_wfmup) s_udbut = 3;
	}	
}

void GComboBox::ddbut_MouseOut()
{
	s_udbut = 0;
}

void GComboBox::lst_onSelectionChanged(int indx)
{
	lst.visible = false;
	text = lst.ListData[indx];
	sig_event = 0;
	sig_cbox = false;
}

void GComboBox::lst_onLostFocus()
{
	if (focused != &ddbut && focused != this && focused != &lst.scr && focused != &lst.scr.bup && focused != &lst.scr.bdwn && focused != &lst.scr.bthm )
	{
		lst.visible = false;
		sig_cbox = false;
	}
}

void GComboBox::initialize()
{
	h = cskin->cbb_h + 2;
	
	ddbut.w = cskin->cbb_w;
	ddbut.h = cskin->cbb_h;
	ddbut.x = w - ddbut.w - 1;
	ddbut.y = 1;

	lst.cskin = cskin;
	
	lst.x = 0;
	lst.y = h+1;
	lst.w = w;
	lst.h = cskin->basefont->fsize*7 +3;
	lst.refresh();

	SetRelative();
	ddbut.SetRelative();
	lst.SetRelative();
}

void GComboBox::doevents()
{
	if (!visible) return;
	ddbut.DeliverEvents();
	lst.doevents();
	DeliverEvents();
}

void GComboBox::update()
{
	SetRelative();
	ddbut.SetRelative();
	//SetRelative(&lst);
	lst.update();
}

void GComboBox::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);

	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->sunkedge_quad[state].Render(this);

	cskin->cbb_quad[s_udbut].Render(&ddbut);
	/*glBegin(GL_QUADS);
		switch(s_udbut)
		{
			case 0:
				DrawQuad(ddbut.x,ddbut.y,ddbut.x+ddbut.w,ddbut.y+ddbut.h,0.0f,0.0f,0.25f,1.0f);
				break;
			case 1:
				DrawQuad(ddbut.x,ddbut.y,ddbut.x+ddbut.w,ddbut.y+ddbut.h,0.25f,0.0f,0.50f,1.0f);
				break;
			case 2:
				DrawQuad(ddbut.x,ddbut.y,ddbut.x+ddbut.w,ddbut.y+ddbut.h,0.50f,0.0f,0.75f,1.0f);
				break;
			case 3:
				DrawQuad(ddbut.x,ddbut.y,ddbut.x+ddbut.w,ddbut.y+ddbut.h,0.75f,0.0f,1.0f,1.0f);
				break;
		}
		//DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.0f,0.0f,0.33f,1.0f);
	glEnd();*/
	
	lst.render();
	
	glColor4f(cbox_fc[0],cbox_fc[1],cbox_fc[2],cbox_fc[3]);
	if (text != 0)
	{
		cskin->basefont->PrintText(sx+4,sy+cskin->basefont->fsize+1,text);
	}
	
	
	//glTranslated(ddbut.x,0,0);
	
	
	//glCallList(cskin->vsb_base+s_udbut);

}