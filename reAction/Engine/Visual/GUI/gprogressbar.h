
GProgressBar::GProgressBar()
{
	percent = 0.0f;
	showpercent = false;
	smooth = true;
}

void GProgressBar::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	cskin->pbar_quad.Render(this);

	if (percent > 100.f)
	{
		percent = 100.0f;
	}
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	float full;

	
	if (!cskin->presizing)
	{
		glLoadIdentity();
		glTranslated(sx+2,sy+1,0);
		full = (float)w - 4.0f;
		full *= (percent / 100.0f);
		float tu2;
		tu2 = full / (float)cskin->pchk_wid;
		if (!smooth)
		{
			tu2 = (float)((int)tu2);
			full = (float)cskin->pchk_wid * tu2;
		}
	
		glBegin(GL_QUADS);
			DrawQuad(0,0,full,h-2,0,0,tu2,1);
		glEnd();
	}
	else
	{
		full = (float)w;
		full *= (percent / 100.0f);
		cskin->pchk_quad.Render(sx,sy,full,h);
	}
	glDisable( GL_ALPHA_TEST );

	if (showpercent)
	{
		sprintf(cperc,"%.0f%%%%",percent);
		cperclen = cskin->basefont->GetStringLen("55%%");
		glColor4f(0,0,0,1);

		cskin->basefont->PrintText(sx+ ((w>>1) - (cperclen>>1)) ,sy+cskin->basefont->fsize+1,cperc);
	}
}