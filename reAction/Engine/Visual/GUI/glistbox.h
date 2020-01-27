
//////////////////////////////////////////////////////////////////////////
///  LIST BOX
//////////////////////////////////////////////////////////////////////////

GListBox::GListBox()
{
	ListCount = 0;
	ListIndex = 0;
	ListData = 0;
//	scr.smin = 0;
	scr.value = 0;
	scr.pid = this;
	state = 0;
	onSelectionChanged = 0;
	list_fc[0] = 0.0f;
	list_fc[1] = 0.0f;
	list_fc[2] = 0.0f;
	list_fc[3] = 1.0f;
	viewmax = 0;

	pClass = this;
	void (GListBox::*A)(int,int,int,bool)			= &GListBox::MouseUp;
	void (GListBox::*B)(int,int,int)				= &GListBox::MouseWheel;
	void (GListBox::*C)()				= &GListBox::LostFocus;

	AttachFunctionPointer(MouseUp0		,A);
	AttachFunctionPointer(MouseWheel0	,B);
	AttachFunctionPointer(LostFocus0	,C);

}

void GListBox::MouseUp(int x,int y,int button,bool inside)
{
	if (inside)
	{
		int newindex;
		
		if (ListCount < viewmax)
		{
			scr.value = 0;
		}
		newindex = scr.value + (int)(ceil(y / cskin->basefont->fsize));
		if ( newindex > ListCount) 	return;
		if ( newindex < 0 ) return;
		if (newindex != ListIndex)
		{
			if ((*onSelectionChanged) != NULL)
			{

				void * f;
				void * pcls;
				f = (void*)((long)(*onSelectionChanged));
				pcls = pEventClass;
				_asm
				{
					mov eax,newindex
					push eax
					mov eax,pcls
					cmp eax,0
					je os
					mov ecx,pcls
				os: call f
				}
			}
		}
		ListIndex = newindex;

	}
}

void GListBox::LostFocus()
{
	if ((*onLostFocus) != NULL)
	{

		void * f;
		void * pcls;
		f = (void*)((long)(*onLostFocus));
		pcls = pEventClass;
		_asm
		{
			mov eax,pcls
			cmp eax,0
			je os
			mov ecx,pcls
		os: call f
		}
	}
}

void GListBox::MouseWheel(int x,int y, int delta)
{
	scr.value -= delta / WHEEL_DELTA; //120 is WHEEL_DELTA
	scr.valuechanged();
	//QuickInfo("%i",delta);
}

void GListBox::refresh()
{
	viewmax = (int)(ceil(h / cskin->basefont->fsize)+1);
	scr.w = cskin->vsb_w;
	scr.x = w - scr.w - cskin->sunkedge_quad[0].sc_h[1]+1;
	scr.y = cskin->sunkedge_quad[0].sc_h[2]-1;
	scr.h = h - cskin->sunkedge_quad[0].sc_h[3]-1;
	//scr.pid = this;
	scr.cskin = cskin;
	scr.stotal = 1;

	scr.svisible = viewmax-1;
	scr.initialize();
	scr.refresh();

//	shaftlen = h - cskin->vsb_h - cskin->vsb_h;

	SetRelative();
	scr.SetRelative();
	//scr.calcthings();
}

void GListBox::doevents()
{
	if (!visible) return;
	if (ListCount >= viewmax)
	{
		scr.doevents();
	}
	DeliverEvents();
}

void GListBox::update()
{
		SetRelative();
		//SetRelative(&scr);
		scr.update();
}

void GListBox::render()
{
	if (!visible) return;
	int stp;
	int lx,ly; // list x ,y
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	cskin->sunkedge_quad[state].Render(this);
	glColor4f(list_fc[0],list_fc[1],list_fc[2],list_fc[3]);

	stp = cskin->basefont->fsize;
	ly = sy + stp+1; // start of y axis
	lx = sx + 3;
	mEngine.EScissor(sx,sy,w,h-3);
	if (ListCount < viewmax)
	{
		scr.value = 0;
	}
	for (int i=scr.value , rw=0 ;i<scr.value+viewmax,rw<viewmax;i++,rw++)
	{
		if (i == ListCount)
		{
			break;
		}
		if (i == ListIndex)
		{
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);
			//glColor3b(87,104,127);
			glColor4f(0.341f,0.407f,0.5f,1.0f);
			glBegin(GL_QUADS);
				DrawQuad(sx+2,ly-stp+2,scr.sx+scr.w,ly+3,0,0,1,1);
			glEnd();
			glColor4f(1,1,1,1);
			cskin->basefont->PrintText(lx,ly,ListData[i]);
			glColor4f(list_fc[0],list_fc[1],list_fc[2],list_fc[3]);
		}
		else
		{
			cskin->basefont->PrintText(lx,ly,ListData[i]);
		}
		ly = ly +stp;
		
	}
	//glDisable(GL_SCISSOR_TEST);
	mEngine.DScissor();
	if (ListCount >= viewmax)
	{
		scr.render();
	}
}

void GListBox::AddItem(char *st,...)
{
	char text[256];
	va_list ap;
	int slen;

	if (st == NULL) return;
	va_start(ap,st);
		vsprintf(text,st,ap);
	va_end(ap);
	slen = strlen(text);

	char* bos;
	bos = new char [slen+1];
	strcpy(bos,text);

	Precache();
	ListData[ListCount] = bos;
	bos = 0;
	//scr.smax = ListCount - viewmax+2;
	
	ListCount++;

	scr.stotal = ListCount;
	scr.initialize();
	scr.refresh();
}

void GListBox::Precache()
{
	if ( ListCount == 0)
	{
		ListData = new char* [64];
		return;
	}
	else
	{
		int k;
		k = ListCount % 64; // bitmaskla yap bunu kasmasin aq
		if (k == 0)
		{
			char** bos;
			bos = new char * [ListCount+64];
			memcpy(bos,ListData,4 * ListCount);
			delete [] ListData;
			ListData = bos;
		}
	}
}
