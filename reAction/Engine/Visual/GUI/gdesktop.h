
GDesktop::GDesktop()
{
	owner = this;
}

void GDesktop::CheckWindowActivation()
{
	//if (sig_event != GEVENT_MOUSEDOWN) return; // 100% possibly to nothing will got focus // moved to host function

	ObjectHandle* curobj;
	curobj = lastobject;
	for (long i=0;i<objectcount;i++)
	{
		if ( curobj->visible ) 
		{
			if ( curobj->IsMouseInside() )
			{
				if (((GWindow*)curobj)->inactive == 0) return; // it is alredy active

				if (lastobject->classID == GWINDOW_CLASSID)
				{
					GWindow* pwin = (GWindow*)lastobject;
					pwin->inactive = 1;
				}
				ActivateObject(curobj);
				if (curobj->classID == GWINDOW_CLASSID)
				{
					GWindow* pwin = (GWindow*)lastobject;
					pwin->inactive = 0;
				}

				return;
			}
		}
		curobj = curobj->prevobject;
	}
}

void GDesktop::DrawGui()
{
	mEngine.Enter2D();
	BeginGUI();

	if (visible)
	{
		if (sig_event == GEVENT_MOUSEDOWN) CheckWindowActivation();
		ContainerSendEvents();
		ContainerRender();
	}
	
	EndGUI();
	mEngine.Exit2D();
}

void GDesktop::CustomDrawGui()
{
	if (visible)
	{
		if (sig_event == GEVENT_MOUSEDOWN) CheckWindowActivation();
		ContainerSendEvents();
		ContainerRender();

		//focused->RenderDebugArea();
	}
}