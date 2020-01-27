
//////////////////////////////////////////////////////////////////////////
/// CHECK BOX
//////////////////////////////////////////////////////////////////////////

GCheckBox::GCheckBox()
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
	value = 0;
	enabled = true;

	// Set Callbacks
	pClass = this;
	
	void (GCheckBox::*B)(int,int,int,bool)	= &GCheckBox::MouseUp;
	void (GCheckBox::*C)(int,int,int)		= &GCheckBox::MouseMove;
	void (GCheckBox::*D)()					= &GCheckBox::MouseOut;

	*(long*)(&(MouseUp0))					= (long)((void*)(*(long*)((long)&B)));
	*(long*)(&(MouseMove0))					= (long)((void*)(*(long*)((long)&C)));
	*(long*)(&(MouseOut0))					= (long)((void*)(*(long*)((long)&D)));
}

void GCheckBox::SetValue(int vl)
{
	value = vl;
	value ? state = 4 : state = 0;
}

void GCheckBox::doevents()
{
	DeliverEvents();
}

void GCheckBox::update()
{
	SetRelative();
	cx = sx + rx;
	cy = sy + ry;
}

void GCheckBox::MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		value == 0 ? value = 1 : value = 0;
	}
}

void GCheckBox::MouseOut()
{
	value ? state = 4 : state = 0;
}

void GCheckBox::MouseMove(int x,int y,int button)
{
	if (focused == this)
	{
		if (value == 0)
		{
			sig_wfmup ? state = 2: state = 1;
		}
		if (value == 1)
		{
			sig_wfmup ? state = 6: state = 5;
		}
	}
	else
	{
		if (value == 0)
		{
			state = 1;	
		}
		if (value == 1)
		{
			state = 5;
		}
	}
}

void GCheckBox::render()
{
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	//glBindTexture(GL_TEXTURE_2D,cskin->button_tex);

	
	// the rendering code
	glLoadIdentity();
	glTranslated(sx,sy,0);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	/*glBegin(GL_QUADS);
		DrawQuad(0,0,cskin->cbox_w,cskin->cbox_h,0.083f*state,0,0.083f*(state+1),1.0f);
	glEnd();*/
	/*glPushAttrib(GL_LIST_BIT);
	glListBase(cskin->cbox_base);
	glCallList(state);
	glPopAttrib();*/
	glCallList(cskin->cbox_base+state);
	// draw label
	glColor4f(caption_fc[0],caption_fc[1],caption_fc[2],caption_fc[3]);
	cskin->basefont->PrintText(cx,cy,caption);
	
}

void GCheckBox::setcaption(char* st,...)
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
	cx = x + cskin->cbox_w + 4;
	cy = y + cskin->cbox_h/2 + cskin->basefont->fsize/3;

	// relative position
	rx = cx-x;
	ry = cy-y;

	cx = sx + rx;
	cy = sy + ry;
}