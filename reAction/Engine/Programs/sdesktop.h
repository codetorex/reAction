
#ifndef SDESKTOP_H
#define SDESKTOP_H

class EditorMode
{
public:
	GMenuBar mainmenu;
	GToolbar maintoolbar;

};

namespace SystemDesktop
{
	GImageList* menuilist;
	//GImageList menuilist;
	GMenuBar desktopmenubar;
	GMenu applications;
	GMenu settings;
	GImage menubg;
	GWindow* scene_exp;

	void SceneExplorer_Click()
	{
		scene_exp->visible = true;
	}

	void InitializeObjects()
	{
		VTexture bostex;


		

		desktopmenubar.LoadHandle(24,0,mEngine.rdWidth,18,NULL);
		desktopmenubar.pid = &mDesktop;

		desktopmenubar.SetRelative();

		mDesktop.AddObj(&desktopmenubar);
		
		applications.LoadHandle(0,0,4,4,NULL);

		bostex.loadtex("../data/icons/system-file-manager.tex"); // rcb editor
		applications.AddItem("reAction Cabinet Editor",menuilist->AddImage(&bostex),0,menuilist);

		bostex.loadtex("../data/icons/applications-graphics.tex"); // texture editor
		applications.AddItem("reAction Texture Editor",menuilist->AddImage(&bostex),0,menuilist);

		bostex.loadtex("../data/icons/utilities-terminal.tex"); // terminal
		applications.AddItem("Terminal",menuilist->AddImage(&bostex),0,menuilist);

		bostex.loadtex("../data/icons/document-properties.tex"); // terminal
		applications.AddItem("Scene Info",menuilist->AddImage(&bostex),SceneExplorer_Click,menuilist);

		desktopmenubar.AddItem("Applications",0,&applications);
		bostex.loadbmp("../data/menubar_bg.bmp"); // menu background
		
		menubg.LoadHandle(0,0,mEngine.rdWidth,mEngine.rdHeight,0);
		menubg.PrepareImage(&bostex);
		mDesktop.AddObj(&menubg);

		mDesktop.ActivateObject(&desktopmenubar);

		mDesktop.ContainerUpdate();

	};


};


#endif