
GToolbarButton::GToolbarButton()
{
	parent = 0;
	caption = 0;
	imgid = -1;
	state = 0;
	
	onClick = 0;
	pClass = this;

	classID = GTOOLBARBUTTON_CLASSID;

	void (GToolbarButton::*A)(int,int,int)			= &GToolbarButton::MouseDown;
	void (GToolbarButton::*B)(int,int,int,bool)		= &GToolbarButton::MouseUp;
	void (GToolbarButton::*C)(int,int,int)			= &GToolbarButton::MouseMove;
	void (GToolbarButton::*D)()						= &GToolbarButton::MouseOut;

	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseUp0,B);
	AttachFunctionPointer(MouseMove0,C);
	AttachFunctionPointer(MouseOut0,D);
}

void GToolbarButton::MouseDown(int x,int y,int button)
{
	state = 1;
}

void GToolbarButton::MouseUp(int x,int y,int button,bool inside)
{
	if ( inside )
	{
		if ( onClick != 0)
		{
			(*onClick)();
		}
	}
}

void GToolbarButton::MouseMove(int x,int y,int button)
{
	if (focused == this)
	{
		sig_wfmup ? state = 1 : state = 3;
	}
	else
	{
		if (!sig_wfmup) state = 3;
	}
}

void GToolbarButton::MouseOut()
{
	state = 0;
}

void GToolbarButton::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->tbb_quad[state].Render(this);

	/*if (bstyle == 0)
	{
		if (imglst!=0)
		{
			imglst->render(sx+4,sy+4,imgid,true);
		}
	}

	if (bstyle == 1)
	{
		if (imglst!=0)
		{
			imglst->render(sx+4,sy+4,imgid,true);
		}

		glColor3ubv(cskin->deffontclr);
		cskin->basefont->PrintText(sx+iw+8,sy+cskin->basefont->fsize+4,caption);
	}

	if (bstyle == 2)
	{
		glColor3ubv(cskin->deffontclr);
		cskin->basefont->PrintText(sx+5,sy+cskin->basefont->fsize+4,caption);
	}*/

	if (bstyle == 0)
	{
		if (imglst!=0)
		{
			imglst->render(sx+ix,sy+iy,imgid,true);
		}
	}

	if (bstyle == 1)
	{
		if (imglst!=0)
		{
			imglst->render(sx+ix,sy+iy,imgid,true);
		}

		glColor3ubv(cskin->deffontclr);
		cskin->basefont->PrintText(sx+cx,sy+cy,caption);
	}

	if (bstyle == 2)
	{
		glColor3ubv(cskin->deffontclr);
		cskin->basefont->PrintText(sx+cx,sy+cy,caption);
	}
}