
// the legendary nodes
GTreeNode::GTreeNode()
{
	owner = this;
	collapsed = true;
	parent = 0;
	imglst = 0;
	caption = 0;
	imgid = 0;
//	lasty = 16; // self height

	classID = GTREENODE_CLASSID;

	pClass = this;
	void (GTreeNode::*A)(int,int,int)	= &GTreeNode::MouseDown;
	void (GTreeNode::*B)(int,int,int)	= &GTreeNode::MouseWheel;

	AttachFunctionPointer(MouseDown0,A);
	AttachFunctionPointer(MouseWheel0,B);
}

char* GTreeNode::GetFullPath()
{
	int i;
	char **bos;
	bos = new char* [level+1];
	GTreeNode* op = this;
	char* tempbos;

	for ( i = 0; i <= level ; i++)
	{
		bos[i] = op->caption;
		op = (GTreeNode*)op->pid;
	}

	tempbos = new char[512];
	memset(tempbos,0,512);

	for (i = level; i >= 0 ; i--)
	{
		strcat(tempbos,bos[i]);
		strcat(tempbos,parent->pathseprator);
	}

	return tempbos;
}

void GTreeNode::MouseWheel(int x,int y, int delta)
{
	/*parent->scr.value -= delta / 8; // >> 3
	parent->scr.valuechanged();*/
	parent->MouseWheel(x,y,delta);
}

GTreeNode* GTreeNode::AddItem(char* caption,int imgid,GImageList* imglst)
{
	int cplen;
	cplen = cskin->basefont->GetStringLen(caption);

	GTreeNode* bosnode;
	bosnode = new GTreeNode;

	bosnode->LoadHandle(16,0,cplen+32,16,NULL);
	AddObj(bosnode);
	//lasty += 16;

	bosnode->caption = caption;
	bosnode->imgid = imgid;
	bosnode->imglst = imglst;
	bosnode->parent = parent;
	bosnode->level = level+1;
	bosnode->defcarescissor = true;
	bosnode->carescissor = true;

	return bosnode;
}

void GTreeNode::MouseDown(int x,int y,int button)
{
	collapsed = !collapsed;
	if (parent->snode != this)
	{
		parent->snode = this;
		if (parent->onSelectionChanged != NULL)
		{
			parent->onSelectionChanged();
		}
	}
}

/*void GTreeNode::update()
{
	SetRelative();
}*/


void GTreeNode::doevents()
{
	if (!collapsed)
	{
		ContainerSendEvents();
	}
	if (sy+h > parent->sy && sy < parent->sy + parent->h )
	{
		DeliverEvents();
	}
}

void GTreeNode::render()
{

	if (pid->classID == GTREENODE_CLASSID)
	{
		y = ((GTreeNode*)pid)->starty;
		SetRelative();
	}

	if (parent->showln)
	{
		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D,0);
		glLineStipple(1,0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glColor3ub(128,128,128);
		glBegin(GL_LINES);
			// yatay cizgi
			glVertex2i(sx+9,sy+9);
			glVertex2i(sx+24,sy+9);

			//dikey tam cizgi
			/*if (((ObjectContainer*)pid)->lastobject != this)
			{
				glVertex2i(sx+8,sy);
				glVertex2i(sx+8,sy+h);
			}
			else
			{
				// dikey yarim cizgi
				glVertex2i(sx+8,sy);
				glVertex2i(sx+8,sy+9);
			}*/



			if (pid->classID == GTREENODE_CLASSID)
			{
				if (((ObjectContainer*)pid)->firstobject == this)
				{	
					glVertex2i(sx+8,sy-1);
					glVertex2i(sx+8,sy+9);
				}
				if (((ObjectContainer*)pid)->lastobject != this)
				{
					glVertex2i(sx+8,sy+9);
					glVertex2i(sx+8,nextobject->sy+9);
				}
			}


		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}


	if (sy+h > parent->sy && sy < parent->sy + parent->h )
	{

		// kendini render et
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,cskin->skinID);

		int incr = 1;

		if (parent->showpm)
		{
			if (objectcount >0)
			{
				cskin->tv_quad[collapsed ? 0 : 1].Render(sx+incr,sy+cskin->tv_oy,cskin->tv_w,cskin->tv_h);
			}
			incr += cskin->tv_w + 4;
		}
		

		if (parent->showpic)
		{
			imglst->render(sx+incr,sy,imgid,true);
			incr += 16;
		}

		/*if (parent->snode == this) // draw a over line
		{
			glColor4ub(255,255,255,255);
		}
		else
		{*/
			glColor3ubv(cskin->deffontclr);
		//}
		cskin->basefont->PrintText(sx+incr,sy+cskin->basefont->fsize,caption);
		if (parent->snode == this) // draw a over line
		{
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);
			glLineStipple(1,0xAAAA);
			glEnable(GL_LINE_STIPPLE);
			glColor3ub(128,128,128);
			glBegin(GL_LINES);
			int nsx = sx + incr;
				glVertex2i(nsx,sy);
				glVertex2i(sx+w,sy);

				glVertex2i(nsx,sy);
				glVertex2i(nsx,sy+h);

				glVertex2i(nsx,sy+h);
				glVertex2i(sx+w,sy+h);

				glVertex2i(sx+w,sy+h);
				glVertex2i(sx+w,sy);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			/*glEnable(GL_COLOR_LOGIC_OP); 
			glLogicOp(GL_XOR);

			glColor4ub(99,137,209,127);
			glBegin(GL_QUADS);
				DrawQuad(nsx,sy,sx+w,sy+h,0,0,0,0);
			glEnd();

			glDisable(GL_COLOR_LOGIC_OP);*/
		}

	}



	// starty += y + 16; // gendi
	// cocuklarini render et. ama nasil hmm...
	starty = 16; 
	if (!collapsed)
	{
		ContainerRender();
	}

	/*if (objectcount>0)
	{
		glLineStipple(1,0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glColor3ub(128,128,128);
		glBegin(GL_LINES);
			glVertex2i(firstobject->sx+6,firstobject->sy);
			glVertex2i(lastobject->sx+6,lastobject->sy);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}*/

	if (pid->classID == GTREENODE_CLASSID)
	{
		((GTreeNode*)pid)->starty += starty;
	}
}

