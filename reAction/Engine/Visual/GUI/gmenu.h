
// GMenu Functions
GMenu::GMenu()
{
	maxwidth = 0;
	owner = this;
	visible = false;
	lasty = 2;
//	imglst = 0;

	pClass = this;
	void (GMenu::*A)()				= &GMenu::LostFocus;
	AttachFunctionPointer(LostFocus0,A);
}

void GMenu::FixWidths()
{
	ObjectHandle* curobj;
	curobj = firstobject;

	for (long i = 0;i<objectcount;i++)
	{
		curobj->w = w - 4;;
		curobj = curobj->nextobject;
	}
}

void GMenu::LostFocus()
{
	visible = false;
}

void GMenu::AddItem(char* caption,int imgid,void *func,GImageList* imglst)
{
	int cplen; // caption length in pixels
	cplen = cskin->basefont->GetStringLen(caption);
	
	if (cplen > maxwidth)
	{
		maxwidth = cplen;
		w = cplen + 40; // 40 is for 14x14 icon and submenu arrow
		FixWidths();
	}
	
	GMenuItem* bositem;
	bositem = new GMenuItem;
	bositem->LoadHandle(2,lasty,w-4,19,NULL);
	AddObj(bositem);
	h += 19;
	lasty += 19;
	bositem->parent = this;

	//void (*A)() = func;
	
	bositem->caption = caption;
	bositem->imgid = imgid;
	bositem->seprator = false;
	bositem->imglst = imglst;
	AttachFunctionPointer(bositem->onClick,func);
}

void GMenu::AddSeprator()
{	
	GMenuItem* bositem;
	bositem = new GMenuItem;
	bositem->LoadHandle(2,lasty,w-4,8,NULL);
	AddObj(bositem);
	h += 8;
	lasty += 8;
	bositem->parent = this;
	bositem->seprator = true;
}

void GMenu::update()
{
	SetRelative();
	ContainerUpdate();
}

void GMenu::doevents()
{
	if (!visible) return;
	ContainerSendEvents();
	DeliverEvents();
}

void GMenu::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->mnb_quad.Render(this);
	ContainerRender();
}