// Copyright (C) 2000 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
///////////////////////////////////////////////////////////////////////////////
// DLLENTRY.CPP
//
// DESCR:
//     MAX and Win32 DLL Exports
//
// CHANGE LOG:
//     12/99 : DY : Created
//		09/2001 : rjc : modifications to bring it up to 3ds max r4.2 spec
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exporter.h"

#define DLLEXPORT __declspec(dllexport)


///////////////////////////////////////////////////////////////////////////////
// globals
///////////////////////////////////////////////////////////////////////////////

HINSTANCE g_hInstance;
int g_controlsInit = FALSE;


BOOL WINAPI DllMain(HANDLE hModule,
                    ULONG ul_reason_for_call,
                    LPVOID lpvReserved) 
{
    // cache the DLL instance handle
    g_hInstance = (HINSTANCE)hModule;

    if (!g_controlsInit) {
        g_controlsInit = TRUE;
        InitCustomControls(g_hInstance); // init MAX custom controls
        InitCommonControls();            // init Win custom controls
    }

    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}


// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
DLLEXPORT const TCHAR * LibDescription() 
{ 
    return GetString(IDS_LIBDESC); 
}


// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
DLLEXPORT int LibNumberClasses() 
{
    return 1;
}


// For each plug-in class we claim this DLL provides
// (from LibNumberClasses above) we must have a ClassDesc.
DLLEXPORT ClassDesc* LibClassDesc(int i) 
{
    switch (i) {
    case 0: return GetSceneExportDesc();
    default: return 0;
    }
}


// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
DLLEXPORT ULONG LibVersion() 
{ 
    return VERSION_3DSMAX; 
}


// Utility fcn to get string from DLL string table
// (not exported)
TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (g_hInstance)
        return LoadString(g_hInstance, id, buf, sizeof(buf)) ? buf : NULL;

    return NULL;
}





