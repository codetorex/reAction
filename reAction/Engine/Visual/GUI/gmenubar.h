
GMenuBar::GMenuBar()
{
	lastx = 0;
	owner = this;
	lastx = 2;
	state = false;

	pClass = this;
	void (GMenuBar::*A)()			= &GMenuBar::LostFocus;
	AttachFunctionPointer(LostFocus0,A);
}

void GMenuBar::CloseAllMenus()
{
	ObjectHandle* curobj;
	curobj = firstobject;

	for (long i = 0;i<objectcount;i++)
	{
		if (curobj->classID == GMENUBARITEM_CLASSID)
		{
			GMenuBarItem* menbar;
			menbar = (GMenuBarItem*)curobj;
			if (menbar->submenu!=0) menbar->submenu->visible = false;
		}
		curobj = curobj->nextobject;
	}	
}

void GMenuBar::LostFocus()
{
	state = false;
	CloseAllMenus();
}

void GMenuBar::SetupToWindow(GWindow* tw)
{
	cskin = tw->cskin;
	//pid = tw;
	h = 18;
	tw->AddObj(this);
	Dock(GDOCK_TOP,(ObjectContainer*)tw);
	SetRelative();
	tw->ContainerUpdate();

	/*cskin = tw->cskin;
	
	ObjectHandle* curobj;
	curobj = tw->firstobject;

	for (long i = 0;i< tw->objectcount;i++)
	{
		curobj->y += 18;
		curobj = curobj->nextobject;
	}
	tw->h += 18;

	x = cskin->window_quad.sc_h[0];
	y = cskin->window_quad.sc_h[2];
	w = tw->w - x - cskin->window_quad.sc_h[1];
	h = 18;

	pid = tw;
	tw->AddObj(this);
	SetRelative(this);

	tw->ContainerUpdate();*/
}


GMenuBarItem* GMenuBar::AddItem(char* caption,int imgid,void* func)
{
	int cplen; // caption length in pixels
	cplen = cskin->basefont->GetStringLen(caption);
	
	GMenuBarItem* bositem;
	bositem = new GMenuBarItem;
	bositem->LoadHandle(lastx,0,cplen+8,18,NULL);
	AddObj(bositem);
	//h += 19;
	lastx += cplen + 8;

	//void (*A)() = func;
	
	bositem->caption = caption;
	bositem->imgid = imgid;
	bositem->parent = this;
	AttachFunctionPointer(bositem->onClick,func);

	return bositem;
}

GMenuBarItem* GMenuBar::AddItem(char* caption,int imgid,GMenu* submenu)
{
	GMenuBarItem* bositem;
	bositem = AddItem(caption,imgid,(void*)0);
	bositem->submenu = submenu;
	AddObj(submenu);
	submenu->pid = this;
	submenu->x = bositem->x;
	submenu->y = h;
	//submenu->pid = bositem;
	submenu->SetRelative();
	currentmenu = submenu;
	
	
	return bositem;
}

void GMenuBar::update()
{
	SetRelative();
	ContainerUpdate();
}

void GMenuBar::doevents()
{
	if (!visible) return;
	ContainerSendEvents();
	//DeliverEvents();
}

void GMenuBar::render()
{
	if (!visible) return;
	/*glColor4f(1,0,0,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glBegin(GL_LINES);
		glVertex3f(sx,sy,0);
		glVertex3f(sx+w,sy,0);

		glVertex3f(sx,sy,0);
		glVertex3f(sx,sy+h,0);

		glVertex3f(sx,sy+h,0);
		glVertex3f(sx+w,sy+h,0);

		glVertex3f(sx+w,sy+h,0);
		glVertex3f(sx+w,sy,0);
	glEnd();*/
	/*glColor4f(0,0,0,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glBegin(GL_LINES);
		glVertex3f(sx,sy,0);
		glVertex3f(sx+w,sy,0);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->mbi_quad[1].Render(this);*/
	ContainerRender();
}