
GHSlider::GHSlider()
{
	style = 0;
	minv = 0;
	maxv = 100;
	value = 0;
	s_bthm = 0;

	bthm.pClass = this;

	void (GHSlider::*A)(int,int,int)		= &GHSlider::bthm_MouseDown;
	void (GHSlider::*B)(int,int,int,bool)	= &GHSlider::bthm_MouseUp;
	void (GHSlider::*C)(int,int,int)		= &GHSlider::bthm_MouseMove;
	void (GHSlider::*D)()					= &GHSlider::bthm_MouseOut;

	AttachFunctionPointer(bthm.MouseDown0	,A);
	AttachFunctionPointer(bthm.MouseUp0		,B);
	AttachFunctionPointer(bthm.MouseMove0	,C);
	AttachFunctionPointer(bthm.MouseOut0	,D);

	bthm.pid = this;
	moving = false;

}

void GHSlider::bthm_MouseDown(int x,int y,int button)
{
	s_bthm = 1;
	bthm.SetRelative();
	dtx = *mx - bthm.sx;
	moving = true;
}

void GHSlider::bthm_MouseUp(int x,int y,int button,bool inside)
{
	moving = false;
	valuechanged();
//QuickInfo("mout");
}

void GHSlider::bthm_MouseMove(int x,int y,int button)
{
	if (focused == &bthm)
	{
		sig_wfmup ? s_bthm = 1 : s_bthm = 3;
	}
	else
	{
		if (!sig_wfmup) s_bthm = 3;
	}
}


void GHSlider::bthm_MouseOut()
{
	s_bthm = 0;
}

void GHSlider::initialize()
{
	py = (h / 2) - (cskin->trbar_h / 2);
	bthm.x = 0;
	bthm.y = (h / 2) - (cskin->h_sthm_w);
	bthm.w = cskin->h_sthm_w;
	bthm.h = cskin->h_sthm_h;
	bthm.SetRelative();
}

void GHSlider::valuechanged()
{
	if (value < minv ) value = minv;	
	if ( value > maxv ) value = maxv;

	//bthm.x = cskin->vsb_h + (int)((float)value / freq);
	bthm.x = ((value - minv) / freq);//* (float)w) / stotal; 
	bthm.SetRelative();
}

void GHSlider::refresh()
{
	stotal = abs(maxv - minv);
	freq = stotal / ((float)(w - cskin->h_sthm_w));
	//bthm.x = ((value - minv) * (float)w) / stotal; 
	valuechanged();
}

void GHSlider::update()
{
	SetRelative();
	bthm.SetRelative();
	py = (h >> 1) - (cskin->trbar_h >> 1);
}

void GHSlider::doevents()
{
	bthm.DeliverEvents();
	DeliverEvents();
	if (moving)
	{
		bthm.x = *mx - dtx - sx;
		//bthm.SetRelative();
		value = ((float)bthm.x * freq) + minv; //(int)((float)(bthm.x-cskin->vsb_h) * freq);
		if (fixedvalue)
		{
			value = (float)((int)(value + 0.5f));
		}
		valuechanged();
	}
}

void GHSlider::render()
{
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	glLoadIdentity();

	//py = (h / 2) - (cskin->trbar_h / 2);
	//cskin->htrbar.Render(this);
	//cskin->htrbar.Render(sx,sy,w,cskin->trbar_h);

	cskin->htrbar.Render(sx,sy+py,w,cskin->trbar_h);

	cskin->hsthm[style][s_bthm].Render(&bthm);
}
