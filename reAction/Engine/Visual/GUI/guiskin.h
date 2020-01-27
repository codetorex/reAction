
//////////////////////////////////////////////////////////////////////////
/// SKIN
//////////////////////////////////////////////////////////////////////////

GuiSkin::GuiSkin()
{
	window_cac = false;
	presizing = false;
	skinID = 0;
	basefont = 0;
	lv_imgoffsetx = 43;
	lv_imgoffsety = 43;
}

void GuiSkin::BeginCreatingSkin(int w,int h)
{
	basetex.createbuffer(w,h,GL_RGBA); // start as gl_rgba hmm
	basequad.width = w;
	basequad.height = h;
	//basetex.clear(RGBA(0,0,0,0));
}

void GuiSkin::EndCreatingSkin()
{
	basetex.gettexid(GL_NEAREST,GL_NEAREST);
	skinID = basetex.glID;
}

void GuiSkin::LoadWindow(VTexture *tex,long mleft,long mright,long mtop,long mbottom)
{

	if (tex->bytes != 0 && tex->glID == 0)
	{
		tex->gettexid();
	}
	window_tex[0] = tex->glID;

	window_quad.sc_h[0] = mleft;
	window_quad.sc_h[1] = mright;
	window_quad.sc_h[2] = mtop;
	window_quad.sc_h[3] = mbottom;
	window_quad.GenerateUV(tex,0,0,tex->twidth,tex->theight);
	/*for (int i = 0 ;i<4;i++)
	{
		window_quad.sc_h[i] *= 2;
	}*/
}

void GuiSkin::CreateLoadWindowTexture(VTexture *tex,int w,int h,char* bbpath,char* blpath,char* brpath,char* btpath,int tl,int tr,int lt,int lb,int rt,int rb,int bl,int br,long bgclr)
{
	CreateWindowTexture(tex,true,w,h,bbpath,blpath,brpath,btpath,tl,tr,lt,lb,rt,rb,bl,br,bgclr);
	tex->colorkey(RGB(255,0,255));
	tex->gettexid(GL_NEAREST,GL_NEAREST);
	window_tex[0] = tex->glID;

	CreateWindowTexture(tex,false,w,h,bbpath,blpath,brpath,btpath,tl,tr,lt,lb,rt,rb,bl,br,bgclr);
	tex->colorkey(RGB(255,0,255));
	tex->gettexid(GL_NEAREST,GL_NEAREST);
	window_tex[1] = tex->glID;

	window_quad.GenerateUV(tex,0,0,w,h);
}

void GuiSkin::CreateWindowTexture(VTexture *tex,bool active,int w,int h,char* bbpath,char* blpath,char* brpath,char* btpath,int tl,int tr,int lt,int lb,int rt,int rb,int bl,int br,long bgclr)
{
	tex->freebuffer();
	tex->createbuffer(w,h,GL_RGB);
	tex->clear(bgclr);

	VTexture borderbottom;
	VTexture borderleft;
	VTexture borderright;
	VTexture bordertop;

	borderbottom.loadbmp(bbpath);
	borderleft.loadbmp(blpath);
	borderright.loadbmp(brpath);
	bordertop.loadbmp(btpath);

	int leftw;
	int rightw;
	int toph;
	int bottomh;

	leftw = borderleft.twidth>>1; // divide 2
	rightw = borderright.twidth>>1;
	toph = bordertop.theight>>1;
	bottomh = borderbottom.theight>>1;

	// also prepare the quad
	window_quad.sc_h[0] = leftw;
	window_quad.sc_h[1] = rightw;
	window_quad.sc_h[2] = toph;
	window_quad.sc_h[3] = bottomh;

	if (active)
	{
		tex->subimage(&borderleft,0,0,0,0,leftw,lt); // sol ust kosenin parcasini koy
		tex->subimage(&borderleft,0,borderleft.theight-lb,0,h-lb,leftw,lb); //sol alt kosenin parcasini koy

		tex->subimage(&borderright,0,0,w-rightw,0,rightw,rt); // sag ust kosenin parcasini koy
		tex->subimage(&borderright,0,borderright.theight-rb,w-rightw,h-rb,rightw,rb); // sag alt kosenin parcasini koy

		tex->subimage(&bordertop,0,0,leftw,0,tl,toph); // ust sol tarafi koy
		tex->subimage(&bordertop,bordertop.twidth-tr,0,w-rightw-tr,0,tr,toph); // ust sag tarafi koy

		tex->subimage(&borderbottom,0,0,leftw,h-bottomh,bl,bottomh);
		tex->subimage(&borderbottom,borderbottom.twidth-br,0,w-rightw-br,h-bottomh,br,bottomh);

		tex->tileimage(&bordertop,tl,0,bordertop.twidth-tl-tr,toph,tl,0,w-tl-tr,toph); // tile top part
		tex->tileimage(&borderbottom,bl,0,borderbottom.twidth-bl-br,bottomh,bl,h-bottomh,w-bl-br,bottomh);
		tex->tileimage(&borderleft,0,lt,leftw,borderleft.theight-lt-lb,0,lt,leftw,h-lt-lb);
		tex->tileimage(&borderright,0,rt,rightw,borderright.theight-rt-rb,w-rightw,rt,rightw,h-rt-rb);
	}
	else
	{
		tex->subimage(&borderleft,leftw,0,0,0,leftw,lt); // sol ust kosenin parcasini koy
		tex->subimage(&borderleft,leftw,borderleft.theight-lb,0,h-lb,leftw,lb); //sol alt kosenin parcasini koy

		tex->subimage(&borderright,rightw,0,w-rightw,0,rightw,rt); // sag ust kosenin parcasini koy
		tex->subimage(&borderright,rightw,borderright.theight-rb,w-rightw,h-rb,rightw,rb); // sag alt kosenin parcasini koy

		tex->subimage(&bordertop,0,toph,leftw,0,tl,toph); // ust sol tarafi koy
		tex->subimage(&bordertop,bordertop.twidth-tr,toph,w-rightw-tr,0,tr,toph); // ust sag tarafi koy

		tex->subimage(&borderbottom,0,bottomh,leftw,h-bottomh,bl,bottomh);
		tex->subimage(&borderbottom,borderbottom.twidth-br,bottomh,w-rightw-br,h-bottomh,br,bottomh);

		tex->tileimage(&bordertop,tl,toph,bordertop.twidth-tl-tr,toph,tl,0,w-tl-tr,toph); // tile top part
		tex->tileimage(&borderbottom,bl,bottomh,borderbottom.twidth-bl-br,bottomh,bl,h-bottomh,w-bl-br,bottomh);
		tex->tileimage(&borderleft,leftw,lt,leftw,borderleft.theight-lt-lb,0,lt,leftw,h-lt-lb);
		tex->tileimage(&borderright,rightw,rt,rightw,borderright.theight-rt-rb,w-rightw,rt,rightw,h-rt-rb);

	}
}

void GuiSkin::LoadButton(VTexture *tex,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(tex,0,0,nx,ny,tex->twidth,tex->theight);



	for (int i = 0;i< 5;i++)
	{
		button_quad[i].sc_h[0] = mleft;
		button_quad[i].sc_h[1] = mright;
		button_quad[i].sc_h[2] = mtop;
		button_quad[i].sc_h[3] = mbottom;
		button_quad[i].GenerateUV(&basetex,nx+ (tex->twidth/5)*i,ny,nx + (tex->twidth/5)*(i+1),ny + tex->theight);
	}
}

void GuiSkin::LoadSunkEdge(VTexture *tex,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(tex,0,0,nx,ny,tex->twidth,tex->theight);


	for (int i = 0; i<2 ;i++)
	{
		sunkedge_quad[i].sc_h[0] = mleft;
		sunkedge_quad[i].sc_h[1] = mright;
		sunkedge_quad[i].sc_h[2] = mtop;
		sunkedge_quad[i].sc_h[3] = mbottom;
	}
	sunkedge_quad[0].GenerateUV(&basetex,nx+(tex->twidth/4)*0,ny,nx+(tex->twidth/4)*1,ny+tex->theight);
	sunkedge_quad[1].GenerateUV(&basetex,nx+(tex->twidth/4)*3,ny,nx+(tex->twidth/4)*4,ny+tex->theight);
}

void GuiSkin::LoadCheckBox(VTexture *tex)
{
	float ca,cb;
	cbox_w = tex->twidth / 12;
	cbox_h = tex->theight;

	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);
	
	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(tex,0,0,nx,ny,tex->twidth,tex->theight);

	cbox_base = glGenLists(12);

	float cc,cd;
	cc = Ratio(basetex.theight,1.0f,ny);
	cd = Ratio(basetex.theight,1.0f,ny+cbox_h);

	for (int i = 0;i<12;i++)
	{
		ca = Ratio(basetex.twidth,1.0f,nx+i*cbox_w);
		cb = Ratio(basetex.twidth,1.0f,nx+(i+1)*cbox_w);

		glNewList(cbox_base+i,GL_COMPILE );
			glBegin(GL_QUADS);
				DrawQuad(0,0,cbox_w,cbox_h,ca,cc,cb,cd);
			glEnd();
		glEndList();	
	}
}

void GuiSkin::LoadScrollButton(VTexture *but)
{
	float ca,cb;
	vsb_w = but->twidth / 23;
	vsb_h = but->theight;

	// Insert it
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	
	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(but,0,0,nx,ny,but->twidth,but->theight);

	float cc,cd;
	cc = Ratio(basetex.theight,1.0f,ny);
	cd = Ratio(basetex.theight,1.0f,ny+vsb_h);

	vsb_base = glGenLists(23);

	for (int i=0;i<23;i++)
	{
		ca = Ratio(basetex.twidth,1.0f,nx+i*vsb_w);
		cb = Ratio(basetex.twidth,1.0f,nx+(i+1)*vsb_w);

		glNewList(vsb_base+i,GL_COMPILE );
			glBegin(GL_QUADS);
				DrawQuad(0,0,vsb_w,vsb_h,ca,cc,cb,cd);
			glEnd();
		glEndList();	
	}// its done
}

void GuiSkin::LoadVScrollShaft(VTexture *shaft )
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(shaft->twidth+2,shaft->theight+2);
	basetex.subimage(shaft,0,0,newquad->x+1,newquad->y+1,shaft->twidth,shaft->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 4;i++)
	{
		vss_quad[i].GenerateUV(&basetex,nx+ (shaft->twidth/4)*i,ny,nx + (shaft->twidth/4)*(i+1),ny + shaft->theight);
	}
}

void GuiSkin::LoadHScrollShaft(VTexture *shaft )
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(shaft->twidth+2,shaft->theight+2);
	basetex.subimage(shaft,0,0,newquad->x+1,newquad->y+1,shaft->twidth,shaft->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 4;i++)
	{
		hss_quad[i].GenerateUV(&basetex,nx+ (shaft->twidth/4)*i,ny,nx + (shaft->twidth/4)*(i+1),ny + shaft->theight);
	}
}

void GuiSkin::LoadVScrollThumb(VTexture *thm,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(thm->twidth+2,thm->theight+2);
	basetex.subimage(thm,0,0,newquad->x+1,newquad->y+1,thm->twidth,thm->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 3;i++)
	{
		vst_quad[i].sc_h[0] = mtop;
		vst_quad[i].sc_h[1] = mbottom;
		vst_quad[i].GenerateUV(&basetex,nx+ (thm->twidth/3)*i,ny,nx + (thm->twidth/3)*(i+1),ny + thm->theight);
	}
}

void GuiSkin::LoadHScrollThumb(VTexture *thm,long mleft,long mright)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(thm->twidth+2,thm->theight+2);
	basetex.subimage(thm,0,0,newquad->x+1,newquad->y+1,thm->twidth,thm->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 3;i++)
	{
		hst_quad[i].sc_h[0] = mleft;
		hst_quad[i].sc_h[1] = mright;
		hst_quad[i].GenerateUV(&basetex,nx+ (thm->twidth/3)*i,ny,nx + (thm->twidth/3)*(i+1),ny + thm->theight);
	}
}
void GuiSkin::CreateVScrollButton(VTexture *but, VTexture *gly)
{
	float gw,gh,gx,gy; // first glyph's width, height, center x,center y
	float bw,bh,bx,by; // first buton's width, height, center x,center y
	float dx,dy; // desired x, desired y

	gw = gly->twidth / 23.0f;
	gh = gly->theight;
	gy = gh / 2.0f;
	gx = gw / 2.0f;

	bw = but->twidth / 23.0f;
	bh = but->theight;
	by = bh / 2.0f;
	bx = bw / 2.0f;

	dx = bx - gx;
	dy = by - gy;

	for( float i=0;i<23;i++)
	{
		but->subimage(gly,gw*i,0,dx+(i*bw),dy,gw,gh);
	}
}

void GuiSkin::CreateComboBoxButton(VTexture *but, VTexture *gly)
{
	float gw,gh,gx,gy; // first glyph's width, height, center x,center y
	float bw,bh,bx,by; // first buton's width, height, center x,center y
	float dx,dy; // desired x, desired y

	gw = gly->twidth / 4.0f;
	gh = gly->theight;
	gy = gh / 2.0f;
	gx = gw / 2.0f;

	bw = but->twidth / 4.0f;
	bh = but->theight;
	by = bh / 2.0f;
	bx = bw / 2.0f;

	dx = bx - gx;
	dy = by - gy;

	for( float i=0;i<4;i++)
	{
		but->subimage(gly,gw*i,0,dx+(i*bw),dy,gw,gh);
	}
}

void GuiSkin::LoadComboBoxButton(VTexture *but)
{
	cbb_w = but->twidth / 4;
	cbb_h = but->theight;

	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 4;i++)
	{
		cbb_quad[i].GenerateUV(&basetex,nx+ (but->twidth/4)*i,ny,nx + (but->twidth/4)*(i+1),ny + but->theight);
	}
}

void GuiSkin::LoadTextBeam(VTexture *beam )
{
	if (beam->bytes != 0 && beam->glID == 0)
	{
		beam->gettexid();
	}
	tbeam_tex = beam->glID;
}

void GuiSkin::LoadProgressBar(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(but,0,0,nx,ny,but->twidth,but->theight);

	pbar_quad.sc_h[0] = mleft;
	pbar_quad.sc_h[1] = mright;
	pbar_quad.sc_h[2] = mtop;
	pbar_quad.sc_h[3] = mbottom;
	
	pbar_quad.GenerateUV(&basetex,nx,ny,nx+but->twidth,ny+but->theight);
}

void GuiSkin::LoadProgressResizingChunk(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(but,0,0,nx,ny,but->twidth,but->theight);

	pchk_quad.sc_h[0] = mleft;
	pchk_quad.sc_h[1] = mright;
	pchk_quad.sc_h[2] = mtop;
	pchk_quad.sc_h[3] = mbottom;
	
	pchk_quad.GenerateUV(&basetex,nx,ny,nx+but->twidth,ny+but->theight);
	presizing = true;
}

void GuiSkin::LoadProgressChunk(VTexture *but)
{
	/*if (but->bytes != 0 && but->glID == 0)
	{
		but->gettexid(GL_NEAREST,GL_NEAREST);
	}
	pchk_tex = but->glID;
	pchk_wid = but->twidth;*/
}

void GuiSkin::LoadMenuBackground(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(but,0,0,nx,ny,but->twidth,but->theight);	

	mnb_quad.sc_h[0] = mleft;
	mnb_quad.sc_h[1] = mright;
	mnb_quad.sc_h[2] = mtop;
	mnb_quad.sc_h[3] = mbottom;
	
	mnb_quad.GenerateUV(&basetex,nx,ny,nx+but->twidth,ny+but->theight);
}

void GuiSkin::LoadMenuItem(VTexture *but,long mleft,long mright,long mtop,long mbottom,long sep_top,long sep_bottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0; i<5 ;i++)
	{
		mni_quad[i].sc_h[0] = mleft;
		mni_quad[i].sc_h[1] = mright;
		mni_quad[i].sc_h[2] = mtop;
		mni_quad[i].sc_h[3] = mbottom;
		mni_quad[i].GenerateUV(&basetex,nx+(but->twidth/5)*i,ny,nx+(but->twidth/5)*(i+1),ny+but->theight);
	}
	mis_quad.GenerateUV(&basetex,nx+(but->twidth/5)*1,ny+sep_top,nx+(but->twidth/5)*2,ny+(but->theight-sep_bottom));
}

void GuiSkin::LoadMenuBarItem(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0; i<2 ;i++) // only make 2 of 5
	{
		mbi_quad[i].sc_h[0] = mleft;
		mbi_quad[i].sc_h[1] = mright;
		mbi_quad[i].sc_h[2] = mtop;
		mbi_quad[i].sc_h[3] = mbottom;
		mbi_quad[i].GenerateUV(&basetex,nx+(but->twidth/5)*i,ny,nx+(but->twidth/5)*(i+1),ny+but->theight);
	}
}

void GuiSkin::LoadToolbarBackground(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	basetex.subimage(but,0,0,nx,ny,but->twidth,but->theight);	

	tbr_quad.sc_h[0] = mleft;
	tbr_quad.sc_h[1] = mright;
	tbr_quad.sc_h[2] = mtop;
	tbr_quad.sc_h[3] = mbottom;
	
	tbr_quad.GenerateUV(&basetex,nx,ny,nx+but->twidth,ny+but->theight);
}

void GuiSkin::LoadToolbarButton(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0; i<5 ;i++) // only make 2 of 5
	{
		tbb_quad[i].sc_h[0] = mleft;
		tbb_quad[i].sc_h[1] = mright;
		tbb_quad[i].sc_h[2] = mtop;
		tbb_quad[i].sc_h[3] = mbottom;
		tbb_quad[i].GenerateUV(&basetex,nx+(but->twidth/5)*i,ny,nx+(but->twidth/5)*(i+1),ny+but->theight);
	}
}


void GuiSkin::LoadGroupBox(VTexture *but,long mleft,long mright,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0; i<2 ;i++)
	{
		gbx_quad[i].sc_h[0] = mleft;
		gbx_quad[i].sc_h[1] = mright;
		gbx_quad[i].sc_h[2] = mtop;
		gbx_quad[i].sc_h[3] = mbottom;
		gbx_quad[i].GenerateUV(&basetex,nx+(but->twidth/2)*i,ny,nx+(but->twidth/2)*(i+1),ny+but->theight);
	}
}


void GuiSkin::LoadRebarGrip(VTexture *but,long mtop,long mbottom)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;
	
	rbg_quad.sc_h[0] = mtop;
	rbg_quad.sc_h[1] = mbottom;
	rbg_quad.GenerateUV(&basetex,nx,ny,nx+but->twidth,ny+but->theight);
}

void GuiSkin::LoadTreeViewExpander(VTexture *but)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	tv_w = but->twidth/2;
	tv_h = but->theight;
	tv_oy = (16 / 2) - (tv_h/2);

	tv_quad[0].GenerateUV(&basetex,nx,ny,nx+tv_w,ny+tv_h);
	tv_quad[1].GenerateUV(&basetex,nx+tv_w,ny,nx+(but->twidth),ny+tv_h);
}

void GuiSkin::LoadColumnHeader ( VTexture *but,long mleft,long mright)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	lv_colh_h = but->theight;

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i< 5;i++)
	{
		lv_colheader[i].sc_h[0] = mleft;
		lv_colheader[i].sc_h[1] = mright;
		lv_colheader[i].GenerateUV(&basetex,nx+ (but->twidth/5)*i,ny,nx + (but->twidth/5)*(i+1),ny + but->theight);
	}

}

void GuiSkin::LoadWindowCloseButton(VTexture* but)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(but->twidth+2,but->theight+2);
	basetex.subimage(but,0,0,newquad->x+1,newquad->y+1,but->twidth,but->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	win_closeh = but->theight;
	win_closew = but->twidth/6;

	for (int i = 0;i< 6;i++)
	{
		win_close[i].GenerateUV(&basetex,nx+ (but->twidth/6)*i,ny,nx + (but->twidth/6)*(i+1),ny + but->theight);
	}
}

void GuiSkin::LoadHSliderTrackbar( VTexture *tex , int mleft,int mright)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);
	basetex.subimage(tex,0,0,newquad->x+1,newquad->y+1,tex->twidth,tex->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	htrbar.sc_h[0] = mleft;
	htrbar.sc_h[1] = mright;
	htrbar.GenerateUV(&basetex,nx,ny,nx + tex->twidth,ny + tex->theight);

	trbar_h = tex->theight;

	/*for (int i = 0;i< 6;i++)
	{
		win_close[i].GenerateUV(&basetex,nx+ (but->twidth/6)*i,ny,nx + (but->twidth/6)*(i+1),ny + but->theight);
	}*/

}

void GuiSkin::LoadVSliderTrackbar( VTexture *tex )
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);
	basetex.subimage(tex,0,0,newquad->x+1,newquad->y+1,tex->twidth,tex->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	vtrbar.GenerateUV(&basetex,nx,ny,nx + tex->twidth,ny + tex->theight);

	trbar_w = tex->twidth;
}

void GuiSkin::LoadHSliderThumb( VTexture* tex, int style )
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);
	basetex.subimage(tex,0,0,newquad->x+1,newquad->y+1,tex->twidth,tex->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i<5;i++)
	{
		hsthm[style][i].GenerateUV(&basetex,nx+ (tex->twidth/5)*i,ny,nx + (tex->twidth/5)*(i+1),ny + tex->theight);
	}

	h_sthm_w = tex->twidth/5;
	h_sthm_h = tex->theight;
}

void GuiSkin::LoadVSliderThumb( VTexture* tex, int style )
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tex->twidth+2,tex->theight+2);
	basetex.subimage(tex,0,0,newquad->x+1,newquad->y+1,tex->twidth,tex->theight);

	int nx,ny;
	nx = newquad->x+1;
	ny = newquad->y+1;

	for (int i = 0;i<5;i++)
	{
		vsthm[style][i].GenerateUV(&basetex,nx+ (tex->twidth/5)*i,ny,nx + (tex->twidth/5)*(i+1),ny + tex->theight);
	}

	v_sthm_w = tex->twidth / 5;
	v_sthm_h = tex->theight;
}


void GuiSkin::MakeDefault()
{
	DefaultSkin = this;
}