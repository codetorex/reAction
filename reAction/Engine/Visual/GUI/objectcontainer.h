
//////////////////////////////////////////////////////////////////////////
/// ObjectContainer Functions
//////////////////////////////////////////////////////////////////////////

ObjectContainer::ObjectContainer()
{
	firstobject = 0;
	lastobject = 0;
	objectcount = 0;
	owner = 0;
	padtop = 0;
	padleft = 0;
	padbottom = 0;
	padright = 0;
	defcarescissor = false;

	//object handle data changes
	container=true;
}

void ObjectContainer::AddObj(ObjectHandle* bs)
{
	if (firstobject == 0)
	{
		firstobject = bs;
		lastobject = bs;
		bs->nextobject = 0;
		bs->prevobject = 0;
		objectcount++;
	}
	else
	{
		lastobject->nextobject = bs;
		bs->prevobject = lastobject;
		bs->nextobject = 0;
		lastobject = bs;
		objectcount++;
	}
	
	bs->x += padleft;
	bs->y += padtop;

	bs->pid = owner;
	bs->carescissor = defcarescissor;
	bs->update();
	//bs->SetRelative();
}

void ObjectContainer::DelObj(ObjectHandle* bs)
{
	// todo: rewrite this
	ObjectHandle* curobj;
	curobj = firstobject;
	for (long i = 0;i<objectcount;i++)
	{
		if (curobj == bs)
		{
			bs->prevobject->nextobject = bs->nextobject;
			bs->nextobject->prevobject = bs->prevobject;
			break;
		}
		curobj = curobj->nextobject;
	}
}

void ObjectContainer::ContainerRender()
{
	ObjectHandle* curobj;
	curobj = firstobject;
	for (long i = 0;i<objectcount;i++)
	{
		if (curobj->visible) curobj->render();
		curobj = curobj->nextobject;
	}
}

/*void ObjectContainer::SendEvents() // old version, sends events first to last
{
	ObjectHandle* curobj;
	curobj = firstobject;
	for (long i = 0;i<objectcount;i++)
	{
		curobj->doevents();
		curobj = curobj->nextobject;
	}
}*/

void ObjectContainer::ContainerSendEvents() // ver 2.0, sends event from last to first
{
	ObjectHandle* curobj;
	curobj = lastobject;
	for (long i = 0;i<objectcount;i++)
	{
		if (curobj->visible) curobj->doevents();
		curobj = curobj->prevobject;
	}
}

void ObjectContainer::ContainerUpdate()
{
	ObjectHandle* curobj;
	curobj = firstobject;
	for (long i = 0;i<objectcount;i++)
	{
		curobj->update();
		curobj = curobj->nextobject;
	}
}

void ObjectContainer::DrawHitboxes()
{
	ObjectHandle* curobj;
	curobj = firstobject;
	glBindTexture(GL_TEXTURE_2D,0);
	glColor4f(0.0f,0.7f,0.0f,1.0f);
	for (long i = 0;i<objectcount;i++)
	{
		//curobj->render();
		glBegin(GL_LINE_STRIP);
			glVertex2i(curobj->sx,curobj->sy);

			glVertex2i(curobj->sx + curobj->w,curobj->sy); 
				
			glVertex2i(curobj->sx+ curobj->w ,curobj->sy+curobj->h);

			glVertex2i(curobj->sx,curobj->sy+curobj->h);

			glVertex2i(curobj->sx,curobj->sy);
		glEnd();	
		
		curobj = curobj->nextobject;
	}
}

void ObjectContainer::ActivateObject(ObjectHandle* bs)
{
	if (lastobject == bs) return; // zaten aktif veya zaten tek obje ise

	if (firstobject == bs)
	{
		ObjectHandle *obj1,*obj2,*obj4;
		obj1 = firstobject;
		obj2 = obj1->nextobject;
		obj4 = lastobject;
		
		// ilk objeyi last obje yapiyoruz
		obj4->nextobject = obj1;
		obj1->nextobject = 0;
		obj1->prevobject = obj4;
		lastobject = obj1;

		// firstobjenin nextini first obje yapiyoruz
		obj2->prevobject = 0;
		firstobject = obj2;
	}
	else
	{
		ObjectHandle *obj1,*obj2;
		// bs nin previni nexte, nextini previne bagliyoruz
		obj1 = bs->prevobject;
		obj2 = bs->nextobject;
		obj1->nextobject = obj2;
		obj2->prevobject = obj1;

		// bs yi son obje yapiyoruz
		lastobject->nextobject = bs;
		bs->prevobject = lastobject;
		bs->nextobject = 0;
		lastobject = bs;
	}
}

void ObjectContainer::SetUpdating(bool val)
{
	ObjectHandle* curobj;
	curobj = firstobject;
	for (long i = 0;i<objectcount;i++)
	{
		curobj->updating = val;
		curobj = curobj->nextobject;
	}
}