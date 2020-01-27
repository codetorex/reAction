
//////////////////////////////////////////////////////////////////////////
/// SCALABLE QUAD
//////////////////////////////////////////////////////////////////////////

void GuiScalableQuad::GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2)
{

	sc_uv[0] = Ratio((float)tx->twidth,1.0f,(float)x1);
	sc_uv[1] = Ratio((float)tx->twidth,1.0f,(float)x1 + sc_h[0]);
	sc_uv[2] = Ratio((float)tx->twidth,1.0f,(float)x2 - sc_h[1]);
	sc_uv[3] = Ratio((float)tx->twidth,1.0f,(float)x2);
	
	sc_uv[4] = Ratio((float)tx->theight,1.0f,(float)y1 + sc_h[2]);
	sc_uv[5] = Ratio((float)tx->theight,1.0f,(float)y2 - sc_h[3]);
	sc_uv[6] = Ratio((float)tx->theight,1.0f,(float)y2);
	sc_uv[7] = Ratio((float)tx->theight,1.0f,(float)y1);
}

void GuiScalableQuad::Render(ObjectHandle *dp)
{
	glLoadIdentity();
	glTranslated(dp->sx,dp->sy,0);

	int q1,q2;
	q1 = dp->w - sc_h[1];
	q2 = dp->h - sc_h[3];

	glBegin(GL_QUADS);
		DrawQuad(0,0,sc_h[0],sc_h[2],sc_uv[0],sc_uv[7],sc_uv[1],sc_uv[4]); // sol ust
		DrawQuad(sc_h[0],0,q1,sc_h[2],sc_uv[1],sc_uv[7],sc_uv[2],sc_uv[4]); // ust orta
		DrawQuad(q1,0,dp->w,sc_h[2],sc_uv[2],sc_uv[7],sc_uv[3],sc_uv[4]); // sag ust

		DrawQuad(0,sc_h[2],sc_h[0],q2,sc_uv[0],sc_uv[4],sc_uv[1],sc_uv[5]); // sol
		DrawQuad(sc_h[0],sc_h[2],q1,q2,sc_uv[1],sc_uv[4],sc_uv[2],sc_uv[5]);// orta
		DrawQuad(q1,sc_h[2],dp->w,q2,sc_uv[2],sc_uv[4],sc_uv[3],sc_uv[5]); // sag
		
		DrawQuad(0,q2,sc_h[0],dp->h,sc_uv[0],sc_uv[5],sc_uv[1],sc_uv[6]); // sol alt
		DrawQuad(sc_h[0],q2,q1,dp->h,sc_uv[1],sc_uv[5],sc_uv[2],sc_uv[6]); // orta alt
		DrawQuad(q1,q2,dp->w,dp->h,sc_uv[2],sc_uv[5],sc_uv[3],sc_uv[6]); // sag alt
	glEnd();
}

void GuiScalableQuad::Render(long x,long y,long w,long h)
{
	glLoadIdentity();
	glTranslated(x,y,0);

	int q1,q2;
	q1 = w - sc_h[1];
	q2 = h - sc_h[3];

	glBegin(GL_QUADS);
		DrawQuad(0,0,sc_h[0],sc_h[2],sc_uv[0],sc_uv[7],sc_uv[1],sc_uv[4]); // sol ust
		DrawQuad(sc_h[0],0,q1,sc_h[2],sc_uv[1],sc_uv[7],sc_uv[2],sc_uv[4]); // ust orta
		DrawQuad(q1,0,w,sc_h[2],sc_uv[2],sc_uv[7],sc_uv[3],sc_uv[4]); // sag ust

		DrawQuad(0,sc_h[2],sc_h[0],q2,sc_uv[0],sc_uv[4],sc_uv[1],sc_uv[5]); // sol
		DrawQuad(sc_h[0],sc_h[2],q1,q2,sc_uv[1],sc_uv[4],sc_uv[2],sc_uv[5]);// orta
		DrawQuad(q1,sc_h[2],w,q2,sc_uv[2],sc_uv[4],sc_uv[3],sc_uv[5]); // sag
		
		DrawQuad(0,q2,sc_h[0],h,sc_uv[0],sc_uv[5],sc_uv[1],sc_uv[6]); // sol alt
		DrawQuad(sc_h[0],q2,q1,h,sc_uv[1],sc_uv[5],sc_uv[2],sc_uv[6]); // orta alt
		DrawQuad(q1,q2,w,h,sc_uv[2],sc_uv[5],sc_uv[3],sc_uv[6]); // sag alt
	glEnd();
}

//////////////////////////////////////////////////////////////////////////
/// NON SCALABLE QUAD
//////////////////////////////////////////////////////////////////////////


void GuiQuad::GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2)
{
	sc_uv[0] = Ratio((float)tx->twidth,1.0f,(float)x1);
	sc_uv[1] = Ratio((float)tx->theight,1.0f,(float)y1);
	sc_uv[2] = Ratio((float)tx->twidth,1.0f,(float)x2);
	sc_uv[3] = Ratio((float)tx->theight,1.0f,(float)y2);
/*	w = (x2-x1)+1;
	h = (y2-y1)+1;*/
}

void GuiQuad::Render(ObjectHandle *dp)
{
	glLoadIdentity();
	glTranslated(dp->sx,dp->sy,0);

	glBegin(GL_QUADS);
		DrawQuad(0,0,dp->w,dp->h,sc_uv[0],sc_uv[1],sc_uv[2],sc_uv[3]); // tek parca
	glEnd();
}

void GuiQuad::Render(long x,long y,long w,long h,bool ld_idty)
{
	if (ld_idty)
	{
		glLoadIdentity();
		glTranslated(x,y,0);

		glBegin(GL_QUADS);
			DrawQuad(0,0,w,h,sc_uv[0],sc_uv[1],sc_uv[2],sc_uv[3]); // tek parca
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			DrawQuad(x,y,x+w,y+h,sc_uv[0],sc_uv[1],sc_uv[2],sc_uv[3]); // tek parca
		glEnd();
	}
}


//////////////////////////////////////////////////////////////////////////
/// HORIZONTAL SCALABLE QUAD
//////////////////////////////////////////////////////////////////////////

void GuiHorizontalQuad::GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2)
{
	sc_uv[0] = Ratio((float)tx->theight,1.0f,(float)y1);
	sc_uv[1] = Ratio((float)tx->twidth,1.0f,(float)x1);
	sc_uv[2] = Ratio((float)tx->twidth,1.0f,(float)x1+sc_h[0]);
	sc_uv[3] = Ratio((float)tx->theight,1.0f,(float)y2);
	sc_uv[4] = Ratio((float)tx->twidth,1.0f,(float)x2-sc_h[1]);
	sc_uv[5] = Ratio((float)tx->twidth,1.0f,(float)x2);
}

void GuiHorizontalQuad::Render(ObjectHandle *dp)
{
	glLoadIdentity();
	glTranslated(dp->sx,dp->sy,0);
	
	int q1;
	q1 = dp->w - sc_h[1];

	glBegin(GL_QUADS);
		DrawQuad(0,0,sc_h[0],dp->h,sc_uv[1],sc_uv[0],sc_uv[2],sc_uv[3]);
		DrawQuad(sc_h[0],0,q1,dp->h,sc_uv[2],sc_uv[0],sc_uv[4],sc_uv[3]);
		DrawQuad(q1,0,dp->w,dp->h,sc_uv[4],sc_uv[0],sc_uv[5],sc_uv[3]);
	glEnd();

	//QuickInfo("%i",sc_h[0]);
}

void GuiHorizontalQuad::Render(long x,long y,long w,long h)
{
	glLoadIdentity();
	glTranslated(x,y,0);
	
	int q1;
	q1 = w - sc_h[1];

	glBegin(GL_QUADS);
		DrawQuad(0,0,sc_h[0],h,sc_uv[1],sc_uv[0],sc_uv[2],sc_uv[3]);
		DrawQuad(sc_h[0],0,q1,h,sc_uv[2],sc_uv[0],sc_uv[4],sc_uv[3]);
		DrawQuad(q1,0,w,h,sc_uv[4],sc_uv[0],sc_uv[5],sc_uv[3]);
	glEnd();

	//QuickInfo("%i",sc_h[0]);
}

//////////////////////////////////////////////////////////////////////////
/// VERTICAL SCALABLE QUAD
//////////////////////////////////////////////////////////////////////////

void GuiVerticalQuad::GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2)
{
	sc_uv[0] = Ratio((float)tx->twidth,1.0f,(float)x1);
	sc_uv[1] = Ratio((float)tx->theight,1.0f,(float)y1);
	sc_uv[2] = Ratio((float)tx->theight,1.0f,(float)y1+sc_h[0]);
	sc_uv[3] = Ratio((float)tx->twidth,1.0f,(float)x2);
	sc_uv[4] = Ratio((float)tx->theight,1.0f,(float)y2-sc_h[1]);
	sc_uv[5] = Ratio((float)tx->theight,1.0f,(float)y2);
}

void GuiVerticalQuad::Render(ObjectHandle *dp)
{
	glLoadIdentity();
	glTranslated(dp->sx,dp->sy,0);
	
	int q1;
	q1 = dp->h - sc_h[1];

	glBegin(GL_QUADS);
		DrawQuad(0,0,dp->w,sc_h[0],sc_uv[0],sc_uv[1],sc_uv[3],sc_uv[2]);
		DrawQuad(0,sc_h[0],dp->w,q1,sc_uv[0],sc_uv[2],sc_uv[3],sc_uv[4]);
		DrawQuad(0,q1,dp->w,dp->h,sc_uv[0],sc_uv[4],sc_uv[3],sc_uv[5]);
	glEnd();
	//QuickInfo("%i",sc_h[0]);
}

void GuiVerticalQuad::Render(long x,long y,long w,long h)
{
	glLoadIdentity();
	glTranslated(x,y,0);
	
	int q1;
	q1 = h - sc_h[1];

	glBegin(GL_QUADS);
		DrawQuad(0,0,w,sc_h[0],sc_uv[0],sc_uv[1],sc_uv[3],sc_uv[2]);
		DrawQuad(0,sc_h[0],w,q1,sc_uv[0],sc_uv[2],sc_uv[3],sc_uv[4]);
		DrawQuad(0,q1,w,h,sc_uv[0],sc_uv[4],sc_uv[3],sc_uv[5]);
	glEnd();
}