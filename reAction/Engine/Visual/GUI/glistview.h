
GListview::GListview()
{
	classID = GLISTVIEW_CLASSID;
	items = 0;
	columns = 0;
	rendermode = 0;
	itemcount = 0;
	itemarraysize = 0;
	columncount = 0;
	columnarraysize = 0;
	sitem = 0;
	maxstrpw = 0;

	multiselect = false;

	scr.pid = this;
	
	//vscr.pid = this;
	scr.vscr.smallchange = 16;
	scr.vscr.largechange = 32;

	//hscr.pid = this;
	scr.hscr.smallchange = 1;
	scr.hscr.largechange = 2;

	scr.pEventClass = this;
	
	void (GListview::*A)(int)	= &GListview::StateChanged;
	AttachFunctionPointer(scr.onStateChanged,A);

	twosbars = false;

	columnwidth = 0;

	pClass = this;

	void (GListview::*B)(int,int,int) = &GListview::MouseWheel;
	AttachFunctionPointer(MouseWheel0,B);
}

GListItem* GListview::AddItem(char* caption,int smlimgid,int imgid,GImageList* imglst)
{
	int i;
	PrecacheItems();

	GListItem* bositem;
	bositem = new GListItem;

	bositem->pid = this;
	bositem->parent = this;
	bositem->imglst = imglst;
	//bositem->caption = caption; it maybe a dynamic number
	bositem->imgid = imgid;
	bositem->smlimgid = smlimgid;
	bositem->cskin = cskin;
	bositem->SetCaption(caption);
	bositem->carescissor = true;

	// prepare subdata array
	if (columncount > 0)
	{
		bositem->subdata = new GListItemData * [columncount-1]; // because first column is attached to main
		for (i = 0;i < columncount-1;i++)
		{
			bositem->subdata[i] = 0;
		}
	}

	items[itemcount] = bositem;
	itemcount++;

	return bositem;
}

void GListview::PrecacheItems()
{
	if ( itemcount == 0)
	{
		items = new GListItem* [8];
		itemarraysize = 8;
		return;
	}
	else
	{
		if (itemcount == itemarraysize)
		{
			GListItem** bos;
			itemarraysize = itemarraysize << 1; // * 2
			bos = new GListItem * [itemarraysize];
			memcpy(bos,items,4 * itemcount);
			delete [] items;
			items = bos;
		}
	}	
}

void GListview::UpdateColumnWidth()
{
	int i;
	columnwidth = 0;
	for (i = 0;i<columncount;i++)
	{
		if (columns[i]->visible)
		{
			columnwidth += columns[i]->w;	
		}
	}
}

GColumnHeader* GListview::AddColumn(char* caption,int width,int textalign,int imgid,GImageList* imglst)
{
	PrecacheColumns();

	GColumnHeader* bositem;
	bositem = new GColumnHeader;

	bositem->parent = this;
	bositem->pid = this;
	bositem->w = width;
	bositem->h = cskin->lv_colh_h;
	bositem->imgid = imgid;
	bositem->imglst = imglst;
	bositem->cskin = cskin;
	bositem->textalignment = textalign;
	bositem->SetCaption(caption);
	
	columnwidth += width;
	columns[columncount] = bositem;
	if (columncount == 0)
	{
		bositem->prv = 0;
	}
	else
	{
		bositem->prv = columns[columncount-1];
	}
	columncount++;

	return bositem;
}

void GListview::PrecacheColumns()
{
	if ( columncount == 0)
	{
		columns = new GColumnHeader* [8];
		columnarraysize = 8;
		return;
	}
	else
	{
		if (columncount == columnarraysize)
		{
			GColumnHeader** bos;
			columnarraysize = columnarraysize << 1; // * 2
			bos = new GColumnHeader * [columnarraysize];
			memcpy(bos,columns,4 * columncount);
			delete [] columns;
			columns = bos;
		}
	}	
}


void GListview::refresh()
{
	scr.LoadHandle(cskin->sunkedge_quad[0].sc_h[0]-1,cskin->sunkedge_quad[0].sc_h[2]-1,w - cskin->sunkedge_quad[0].sc_h[3]-1,h - cskin->sunkedge_quad[0].sc_h[3]-1,NULL);
	scr.pid = this;
	scr.SetRelative();
	scr.cskin = cskin;
	scr.vscr.cskin = cskin;
	scr.hscr.cskin = cskin;
//	scr.Resize();
	scr.SwitchState(0);
	scr.update();
/*	//viewmax = (int)(ceil(h / cskin->basefont->fsize)+1);
	vscr.w = cskin->vsb_w;
	vscr.x = w - vscr.w - cskin->sunkedge_quad[0].sc_h[1]+1;
	vscr.y = cskin->sunkedge_quad[0].sc_h[2]-1;
	vscr.h = h - cskin->sunkedge_quad[0].sc_h[3]-1;
	//scr.pid = this;
	vscr.cskin = cskin;
	vscr.stotal = 1;

	vscr.svisible = h;
	vscr.initialize();
	vscr.refresh();

	hscr.w = w - cskin->sunkedge_quad[0].sc_h[3]-1;
	hscr.h = cskin->vsb_h;
	hscr.x = cskin->sunkedge_quad[0].sc_h[0]-1;
	hscr.y = h - hscr.h - cskin->sunkedge_quad->sc_h[3];

	hscr.cskin = cskin;
	hscr.stotal = w*3;

	hscr.svisible = w;
	hscr.initialize();
	hscr.refresh();

//	shaftlen = h - cskin->vsb_h - cskin->vsb_h;

	SetRelative();
	vscr.SetRelative();
	hscr.SetRelative();
	//scr.calcthings();*/
}

void GListview::SetVisiblePortion()
{

	
	float temp;
	scr.vscr.visible = false;
	scr.hscr.visible = false;

	if (rendermode == GLISTVIEW_ICON)
	{
		int iconw;
		iconw = 32 + cskin->lv_imgoffsetx;
		temp = (float)w / (float)iconw;
		maxhiconcount = (int)(floor(temp));

		int iconh;
		iconh = 32 + cskin->lv_imgoffsety;
		temp = (float)h / (float)iconh;
		maxviconcount = (int)(ceil(temp));
		

		scr.vscr.value = 0;


		UpdateScrollbar();
	}

	if (rendermode == GLISTVIEW_LIST)
	{
		int iconw;
		iconw = 16 + maxstrpw + 20;
		temp = (float)w / (float)iconw;
		maxhiconcount = (int)(ceil(temp));

		temp = (float)h / 16.0f;
		maxviconcount = (int)(floor(temp));

		reqw = ((ceil( (float)itemcount / (float)maxviconcount ) ))+1;
		if (reqw > maxhiconcount)
		{
			temp = (float)(h - cskin->vsb_h) / 16.0f;
			maxviconcount = (int)(floor(temp));
		}

		scr.hscr.value = 0;
		UpdateScrollbar();
	}

	if (rendermode == GLISTVIEW_REPORT)
	{
		maxhiconcount = 1;
		int mxh;
		mxh = h - (2 + cskin->lv_colh_h);
		maxviconcount =  (float)(mxh) / 16.0f;
		/*if (itemcount > maxviconcount)
		{
			mxh = hscr.y - (2 + cskin->lv_colh_h);
			maxviconcount =  (float)(mxh) / 16.0f;
		}*/

		scr.vscr.value = 0;
		UpdateScrollbar();
	}

	// we dont have scrollbar at this time
	//r_startitem = 0;
	//r_enditem = itemcount;
	
}

void GListview::StateChanged(int newst)
{
	if (rendermode == GLISTVIEW_REPORT && newst == 3)
	{
		int mxh;
		mxh = h - (2 + cskin->lv_colh_h) - cskin->vsb_h;
		maxviconcount =  (float)(mxh) / 16.0f;
		int oval = scr.vscr.value;

		scr.vscr.svisible = maxviconcount;
		scr.vscr.fixedrefresh();

		/*bthm.y = *my - dty - sy;
		value = (int)((float)(bthm.y-cskin->vsb_h) * freq);
		valuechanged();*/

		return;
	}

	if (rendermode == GLISTVIEW_REPORT && newst == 1)
	{
		int mxh;
		mxh = h - (2 + cskin->lv_colh_h);
		maxviconcount =  (float)(mxh) / 16.0f;
		scr.vscr.svisible = maxviconcount;
		scr.vscr.fixedrefresh();

		/*scr.hscr.value = 0;
		scr.hscr.valuechanged();*/

		return;
	}
}

void GListview::UpdateScrollbar()
{
	if (rendermode == GLISTVIEW_ICON)
	{
		reqh = (ceil((float)itemcount / (float)maxhiconcount)) * (32 + cskin->lv_imgoffsety);
		scr.vscr_enabled = true;
		scr.hscr_enabled = false;
		scr.vscr.svisible = h;
		scr.vscr.stotal = reqh;
		scr.vscr.smallchange = 16;

		if (reqh > h)
		{

			scr.vscr.initialize();
			scr.vscr.refresh();
			scr.vscr.value = 0;
			scr.vscr.valuechanged();

			scr.SwitchState(1);
			scr.update();
		}
	}

	if (rendermode == GLISTVIEW_LIST)
	{
		reqw = ((ceil( (float)itemcount / (float)maxviconcount ) ))+1;
		scr.hscr_enabled = true;
		scr.vscr_enabled = false;
		scr.hscr.svisible = maxhiconcount;
		scr.hscr.stotal = reqw;
		if (reqw > maxhiconcount)
		{

			scr.hscr.initialize();
			scr.hscr.refresh();
			scr.hscr.value = 0;
			scr.hscr.valuechanged();
			scr.SwitchState(2);
			scr.update();
		}
	}

	if (rendermode == GLISTVIEW_REPORT)
	{
		reqh = (itemcount);
		scr.vscr_enabled = true;
		scr.hscr_enabled = true;

		scr.vscr.svisible = maxviconcount;
		scr.vscr.stotal = reqh;

		if (reqh < maxviconcount)
		{
			scr.hscr.svisible = w-5;
		}
		else
		{
			scr.hscr.svisible = w - cskin->vsb_w-4;
		}
		scr.hscr.stotal = columnwidth;

		scr.vscr.smallchange = 1;

		if (reqh > maxviconcount)
		{
			scr.vscr.initialize();
			scr.vscr.refresh();

			scr.hscr.initialize();
			scr.hscr.refresh();

			scr.SwitchState(3);
			scr.update();
		}
	}
}

void GListview::CalculateIconViewStartEnd()
{
	r_startitem = (scr.vscr.value / 75) * maxhiconcount;
	r_enditem = r_startitem + (maxhiconcount * (maxviconcount+1));
	if (r_enditem>itemcount)
	{
		r_enditem = itemcount;
	}
}

void GListview::CalculateListViewStartEnd()
{
	//r_startitem = (hscr.value / (16+maxstrpw+20)) * maxviconcount;
	
	r_startitem = (scr.hscr.value * maxviconcount);
	r_enditem = r_startitem + (maxviconcount * (maxhiconcount));
	if (r_startitem<0)
	{
		r_startitem = 0;
	}
	if (r_enditem>itemcount)
	{
		r_enditem = itemcount;
	}

}


void GListview::CalculateReportStartEnd()
{
	r_startitem = scr.vscr.value;
	r_enditem = r_startitem + (maxviconcount);
	if (r_enditem>itemcount)
	{
		r_enditem = itemcount;
	}
}

void GListview::update()
{
	SetRelative();

	/*scr.LoadHandle(cskin->sunkedge_quad[0].sc_h[0]-1,cskin->sunkedge_quad[0].sc_h[2]-1,w - cskin->sunkedge_quad[0].sc_h[3]-1,h - cskin->sunkedge_quad[0].sc_h[3]-1,NULL);
	scr.pid = this;*/
	scr.update();
	/*vscr.update();
	hscr.update();*/
//	ContainerUpdate();
}

void GListview::ChangeViewMode(int rmode)
{
	// todo: expand this function's functionality
	rendermode = rmode;
	SetVisiblePortion();
}

void GListview::MouseWheel(int x,int y, int delta)
{
	if (rendermode == GLISTVIEW_ICON)
	{
		if (scr.vscr.svisible < scr.vscr.stotal)
		{
			scr.vscr.value -= (scr.vscr.smallchange * (delta/WHEEL_DELTA));
			scr.vscr.valuechanged();
		}
	}

	if (rendermode == GLISTVIEW_LIST)
	{
		if (scr.hscr.svisible < scr.hscr.stotal)
		{
			scr.hscr.value -= (scr.hscr.smallchange * (delta/WHEEL_DELTA));
			scr.hscr.valuechanged();
		}
	}

	if (rendermode == GLISTVIEW_REPORT)
	{
		if (scr.vscr.svisible < scr.vscr.stotal)
		{
			scr.vscr.value -= (scr.vscr.smallchange * (delta/WHEEL_DELTA));
			scr.vscr.valuechanged();
		}
	}
}

void GListview::doevents()
{
	if (!visible) return;

	EnableEventScissor(sx,sy+3,w,h-6);
		//basenode.doevents();
	scr.doevents();
	if (rendermode == GLISTVIEW_ICON)
	{
		/*if (reqh > h)
		{
			scr.vscr.doevents();
		}*/
		

		int i;
		int cx,cy;
		int mx;
		mx = cskin->lv_imgoffsetx / 2;
		cx = mx;
		cy = 6 - (scr.vscr.value % 75);
		CalculateIconViewStartEnd();
		for (i = r_startitem; i<r_enditem;i++)
		{
			items[i]->h = 32;
			items[i]->w = 32;
			/*items[i]->x = cx;
			items[i]->y = cy;*/
			items[i]->sx = sx + cx;
			items[i]->sy = sy + cy;
			if ((items[i]->sx+32 + mx) > sx+w )
			{
				cy += cskin->lv_imgoffsety + 32;
				cx = mx;
				items[i]->sx = sx + cx;
				items[i]->sy = sy + cy;
				/*items[i]->x = cx;
				items[i]->y = cy;*/
			}
			items[i]->doevents();
			cx += 32 + cskin->lv_imgoffsetx;
			//cy += cskin->lv_imgoffsety;
		}
	}

	if (rendermode == GLISTVIEW_LIST)
	{

		/*if (reqw > maxhiconcount)
		{
			scr.hscr.doevents();
		}*/
		//scr.doevents();

		int i;
		int cx,cy;
		cx = 6;
		cy = 6;
		
		// todo: use a calculated function
		CalculateListViewStartEnd();
		
		for (i=r_startitem;i<r_enditem;i++)
		{
			items[i]->h = 16;
			items[i]->w = maxstrpw +16;
			items[i]->sx = sx + cx;
			items[i]->sy = sy + cy;
			
			if (reqw > maxhiconcount)
			{
				if (items[i]->sy +16 > scr.hscr.sy)
				{
					cy = 6;
					cx += items[i]->w + 20;
					items[i]->sx = sx + cx;
					items[i]->sy = sy + cy;
				}
			}
			else
			{
				if (items[i]->sy +16 > sy+h)
				{
					cy = 6;
					cx += items[i]->w + 20;
					items[i]->sx = sx + cx;
					items[i]->sy = sy + cy;
				}
			}
			
				items[i]->doevents();
			cy += 16;
		}
	}

	if (rendermode == GLISTVIEW_REPORT)
	{
		/*if (reqh > (maxviconcount))
		{
			vscr.doevents();
		}*/

		int i;
		int cx,cy;
		cx = 2 - scr.hscr.value;
		cy = 2;
		
		// todo: use a calculated value
		for (i=0;i<columncount;i++)
		{
			if (columns[i]->visible)
			{
				columns[i]->h = cskin->lv_colh_h;
				columns[i]->sx = sx + cx;
				columns[i]->sy = sy + cy;
				columns[i]->doevents();
				cx += columns[i]->w;
			}
		}

		CalculateReportStartEnd();
		cx = 2  - scr.hscr.value;
		cy = (2 + cskin->lv_colh_h + 1) - (scr.vscr.value % 16);
		for (i=r_startitem;i<r_enditem;i++)
		{
			items[i]->h = 16;
			items[i]->w = columns[0]->w;
			items[i]->sx = sx + cx;
			items[i]->sy = sy + cy;
			items[i]->doevents();
			cy += 16;
		}

	}
	sig_scissor = false;

	DeliverEvents();
//	ContainerSendEvents();
}

void GListview::FixColumnPrevs()
{
	GColumnHeader* pcol=0;
	int i;
	for (i=0;i<columncount;i++)
	{
		if (columns[i]->visible)
		{
			columns[i]->prv = pcol;
			pcol = columns[i];
		}
	}
	UpdateColumnWidth();
}

void GListview::HideColumn(int cid)
{
	if (columns[cid]->visible)
	{
		columns[cid]->visible = false;
		FixColumnPrevs();
	}
}

void GListview::ShowColumn(int cid)
{
	if (!columns[cid]->visible)
	{
		columns[cid]->visible = true;
		FixColumnPrevs();
	}
}

void GListview::render()
{
	if (!visible) return;
//	ContainerRender();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,cskin->skinID);
	cskin->sunkedge_quad[0].Render(this);

	mEngine.EScissor(sx,sy+3,w,h-6); // makasla babos

	if (rendermode == GLISTVIEW_ICON)
	{
		int i;
		int cx,cy;
		int mx;
		mx = cskin->lv_imgoffsetx / 2;
		cx = mx;
		cy = 6 - (scr.vscr.value % 75);
		CalculateIconViewStartEnd();
		for (i = r_startitem; i<r_enditem;i++)
		{
			items[i]->h = 32;
			items[i]->w = 32;
			/*items[i]->x = cx;
			items[i]->y = cy;*/
			items[i]->sx = sx + cx;
			items[i]->sy = sy + cy;
			if ((items[i]->sx+32 + mx) > sx+w )
			{
				cy += cskin->lv_imgoffsety + 32;
				cx = mx;
				items[i]->sx = sx + cx;
				items[i]->sy = sy + cy;
				/*items[i]->x = cx;
				items[i]->y = cy;*/
			}
			items[i]->render();
			cx += 32 + cskin->lv_imgoffsetx;
			//cy += cskin->lv_imgoffsety;
		}

		/*if (reqh > h)
		{
			vscr.render();
		}*/
	}

	if (rendermode == GLISTVIEW_LIST)
	{
		int i;
		int cx,cy;
		cx = 6;
		cy = 6;
		
		CalculateListViewStartEnd();
		
		for (i=r_startitem;i<r_enditem;i++)
		{
			items[i]->h = 16;
			items[i]->w = maxstrpw +16;
			items[i]->sx = sx + cx;
			items[i]->sy = sy + cy;
			
			if (reqw > maxhiconcount)
			{
				if (items[i]->sy +16 > scr.hscr.sy)
				{
					cy = 6;
					cx += items[i]->w + 20;
					items[i]->sx = sx + cx;
					items[i]->sy = sy + cy;
				}
			}
			else
			{
				if (items[i]->sy +16 > sy+h)
				{
					cy = 6;
					cx += items[i]->w + 20;
					items[i]->sx = sx + cx;
					items[i]->sy = sy + cy;
				}
			}
			items[i]->render();
			cy += 16;
		}

		/*if (reqw > maxhiconcount)
		{
			hscr.render();
		}*/

	}
	
	if (rendermode == GLISTVIEW_REPORT)
	{
		int i,j;
		int cx,cy; // column header x, colh y
		int rx,ry; // row x, row y

		if (scr.vscr.stotal < scr.vscr.svisible)
		{
			mEngine.EScissor(sx+2,sy+3,w-4,h-6); // vertical scrollbar is not visible
		}
		else
		{
			mEngine.EScissor(sx+2,sy+3,w-cskin->vsb_w-4,h-6); // vertical scrollbar is visible
		}
		
		cx = 2 - scr.hscr.value;
		cy = 2;
		//mEngine.EScissor(sx,sy+3,w,h-6); // makasla babos
		// todo: use a calculated value
		for (i=0;i<columncount;i++)
		{
			if (columns[i]->visible)
			{
				columns[i]->h = cskin->lv_colh_h;
				columns[i]->sx = sx + cx;
				columns[i]->sy = sy + cy;
				columns[i]->render();
				cx += columns[i]->w;
			}

		}

		CalculateReportStartEnd();
		for (i=0;i<columncount;i++)
		{	
			if (!columns[i]->visible)
			{
				continue;
			}

			int colright,mright;
			mright = sx + w-3;
			colright = columns[i]->sx + columns[i]->w;
			if (colright < sx)
			{
				continue;
			}
			if ( columns[i]->sx > mright)
			{
				continue;
			}

			int nw;
			if (columns[i]->sx < sx) // scrollbarin gazabina uramis yazzik 
			{
				nw = columns[i]->sx - sx;
				if (colright > mright )
				{
					int nw2;
					nw2 = colright - mright;
					mEngine.EScissor(sx+3,columns[0]->sy,columns[0]->w+1 + nw - nw2,h-6); // makasla babos
				}
				else
				{
					mEngine.EScissor(sx+3,columns[0]->sy,columns[0]->w+1 + nw,h-6); // makasla babos
				}
			}
			else
			{
				if (scr.vscr.stotal > scr.vscr.svisible)
				{
					nw = cskin->vsb_w;
				}
				else
				{
					nw = 0;
				}
				
				if (colright > mright )
				{
					int nw2;
					nw2 = colright - mright;
					mEngine.EScissor(columns[i]->sx,columns[i]->sy,columns[i]->w+1 - nw2 - nw,h-6); // makasla babos
				}
				else
				{
					mEngine.EScissor(columns[i]->sx,columns[i]->sy,columns[i]->w+1,h-6); // makasla babos
				}
			}
			

			if (gridcollines)
			{
				glColor3ub(218,217,223);
				glBegin(GL_LINES);
					glVertex2i(colright,columns[i]->sy+1);
					glVertex2i(colright,columns[i]->sy+h);
				glEnd();
			}

			cx = columns[i]->sx;
			cy = columns[i]->sy+cskin->lv_colh_h;
			if (i == 0)
			{
				for (j=r_startitem;j<r_enditem;j++)
				{
					items[j]->h = 16;
					items[j]->w = columns[0]->w;
					items[j]->sx = cx;
					items[j]->sy = cy;					
					items[j]->render();
					cy += 16;
				}
			}
			else
			{
				for (j=r_startitem;j<r_enditem;j++)
				{

					if (items[j]->subdata[i-1] != 0)
					{
						glColor3ubv(cskin->deffontclr);
						cskin->basefont->PrintText(cx+2,cy+cskin->basefont->fsize,items[j]->subdata[i-1]->strrep);
					}
					cy += 16;
				}
			}

		}

		
		if (gridrowlines)
		{
			mEngine.EScissor(sx+2,sy+3,w-4,h-6);
			glLoadIdentity();
			glColor3ub(218,217,223);
			glBegin(GL_LINES);
			for (i=r_startitem;i<r_enditem;i++)
			{
				glVertex2i(items[i]->sx,items[i]->sy+16);
				glVertex2i(items[i]->sx+columnwidth,items[i]->sy+16);
			}
			glEnd();
		}

		/*if (reqh > (maxviconcount))
		{
			vscr.render();
		}*/

	}
	glDisable(GL_SCISSOR_TEST);
	scr.render();


	/*glLoadIdentity();
	RenderDebugArea();*/
}
