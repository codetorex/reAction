// reAction.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include <vld.h>
#include "Engine/Visual/vengine.h"
#include "Engine/Tools/rcabinet.h"
#include "Engine/Visual/vtexture.h"
#include "Engine/Tools/rtools.h"
#include "Engine/Tools/rfilesystem.h"
#include "Engine/Visual/vcolor.h"
#include "Engine/Visual/GUI/vgui.h"
#include "Engine/Visual/vmodel.h"
#include "Engine/Visual/vcamera.h"
#include "Engine/Visual/vradiosity.h"
#include "Engine/Mathematics/mvector.h"
#include "Engine/Mathematics/mquaternion.h"
#include "Engine/Visual/VSkyBox.h"
#include "Engine/Visual/ext/vextensions.h"
#include "Engine/Chemistry/celements.h"
#include "Engine/Physics/pspace.h"
#include "Engine/Visual/vterrain.h"
#include "Engine/Visual/vshader.h"
#include "Engine/Network/nserver.h"


using namespace RTools; 

//VTexture test,test2;
VFont		tfont;

VTexture	ttex;
//VTexture ttex2;

GuiSkin		tskin; // test skin
VCamera		cam;

RMDModel*	sktest; // skeletal test model
RMDModel*	walktest;

RMDModel*	maptest;


bool done=FALSE;


vec3 mfn;

PPlanet		earth; // test planet
VTerrain	terr;
VTexture	screenshot;
VSkyBox		tskybox;

#include "Engine/Programs/sdesktop.h"

VShader helloworld;
VRadiosity radtest;
int curplane = 0;

GWindow twin;
GImage timg;
GButton tbutdraw;

VRLinePart* spart = 0; // selected part
VRLightmap* tp;
VTexture* radtex;
VRFace* oha = 0;
vec3 pr;
tri2 tx;

int px,py;
ray3 rp;

void timg_MouseDown(int x,int y,int button)
{
	px = (int)(((float)x / 128.0f) * 1024.0f);
	py = (int)(((float)y / 128.0f) * 1024.0f);
	spart = tp->GetLinePart(px,py);
}

void PrepareGUI()
{
//	LoadCommonControls();

	GUIOptimize(&mEngine);
	tskin.basefont = &tfont;
	tskin.window_cac = false;

	// Skin Load Code Hea
	tskin.BeginCreatingSkin(512,512);
	tskin.CreateLoadWindowTexture(&ttex,256,256,"newskin/border_bottom.bmp","newskin/border_left.bmp","newskin/border_right.bmp","newskin/border_top.bmp",10,10,28,8,28,8,10,10,RGB(233,233,238));
	
	ttex.loadtex("newskin/buttons.tex");
	tskin.LoadButton(&ttex,6,6,5,5);
	//tskin.basetex.setpixel(1,1,RGBA(255,0,0,255));

	ttex.loadtex("newskin/checkbox_new.tex");
	tskin.LoadCheckBox(&ttex);

	ttex.loadbmp("newskin/scrollbar_buttons.bmp");
	tskin.LoadScrollButton(&ttex);

	ttex.loadbmp("newskin/scrollbar_back_v.bmp");
	tskin.LoadVScrollShaft(&ttex);

	ttex.loadbmp("newskin/scrollbar_v.bmp");
	tskin.LoadVScrollThumb(&ttex,8,8);
	
	ttex.loadbmp("newskin/scrollbar_back_h.bmp");
	tskin.LoadHScrollShaft(&ttex);

	ttex.loadbmp("newskin/scrollbar_h.bmp");
	tskin.LoadHScrollThumb(&ttex,8,8);

	ttex.loadbmp("newskin/sunk_edge.bmp");
	tskin.LoadSunkEdge(&ttex,3,3,3,3);

	ttex.loadtex("newskin/combo_box_button.tex");
	tskin.LoadComboBoxButton(&ttex);

	ttex.loadtex("cursors/IBeam.tex");
	tskin.LoadTextBeam(&ttex);

	ttex.loadtex("newskin/progress_back_XP_h.tex");
	tskin.LoadProgressBar(&ttex,3,3,3,3);

	ttex.loadtex("newskin/progress_shunk_XP_h.tex");
	tskin.LoadProgressResizingChunk(&ttex,2,2,3,3);

	ttex.loadtex("newskin/toolbar_buttons.tex");
	tskin.LoadToolbarButton(&ttex,4,4,4,4);

	ttex.loadtex("newskin/menu_back_alpha.tex");
	tskin.LoadMenuBackground(&ttex,22,4,4,4);

	ttex.loadtex("newskin/menu_item.tex");
	tskin.LoadMenuItem(&ttex,2,2,2,2,10,0);

	ttex.loadtex("newskin/menu_bar_button.tex");
	tskin.LoadMenuBarItem(&ttex,4,4,4,4);

	ttex.loadbmp("newskin/toolbar_background.bmp");
	tskin.LoadToolbarBackground(&ttex,1,1,2,1);

	ttex.loadtex("newskin/rebar_grip.tex");
	tskin.LoadRebarGrip(&ttex,3,3);

	ttex.loadbmp("newskin/tree_view.bmp");
	tskin.LoadTreeViewExpander(&ttex);

	ttex.loadbmp("newskin/group_box.bmp");
	tskin.LoadGroupBox(&ttex,2,4,16,4);

	ttex.loadbmp("newskin/list_view_headbar.bmp");
	tskin.LoadColumnHeader(&ttex,2,3);

	ttex.loadtex("newskin/but_close_i.tex");
	tskin.LoadWindowCloseButton(&ttex);

	ttex.loadbmp("newskin/track_bar_h.bmp");
	tskin.LoadHSliderTrackbar(&ttex,2,2);

	ttex.loadbmp("newskin/track_bar_v.bmp");
	tskin.LoadVSliderTrackbar(&ttex);
	
	ttex.loadtex("newskin/track_button_up.tex");
	tskin.LoadHSliderThumb(&ttex,0);

	ttex.loadtex("newskin/track_button_middle.tex");
	tskin.LoadHSliderThumb(&ttex,1);

	ttex.loadtex("newskin/track_button_down.tex");
	tskin.LoadHSliderThumb(&ttex,2);

	ttex.loadtex("newskin/track_button_left.tex");
	tskin.LoadVSliderThumb(&ttex,0);

	ttex.loadtex("newskin/track_button_center.tex");
	tskin.LoadVSliderThumb(&ttex,1);

	ttex.loadtex("newskin/track_button_right.tex");
	tskin.LoadVSliderThumb(&ttex,2);


	tskin.basetex.writebmp("selam.bmp");
	tskin.EndCreatingSkin();

	//50,86,109
	tskin.deffontclr[0] = 50;
	tskin.deffontclr[1] = 86;
	tskin.deffontclr[2] = 109;

	// Skin Load Code End

	tskin.MakeDefault();

	mDesktop.LoadHandle(0,0,mEngine.rdWidth,mEngine.rdHeight,NULL);
	SetFocused(&mDesktop);

//	SystemDesktop::menuilist = &timglst;
	//SystemDesktop::InitializeObjects();

	twin.LoadHandle(100,100,278,326,NULL);
	mDesktop.AddObj(&twin);
	twin.inactive = false;
	twin.setcaption("Test Application");

	tbutdraw.LoadHandle(5,265,70,29,NULL);
	twin.AddObj(&tbutdraw);
	tbutdraw.setcaption("TraceRay");

	mDesktop.ActivateObject(&twin);

}

void DrawGUI()
{
	// GUI MUST BE DRAWED IN 2D MODE
	mEngine.Enter2D();
	BeginGUI();
	
	//if (!mEngine.KeyState[VK_TAB])
	{
		mDesktop.CustomDrawGui();
		//desktop.CustomDrawGui();
	}

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(0,1,0);
	glLoadIdentity();
	float sxy[6];
	if (oha)
	{
		sxy[0] = timg.sx + (tx.v1.x);
		sxy[1] = timg.sy + (tx.v1.y);
		sxy[2] = timg.sx + (tx.v2.x);
		sxy[3] = timg.sy + (tx.v2.y);
		sxy[4] = timg.sx + (tx.v3.x);
		sxy[5] = timg.sy + (tx.v3.y);

		glBegin(GL_LINES);
			glVertex2fv(&sxy[0]);
			glVertex2fv(&sxy[2]);

			glVertex2fv(&sxy[2]);
			glVertex2fv(&sxy[4]);

			glVertex2fv(&sxy[4]);
			glVertex2fv(&sxy[0]);
		glEnd();
	}

	glColor3f(1.0f,1.0f,1.0f);
	tfont.PrintText(950,14,"FPS:%i",mEngine.GetFPS());

	glColor3f(0,0,0);
	tfont.PrintText(tbutdraw.sx + tbutdraw.w+5,tbutdraw.sy+11,"%i",px);
	tfont.PrintText(tbutdraw.sx + tbutdraw.w+5,tbutdraw.sy+25,"%i",py);

	

	EndGUI();
	mEngine.Exit2D();
}

BOOL DoEvents()
{
        MSG msg;
        
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
                if(msg.message==WM_QUIT)
                        return FALSE;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
        return TRUE;
}

float lp[4];



int DrawScene()
{	

	if ( mEngine.KeyState[VK_SHIFT]) cam.GetInput();

	if (mEngine.KeyState[VK_F7])
	{
		screenshot.screenshot("screenshot.bmp");
	}

	if (mEngine.KeyState[VK_F8])
	{
		curplane++;
		Sleep(40);
	}

	if (mEngine.KeyState[VK_F9])
	{
		curplane--;
		Sleep(40);
	}

//	curplane = curplane % radtest.fplanes.count;




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	cam.render();

	if (mEngine.MouseState[0])
	{
		rp.p.x = cam.xPos;
		rp.p.y = cam.yPos;
		rp.p.z = cam.zPos;

		rp.d.x = cam.xView;
		rp.d.y = cam.yView;
		rp.d.z = cam.zView;

		rp.d = rp.d - rp.p;

		oha = radtest.TestProjectRay(rp,pr,tx);
		tx.v1 = tx.v1 * 256.0f;
		tx.v2 = tx.v2 * 256.0f;
		tx.v3 = tx.v3 * 256.0f;
	}

	glShadeModel(GL_SMOOTH);
	
	glDisable(GL_DEPTH_TEST);
	tskybox.RenderSkyBox();
	glEnable(GL_DEPTH_TEST);
	
	/*float lp[3];
	lp[0] = cam.xPos;
	lp[1] = cam.yPos;
	lp[2] = cam.zPos;*/


	//if (myengin->MouseState[0])
	{
		lp[0] = cam.xPos;
		lp[1] = cam.yPos;
		lp[2] = cam.zPos;
	}

	/*if (myengin->MouseState[1])
	{
		lp[0] = sktest->lights[0].lightpos[0];
		lp[1] = sktest->lights[0].lightpos[1];
		lp[2] = sktest->lights[0].lightpos[2];
	}*/

	lp[3] = 1.0f;

	/*float lp[4];
	lp[0] = sktest->lights[0].lightpos[0] * sin( DEGTORAD( (GetTickCount()/10)) );
	lp[1] = sktest->lights[0].lightpos[1] ;
	lp[2] = sktest->lights[0].lightpos[2] * cos( DEGTORAD( (GetTickCount()/10))  );
	lp[3] = 1.0f;*/

	float yellowAmbientDiffuse[] = {0.8f, 0.8f, 0.3f, 1.0f};
	float specular[] = {0.8f, 0.8f, 0.8f , 1.0f};
	

	glLightfv(GL_LIGHT0,GL_POSITION,lp);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//glLightfv(GL_LIGHT0,GL_POSITION, sktest->lights[0].lightpos);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, yellowAmbientDiffuse);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, yellowAmbientDiffuse);

	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/
	//sktest->Render();
	//sktest->RenderSkeleton((GetTickCount()/40)%walktest->skeleton->keyend);
	//sktest->RenderSkeleton(frameid);
	//sktest->Render();
	//earth.Render();
	//terr.Render();
	//terr.RenderWith3DTexture();
	/*glUseProgram(helloworld.program);
	terr.RenderWithBaseTexture();
	glUseProgram(0);*/

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//maptest->Render();
	radtest.TestRender();
	//radtest.TestRenderLighting();

	/*if (spart)
	{
		spart->VisualizeRay(px);
	}*/

	if (oha)
	{	
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0,1,0);

		glBegin(GL_LINES);
			oha->triangle.v1.render();
			oha->triangle.v2.render();
			
			oha->triangle.v2.render();
			oha->triangle.v3.render();

			oha->triangle.v3.render();
			oha->triangle.v1.render();
			
			rp.p.render();
			pr.render();

			vec3 tmp;
			tmp = pr + (oha->fnor * 5.0f);
			pr.render();
			tmp.render();
		glEnd();
	}
	//radtest.TestRenderNormals();
	//radtest.TestRenderParts();

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f(0,1,0,0.5f);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,100,0);
	glVertex3f(100,100,0);
	glVertex3f(100,0,0);
	glEnd();

	glColor4f(1,0,0,1);
	VRPlanar* tst;
	tst = (VRPlanar*)(radtest.fplanes.pArray[curplane]);
	tst->Render();
	glDisable(GL_BLEND);

	glLineWidth(3);
	glColor4f(0,0,1,1);
	glBegin(GL_LINES);
		tst->lowcorner.render();
		tst->highcorner.render();
	glEnd();*/

	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);

	//sktest->RenderBaseSkeleton();
	//sktest->RenderAnimSkeleton();

glDisable(GL_CULL_FACE);

	DrawGUI();
	//desktop.DrawGui();

	SwapBuffers(mEngine.hDC);
	DoEvents();
	return true;
}


/*
la8er folks
 class HTTPServer: public NServer
{
public:
	void Received	(NSocket* s,NIO* lpIOContext);
	void Sent		(NSocket* s,NIO* lpIOContext);
};

void HTTPServer::Received(NSocket* s,NIO* lpIOContext)
{
	QuickInfo(lpIOContext->Buffer);
	
}



void servertest()
{
	HTTPServer mserver;

	mserver.StartServer("789");

}*/

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG msg;
	/*VMaterialParameter mp;
	mp.p_data.clr.lclr = RGB(64,128,255);
	
	QuickInfo("%i %i %i", mp.p_data.clr.bclr[0], mp.p_data.clr.bclr[1],mp.p_data.clr.bclr[2]);*/

	/*VMaterial tp;
	tp.LoadRMT("../data/skeleton/carptgry.rmt");
	tp.LoadRMT("../data/skeleton/eyeball.rmt");*/
	
//	tst6.memstatus();

	if (!mEngine.StartVEngine("ReAction 3D",1024,768,16,false))
	{
		return 0;
	}

	/*const byte* ap = glGetString(GL_VERSION);
	char* pa = (char*)ap;
	
	QuickInfo(pa);
	return 0;*/

	RFileSystem* mfs = GetFileSystem();
	mfs->MountFolder("../data/");
	mfs->SetDefaultSaveMount(mfs->MountFolder("../data/save/",P_READABLE | P_WRITABLE));

	// lets do this
	



/*	earth.Calculate();

	earth.Generate();*/

	earth.cam = &cam;



	//QuickInfo("%f %f %f",earth.circumference,earth.surfacearea,earth.gravity);

	/*TEcosystem* testecosystem = new TEcosystem;
	TEcosystemNode* curnode;

	curnode = testecosystem->AddNode(-10.0f,0.0f,0.0f);
	curnode->data->PrepareData(1);
	curnode->data->AddTexture(LoadBMP("terrain_underwater01.bmp"),1.0f);

	curnode = testecosystem->AddNode(0.0f,0.0f,0.0f);
	curnode->data->PrepareData(2);
	curnode->data->AddTexture(LoadBMP("terrain_grass02.bmp"),0.3f);
//	curnode->data->AddTexture(LoadBMP("terrain_grass03.bmp"),0.01f);
	curnode->data->AddTexture(LoadBMP("terrain_grass01.bmp"),0.7f); // son eklenen en yetkin olan

	curnode = testecosystem->AddNode(0.0f,35.0f,0.0f);
	curnode->data->PrepareData(1);
	curnode->data->AddTexture(LoadBMP("terrain_soil01.bmp"),1.0f);


	curnode = testecosystem->AddNode(8.0f,0.0f,0.0f);
	curnode->data->PrepareData(1);
	curnode->data->AddTexture(LoadBMP("terrain_snow01.bmp"),1.0f);
	
	curnode = testecosystem->AddNode(8.0f,45.0f,0.0f);
	curnode->data->PrepareData(1);
	curnode->data->AddTexture(LoadBMP("terrain_rock01.bmp"),1.0f);


	
	testecosystem->Create3DTexture();

	VTexture* bostx = new VTexture;
	bostx->loadbmp("y.bmp");

	terr.cam = &cam;

	// set ecosystem
	terr.ecosystem = testecosystem;
	
	terr.GenerateFromHeightMap(bostx);

	terr.SynthesizeBaseTexture();
	terr.LoadBaseTexture("testterra.bmp");

	helloworld.LoadShader("shaders/helloworld.rvs","shaders/helloworld.rfs");
	glUseProgram(0);*/




// *************	tmdl.LoadRMD("../data/construct3.RMD");

	sktest = LoadRMD("skeleton/skeleton_test3.rmd","skeleton/");
	walktest = LoadRMD("skeleton/walk_test.rmd","skeleton/",RMD_SKELETAL);
	sktest->skanim = walktest->skeleton; // for testing purposes only, later we will change the system

	maptest = LoadRMD("maptest/maptest.rmd","maptest/");

	//walktest->cameras[0].SetCamera(&cam);
	sktest->cameras[0].SetCamera(&cam);

/*	lp[0] = sktest->lights[0].lightpos[0];
	lp[1] = sktest->lights[0].lightpos[1];
	lp[2] = sktest->lights[0].lightpos[2];*/

	tfont.LoadFromSystem("Verdana",12);
	PrepareGUI();

//	cam.SetCamera(tmdl.oc[0].camera[0], tmdl.oc[0].camera[1], tmdl.oc[0].camera[2],tmdl.oc[0].target[0], tmdl.oc[0].target[1], tmdl.oc[0].target[2],0.0f, 1.0f, 0.0f);
	//cam.SetCamera(0,0,0,1,0,0,0.0f, 1.0f, 0.0f);


	tskybox.cam = &cam;
	tskybox.size = mv3f(1024,1024,1024);

	// SKYBOX SETTINGS
	tskybox.day[SKYBOX_UP].loadbmp("sky_day02_01up.bmp",true);
	tskybox.day[SKYBOX_DOWN].loadbmp("sky_day02_01dn.bmp",true);
	tskybox.day[SKYBOX_RIGHT].loadbmp("sky_day02_01rt.bmp",true);
	tskybox.day[SKYBOX_LEFT].loadbmp("sky_day02_01lf.bmp",true);
	tskybox.day[SKYBOX_FRONT].loadbmp("sky_day02_01ft.bmp",true);
	tskybox.day[SKYBOX_BACK].loadbmp("sky_day02_01bk.bmp",true);


	// END OF SKYBOX SETTINGS
	radtest.radenv.SetupEnvironment(*maptest,tskybox,cam);
	radtest.radenv.LoadHemicube("hemicube_front.bmp","hemicube_bottom.bmp");

	radtest.PrepareGeometry(256.0f / 700.0f);
	tp = (VRLightmap*)radtest.lightmaps.pArray[0];

	// dosyadan initial setupu okumak icim
	tp->sinitial->loadbmp("sinitial.bmp");	
	tp->is_ilc = true;
	//tp->stexture->loadbmp("sinitial.bmp");	

	// dosyadan radiosity okumak icin burayi kullanabiliriz
	tp->stexture->loadbmp("stexture.bmp");
	tp->stexture->gettexid();

	//radtest.radenv.Precache();

	//radtest.CalculateRadiosity();

	//tp->stexture->updatetex(true);
	//tp->stexture->writebmp("stexture2.bmp");
	

	screenshot.createbuffer(1024,768,GL_RGB);

	radtex = tp->sinitial;
	timg.LoadHandle(5,5,256,256,NULL);
	twin.AddObj(&timg);
	timg.PrepareImage(radtex);
	timg.w = 256;
	timg.h = 256;
	timg.onMouseDown = timg_MouseDown;


	//ShowCursor(false);

	
	while(!done)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if ( mEngine.active )
			{

				if (mEngine.KeyState[VK_ESCAPE])
				{
					done = true;
				}
				else
				{
					DrawScene();
				}
			}
		}
	}
	//ShowCursor(true);
	mEngine.KillVWindow();
	return (msg.wParam);
}

