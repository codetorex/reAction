
//////////////////////////////////////////////////////////////////////////
/// SMALL TOLLS
//////////////////////////////////////////////////////////////////////////

/*bool IsInside(ObjectHandle* obj)
{
	if (obj->carescissor)
	{

	}
	else
	{
		if (*mx >= obj->sx)
		{
			if (*my >= obj->sy)
			{
				if (*mx <= obj->sx + obj->w)
				{
					if (*my <= obj->sy + obj->h )
					{
						return true;
					}
				}
			}
		}
		return false;
	}
}*/

bool IsInside(int x1,int y1,int x2,int y2)
{
	if (*mx >= x1)
	{
		if (*my >= y1)
		{
			if (*mx <= x2)
			{
				if (*my <= y2 )
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*void SetRelative(ObjectHandle* obj)
{
	if (obj->pid != NULL)
	{
		obj->sx = obj->pid->sx + obj->x;
		obj->sy = obj->pid->sy + obj->y;
		obj->style |= (obj->pid->style & GSTYLE_UPDATING);
	}
	else
	{
		obj->sx = obj->x;
		obj->sy = obj->y;
	}
	

	ObjectHandle* next_obj;
	next_obj = obj;*/
	/*while (next_obj->pid != 0)
	{
		next_obj = next_obj->pid;
		obj->sx += next_obj->x;
		obj->sy += next_obj->y;
	}
}*/

void GUIOptimize(VEngine *e)
{
	mx = &mEngine.MouseX;
	my = &mEngine.MouseY;
}

void BeginGUI()
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_BLEND);		
	glPushMatrix();
}

void EndGUI()
{
	glPopAttrib();
	glPopMatrix();
	sig_event = 0;
}

/*void LoadHandle( ObjectHandle *hnd,int x,int y,int w,int h,bool vis, ObjectHandle *parent)
{
	hnd->x = x;
	hnd->y = y;
	hnd->w = w;
	hnd->h = h;
//	hnd->visible = vis;
	hnd->pid = parent;
//	hnd->upd = false;
	SetRelative(hnd);
}*/

void DrawQuad(int x1,int y1,int x2,int y2,float s1,float t1,float s2,float t2)
{
	glTexCoord2f(s1,t1);
	glVertex2i(x1,y1);

	glTexCoord2f(s2,t1);
	glVertex2i(x2,y1); 
	
	glTexCoord2f(s2,t2);	
	glVertex2i(x2,y2);

	glTexCoord2f(s1,t2);
	glVertex2i(x1,y2);
}

float Ratio(float sval, float smax,float val )
{
	return (val * smax)/sval;
}
