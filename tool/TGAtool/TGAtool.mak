# Microsoft Developer Studio Generated NMAKE File, Based on TGAtool.dsp
!IF "$(CFG)" == ""
CFG=TGAtool - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TGAtool - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TGAtool - Win32 Release" && "$(CFG)" != "TGAtool - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TGAtool.mak" CFG="TGAtool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TGAtool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TGAtool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP="F:\Program Files\Intel\Compiler50\ia32\bin\icl.exe"
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TGAtool - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TGAtool.exe"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TGAtool.obj"
	-@erase "$(INTDIR)\TGAtool.res"
	-@erase "$(INTDIR)\TGAtoolDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TGAtool.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\TGAtool.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TGAtool.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TGAtool.bsc" 
BSC32_SBRS= \
	
LINK32="F:\Program Files\Intel\iselect\bin\xilink6.exe"
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TGAtool.pdb" /machine:I386 /out:"$(OUTDIR)\TGAtool.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TGAtool.obj" \
	"$(INTDIR)\TGAtoolDlg.obj" \
	"$(INTDIR)\TGAtool.res"

"$(OUTDIR)\TGAtool.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TGAtool.exe"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TGAtool.obj"
	-@erase "$(INTDIR)\TGAtool.res"
	-@erase "$(INTDIR)\TGAtoolDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TGAtool.exe"
	-@erase "$(OUTDIR)\TGAtool.ilk"
	-@erase "$(OUTDIR)\TGAtool.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/Zp1 /MDd /W3 /GX /ZI /Od /I "F:\Program Files\Microsoft Visual Studio\VC98\STLport" /I "F:\Program Files\Intel\Compiler50\ia32\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\atl\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\mfc\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Qwd985 /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TGAtool.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TGAtool.bsc" 
BSC32_SBRS= \
	
LINK32="F:\Program Files\Intel\iselect\bin\xilink6.exe"
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TGAtool.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TGAtool.exe" /pdbtype:sept /libpath:"F:\Program Files\Intel\Compiler50\ia32\lib" /libpath:"F:\Program Files\Microsoft Visual Studio\VC98\mfc\lib" /libpath:"F:\Program Files\Microsoft Visual Studio\VC98\lib" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TGAtool.obj" \
	"$(INTDIR)\TGAtoolDlg.obj" \
	"$(INTDIR)\TGAtool.res"

"$(OUTDIR)\TGAtool.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TGAtool.dep")
!INCLUDE "TGAtool.dep"
!ELSE 
!MESSAGE Warning: cannot find "TGAtool.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TGAtool - Win32 Release" || "$(CFG)" == "TGAtool - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj" /Fp"$(INTDIR)\TGAtool.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

CPP_SWITCHES=/Zp1 /MDd /W3 /GX /ZI /Od /I "F:\Program Files\Microsoft Visual Studio\VC98\STLport" /I "F:\Program Files\Intel\Compiler50\ia32\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\atl\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\mfc\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Qwd985 /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TGAtool.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj" /Fp"$(INTDIR)\TGAtool.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TGAtool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

CPP_SWITCHES=/Zp1 /MDd /W3 /GX /ZI /Od /I "F:\Program Files\Microsoft Visual Studio\VC98\STLport" /I "F:\Program Files\Intel\Compiler50\ia32\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\atl\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\mfc\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Qwd985 /c 

"$(INTDIR)\TGAtool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TGAtool.rc

!IF  "$(CFG)" == "TGAtool - Win32 Release"


"$(INTDIR)\TGAtool.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\TGAtool.res" /d "NDEBUG" /d "_CONFIGCP_Win32_20Release" /d "_PRJCP_TGAtool_2Evprj" $(SOURCE)


!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"


"$(INTDIR)\TGAtool.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\TGAtool.res" /d "_DEBUG" /d "_AFXDLL" /d "_CONFIGCP_Win32_20Debug" /d "_PRJCP_TGAtool_2Evprj" $(SOURCE)


!ENDIF 

SOURCE=.\TGAtoolDlg.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj" /Fp"$(INTDIR)\TGAtool.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TGAtoolDlg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

CPP_SWITCHES=/Zp1 /MDd /W3 /GX /ZI /Od /I "F:\Program Files\Microsoft Visual Studio\VC98\STLport" /I "F:\Program Files\Intel\Compiler50\ia32\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\atl\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\mfc\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Qwd985 /c 

"$(INTDIR)\TGAtoolDlg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

