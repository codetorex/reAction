
GScrollbarArea::GScrollbarArea()
{
	vscr.pid = this;
	hscr.pid = this;
	vscr.visible = false;
	hscr.visible = false;

	state = 0;

	vscr_enabled = false;
	hscr_enabled = false;

	onStateChanged = 0;
}

void GScrollbarArea::RefreshScrollbars()
{
	
}

void GScrollbarArea::update()
{
	SetRelative();
	vscr.update();
	hscr.update();
	//SwitchState(state);
}

void GScrollbarArea::DisableBoth()
{
	vscr.visible = false;
	vscr.value = 0;

	hscr.visible = false;
	hscr.value = 0;
}

void GScrollbarArea::PrepareOnlyVertical()
{
	vscr.w = cskin->vsb_w;
	vscr.x = w - vscr.w;
	vscr.y = 0;
	vscr.h = h;

	vscr.initialize();
	vscr.refresh();	

	vscr.visible = true;
	hscr.visible = false;
	hscr.value = 0;

	SetRelative();
	vscr.update();
}

void GScrollbarArea::PrepareOnlyHorizontal()
{
	hscr.w = w;
	hscr.h = cskin->vsb_h;
	hscr.x = 0;
	hscr.y = h - hscr.h;

	hscr.cskin = cskin;

	hscr.initialize();
	hscr.refresh();

	hscr.visible = true;
	vscr.visible = false;
	vscr.value = 0;

	SetRelative();
	hscr.update();
}

void GScrollbarArea::PrepareBoth()
{
	vscr.w = cskin->vsb_w;
	vscr.x = w - vscr.w;
	vscr.y = 0;
	vscr.h = h - cskin->vsb_h;

	vscr.initialize();
	vscr.refresh();
	
	hscr.w = w - cskin->vsb_w;
	hscr.h = cskin->vsb_h;
	hscr.x = 0;
	hscr.y = h - hscr.h;

	hscr.cskin = cskin;

	hscr.initialize();
	hscr.refresh();

	vscr.visible = true;
	hscr.visible = true;
	
	cornerx = hscr.x + hscr.w;
	cornery = vscr.y + vscr.h;

	SetRelative();
	vscr.update();
	hscr.update();
}

void GScrollbarArea::SwitchState(int newst)
{
	//if ( newst == state ) return;
	state = newst;
	switch (newst)
	{
		case 0:
			DisableBoth();
			break;
			
		case 1:
			PrepareOnlyVertical();
			break;

		case 2:
			PrepareOnlyHorizontal();
			break;

		case 3:
			PrepareBoth();
			break;
	}

	// event callback
	if ( (*onStateChanged) != NULL)
	{
		void * f;
		void * pcls;
		f = (void*)((long)(*onStateChanged));
		pcls = pEventClass;
		_asm
		{
			mov eax,newst
			push eax
			mov eax,pcls
			cmp eax,0
			je os
			mov ecx,pcls
		os: call f
		}		
	}
}

void GScrollbarArea::doevents() // ve dont have anything to deliver events?
{
	a = ((vscr.stotal > vscr.svisible) && vscr_enabled);
	b = ((hscr.stotal > hscr.svisible) && hscr_enabled);

	if (a && b)
	{
		if ( state != 3 )
		{
			SwitchState(3);
		}
		vscr.doevents();
		hscr.doevents();

	}
	else
	{
		if (a)
		{
			if ( state != 1 )
			{
				SwitchState(1);
			}
			vscr.doevents();
		}

		if (b)
		{
			if ( state != 2 )
			{
				SwitchState(2);
			}
			hscr.doevents();
		}
		
		if ( !a && !b )
		{
			if (state != 0)
			{
				SwitchState(0);
			}
		}
	}

}

void GScrollbarArea::render()
{
	a = (vscr.stotal > vscr.svisible) && vscr_enabled;
	b = (hscr.stotal > hscr.svisible) && hscr_enabled;

	// todo: state ilen de calisabilir
	if (a && b) // 2 side lazým
	{
		vscr.render();
		hscr.render();

		// draw a corner image
		glBindTexture(GL_TEXTURE_2D,cskin->skinID);
		glLoadIdentity();
		glTranslated(sx+cornerx,sy+cornery,0);
		glCallList(cskin->vsb_base+22);

	}
	else
	{
		if (a) // if it was not in position like our desire
		{
			vscr.render();
		}

		if (b) // if it was not in position like our desire
		{
			hscr.render();
		}
	}
}