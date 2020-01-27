
//////////////////////////////////////////////////////////////////////////
/// WINDOW
//////////////////////////////////////////////////////////////////////////

GWindow::GWindow()
{
	//caption = 0;
	//cap_len = 0;
	pid = 0; // windows doesn't have parent
	x = 0;
	y = 0;
	w = 100;
	h = 100;
	cx = 0;
	cy = 0;
//	handle.visible = true;
	inactive = 1;
	sx = sy = 0;
	moving = false;
	caption_fc[0] = 1.0f;
	caption_fc[1] = 1.0f;
	caption_fc[2] = 1.0f;
	caption_fc[3] = 1.0f;
	owner = this;
	// Set Callbacks
	pClass = this;

	void (GWindow::*A)(int,int,int)			= &GWindow::MouseDown;
	void (GWindow::*B)(int,int,int,bool)	= &GWindow::MouseUp;
	void (GWindow::*C)(int,int,int);
	void (GWindow::*D)();


	*(long*)(&(MouseDown0)) = (long)((void*)(*(long*)((long)&A)));
	*(long*)(&(MouseUp0)) = (long)((void*)(*(long*)((long)&B)));
	//AttachFunctionPointer(LostFocus0,D);

	container = true;
	classID = GWINDOW_CLASSID;

	cbutton.pid = this;
	cbutton.pClass = this;

	A = &GWindow::cbutton_MouseDown;
	B = &GWindow::cbutton_MouseUp;
	C = &GWindow::cbutton_MouseMove;
	D = &GWindow::cbutton_MouseOut;

	AttachFunctionPointer(cbutton.MouseDown0	,A);
	AttachFunctionPointer(cbutton.MouseUp0		,B);
	AttachFunctionPointer(cbutton.MouseMove0	,C);
	AttachFunctionPointer(cbutton.MouseOut0		,D);

	cbstate = 0;

}

// close button events
void GWindow::cbutton_MouseDown(int x,int y,int button)
{
	cbstate = 1;
}

void GWindow::cbutton_MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		visible = false;
	}
	/*if (inside)
	{
		value -= smallchange;
		valuechanged();
	}*/
}

void GWindow::cbutton_MouseMove(int x,int y,int button)
{
	if (focused == &cbutton)
	{
		sig_wfmup ? cbstate = 1 : cbstate = 2;
	}
	else
	{
		if (!sig_wfmup) cbstate = 2;
	}
}

void GWindow::cbutton_MouseOut()
{
	cbstate = 0;
}

// END OF CBUTTON EVENTS

void GWindow::LoadHandle(int in_x,int in_y,int in_w,int in_h,long in_style,GuiSkin* in_skin)
{
	x = in_x;
	y = in_y;
	w = in_w;
	h = in_h;
	style = in_style;
	pid = 0 ;
	if (in_skin != NULL)
	{
		cskin = in_skin;
	}
	else
	{
		cskin = DefaultSkin;
	}
	
	padtop = cskin->window_quad.sc_h[2];
	padleft = cskin->window_quad.sc_h[0];
	padright = cskin->window_quad.sc_h[1];
	padbottom = cskin->window_quad.sc_h[3];

	SetRelative();
	cbutton.LoadHandle(w-23,2,cskin->win_closew,cskin->win_closeh,NULL);
	cbutton.pid = this;
	cbutton.SetRelative();
}

void GWindow::MouseDown(int x,int y,int button)
{
	if ( y <= cskin->window_quad.sc_h[2])
	{
		stx = *mx;
		sty = *my;
		updating = true;
		SetUpdating(true);
		SetRelative();
		dtx = stx - sx;
		dty = sty - sy;
		moving = true;
	}
}

void GWindow::MouseUp(int x,int y,int button,bool inside)
{
	if (moving)
	{
		moving = false;
		SetUpdating(false);
		updating = false;
	}
	if ( inside )
	{
		focused = this;
	}
}

void GWindow::doevents()
{
	if (!visible) return;
	cbutton.DeliverEvents();
	ContainerSendEvents();
	DeliverEvents();
	if (moving)
	{
		x = *mx - dtx;
		y = *my - dty;
		SetRelative();
		refresh();
		ContainerUpdate();
		cbutton.SetRelative();
	}
}

bool GWindow::IsGrab()
{
	if (*mx >= sx)
	{
		if (*my >= sy)
		{
			if (*mx <= sx + w)
			{
				if (*my <= sy + cskin->window_quad.sc_h[2] )
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GWindow::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->window_tex[inactive]); // attach left
	cskin->window_quad.Render(this);
	glColor4f(caption_fc[0],caption_fc[1],caption_fc[2],caption_fc[3]);
	cskin->basefont->PrintText(cx,cy,caption);

	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	if (inactive)
	{
		cskin->win_close[cbstate+3].Render(&cbutton);
	}
	else
	{
		cskin->win_close[cbstate].Render(&cbutton);
	}
	
	ContainerRender();
	//container.DrawHitboxes();
}

void GWindow::setcaption(char* st,...)
{
	char text[256];
	va_list ap;
	if (st == NULL) return;
	va_start(ap,st);
		vsprintf(text,st,ap);
	va_end(ap);
	slen = strlen(text);	
	if (caption == 0)
	{
		caption = new char [slen+1];
	}	
	strcpy(caption,text);
	
	swid = cskin->basefont->GetStringLen(caption,slen);

	if (cskin->window_cac)
	{
		cx = x + (w/2) - (swid/2);
		cy = y + cskin->window_quad.sc_h[2]/2 + cskin->basefont->fsize/3;
	}
	else
	{
		cx = x + (cskin->basefont->fsize/2) + cskin->window_quad.sc_h[0];
		cy = y + cskin->window_quad.sc_h[2]/2 + cskin->basefont->fsize/3;
	}
	
	rx = cx-x;
	ry = cy-y;

	
}

void GWindow::refresh()
{
	cx = x + rx;
	cy = y + ry;
}