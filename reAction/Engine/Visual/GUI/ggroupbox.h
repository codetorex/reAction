
GGroupBox::GGroupBox()
{
	owner = this;
	caption = 0;
	classID == GGROUPBOX_CLASSID;
}

void GGroupBox::update()
{
	SetRelative();
	ContainerUpdate();
}

void GGroupBox::doevents()
{
	if (!visible) return;
	ContainerSendEvents();
}

void GGroupBox::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	if (caption== 0)
	{
		cskin->gbx_quad[1].Render(this);
	}
	else
	{
		cskin->gbx_quad[0].Render(this);

		glColor3ubv(cskin->deffontclr);
		cskin->basefont->PrintText(sx+16,sy+cskin->basefont->fsize,caption);
	}


	ContainerRender();
}