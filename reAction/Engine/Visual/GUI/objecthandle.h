
//////////////////////////////////////////////////////////////////////////
/// ObjectHandle Functions
//////////////////////////////////////////////////////////////////////////

ObjectHandle::ObjectHandle()
{
	visible		= true;
	pClass		= 0;
	MouseDown0	= 0;
	MouseUp0	= 0;
	MouseMove0	= 0;
	MouseOut0	= 0;
	KeyDown0	= 0;
	KeyUp0		= 0;
	GotFocus0	= 0;
	LostFocus0	= 0;
	MouseWheel0 = 0;
	KeyPress0	= 0;
	carescissor = false;
	classID		= GOBJECTHANDLE_CLASSID;
}

bool ObjectHandle::IsMouseInside()
{
	bool rval = false;

	if (*mx >= sx)
	{
		if (*my >= sy)
		{
			if (*mx <= sx + w)
			{
				if (*my <= sy + h )
				{
					rval = true;
				}
			}
		}
	}

	if (carescissor)
	{
		if ( sig_scissor ) // scissor test is not active
		{
			bool scistest = false;
			if (*mx >= sig_scissor_x)
			{
				if (*my >= sig_scissor_y)
				{
					if (*mx <= sig_scissor_x + sig_scissor_w)
					{
						if (*my <= sig_scissor_y + sig_scissor_h )
						{
							scistest = true;
						}
					}
				}
			}
			return rval & scistest;
		}
	}

	return rval;
}

void ObjectHandle::RenderDebugArea()
{
	glLoadIdentity();
	glColor4f(1,0,0,1);
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
	glEnd();
}

void ObjectHandle::Dock(int pos,ObjectContainer* cont)
{
	long i;
/*
	#define GDOCK_NODOCK		0
	#define GDOCK_TOP			1
	#define GDOCK_LEFT			2
	#define GDOCK_RIGHT			3
	#define GDOCK_BOTTOM		4
	#define GDOCK_FILL			5
*/
	pdock = pos;
	if (pos == 0) return; // why somebody do this?
	if (cont != 0)
	{
		//ObjectContainer* cont = (ObjectContainer*)pid;
		ObjectHandle* curobj;
		curobj = cont->firstobject;

		if (pos == GDOCK_TOP)
		{
			// adjust sizes of object
			x = cont->padleft;
			y = cont->padtop;
			cont->padtop += h;
			pid->h += h;
			w = pid->w - cont->padleft - cont->padright;

			// adjust the objects that added before this object
			for (long i = 0;i< cont->objectcount;i++)
			{
				if(curobj->pdock == 0 ) curobj->y += h;
				curobj = curobj->nextobject;
			}
		}

		if (pos == GDOCK_FILL)
		{
			x = cont->padleft;
			y = cont->padtop;
			w = pid->w - cont->padleft - cont->padright;
			h = pid->h - cont->padtop - cont->padbottom;
			update();
		}
	}
}

void ObjectHandle::SetRelative()
{
	if (pid != NULL)
	{
		sx = pid->sx + x;
		sy = pid->sy + y;
		style |= (pid->style & GSTYLE_UPDATING);
	}
	else
	{
		sx = x;
		sy = y;
	}
}

void ObjectHandle::LoadHandle(int in_x,int in_y,int in_w,int in_h,long in_style,GuiSkin* in_skin)
{
	x = in_x;
	y = in_y;
	w = in_w;
	h = in_h;
	style = in_style;
	pid = 0 ;
	if (in_skin != NULL)
	{
		cskin = in_skin;
	}
	else
	{
		cskin = DefaultSkin;
	}
	SetRelative();
}

void ObjectHandle::SetFocus()
{
	if (focused != this)
	{
		ObjectHandle* oldfocus;
		oldfocus = focused;
		focused = this;
		if ( oldfocus->LostFocus0 != NULL )
		{
			void * f;
			f = (void*)((long)(*(oldfocus->LostFocus0)));
			void * pcls = oldfocus->pClass;
			_asm
			{
				mov eax,pcls
				cmp eax,0
				je lf
				mov ecx,pcls
			lf: call f
			}
			dbox->AddItem("%x LostFocus",oldfocus);
		}

		if ( (*GotFocus0) != NULL)
		{
			void * f;
			f = (void*)((long)(*GotFocus0));
			void * pcls = pClass;
			_asm
			{
				mov eax,pcls
				cmp eax,0
				je gf
				mov ecx,pcls
			gf: call f
			}
			dbox->AddItem("%x GotFocus",this);
		}
		
	}
}



void ObjectHandle::DeliverEvents()
{
	void * pcls = pClass;
	switch(sig_event)
	{
		case GEVENT_MOUSEDOWN:
			if (IsMouseInside())
			{
				sig_event=0;
				SetFocus();
				IsMouseIn = true;
				if ( (*MouseDown0) != 0)
				{
					int x,y;
					char c;
					void * f;
					f = (void*)((long)(*MouseDown0));
					x = sig_x - sx;
					y = sig_y - sy;
					c = sig_char;
					_asm
					{
						xor eax,eax
						mov al,c
						push eax
						mov eax,y
						push eax
						mov eax,x
						push eax
						mov eax,pcls
						cmp eax,0
						je ld
						mov ecx,pcls
					ld: call f
						//add esp,12
					}
				}
			}

			break;

		case GEVENT_MOUSEUP:		
			if (focused == this)
			{
				if ( (*MouseUp0) != 0)
				{
					int x,y;
					char c;
					bool isi;
					void * f;
					f = (void*)((long)(*MouseUp0));
					x = sig_x - sx;
					y = sig_y - sy;
					c = sig_char;
					isi = IsMouseInside();

					_asm
					{
						xor eax,eax
						mov al,isi
						push eax
						mov al,c
						push eax							
						mov eax,y
						push eax							
						mov eax,x
						push eax
						mov eax,pcls
						cmp eax,0
						je lu
						mov ecx,pcls
					lu: call f
					}
				}
			}
			break;

		case GEVENT_MOUSEWHEEL:
			if (IsMouseInside())
			{
				sig_event = 0;
				IsMouseIn = true;
				if ( (*MouseWheel0) != 0)
				{
					int x,y,delt;
					void * f;
					f = (void*)((long)(*MouseWheel0));
					x = sig_x - sx;
					y = sig_y - sy;
					delt = sig_delta;
					_asm
					{
						mov eax,delt
						push eax
						mov eax,y
						push y
						mov eax,x
						push x
						mov eax,pcls
						cmp eax,0
						je mw
						mov ecx,pcls
					mw: call f
					}
				}
			}

		case GEVENT_KEYDOWN:
			if ( (*KeyDown0) != 0)
			{
				int c;
				void * f;
				c = sig_char;
				f = (void*)((long)(*KeyDown0));
				_asm
				{
					xor eax,eax
					mov eax,c
					push eax
					mov eax,pcls
					cmp eax,0
					je kd
					mov ecx,pcls
				kd: call f	
				}
			}
			break;

		case GEVENT_KEYUP:
			if ( (*KeyUp0) != 0)
			{
				int c;
				void * f;
				c = sig_char;
				f = (void*)((long)(*KeyUp0));
				_asm
				{
					xor eax,eax
					mov eax,c
					push eax	
					mov eax,pcls
					cmp eax,0
					je ku
					mov ecx,pcls
				ku: call f
				}
			}
			break;

		case GEVENT_KEYPRESS:
			if ( (*KeyPress0) != 0)
			{
				int c;
				void * f;
				c = sig_char;
				f = (void*)((long)(*KeyPress0));
				_asm
				{
					xor eax,eax
					mov eax,c
					push eax	
					mov eax,pcls
					cmp eax,0
					je zu
					mov ecx,pcls
				zu: call f
				}
			}
			break;

		default:
			//state = 0;
			if (IsMouseInside())
			{
				IsMouseIn = true;
				if ( (*MouseMove0) != 0)
				{
					int x,y;
					char c;
					void * f;
					f = (void*)((long)(*MouseMove0));
					x = *mx - sx;
					y = *my - sy;
					c = sig_char;
					_asm
					{
						xor eax,eax
						mov al,c
						push eax
						mov eax,y
						push eax
						mov eax,x
						push eax
						mov eax,pcls
						cmp eax,0
						je mm
						mov ecx,pcls
					mm: call f
					}
				}
			}
			else
			{
				if (IsMouseIn)
				{
					if ( (*MouseOut0) != 0)
					{
						void * f;
						f = (void*)((long)(*MouseOut0));
						_asm
						{
							mov eax,pcls
							cmp eax,0
							je mo
							mov ecx,pcls
						mo: call f
						}
					}
				}
				IsMouseIn = false;
			}
			
			break;
	}
}