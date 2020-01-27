
GColumnHeader::GColumnHeader()
{
	caption = 0;
	parent = 0;
	imglst = 0;
	state = 0;

	cx = cy = 0;

	classID = GCOLUMNHEADER_CLASSID;

	moving = false;

	pClass = this;

	void (GColumnHeader::*A)(int,int,int)			= &GColumnHeader::MouseDown;
	void (GColumnHeader::*B)(int,int,int,bool)		= &GColumnHeader::MouseUp;
	void (GColumnHeader::*C)(int,int,int)			= &GColumnHeader::MouseMove;
	void (GColumnHeader::*D)()						= &GColumnHeader::MouseOut;

	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseUp0,B);
	AttachFunctionPointer(MouseMove0,C);
	AttachFunctionPointer(MouseOut0,D);
}

void GColumnHeader::CalculateTextAlignment()
{
	cy = (h/2) + (cskin->basefont->fsize / 2) - 1;
	if (textalignment == GTEXTALIGN_LEFT)
	{
		if (imglst != 0)
		{
			cx = 18; // 2 + 16
		}
		else
		{
			cx = 2;
		}
		return;
	}
	if (textalignment == GTEXTALIGN_CENTER)
	{
		cx = (w/2) - (strpxlen/2);
		return;
	}
	if (textalignment == GTEXTALIGN_RIGHT)
	{
		cx = w - strpxlen - 2;
	}
}

void GColumnHeader::SetCaption(char* capi)
{
	caption = capi;
	strln = strlen(capi);
	strpxlen = cskin->basefont->GetStringLen(capi);
	CalculateTextAlignment();
}

void GColumnHeader::MouseDown(int x,int y,int button)
{
	state = 1;
	if (x > w - 5)
	{
		movingfromleft = false;
		stx = *mx;
		//sty = *my;
		//updating = true;
		//panel1.SetUpdating(true);
		//panel2.SetUpdating(true);
		SetRelative();
		dtx = stx - w;
		dtw = stx - parent->columnwidth;
		//dty = sty - y;
		moving = true;
	}
	if (x < 5)
	{
		if (prv != 0)
		{			
			movingfromleft = true;
			stx = *mx;
			//sty = *my;
			//updating = true;
			//panel1.SetUpdating(true);
			//panel2.SetUpdating(true);
			SetRelative();
			dtx = stx - prv->w;
			dtw = stx - parent->columnwidth;
			//dty = sty - y;
			moving = true;
		}
	}

}

void GColumnHeader::MouseUp(int x,int y,int button,bool inside)
{
	if (moving)
	{
		moving = false;
		parent->UpdateColumnWidth();

		if (parent->scr.vscr.stotal < parent->scr.vscr.svisible)
		{
			parent->scr.hscr.svisible = parent->w-5;
		}
		else
		{
			parent->scr.hscr.svisible =  parent->w - cskin->vsb_w-5;
		}
		parent->scr.hscr.stotal = parent->columnwidth;
		if ( parent->scr.hscr.svisible > parent->scr.hscr.stotal)
		{
			parent->scr.hscr.value = 0;
		}
		else
		{
			parent->scr.hscr.fixedrefresh();
		}
		if (!movingfromleft)
		{
			CalculateTextAlignment();
		}
		else
		{
			prv->CalculateTextAlignment();
		}
		
		//parent->scr.hscr.valuechanged();
	}
	if ( inside )
	{
		/*if ( onClick != 0)
		{
			(*onClick)();
		}*/
	}
}

void GColumnHeader::MouseMove(int x,int y,int button)
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

void GColumnHeader::MouseOut()
{
	state = 0;
}

void GColumnHeader::doevents()
{
	DeliverEvents();
	if (moving)
	{
		if (!movingfromleft)
		{
			w = *mx - dtx;
			if ( w < 6) w = 6;
			CalculateTextAlignment();
		}
		else
		{
			prv->w = *mx - dtx;
			if ( prv->w < 6) prv->w = 6;
			prv->CalculateTextAlignment();
		}
		
		parent->columnwidth = *mx - dtw;
		if (parent->scr.vscr.stotal < parent->scr.vscr.svisible)
		{
			parent->scr.hscr.svisible = parent->w-5;
		}
		else
		{
			parent->scr.hscr.svisible =  parent->w - cskin->vsb_w-5;
		}
		
		parent->scr.hscr.stotal = parent->columnwidth;
		if ( parent->scr.hscr.svisible > parent->scr.hscr.stotal)
		{
			parent->scr.hscr.value = 0;
		}
		/*else
		{
			//parent->scr.hscr.refresh();
		}*/
		
		//parent->scr.hscr.valuechanged();

		if ( parent->columnwidth < 0) parent->columnwidth = 0; // hmm
	}
}

/*void GColumnHeader::update()
{

}*/

void GColumnHeader::render()
{
	if (!visible) return;

	/*if (parent->gridcollines)
	{
		glLoadIdentity();
		glColor3ub(218,217,223);
		glBegin(GL_LINES);
			glVertex2i(sx+w,sy);
			glVertex2i(sx+w,sy+parent->h);
		glEnd();
	}*/

	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->lv_colheader[state].Render(this);

	if (imglst != 0)
	{
		imglst->render(sx+1,sy+3,imgid);
	}
	glColor3ubv(cskin->deffontclr);
	cskin->basefont->PrintText(sx+cx,sy+cy,caption);
}