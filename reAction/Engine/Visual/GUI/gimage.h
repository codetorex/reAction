

GImage::GImage()
{
	pid = 0; // windows doesn't have parent
	x = 0;
	y = 0;
	w = 100;
	h = 100;
	customdraw = 0;
	tuv[0] = 0.0f;
	tuv[1] = 0.0f;
	tuv[2] = 1.0f;
	tuv[3] = 1.0f;
	
	onMouseDown = 0;
	pClass = this;
	classID = GIMAGE_CLASSID;
	void (GImage::*A)(int,int,int)			= &GImage::MouseDown;
	AttachFunctionPointer(MouseDown0,A);

}

void GImage::MouseDown(int x,int y,int button)
{
	if (onMouseDown != 0)
	{
		onMouseDown(x,y,button);
	}
}


void GImage::render()
{
	if (!visible) return;
	//glLoadIdentity();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,texid);

	glLoadIdentity();
	glTranslated(sx,sy,0);

	glDisable(GL_BLEND);

	glBegin(GL_QUADS);
		DrawQuad(0,0,w,h,tuv[0],tuv[1],tuv[2],tuv[3]);
	glEnd();

	//cskin->button_quad[state].Render(this);
	//glColor4f(caption_fc[0],caption_fc[1],caption_fc[2],caption_fc[3]);
	//cskin->basefont->PrintText(cx,cy,caption);
}

void GImage::PrepareImage(VTexture* vtex)
{
	if (vtex->bytes != 0 && vtex->glID == 0)
	{
		vtex->gettexid();
	}
	texid = vtex->glID;
	mtex = vtex;
	w = vtex->twidth;
	h = vtex->theight;

}

void GImage::PrepareImage(VTexture* vtex,int msx,int msy,int mw,int mh)
{
	if (vtex->bytes != 0 && vtex->glID == 0)
	{
		vtex->gettexid();
	}
	texid = vtex->glID;
	mtex = vtex;
	tuv[0] = (float)msx / (float)vtex->twidth;
	tuv[1] = (float)msy / (float)vtex->theight;
	tuv[2] = (float)(msx + mw) / (float)vtex->twidth;
	tuv[3] = (float)(msy + mh) / (float)vtex->theight;
}