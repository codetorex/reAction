
GSplitContainer::GSplitContainer()
{
	classID = GSPLITCONTAINER_CLASSID;
	splitterwidth = 4; // 4 pixels of width
	orientation = 0;
	
	panel1.LoadHandle(0,0,0,0,NULL);
	panel1.pid = this;
	panel1.defcarescissor = true;

	panel2.LoadHandle(0,0,0,0,NULL);
	panel2.pid = this;
	panel2.defcarescissor = true;

	border = false;

	moving = false;

	pClass = this;
	void (GSplitContainer::*A)(int,int,int)	= &GSplitContainer::MouseDown;
	void (GSplitContainer::*B)(int,int,int,bool) = &GSplitContainer::MouseUp;
	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseUp0,B);
}

void GSplitContainer::UpdatePanels()
{
	if (orientation == 0) // vertical
	{
		panel1.x = 0;
		panel1.y = 0;
		panel1.w = splitterdistance;
		panel1.h = h;
		panel1.update();
		//panel1.SetRelative();

		panel2.x = splitterdistance + splitterwidth;
		panel2.y = 0;
		panel2.w = w - panel2.x;
		panel2.h = h;
		//panel2.SetRelative();
		//panel1.LoadHandle(0,0,splitterdistance,h,NULL);
		panel2.update();
	}
	else
	{
		panel1.x = 0;
		panel1.y = 0;
		panel1.w = w;
		panel1.h = splitterdistance;
		panel1.update();

		panel2.x = 0;
		panel2.y = splitterdistance + splitterwidth;
		panel2.w = w;
		panel2.h = h - panel2.y;
		panel2.update();
	}
}

void GSplitContainer::MouseDown(int x,int y,int button)
{
	if (!fixed)
	{
		if ( orientation == 0)
		{
			if (x < panel1.w || x > panel2.x)
			{
				return;
			}
		}
		else
		{
			if (y < panel1.h || y > panel2.y)
			{
				return;
			}

		}
	

		stx = *mx;
		sty = *my;
		updating = true;
		panel1.SetUpdating(true);
		panel2.SetUpdating(true);
		SetRelative();
		dtx = stx - splitterdistance;
		dty = sty - splitterdistance;
		moving = true;
	}

}


void GSplitContainer::MouseUp(int x,int y,int button,bool inside)
{
	if (moving)
	{
		moving = false;
		panel1.SetUpdating(false);
		panel2.SetUpdating(false);
		updating = false;
	}
}

void GSplitContainer::SetupPanels(int orient,int splitdist)
{
	orientation = orient;
	splitterdistance = splitdist;
	UpdatePanels();
}

void GSplitContainer::update()
{
	SetRelative();
	panel1.update();
	panel2.update();
}

void GSplitContainer::doevents()
{
	if (!visible) return;

	EnableEventScissor(panel1.sx,panel1.sy,panel1.w,panel1.h);
		panel1.ContainerSendEvents();
	
	EnableEventScissor(panel2.sx,panel2.sy,panel2.w,panel2.h);
		panel2.ContainerSendEvents();

	sig_scissor = false;

	DeliverEvents();
	if (moving)
	{
		if (orientation==0) // vertical
		{
			splitterdistance = *mx - dtx;
			if (splitterdistance > w - splitterwidth - 2)
			{
				splitterdistance =  w - splitterwidth - 2;
			}
			if (splitterdistance < splitterwidth)
			{
				splitterdistance = splitterwidth;
			}
		}
		else
		{
			splitterdistance = *my - dty;
			if (splitterdistance > h - splitterwidth - 2)
			{
				splitterdistance = h - splitterwidth - 2;
			}
			if (splitterdistance < splitterwidth)
			{
				splitterdistance = splitterwidth;
			}
		}
		UpdatePanels();
		panel1.update();
		panel2.update();
	}
}

void GSplitContainer::render()
{
	if (!visible) return;
	if (moving)
	{
		float xp;
		glGetFloatv(GL_LINE_WIDTH,&xp);
		
		/*glEnable(GL_COLOR_LOGIC_OP); 
		glLogicOp(GL_XOR);*/

		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D,0);
		glLineWidth(splitterwidth);
		/*glLineStipple(1,0xAAAA);
		glEnable(GL_LINE_STIPPLE);*/
		glColor3ub(128,128,128);
		glBegin(GL_LINES);
		if (orientation == 0)
		{
			glVertex2i(sx+splitterdistance+2,sy);
			glVertex2i(sx+splitterdistance+2,sy+h);
		}
		else
		{
			glVertex2i(sx,sy+splitterdistance+2);
			glVertex2i(sx+w,sy+splitterdistance+2);
		}


		glEnd();

		glLineWidth(xp);
		//glDisable(GL_LINE_STIPPLE);
		//glDisable(GL_COLOR_LOGIC_OP); 
	}

	if (border)
	{
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,cskin->skinID);

		cskin->gbx_quad[1].Render(&panel1);
		cskin->gbx_quad[1].Render(&panel2);
	}

	mEngine.EScissor(panel1.sx,panel1.sy,panel1.w,panel1.h); // makasla babos
	panel1.ContainerRender();

	mEngine.EScissor(panel2.sx,panel2.sy,panel2.w,panel2.h); // makasla babos
	panel2.ContainerRender();

	mEngine.DScissor();
//	glDisable(GL_SCISSOR_TEST);
	
	//panel1.RenderDebugArea();
	//panel2.RenderDebugArea();
}