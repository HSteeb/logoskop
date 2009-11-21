# Microsoft Developer Studio Project File - Name="LosFile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LosFile - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LosFile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LosFile.mak" CFG="LosFile - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LosFile - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LosFile - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LosFile - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "LosFile - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LosFile - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Obj\Release\LosFile"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LosFile - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "../Obj/Debug/LosFile/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LosFile - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LosFile___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "LosFile___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseUnicode"
# PROP Intermediate_Dir "..\Obj\ReleaseUnicode\LosFile"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /D "XML_UNICODE_WCHAR_T" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LosFile - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LosFile___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "LosFile___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugUnicode"
# PROP Intermediate_Dir "..\Obj\DebugUnicode\LosFile"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /D "XML_UNICODE_WCHAR_T" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"../Obj/Debug/LosFile/LosFile.bsc"
# ADD BSC32 /nologo /o"../Obj/Debug/LosFile/LosFile.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "LosFile - Win32 Release"
# Name "LosFile - Win32 Debug"
# Name "LosFile - Win32 Release Unicode"
# Name "LosFile - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Losung Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\LosFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Losung.cpp
# End Source File
# Begin Source File

SOURCE=.\LosungObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LosungVector.cpp
# End Source File
# Begin Source File

SOURCE=.\Testimony.cpp
# End Source File
# End Group
# Begin Group "Load Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LosFileStack.cpp
# End Source File
# Begin Source File

SOURCE=..\util\win32filemap.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\util\xmlfile.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\util\XMLStack.cpp
# End Source File
# End Group
# Begin Group "Util Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\MemBuf.cpp
# End Source File
# Begin Source File

SOURCE=..\util\UTF8.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Losung Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\LosFile.h
# End Source File
# Begin Source File

SOURCE=.\Losung.h
# End Source File
# Begin Source File

SOURCE=.\LosungObject.h
# End Source File
# Begin Source File

SOURCE=.\LosungObjectVisitor.h
# End Source File
# Begin Source File

SOURCE=.\LosungVector.h
# End Source File
# Begin Source File

SOURCE=.\Testimony.h
# End Source File
# End Group
# Begin Group "Load Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\filemap.h
# End Source File
# Begin Source File

SOURCE=.\LosFileStack.h
# End Source File
# Begin Source File

SOURCE=..\util\xmlfile.h
# End Source File
# Begin Source File

SOURCE=..\util\XMLStack.h
# End Source File
# End Group
# Begin Group "Util Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\MemBuf.h
# End Source File
# Begin Source File

SOURCE=..\util\UTF8.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\LosFileConst.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
