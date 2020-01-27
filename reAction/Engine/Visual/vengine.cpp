
#include "StdAfx.h"
#include "vengine.h"
#include "GUI/vgui.h"

VEngine mEngine;

inline LONG WINAPI reActionWindowProc (HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return mEngine.WndProc( hWnd, uMsg, wParam, lParam );
}


VEngine::VEngine()
{
	active=TRUE;	
	fullscreen=FALSE;
	hDC=NULL;
	hRC=NULL;	
	hWnd=NULL;
	scissorstate = false;
}

bool VEngine::StartVEngine(char* title, int width, int height, int bits, bool fullscr) // Start Visual Engine
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;					
	DWORD		dwExStyle;			
	DWORD		dwStyle;		
	RECT		WindowRect;			
	WindowRect.left=(long)0;		
	WindowRect.right=(long)width;	
	WindowRect.top=(long)0;			
	WindowRect.bottom=(long)height;	
	rdWidth = width;
	rdHeight = height;
	fullscreen=fullscr;	

	hInstance			= GetModuleHandle(NULL);			
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;	
	wc.lpfnWndProc		= (WNDPROC) reActionWindowProc;			
	wc.cbClsExtra		= 0;								
	wc.cbWndExtra		= 0;								
	wc.hInstance		= hInstance;						
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);		
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground	= NULL;								
	wc.lpszMenuName		= NULL;						
	wc.lpszClassName	= "OpenGL";							

	if (!RegisterClass(&wc))							
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;										
	}
	
	if (fullscreen)									
	{
		DEVMODE dmScreenSettings;							
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);	
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;			
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","ReAction",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;								
			}
		}
	}

	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;							
		dwStyle=WS_POPUP;								
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;						
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	hWnd = GetDesktopWindow();
	if (!(hWnd=CreateWindowEx(	dwExStyle,							
								"OpenGL",					
								title,					
								dwStyle |						
								WS_CLIPSIBLINGS |				
								WS_CLIPCHILDREN,				
								0, 0,							
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,							
								NULL,					
								hInstance,						
								NULL)))							
	{
		KillVWindow();							
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;						
	}

	static	PIXELFORMATDESCRIPTOR pfd=			
	{
		sizeof(PIXELFORMATDESCRIPTOR),			
		1,										
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |					
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,							
		bits,									
		0, 0, 0, 0, 0, 0,						
		0,											
		0,											
		0,											
		0, 0, 0, 0,								
		16,											 
		0,									
		0,										
		PFD_MAIN_PLANE,							
		0,										
		0, 0, 0								
	};
	//hDC = CreateDC(0,0,0,0);
	//SetWindowPos(hWnd,GetDesktopWindow(),0,0,0,0,SWP_SHOWWINDOW);
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillVWindow();						
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillVWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillVWindow();							
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if (!(hRC=wglCreateContext(hDC)))			
	{
		KillVWindow();						
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillVWindow();						
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	ShowWindow(hWnd,SW_SHOW);					
//	SetForegroundWindow(hWnd);				
	SetFocus(hWnd);								
	ReSizeVScene(width, height);
	//SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_SHOWWINDOW | SWP_NOMOVE);
	
	if (!InitializeVEngine())								
	{
		KillVWindow();				
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}
	/*if (glewIsSupported("GL_VERSION_2_0"))
	{
		printf("Ready for OpenGL 2.0\n");
	}
	else 
	{
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}*/
	InitializeAllExtensions();

	return TRUE;
}

GLvoid VEngine::KillVWindow(GLvoid) // Kill Visual Window					
{
	if (fullscreen)									
	{
		ChangeDisplaySettings(NULL,0);			
		ShowCursor(TRUE);							
	}

	if (hRC)										
	{
		if (!wglMakeCurrent(NULL,NULL))			
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))					
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC))				
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;									
	}

	if (hWnd && !DestroyWindow(hWnd))				
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;									
	}

	if (!UnregisterClass("OpenGL",hInstance))		
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							
	}
}

GLvoid VEngine::ReSizeVScene(GLsizei width, GLsizei height)	 // Resize Visual Scene
{
	if (height==0)										
	{
		height=1;									
	}

	glViewport(0,0,width,height);			

	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100000.0f);

	glMatrixMode(GL_MODELVIEW);					
	glLoadIdentity();								
}

int VEngine::InitializeVEngine(GLvoid) // Intialize Visual Engine						
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);						
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			
	glClearDepth(1.0f);								
	glEnable(GL_DEPTH_TEST);					
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	


	return TRUE;									
}

int VEngine::WndProc( HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)		
{
	switch (uMsg)								
	{
		case WM_ACTIVATE:						
		{
			if (!HIWORD(wParam))				
			{
				active=TRUE;				
			}
			else
			{
				active=FALSE;					
			}

			return 0;						
		}

		case WM_SYSCOMMAND:					
		{
			switch (wParam)				
			{
				case SC_SCREENSAVE:				
				case SC_MONITORPOWER:		
				return 0;						
			}
			break;								
		}

		case WM_CLOSE:							
		{
			PostQuitMessage(0);				
			return 0;						
		}

		case WM_KEYDOWN:						
		{

		/*	if ( (*KeyDown_Event) != NULL )
			{
				(*KeyDown_Event)((char)wParam);
			}*/
			

			KeyState[wParam] = TRUE;			
			KeyDownHandler(wParam);
			return 0;						
		}

		case WM_KEYUP:							
		{
			KeyState[wParam] = FALSE;
			KeyUpHandler(wParam);
			/*if ( (*KeyUp_Event) != NULL )
			{
				(*KeyUp_Event)((char)wParam);
			}*/
			return 0;							
		}

		case WM_CHAR:							
		{
			//KeyState[wParam] = FALSE;
			KeyPressHandler(wParam);
			return 0;							
		}

		case WM_LBUTTONDOWN:
		{
			MouseState[0] = true;
			MouseDownHandler(MouseX,MouseY,0);

			//(*LMouseDown_Event)((char)wParam);
			return 0;
		}

		case WM_LBUTTONDBLCLK:
		{
			// intresting tool
			return 0;
		}

		case WM_LBUTTONUP:
		{
			MouseState[0] = false;
			MouseUpHandler(MouseX,MouseY,0);
			//(*LMouseUp_Event)((char)wParam);
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			MouseState[1] = true;
			MouseDownHandler(MouseX,MouseY,1);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			MouseState[1] = false;
			MouseUpHandler(MouseX,MouseY,1);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			MouseX = LOWORD(lParam);
			MouseY = HIWORD(lParam);
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			//zDelta = (int) wParam; // wheel rotation 
			//MouseX = LOWORD(lParam); // horizontal position of pointer 
			//MouseY = HIWORD(lParam); // vertical position of pointer 
			MouseWheelHandler(LOWORD(lParam),HIWORD(lParam),(signed short)(HIWORD(wParam)));
			return 0;
		}

		case WM_SIZE:							
		{
			ReSizeVScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;							
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int VEngine::GetFPS()
{
	fpsc++;
	DWORD y = GetTickCount();
	if (y - ctickc > 1000)
	{
		ctickc = y;
		FPS = fpsc;
		fpsc = 0;
	}
	return FPS;
}

void VEngine::Enter2D()
{
	glDisable(GL_DEPTH_TEST);						
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();									
	glLoadIdentity();									
	glOrtho(0,rdWidth,rdHeight,0,-1,1);		
	glMatrixMode(GL_MODELVIEW);						
	glPushMatrix();		
}

void VEngine::Exit2D()
{
	glMatrixMode(GL_PROJECTION);					
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);						
	glPopMatrix();									
	glEnable(GL_DEPTH_TEST);
}

void VEngine::EScissor( int x,int y,int width,int height )
{
	glScissor(x ,rdHeight-y-height ,width,height);
	glEnable(GL_SCISSOR_TEST);
	// end with glDisable(GL_SCISSOR_TEST);
}

void VEngine::DScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

BOOL VEngine::DoEvents()
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