# Microsoft Developer Studio Generated NMAKE File, Based on Exporter.dsp
!IF "$(CFG)" == ""
CFG=Exporter - Win32 Hybrid
!MESSAGE No configuration specified. Defaulting to Exporter - Win32 Hybrid.
!ENDIF 

!IF "$(CFG)" != "Exporter - Win32 Release" && "$(CFG)" != "Exporter - Win32 Debug" && "$(CFG)" != "Exporter - Win32 Hybrid"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Exporter - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\im3exp.dle"


CLEAN :
	-@erase "$(INTDIR)\dllentry.obj"
	-@erase "$(INTDIR)\Exporter.obj"
	-@erase "$(INTDIR)\Exporter.pch"
	-@erase "$(INTDIR)\exporter.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\im3exp.dle"
	-@erase "$(OUTDIR)\im3exp.exp"
	-@erase "$(OUTDIR)\im3exp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\exporter.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Exporter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\im3exp.pdb" /machine:I386 /def:".\exporter.def" /out:"$(OUTDIR)\im3exp.dle" /implib:"$(OUTDIR)\im3exp.lib" /libpath:"..\lib" 
DEF_FILE= \
	".\exporter.def"
LINK32_OBJS= \
	"$(INTDIR)\dllentry.obj" \
	"$(INTDIR)\Exporter.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\exporter.res"

"$(OUTDIR)\im3exp.dle" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Exporter - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\$(MAX_ROOT)\plugins\myexporter.dle"


CLEAN :
	-@erase "$(INTDIR)\dllentry.obj"
	-@erase "$(INTDIR)\Exporter.obj"
	-@erase "$(INTDIR)\Exporter.pch"
	-@erase "$(INTDIR)\exporter.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\myexporter.exp"
	-@erase "$(OUTDIR)\myexporter.lib"
	-@erase "$(OUTDIR)\myexporter.pdb"
	-@erase ".\$(MAX_ROOT)\plugins\myexporter.dle"
	-@erase ".\$(MAX_ROOT)\plugins\myexporter.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\exporter.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Exporter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\myexporter.pdb" /debug /machine:I386 /def:".\exporter.def" /out:"$(MAX_ROOT)\plugins\myexporter.dle" /implib:"$(OUTDIR)\myexporter.lib" /pdbtype:sept /libpath:"$(MAXSDK_HOME)\lib" 
DEF_FILE= \
	".\exporter.def"
LINK32_OBJS= \
	"$(INTDIR)\dllentry.obj" \
	"$(INTDIR)\Exporter.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\exporter.res"

".\$(MAX_ROOT)\plugins\myexporter.dle" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Exporter - Win32 Hybrid"

OUTDIR=.\Hybrid
INTDIR=.\Hybrid

ALL : ".\Release\im3exp.dle"


CLEAN :
	-@erase "$(INTDIR)\dllentry.obj"
	-@erase "$(INTDIR)\Exporter.obj"
	-@erase "$(INTDIR)\Exporter.pch"
	-@erase "$(INTDIR)\exporter.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\im3exp.exp"
	-@erase "$(OUTDIR)\im3exp.lib"
	-@erase "$(OUTDIR)\im3exp.pdb"
	-@erase ".\Release\im3exp.dle"
	-@erase ".\release\im3exp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\exporter.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Exporter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib core.lib maxutil.lib bmm.lib geom.lib mesh.lib tessint.lib paramblk2.lib comctl32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\im3exp.pdb" /debug /machine:I386 /def:".\exporter.def" /out:"release/im3exp.dle" /implib:"$(OUTDIR)\im3exp.lib" /pdbtype:sept /libpath:"$(MAXSDK_HOME)\lib" 
DEF_FILE= \
	".\exporter.def"
LINK32_OBJS= \
	"$(INTDIR)\dllentry.obj" \
	"$(INTDIR)\Exporter.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\exporter.res"

".\Release\im3exp.dle" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Exporter.dep")
!INCLUDE "Exporter.dep"
!ELSE 
!MESSAGE Warning: cannot find "Exporter.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Exporter - Win32 Release" || "$(CFG)" == "Exporter - Win32 Debug" || "$(CFG)" == "Exporter - Win32 Hybrid"
SOURCE=.\dllentry.cpp

"$(INTDIR)\dllentry.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Exporter.pch"


SOURCE=.\Exporter.cpp

"$(INTDIR)\Exporter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Exporter.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Exporter - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Exporter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Exporter - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Exporter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Exporter - Win32 Hybrid"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "$(MAXSDK_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXPORTER_EXPORTS" /Fp"$(INTDIR)\Exporter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Exporter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\exporter.rc

"$(INTDIR)\exporter.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

