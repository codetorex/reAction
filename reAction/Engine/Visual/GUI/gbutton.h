
//////////////////////////////////////////////////////////////////////////
/// BUTTON
//////////////////////////////////////////////////////////////////////////

GButton::GButton()
{
	caption = 0;
	//cap_len = 0;
	pid = 0; // windows doesn't have parent
	x = 0;
	y = 0;
	w = 100;
	h = 100;
//	handle.visible = true;
	state = 0;
	//active = true;
	caption_fc[0] = 0.0f;
	caption_fc[1] = 0.0f;
	caption_fc[2] = 0.0f;
	caption_fc[3] = 1.0f;
	enabled = true;
	continuous = false;
	delay = 70;
	tcounter = 0;

	// Set Callbacks
	pClass = this;

	void (GButton::*A)(int,int,int)			= &GButton::MouseDown;
	void (GButton::*B)(int,int,int,bool)	= &GButton::MouseUp;
	void (GButton::*C)(int,int,int)			= &GButton::MouseMove;
	void (GButton::*D)()					= &GButton::MouseOut;

	*(long*)(&(MouseDown0))					= (long)((void*)(*(long*)((long)&A)));
	*(long*)(&(MouseUp0))					= (long)((void*)(*(long*)((long)&B)));
	*(long*)(&(MouseMove0))					= (long)((void*)(*(long*)((long)&C)));
	*(long*)(&(MouseOut0))					= (long)((void*)(*(long*)((long)&D)));
}

void GButton::MouseDown(int x,int y,int button)
{
	state = 1;
}

void GButton::MouseUp(int x,int y,int button,bool inside)
{
	if ( inside )
	{
		if ( onClick != 0)
		{
			(*onClick)();
		}
	}
}

void GButton::MouseMove(int x,int y,int button)
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

void GButton::MouseOut()
{
	state = 0;
}

void GButton::doevents()
{
	DeliverEvents();
	if (continuous)
	{
		if (state == 1)
		{
			if ((int)GetTickCount() - tcounter > delay )
			{
				if ( onClick != 0)
				{
					(*onClick)();
				}
				tcounter = GetTickCount();
			}
		}
	}
}

void GButton::update()
{
	SetRelative();
	cx = sx + rx;
	cy = sy + ry;
}

void GButton::render()
{
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	cskin->button_quad[state].Render(this);
	glColor4f(caption_fc[0],caption_fc[1],caption_fc[2],caption_fc[3]);
	cskin->basefont->PrintText(cx,cy,caption);
}

void GButton::setcaption(char* st,...)
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

	// a buttons caption is always center aligned
	cx = x + w/2 - swid/2;
	cy = y + h/2 + cskin->basefont->fsize/3;

	// relative position
	rx = cx-x;
	ry = cy-y;

	cx = sx + rx;
	cy = sy + ry;
	
}