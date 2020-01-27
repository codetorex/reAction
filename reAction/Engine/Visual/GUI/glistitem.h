
GListItem::GListItem()
{
	parent = 0;
	imglst = 0;
	subdata = 0;

	selected = false;

	pClass = this;

	void (GListItem::*A)(int,int,int)	= &GListItem::MouseDown;
	void (GListItem::*B)(int,int,int)	= &GListItem::MouseWheel;

	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseWheel0,B);
}


void GListItem::MouseDown(int x,int y,int button)
{
	if (!parent->multiselect)
	{
		parent->sitem = this;
	}
}

void GListItem::MouseWheel(int x,int y, int delta)
{
	parent->MouseWheel(x,y,delta);
}

/*void GListItem::doevents()
{
	DeliverEvents();
}*/

/*void GListItem::update()
{

}*/

void GListItem::SetCaption(char* cp)
{
	if ( cp != 0)
	{
		caption.data_type = type_charptr;
		caption.itemdata.chrptr = cp;
		caption.strrep = cp;

		int cppw; // caption pixel width
		cppw = cskin->basefont->GetStringLen(cp);
		if (cppw > parent->maxstrpw) parent->maxstrpw = cppw;
		strpw = cppw;
		cppw /= 2;

		cx = 16; // 32 / 2
		cx -= cppw;
	}
}

void GListItem::render()
{
/*	glLoadIdentity();
	RenderDebugArea();*/
	if (parent->rendermode == GLISTVIEW_ICON)
	{
		if (parent->sitem == this)
		{
			glColor4ub(42,76,137,255);
			imglst->render(sx,sy,imgid,true);
			glBindTexture(GL_TEXTURE_2D,0);
			glLoadIdentity();
			//glTranslated(sx,sy,0);
			glBegin(GL_QUADS);
				DrawQuad(sx+cx-2,sy+33,sx+cx+strpw+2,sy+34+cskin->basefont->fsize+2,0,0,0,0);
			glEnd();

			glColor3ub(255,255,255);
			cskin->basefont->PrintText(sx+cx,sy+32+cskin->basefont->fsize,caption.strrep);
		}
		else
		{
			glColor4f(1,1,1,1);
			imglst->render(sx,sy,imgid,true);
			glColor3ubv(cskin->deffontclr);
			cskin->basefont->PrintText(sx+cx,sy+32+cskin->basefont->fsize,caption.strrep);
		}
	}

	if (parent->rendermode == GLISTVIEW_LIST)
	{
		if (parent->sitem == this)
		{
			glColor4ub(42,76,137,255);
			imglst->render(sx,sy,smlimgid,true);

			glBindTexture(GL_TEXTURE_2D,0);
			glLoadIdentity();
			//glTranslated(sx,sy,0);
			glBegin(GL_QUADS);
				DrawQuad(sx+17,sy,sx+strpw+17,sy+16,0,0,0,0);
			glEnd();

			glColor3ub(255,255,255);
			cskin->basefont->PrintText(sx+17,sy+cskin->basefont->fsize,caption.strrep);
		}
		else
		{
			glColor4f(1,1,1,1);
			imglst->render(sx,sy,smlimgid,true);
			glColor3ubv(cskin->deffontclr);
			cskin->basefont->PrintText(sx+17,sy+cskin->basefont->fsize,caption.strrep);
		}
	}

	if (parent->rendermode == GLISTVIEW_REPORT)
	{
		if (parent->sitem == this)
		{
			glColor4ub(42,76,137,255);
			imglst->render(sx,sy,smlimgid,true);

			glBindTexture(GL_TEXTURE_2D,0);
			glLoadIdentity();
			//glTranslated(sx,sy,0);
			glBegin(GL_QUADS);
				DrawQuad(sx+17,sy,sx+strpw+17,sy+16,0,0,0,0);
			glEnd();

			glColor3ub(255,255,255);
			cskin->basefont->PrintText(sx+17,sy+cskin->basefont->fsize,caption.strrep);
		}
		else
		{
			glColor4f(1,1,1,1);
			imglst->render(sx,sy,smlimgid,true);
			glColor3ubv(cskin->deffontclr);
			cskin->basefont->PrintText(sx+17,sy+cskin->basefont->fsize,caption.strrep);
		}
	}
}

// list item data functions

GListItemData::GListItemData()
{
	strrep = 0;
}


void GListItemData::SetValue(int data)
{
	if (strrep == 0) // allocate and erase mem
	{
		char* bosdata;
		bosdata = new char [16]; // max 10 but we allocated 16
		strrep = bosdata;
		memset(strrep,0,16);
	}
	else
	{
		if (data_type == type_int)
		{
			memset(strrep,0,16); // just erase mem
		}
		else // deallocate before, allocate new 16 byte mem and refresh it
		{
			delete [] strrep;
			strrep = 0;
			data_type == type_int;
			char* bosdata;
			bosdata = new char [16]; // max 10 but we allocated 16
			strrep = bosdata;
			memset(strrep,0,16);
		}
	}

	itemdata.intdata = data;
	itoa(data,strrep,10);
}