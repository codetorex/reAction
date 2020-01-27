# Microsoft Developer Studio Project File - Name="Exporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Exporter - Win32 Hybrid
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Exporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Exporter.mak" CFG="Exporter - Win32 Hybrid"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Exporter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Exporter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Exporter - Win32 Hybrid" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Exporter"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Exporter - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /machine:I386 /out:"Release/im3exp.dle" /libpath:"..\lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Exporter - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /debug /machine:I386 /out:"debug\myexporter.dle" /pdbtype:sept /libpath:"$(MAXSDK_HOME)\lib"

!ELSEIF  "$(CFG)" == "Exporter - Win32 Hybrid"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Hybrid"
# PROP BASE Intermediate_Dir "Hybrid"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Hybrid"
# PROP Intermediate_Dir "Hybrid"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib patch.lib tessint.lib /nologo /dll /debug /machine:I386 /out:"..\plugin\myexporter.dle" /pdbtype:sept /libpath:"..\lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /debug /machine:I386 /out:"release/im3exp.dle" /pdbtype:sept /libpath:"$(MAXSDK_HOME)\lib"

!ENDIF 

# Begin Target

# Name "Exporter - Win32 Release"
# Name "Exporter - Win32 Debug"
# Name "Exporter - Win32 Hybrid"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "general"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tools.cpp
# End Source File
# End Group
# Begin Group "export"

# PROP Default_Filter ""
# Begin Group "im3scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exp_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_entity.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_im3write.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_light.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_physique.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_scene.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_texture.cpp
# End Source File
# End Group
# Begin Group "im3keyframe"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exp_keyframe.cpp
# End Source File
# End Group
# Begin Group "im3bone"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exp_bone.cpp
# End Source File
# Begin Source File

SOURCE=.\exp_imbwrite.cpp
# End Source File
# End Group
# Begin Group "calculations"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tangentspace.cpp
# End Source File
# Begin Source File

SOURCE=.\vertexlight.cpp
# End Source File
# Begin Source File

SOURCE=.\vertexshadow.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Exporter.cpp
# End Source File
# Begin Source File

SOURCE=.\exporter.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Array.h
# End Source File
# Begin Source File

SOURCE=.\dllentry.cpp
# End Source File
# Begin Source File

SOURCE=.\exporter.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\exporter.rc
# End Source File
# Begin Source File

SOURCE=.\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\lostcodes.txt
# End Source File
# End Group
# End Target
# End Project
