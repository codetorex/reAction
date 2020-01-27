
GImageList::GImageList()
{
	imagecount = 0;
}

void GImageList::CreateBuffer(int w,int h)
{
	image.createbuffer(w,h,GL_RGBA);
	image.gettexid(GL_NEAREST,GL_NEAREST);

	basequad.width = w;
	basequad.height = h;
	imagelistID = image.glID;
}

void GImageList::Precache()
{
	if ( imagecount == 0)
	{
		imagequads = new GuiQuad* [32];
		imagew = new int [32];
		imageh = new int [32];
	}
	else
	{
		int k;
		k = imagecount % 32; // bitmaskla yap bunu kasmasin aq
		if (k == 0)
		{
			GuiQuad** bos;
			int* bos1;
			int* bos2;
			bos = new GuiQuad * [imagecount+32];
			bos1 = new int [imagecount+32];
			bos2 = new int [imagecount+32];
			memcpy(bos,imagequads,4 * imagecount);
			memcpy(bos1,imagew,4*imagecount);
			memcpy(bos2,imageh,4*imagecount);
			delete [] imagequads;
			delete [] imagew;
			delete [] imageh;	
			imagequads = bos;
			imagew = bos1;
			imageh = bos2;
		}
	}
}

int GImageList::AddImage(VTexture* tx)
{
	VPackNode* newquad;
	newquad = basequad.InsertQuad(tx->twidth,tx->theight);
	image.subimage(tx,0,0,newquad->x,newquad->y,tx->twidth,tx->theight);
	image.updatetex();

	GuiQuad* bosquad;
	bosquad = new GuiQuad;
	bosquad->GenerateUV(&image,newquad->x,newquad->y,newquad->x+tx->twidth,newquad->y+tx->theight);
	
	Precache();
	imagequads[imagecount] = bosquad;
	imagew[imagecount] = tx->twidth;
	imageh[imagecount] = tx->theight;
	imagecount++;

	return imagecount-1;
}

void GImageList::render(int x,int y,int imgid,bool ld_idty)
{
	glBindTexture(GL_TEXTURE_2D,imagelistID);
	imagequads[imgid]->Render(x,y,imagew[imgid],imageh[imgid],ld_idty);
}