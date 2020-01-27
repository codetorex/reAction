

//////////////////////////////////////////////////////////////////////////
/// FONT
//////////////////////////////////////////////////////////////////////////

VFont::VFont()
{
	fbase = 0;
	ftex = 0;
	cwid = 0;
}

VFont::~VFont()
{
	UnloadFont();
}

void VFont::UnloadFont()
{
   if(fbase != 0) glDeleteLists(fbase, 256);	
}

void VFont::LoadFromSystem( char *fontname, int size )
{
	HFONT hFont;
	fbase = glGenLists(256);
	hFont = CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontname);
	if(!hFont)
      return;
	SelectObject(mEngine.hDC, hFont);
	wglUseFontBitmaps(mEngine.hDC, 0, 256, fbase);
	cwid = new int [256]; // 64 new
	GetCharWidth32(mEngine.hDC,0,255,cwid);
	/*ABC abc[223];
	GetCharABCWidths(mEngine.hDC,32,254,abc);
	int i;
	for (i = 0;i<223;i++)
	{
		cwid[i] = abc[i].abcB + abc[i].abcA + abc[i].abcC;
	}*/
	fsize = size;
}

long VFont::GetStringLen(char* strs,long ln)
{
	if (ln==0) ln = strlen(strs);
	int i;
	byte tmp;
	long snc = 0;
	for (i=0;i<ln;i++)
	{
		tmp = strs[i];
		snc+= cwid[tmp];
	}
	//snc += ln*2; //1px gendinden once 1 px sora
	return snc;
}

void VFont::PrintText(long x,long y,const char* fmt, ...)
{
	char text[512];
	va_list ap;
	if (fmt == NULL) return;
	va_start(ap,fmt);
		vsprintf(text,fmt,ap);
	va_end(ap);

	glLoadIdentity();
	if ( ftex != 0)
	{
		//glDisable(GL_BLEND);
		glTranslated(x,y,0);
		glBindTexture(GL_TEXTURE_2D,ftex);
		glPushAttrib(GL_LIST_BIT);
		glListBase(fbase);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D,0);
		glRasterPos2f(float(x),float(y));
		glPushAttrib(GL_LIST_BIT);
		glListBase(fbase);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}
}
	
void VFont::LoadFromTexture( VTexture* tex,word wp,word hp,word start,word len )
{
	float cx; // texture x of char
	float cy; // texture y of char
	int i;	// loops
	int ct=0; // base counter
	float ctw; // tex cord modifier width
	float cth; // tex cord modifier height
	float cw;
	float ch;
	cw = float(tex->twidth) / float(wp);
	ch = float(tex->theight) / float(hp);

	ctw = cw / tex->twidth;
	cth = ch / tex->theight;

	fbase = glGenLists(len);
	for (i=start;i<len+start;i++)
	{
		cx = (i % wp)*ctw;
		cy = (((i-cx) / wp) * cth) - (cx*cth); // i dont know what is cs*cth i found this by coincidence

		glNewList(fbase+ct,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx,cy);
				glVertex2f(0,-ch);
				glTexCoord2f(cx+ctw,cy);
				glVertex2f(cw,-ch);
				glTexCoord2f(cx+ctw,cy+cth);
				glVertex2f(cw,0);
				glTexCoord2f(cx,cy+cth);
				glVertex2f(0,0);
			glEnd();
			glTranslated(cw,0,0);
		glEndList();
		ct++;
	}
	fsize = tex->theight;
	ftex = tex->glID; // we take it's texture id and
	tex->freebuffer(); // free the buffer because we dont need it anymore
}