// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"WS2_32.LIB")

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS 0x0410
#define WINVER 0x0400

#include <windows.h>

#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>
#include <gl\glext.h>
#include <gl\wglext.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <winsock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	dword;

#define PI			3.1415926535897f
#define HALFPI		1.5707963267948f

#define RADTODEG(rad)		((rad) * (180.0f / PI))
#define DEGTORAD(deg)		((deg) * (PI / 180.0f))

#define MOD_32 0x1F // 31

//#define NETWORK_SYSTEM


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
