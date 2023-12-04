# Microsoft Developer Studio Project File - Name="TGAtool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TGAtool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TGAtool.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP="F:\Program Files\Intel\Compiler50\ia32\bin\icl.exe"
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TGAtool - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32="F:\Program Files\Intel\iselect\bin\xilink6.exe"
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /Zp1 /MDd /W3 /GX /ZI /Od /I "F:\Program Files\Microsoft Visual Studio\VC98\STLport" /I "F:\Program Files\Intel\Compiler50\ia32\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\atl\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\mfc\include" /I "F:\Program Files\Microsoft Visual Studio\VC98\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FD /GZ /Qwd985 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32="F:\Program Files\Intel\iselect\bin\xilink6.exe"
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"F:\Program Files\Intel\Compiler50\ia32\lib" /libpath:"F:\Program Files\Microsoft Visual Studio\VC98\mfc\lib" /libpath:"F:\Program Files\Microsoft Visual Studio\VC98\lib"

!ENDIF 

# Begin Target

# Name "TGAtool - Win32 Release"
# Name "TGAtool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

# ADD CPP /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj"

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

# ADD CPP /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TGAtool.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

# ADD CPP /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj"

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

# ADD CPP /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TGAtool.rc

!IF  "$(CFG)" == "TGAtool - Win32 Release"

# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /d "_CONFIGCP_Win32_20Release" /d "_PRJCP_TGAtool_2Evprj"

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /d "_CONFIGCP_Win32_20Debug" /d "_PRJCP_TGAtool_2Evprj"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TGAtoolDlg.cpp

!IF  "$(CFG)" == "TGAtool - Win32 Release"

# ADD CPP /D "_CONFIGCP_Win32_20Release" /D "_PRJCP_TGAtool_2Evprj"

!ELSEIF  "$(CFG)" == "TGAtool - Win32 Debug"

# ADD CPP /D "_CONFIGCP_Win32_20Debug" /D "_PRJCP_TGAtool_2Evprj"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TGAtool.h
# End Source File
# Begin Source File

SOURCE=.\TGAtoolDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TGAtool.ico
# End Source File
# Begin Source File

SOURCE=.\res\TGAtool.rc2
# End Source File
# End Group
# End Target
# End Project
