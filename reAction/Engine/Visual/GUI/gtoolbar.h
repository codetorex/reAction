
GToolbar::GToolbar()
{
	owner = this;
	lastx = 2;
	maxw = 0;
	maxh = 0;
}

GToolbarButton* GToolbar::AddItem(char* caption,void* func,int bstyle,int imgid,GImageList* imglst)
{
	int rw,rh; // required width,required height
	int iw,ih; // img width, imgh height

	int cplen;
	cplen = cskin->basefont->GetStringLen(caption);


	if (imglst != 0)
	{
		iw = imglst->imagew[imgid];
		ih = imglst->imageh[imgid];
	}
	else
	{
		iw = 0;
		ih = 0;
	}
	rh = h - 4;
	
	GToolbarButton* bositem;
	bositem = new GToolbarButton;
	
	bositem->iw = iw;
	bositem->ih = ih;
	bositem->ix = 5;
	bositem->iy = (rh - ih)>>1;

	switch(bstyle)
	{
		case 0:
		rw = iw + 8;
		bositem->ix = 4;
		break;
	
		case 1:
		rw = iw + 6 + cplen + 8;
		bositem->cx = bositem->ix + iw + 3;
		break;

		case 2:
		rw = cplen +8;
		bositem->cx = bositem->ix;
		break;
	}
	bositem->cy = (rh /2) + (cskin->basefont->fsize/2)-2;



	bositem->LoadHandle(lastx,2,rw,rh,NULL);
	AddObj(bositem);
	lastx += rw;

	bositem->caption = caption;
	bositem->imgid = imgid;
	bositem->bstyle = bstyle;
	bositem->parent = this;
	bositem->imglst = imglst;


	AttachFunctionPointer(bositem->onClick,func);

	return bositem;
}

void GToolbar::SetupToWindow(GWindow* tw,int dock)
{
	cskin = tw->cskin;
	h = 30;
	tw->AddObj(this);
	Dock(dock,(ObjectContainer*)tw);
	SetRelative();
	tw->ContainerUpdate();
}

void GToolbar::doevents()
{
	if (!visible) return;
	ContainerSendEvents();
	//DeliverEvents();
}

void GToolbar::update()
{
	SetRelative();
	ContainerUpdate();
}

void GToolbar::render()
{
	if (!visible) return;


	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->tbr_quad.Render(this);

	ContainerRender();
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
}