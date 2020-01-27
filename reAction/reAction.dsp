# Microsoft Developer Studio Project File - Name="reAction" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=reAction - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "reAction.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "reAction.mak" CFG="reAction - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "reAction - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "reAction - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "reAction - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41f /d "NDEBUG"
# ADD RSC /l 0x41f /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 OpenGL32.lib GLu32.Lib GLaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "reAction - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41f /d "_DEBUG"
# ADD RSC /l 0x41f /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenGL32.lib GLu32.Lib GLaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "reAction - Win32 Release"
# Name "reAction - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\reAction.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Group "Visual"

# PROP Default_Filter ""
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gbutton.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gcheckbox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gcolumnheader.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gcombobox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gdesktop.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\ggroupbox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gimage.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gimagelist.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\glistbox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\glistitem.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\glistview.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gmenu.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gmenubar.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gmenubaritem.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gmenuitem.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gpanel.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gprogressbar.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gscrollbararea.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gscrollbarh.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gscrollbarv.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gsliderh.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gsplitcontainer.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gtextbox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gtoolbar.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gtoolbarbutton.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gtreenode.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gtreeview.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\guiquad.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\guiskin.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\guitools.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\gwindow.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\objectcontainer.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\objecthandle.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\vfont.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\vgui.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\GUI\vgui.h
# End Source File
# End Group
# Begin Group "EXT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Visual\EXT\GL_2_0.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\EXT\GL_ARB_multitexture.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\EXT\GL_ARB_vertex_program.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\EXT\GL_EXT_texture3D.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\EXT\vextensions.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\EXT\vextensions.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Engine\Visual\vcamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vcamera.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vcolor.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vengine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vengine.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vmaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vmaterial.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vmodel.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vmodel.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vradiosity.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vradiosity.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vshader.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vshader.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vskybox.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vskybox.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vterrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vterrain.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vtexture.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Visual\vtexture.h
# End Source File
# End Group
# Begin Group "Mathematics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Mathematics\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mcone.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mcone.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mcylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mcylinder.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mfixedpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mfixedpoint.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mfrustum.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mfrustum.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mmatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mmatrix.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mplane.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mplane.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mquaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mquaternion.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mray.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mray.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\msphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\msphere.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mtriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mtriangle.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mvector.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Mathematics\mvector.h
# End Source File
# End Group
# Begin Group "Physics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Physics\pspace.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Physics\pspace.h
# End Source File
# End Group
# Begin Group "Chemistry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Chemistry\celements.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Chemistry\celements.h
# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Tools\rcabinet.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rcabinet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rconsole.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rconsole.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rfilesystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rfilesystem.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rtools.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Tools\rtools.h
# End Source File
# End Group
# Begin Group "Audio"

# PROP Default_Filter ""
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Network\nserver.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Network\nserver.h
# End Source File
# End Group
# Begin Group "Programs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Programs\sceneinfo.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Programs\sdesktop.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\oldcode.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
