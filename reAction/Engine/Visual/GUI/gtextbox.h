

GTextBox::GTextBox()
{
	tcounter = 0;
	memsize = 64;
	textx = 0;
	text = new char [65];
	cursorx = 0;
	memset(text,0,64);

	// Set Callbacks
	pClass = this;
	wfkeyup = false;

	void (GTextBox::*A)(int)	= &GTextBox::KeyDown;
	void (GTextBox::*B)(int)	= &GTextBox::KeyUp;
	void (GTextBox::*C)(int)	= &GTextBox::KeyPress;


	AttachFunctionPointer(KeyDown0,A);
	AttachFunctionPointer(KeyUp0,B);
	AttachFunctionPointer(KeyPress0,C);
}

void GTextBox::KeyPress(int key)
{
	if (key == 0x08) // back space
	{
		if (textx > 0)
		{
			byte a1;
			a1 = text[textx-1];
			textx--;
			cursorx -= cskin->basefont->cwid[a1];
			//QuickInfo("%i %i %c %i",cskin->basefont->cwid[a1],a1,a1,textx);
			text[textx] = 0;
			//cursorx = cskin->basefont->GetStringLen(text);
		}
	}
	else
	{
		text[textx] = key;
		textx++;
		cursorx += cskin->basefont->cwid[key];
		//cursorx = cskin->basefont->GetStringLen(text);
		//QuickInfo("%i %i %c %i",cskin->basefont->cwid[key],key,key,textx);

		wfkeyup = true;
		lastkey = key;
	}	
}

void GTextBox::KeyDown(int key)
{
	//QuickInfo("%i",key);
	/*char amp[64];
	sprintf(amp,"%i",key);
	MessageBox(0,amp,"Selam",MB_OK);*/
	/*if (key == 8)
	{
		if (textx > 0)
		{
			cursorx -= cskin->basefont->cwid[text[textx-1] - 32] + 2;
			textx--;
			text[textx] = 0;
		}
	}
	else
	{
		text[textx] = key;
		textx++;
		cursorx += cskin->basefont->cwid[key-32] + 2;
		wfkeyup = true;
		lastkey = key;
	}*/
}

void GTextBox::KeyUp(int key)
{
	//wfkeyup = false;
}

/*void GTextBox::doevents()
{
	DeliverEvents();
	if (focused == this && wfkeyup)
	{
		if ((int)GetTickCount() - tcounter > 200 )
		{
			
			tcounter = GetTickCount();
		}
	}
}*/

void GTextBox::render()
{
	if (!visible) return;
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);

	cskin->sunkedge_quad[0].Render(this);
	
	if (focused == this)
	{
		glBindTexture(GL_TEXTURE_2D,cskin->tbeam_tex);

		glLoadIdentity();
		glTranslated(sx+cursorx,sy-3,0);
		
		glBegin(GL_QUADS);
			DrawQuad(0,0,12,24,0,0,1,1);
		glEnd();
	}


	glColor4f(0,0,0,1);

	int stp = cskin->basefont->fsize;
	cskin->basefont->PrintText(sx+3,sy+stp+1,text);
}