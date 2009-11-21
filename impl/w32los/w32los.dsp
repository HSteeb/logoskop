# Microsoft Developer Studio Project File - Name="w32los" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=W32LOS - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "w32los.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "w32los.mak" CFG="W32LOS - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "w32los - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "w32los - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "w32los - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "../obj/Release/w32los/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU" /d "AFX_TARG_NEU" /d "AFX_TARG_NEUD"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"..\Release/Logoskop.exe"

!ELSEIF  "$(CFG)" == "w32los - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\debug"
# PROP Intermediate_Dir "../obj/debug/w32los/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU" /d "AFX_TARG_NEU" /d "AFX_TARG_NEUD"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\debug/Logoskop.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "w32los - Win32 Release"
# Name "w32los - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "App Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LosDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\w32los.cpp
# End Source File
# Begin Source File

SOURCE=.\w32losDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\w32losView.cpp
# End Source File
# End Group
# Begin Group "Util Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\AppPathManager.cpp
# End Source File
# Begin Source File

SOURCE=..\util\Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\util\DateUtil.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\util\dspplain.cpp
# End Source File
# Begin Source File

SOURCE=..\util\Encoding.cpp
# End Source File
# Begin Source File

SOURCE=..\util\FileManager.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=..\util\FileUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\LangNames.cpp
# End Source File
# Begin Source File

SOURCE=..\util\LineDrawer.cpp
# End Source File
# Begin Source File

SOURCE=..\util\LosungDrawer.cpp
# End Source File
# Begin Source File

SOURCE=..\util\PathUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\util\Profile.cpp
# End Source File
# Begin Source File

SOURCE=..\util\StringUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\util\utf8.cpp
# End Source File
# Begin Source File

SOURCE=..\util\VersionInfo.cpp
# End Source File
# End Group
# Begin Group "Messages Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\messages\MsgSingleModule.cpp
# End Source File
# End Group
# Begin Group "FileDesc Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\filedesc\DirectoryFileDescReader.cpp

!IF  "$(CFG)" == "w32los - Win32 Release"

!ELSEIF  "$(CFG)" == "w32los - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDesc.cpp
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescList.cpp
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescListBoxHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescParser.cpp
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdInfoList.cpp
# End Source File
# End Group
# Begin Group "Online Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\online\DownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\online\ErrorMsgFormatter.cpp
# End Source File
# Begin Source File

SOURCE=..\online\LangPackDlg.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=..\online\OrganizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\online\PostPairDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\online\ProxyDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\online\ProxySession.cpp
# End Source File
# Begin Source File

SOURCE=..\online\ServerDataCache.cpp
# End Source File
# Begin Source File

SOURCE=..\online\UserPassDlg.cpp
# End Source File
# End Group
# Begin Group "Signature Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SignatureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SignatureFormatter.cpp
# End Source File
# Begin Source File

SOURCE=.\SignatureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SignatureTemplateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SignatureWriter.cpp
# End Source File
# End Group
# Begin Group "Layout Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\Layout.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutController.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutDlgCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutIO.cpp
# End Source File
# Begin Source File

SOURCE=.\MockLayoutDlg.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\w32los.rc
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "App Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\LosDoc.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\w32los.h
# End Source File
# Begin Source File

SOURCE=.\w32losDoc.h
# End Source File
# Begin Source File

SOURCE=.\w32losView.h
# End Source File
# End Group
# Begin Group "Util Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\AppPathManager.h
# End Source File
# Begin Source File

SOURCE=..\util\Clipboard.h
# End Source File
# Begin Source File

SOURCE=..\Util\DateUtil.h
# End Source File
# Begin Source File

SOURCE=..\util\dspplain.h
# End Source File
# Begin Source File

SOURCE=..\util\Encoding.h
# End Source File
# Begin Source File

SOURCE=..\util\FileManager.h
# End Source File
# Begin Source File

SOURCE=..\util\FileUtil.h
# End Source File
# Begin Source File

SOURCE=..\util\ILineDrawer.h
# End Source File
# Begin Source File

SOURCE=..\util\IProfile.h
# End Source File
# Begin Source File

SOURCE=.\LangNames.h
# End Source File
# Begin Source File

SOURCE=..\util\LineDrawer.h
# End Source File
# Begin Source File

SOURCE=..\util\LosungDrawer.h
# End Source File
# Begin Source File

SOURCE=..\util\PathUtil.h
# End Source File
# Begin Source File

SOURCE=..\util\Profile.h
# End Source File
# Begin Source File

SOURCE=..\util\ProfileDef.h
# End Source File
# Begin Source File

SOURCE=..\util\StringUtil.h
# End Source File
# Begin Source File

SOURCE=..\util\UTF8.h
# End Source File
# Begin Source File

SOURCE=..\util\VersionInfo.h
# End Source File
# End Group
# Begin Group "Messages Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\messages\MsgSingleModule.h
# End Source File
# End Group
# Begin Group "FileDesc Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\filedesc\DirectoryFileDescReader.h
# End Source File
# Begin Source File

SOURCE=..\online\ServerDataCache.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDesc.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescList.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescListBoxHelper.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdFileDescParser.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdInfo.h
# End Source File
# Begin Source File

SOURCE=..\filedesc\TwdInfoList.h
# End Source File
# End Group
# Begin Group "Online Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\online\DownloadDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\ErrorMsgFormatter.h
# End Source File
# Begin Source File

SOURCE=..\online\IAppInterface.h
# End Source File
# Begin Source File

SOURCE=..\online\IErrorMsgFormatter.h
# End Source File
# Begin Source File

SOURCE=..\online\IUserPassDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\LangPackDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\OrganizeDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\PostPairDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\ProxyDlg.h
# End Source File
# Begin Source File

SOURCE=..\online\ProxySession.h
# End Source File
# Begin Source File

SOURCE=..\online\UserPassDlg.h
# End Source File
# End Group
# Begin Group "Signature Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SignatureDlg.h
# End Source File
# Begin Source File

SOURCE=.\SignatureFormatter.h
# End Source File
# Begin Source File

SOURCE=.\SignatureManager.h
# End Source File
# Begin Source File

SOURCE=.\SignatureTemplateDlg.h
# End Source File
# Begin Source File

SOURCE=.\SignatureWriter.h
# End Source File
# End Group
# Begin Group "Layout Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\ILayout.h
# End Source File
# Begin Source File

SOURCE=.\ILayoutDlg.h
# End Source File
# Begin Source File

SOURCE=..\util\Layout.h
# End Source File
# Begin Source File

SOURCE=.\LayoutController.h
# End Source File
# Begin Source File

SOURCE=.\LayoutDlg.h
# End Source File
# Begin Source File

SOURCE=.\LayoutDlgCmd.h
# End Source File
# Begin Source File

SOURCE=.\LayoutIO.h
# End Source File
# Begin Source File

SOURCE=.\MockLayoutDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\computer.ico
# End Source File
# Begin Source File

SOURCE=.\res\copy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copypres.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FontProperties.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fontpropertiesPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\paste.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PasteInsensitive.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PostPair.ico
# End Source File
# Begin Source File

SOURCE=.\res\w32los.rc2
# End Source File
# Begin Source File

SOURCE=.\res\www.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
