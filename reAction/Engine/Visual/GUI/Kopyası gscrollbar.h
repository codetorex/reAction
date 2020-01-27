
//////////////////////////////////////////////////////////////////////////
///  VERTICAL SCROLL BAR
//////////////////////////////////////////////////////////////////////////

GScrollBar::GScrollBar()
{
	ry = 0;
	smin = 0;
	smax = 100;
	value = 0;
	smallchange = 1;
	largechange = 3;
	align = true;
	s_ubut = 8;
	s_dbut = 12;
	s_shft = 0;
	s_tbut = 0;
	bthm.state = 0;
	frq = 75;
	frq2 = 1000;
	lchk = 0;
	lchk2 = 0;
}

void GScrollBar::calcthings()
{
	bup.x = 0;
	bup.y = 0;
	bup.w = cskin->vsb_w;
	bup.h = cskin->vsb_h;

	bdwn.x = 0;
	bdwn.y = h-cskin->vsb_h;
	bdwn.w = cskin->vsb_w;
	bdwn.h = cskin->vsb_h;

	bthm.x = 0;
	bthm.y = cskin->vsb_h;
	bthm.w = cskin->vsb_w;
	bthm.h = h - cskin->vsb_h - cskin->vsb_h;

	shaftlen = bthm.h;
	
	bup.pid = this;
	bdwn.pid = this;
	bthm.pid = this;

	SetRelative(&bup);
	SetRelative(&bdwn);
	SetRelative(&bthm);
}

/*void GScrollBar::updatesize()
{
	float length = h - cskin->vsb_h - cskin->vsb_h;
	//int len = 
	float visfrac = 
}*/

void GScrollBar::doevents()
{
	/*long k = GetTickCount();

	if (bup.state == 2 )
	{
		value-= smallchange;
		if (value < smin) value = smin;
	}
	else
	{
		bup.doevents();
	}

	if (bdwn.state == 2)
	{
		value+= smallchange;
		if (value > smax) value = smax;
	}
	else
	{
		bdwn.doevents();
	}

	if (bthm.state == 1)
	{
		moving = true;
		stx = sig_x;
		sty = sig_y;
		dtx = sig_x - bthm.sx;
		dty = sig_y - bthm.sy;
		//if ( bthm.y > bup.y ) bthm.y = bup.y;
		//if ( bthm.y < (bdwn.y-bthm.h)) bthm.y = bdwn.y-bthm.h;
		bthm.state = 0;
		focused = &bthm;
	}


	if (bthm.state == 2)
	{
		moving = false;
	}
	else
	{
		bthm.doevents();
	}

	if (moving)
	{
		//bthm.sy = *my;
		//bthm.x = *mx - stx;

		bthm.y = *my - sty + bup.h;

		float lq;
		lq = bthm.y - cskin->vsb_h;

		if ( bthm.y < bup.y+bup.h ) bthm.y = bup.y+bup.h;
		if ( bthm.y > (bdwn.y-bthm.h)) bthm.y = bdwn.y-bthm.h;
		value = ((float)smax * lq) / ((float)shaftlen-bthm.h);
		if (value > smax) value = smax;
		if (value < smin) value = smin;
		SetRelative(&bthm);
	}
	else
	{

	}
	
		float lq = ((float)shaftlen-bthm.h) * (float)value / (float)smax;
		bthm.y = lq + cskin->vsb_h;
		SetRelative(&bthm);*/


/*	float length = h - cskin->vsb_h- cskin->vsb_h;
	float slidelen = length - bthm.h;
	float vfrac = (float)value / (float)smax;

	bthm.y = bthm.h * (slidelen * vfrac);*/
	


//	bup.doevents();
/*	bdwn.doevents();
	bthm.doevents();*/

	switch(sig_event)
	{
		case GEVENT_MOUSEDOWN:
			/*if (IsInside(this))
			{
				sig_event=0;
				focused = this;
				//value == 0 ? 1 : 0;	
				// todo: raise event mouse down
			}*/
			/*if (IsInside(sx,sy,cskin->vsb_w+sx,cskin->vsb_h+sy))
			{
				// up button down

			}*/

			break;

		case GEVENT_MOUSEUP:
			//state = 0;	
			if (IsInside(this))
			{
				//focused = this;
				// todo: raise event mouse up
				// todo: raise event changed
//				value == 0 ? value = 1 : value = 0;
				
			}
			break;

		case GEVENT_KEYDOWN:
			
			break;

		case GEVENT_KEYUP:
			
			break;

		default:
//			value ? state = 4 : state = 0;
			if (focused == this)
			{
				if (IsInside(this))
				{
/*					if (value == 0)
					{
						//sig_wfmup ? state = 2: state = 0;
					}
					if (value == 1)
					{
						//sig_wfmup ? state = 6: state = 4;
					}*/
				}
			}
			break;
	}
	if (pid->style & GSTYLE_UPDATING)
	{
		SetRelative(this);
	}
}

void GScrollBar::render()
{
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->button_tex);

	
	// the rendering code
	glLoadIdentity();
	glTranslated(sx,sy,0);

	//render up button
	glBindTexture(GL_TEXTURE_2D,cskin->vsb_tex);
	glCallList(cskin->vsb_base+s_ubut+bup.state);


	glBindTexture(GL_TEXTURE_2D,cskin->vss_tex);
	ry = h - cskin->vsb_h;
	
	glBegin(GL_QUADS);
		switch(s_shft)
		{
			case 0:
				DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.0f,0.0f,0.33f,1.0f);
			case 1:
				DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.33f,0.0f,0.66f,1.0f);
			case 2:
				DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.66f,0.0f,1.0f,1.0f);
		}
		//DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.0f,0.0f,0.33f,1.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,cskin->vst_tex);
	ry = h - cskin->vsb_h;

	s_tbut = bthm.state;
	SetRelative(&bthm);
	glBegin(GL_QUADS);
		switch(s_tbut)
		{
			case 0:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.0f,0.0f,0.33f,1.0f);
			case 1:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.33f,0.0f,0.66f,1.0f);
			case 3:
				DrawQuad(bthm.x,bthm.y,bthm.x+bthm.w,bthm.y+bthm.h,0.66f,0.0f,1.0f,1.0f);
		}
		//DrawQuad(0,cskin->vsb_h,cskin->vsb_w,ry,0.0f,0.0f,0.33f,1.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,cskin->vsb_tex);
	glTranslated(0,ry,0);
	glCallList(cskin->vsb_base+s_dbut+bdwn.state);
	
	
	//glTranslated(0,ry,0);

	/*glBegin(GL_QUADS);
		DrawQuad(0,0,cskin->cbox_w,cskin->cbox_h,0.083f*state,0,0.083f*(state+1),1.0f);
	glEnd();*/
	/*glPushAttrib(GL_LIST_BIT);
	glListBase(cskin->cbox_base);
	glCallList(state);
	glPopAttrib();*/
//	glCallList(cskin->cbox_base+state);
	// draw label
	/*glColor4f(1.0f,1.0f,1.0f,1.0f);
	cskin->basefont->PrintText(100,100,"%i",bthm.state);*/
}