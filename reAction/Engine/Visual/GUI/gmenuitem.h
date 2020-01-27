
GMenuItem::GMenuItem()
{
	parent = 0;
	caption = 0;
	int imgid = -1;
	seprator = false;
	state = 0;
	imglst = 0;
	
	onClick = 0;

	pClass = this;
	void (GMenuItem::*A)(int,int,int)		= &GMenuItem::MouseMove;
	void (GMenuItem::*B)()					= &GMenuItem::MouseOut;
	void (GMenuItem::*C)(int,int,int,bool)	= &GMenuItem::MouseUp;
	AttachFunctionPointer(MouseMove0,A);
	AttachFunctionPointer(MouseOut0,B);
	AttachFunctionPointer(MouseUp0,C);
}

void GMenuItem::MouseMove(int x,int y,int button)
{
	if (!seprator)
	{
		state = 3;
	}
}

void GMenuItem::MouseOut()
{
	if (!seprator)
	{
		state = 0;
	}
}

void GMenuItem::MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		if (onClick != 0)
		{
			(*onClick)();
		}
	}
}

void GMenuItem::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	if (!seprator)
	{
		cskin->mni_quad[state].Render(this);
		if (imglst!=0)
		{
			imglst->render(sx+3,sy+2,imgid,true);
		}

		if (state!=3)
		{
			glColor3ubv(cskin->deffontclr);
		}
		cskin->basefont->PrintText(sx+24,sy+cskin->basefont->fsize+2,caption);
	}
	else
	{
		//cskin->mni_quad[1].Render(this);
		cskin->mis_quad.Render(sx,sy,w,h,true);
	}
}