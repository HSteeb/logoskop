# Microsoft Developer Studio Project File - Name="xmlparse" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xmlparse - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xmlparse.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xmlparse.mak" CFG="xmlparse - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xmlparse - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xmlparse - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xmlparse - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "xmlparse - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Misc/Losung/Win32/Xmlparse", LVCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xmlparse - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "../Obj/Release/xmlparse/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\xmltok" /D "NDEBUG" /D "XML_NS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "../Obj/Debug/xmlparse/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\xmltok" /D "_DEBUG" /D "_WINDOWS" /D "XML_NS" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "xmlparse___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "xmlparse___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugUnicode"
# PROP Intermediate_Dir "../Obj/DebugUnicode/xmlparse/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\xmltok" /D "_DEBUG" /D "_WINDOWS" /D "XML_NS" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\xmltok" /D "_DEBUG" /D "_WINDOWS" /D "XML_NS" /D "WIN32" /D "UNICODE" /D "_UNICODE" /D "XML_UNICODE_WCHAR_T" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "xmlparse___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "xmlparse___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseUnicode"
# PROP Intermediate_Dir "../Obj/ReleaseUnicode/xmlparse/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\xmltok" /D "NDEBUG" /D "XML_NS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\xmltok" /D "NDEBUG" /D "XML_NS" /D "WIN32" /D "_WINDOWS" /D "_LIB" /D "UNICODE" /D "_UNICODE" /D "XML_UNICODE_WCHAR_T" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "xmlparse - Win32 Release"
# Name "xmlparse - Win32 Debug"
# Name "xmlparse - Win32 Debug Unicode"
# Name "xmlparse - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\hashtable.c

!IF  "$(CFG)" == "xmlparse - Win32 Release"

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Debug"

# ADD CPP /MTd

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Debug Unicode"

!ELSEIF  "$(CFG)" == "xmlparse - Win32 Release Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xmlparse.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\hashtable.h
# End Source File
# Begin Source File

SOURCE=..\Xmltok\xmldef.h
# End Source File
# Begin Source File

SOURCE=.\xmlparse.h
# End Source File
# Begin Source File

SOURCE=..\Xmltok\xmlrole.h
# End Source File
# Begin Source File

SOURCE=..\Xmltok\xmltok.h
# End Source File
# End Group
# End Target
# End Project
