# Microsoft Developer Studio Project File - Name="Bidi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Bidi - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Bidi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bidi.mak" CFG="Bidi - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bidi - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Bidi - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Bidi - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "Bidi - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bidi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "../Obj/Release/Encoding/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Bidi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "../Obj/Debug/Encoding/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_LIB" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Bidi - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUnicode"
# PROP BASE Intermediate_Dir "ReleaseUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseUnicode"
# PROP Intermediate_Dir "..\Obj\ReleaseUnicode\Encoding"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Bidi - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugUnicode"
# PROP BASE Intermediate_Dir "DebugUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugUnicode"
# PROP Intermediate_Dir "../Obj/DebugUnicode/Encoding/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /FR"..\Obj\DebugUnicode\Bidi\\" /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Bidi - Win32 Release"
# Name "Bidi - Win32 Debug"
# Name "Bidi - Win32 Release Unicode"
# Name "Bidi - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "FriBidi Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fribidi.c
# End Source File
# Begin Source File

SOURCE=.\fribidi_char_sets.c
# End Source File
# Begin Source File

SOURCE=.\fribidi_get_type.c
# End Source File
# End Group
# Begin Group "Bidi Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bidi.cpp
# End Source File
# Begin Source File

SOURCE=.\bidiCharInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\bidiLigatures.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "FriBidi Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fribidi.h
# End Source File
# Begin Source File

SOURCE=.\fribidi_char_sets.h
# End Source File
# Begin Source File

SOURCE=.\fribidi_tables.i
# End Source File
# Begin Source File

SOURCE=.\fribidi_types.h
# End Source File
# Begin Source File

SOURCE=.\glib.h
# End Source File
# End Group
# Begin Group "Bidi Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bidi.h
# End Source File
# Begin Source File

SOURCE=.\bidiArabicShaping.h
# End Source File
# Begin Source File

SOURCE=.\bidiCharInfo.h
# End Source File
# Begin Source File

SOURCE=.\bidiLigatures.h
# End Source File
# Begin Source File

SOURCE=.\bidiLigTable.h
# End Source File
# Begin Source File

SOURCE=.\bidiShapeTable.h
# End Source File
# Begin Source File

SOURCE=.\bidiTransparent.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
