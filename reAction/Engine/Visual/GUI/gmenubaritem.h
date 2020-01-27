

GMenuBarItem::GMenuBarItem()
{
	parent = 0;
	caption = 0;
	int imgid = -1;
	state = 0;
	submenu = 0;
	
	onClick = 0;

	pClass = this;
	classID = GMENUBARITEM_CLASSID;
	
	void (GMenuBarItem::*A)(int,int,int)		= &GMenuBarItem::MouseDown;
	void (GMenuBarItem::*B)(int,int,int,bool)	= &GMenuBarItem::MouseUp;
	void (GMenuBarItem::*C)(int,int,int)		= &GMenuBarItem::MouseMove;
	void (GMenuBarItem::*D)()					= &GMenuBarItem::MouseOut;

	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseUp0,B);
	AttachFunctionPointer(MouseMove0,C);
	AttachFunctionPointer(MouseOut0,D);
}

void GMenuBarItem::MouseDown(int x,int y,int button)
{
	parent->state = true;
	parent->SetFocus();
	if (submenu != 0)
	{
		parent->currentmenu->visible = false;
		submenu->visible = true;
		parent->currentmenu = submenu;
	}
}

void GMenuBarItem::MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		if (onClick != 0)
		{
			(*onClick)();
		}
		else
		{
			parent->state = false;
		}
	}	
}

void GMenuBarItem::MouseMove(int x,int y,int button)
{
	if (parent->state)
	{
		if (submenu != 0)
		{
			if (parent->currentmenu != submenu)
			{
				parent->currentmenu->visible = false;
				submenu->visible = true;
				parent->currentmenu = submenu;
			}
		}
	}
}

void GMenuBarItem::MouseOut()
{
	
}


void GMenuBarItem::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	if (submenu)
	{
		if (submenu->visible)
		{
			cskin->mbi_quad[1].Render(this);
		}
		else
		{
			cskin->mbi_quad[0].Render(this);
		}
	}

	glColor3ubv(cskin->deffontclr);
	cskin->basefont->PrintText(sx+4,sy+cskin->basefont->fsize+2,caption);

	if (submenu != 0)
	{
		submenu->render();
	}
}