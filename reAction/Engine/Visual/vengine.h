
// reAction Visual Engine Class

#ifndef VENGINE_H
#define VENGINE_H

#include "../../StdAfx.h"
#include "ext/vextensions.h"


class VEngine
{
	DWORD				ctickc;
	int					fpsc;
public:

	int					rdWidth;
	int					rdHeight;
	int					FPS; // frames per second
	bool				active;	
	bool				fullscreen;

	HDC					hDC;
	HGLRC				hRC;	
	HWND				hWnd;
	HINSTANCE			hInstance;

	bool				MouseState[2]; // Stores Mouse Click State
	int					MouseX;
	int					MouseY;
	bool				KeyState[256]; // Stores Key State

	VEngine();

	bool				StartVEngine		(char* title,int width,int height,int bits,bool fullscr); // That will start visual engine
	void				KillVWindow			(GLvoid);
	int					WndProc				(HWND, UINT, WPARAM, LPARAM);
	int					InitializeVEngine	(GLvoid);
	void				ReSizeVScene		(GLsizei width, GLsizei height);
	int					GetFPS				();

	void				Enter2D				(); // LATER CHANGE THEM WITH MORE GOOD THING
	void				Exit2D				();

	bool				scissorstate;
	void				EScissor			(int x,int y,int width,int height);
	void				DScissor			();

	BOOL				DoEvents			();

	static void			(*MouseDown_Event)	(int,int,char); // x , y, button
	static void			(*MouseUp_Event)	(int,int,char); // x , y, button
	static void			(*KeyDown_Event)	(char); // This function will call when key down occurs
	static void			(*KeyUp_Event)		(char);	// This function will call when key up occurs
};

extern VEngine mEngine; // Engine Instance 

#endif