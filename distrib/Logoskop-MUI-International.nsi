; Script generated by the HM NIS Edit Script Wizard.
; Modified by HS 2004-10-30 etc.
;
; HS 2004-11-01:
; lines marked with "; ADAPT" must be adapted for different exe versions or years!

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Logoskop"
!define PRODUCT_LINK_NAME "Das Wort (Logoskop)"
!define PRODUCT_SMFOLDER "$SMPROGRAMS\bible2.net Logoskop"
!define PRODUCT_VERSION "3.0.2" ; ADAPT
!define PRODUCT_PUBLISHER "Helmut Steeb"
!define PRODUCT_WEB_SITE "http://bible2.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\bible2.net Logoskop.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define LEGAL_TRADEMARKS   "Mozilla Public License Version 1.0; http://www.mozilla.org/MPL/\
The Initial Developer of the Original Code is Helmut Steeb."
!define LEGAL_COPYRIGHT "Helmut Steeb hs2010@bible2.net (insert current year)"; ADAPT
!define FILE_DESCRIPTION "Display The Word of Project Bible 2.0"



SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON   "idr_main.ico"
!define MUI_UNICON "idr_main.ico"
; HS 2004-11-01: both icons must have same set of sizes etc.
; It does not work to mix an own MUI_ICON with the standard MUI_UNICON (which has lots of sizes etc.)!
;!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
;!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"


; Language Selection Dialog Settings
; HS 2004-11-22: enabling language selection may make some users assume
;   that they select the text language - not the installation language...
; !define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
; !define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
; !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"



; Welcome page
; HS 2004-11-01: MUI_PAGE_WELCOME - "Es wird empfohlen, ... alle anderen Programme zu schlie�en" :-(((
; A) disable welcome page, by uncommenting "!insertmacro MUI_PAGE_WELCOME"
; B) modify text:
;   - define MUI_WELCOMEPAGE_TEXT, cf. Contrib\Modern UI\Readme.html 3. Pages/Page Settings/Welcome Page Settings)
;   - refer to language string $(WELCOME_INFO_TEXT) which is filled below
!define MUI_WELCOMEPAGE_TEXT $(WELCOME_INFO_TEXT)
!insertmacro MUI_PAGE_WELCOME

; HS 2004-11-01 Components page
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\Logoskop.exe"
; 2008-11-29: only one link to readme is available (?).
; => show DEreadme.htm for now (it contains a link to ENreadme.htm),
;    assuming there will be far more German users.
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\DEreadme.htm"
!define MUI_FINISHPAGE_NOREBOOTSUPPORT
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; Language files
; HS 2004-11-01: must be located behind other "insertmacro"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
;;; 
; --- Define language strings (MUI_LANGUAGE) ---

; Custom welcome text
; (reduced version of contrib/Modern UI/Languages files/*.nlf)
!include "WelcomeInfoText.nsh"

; Section names

; Lang defines:
; (HS 2004-12-24)
; see /windows/D/Programme/Microsoft Visual Studio/vc98/Include/winnt.h
LangString SEC_STARTMENU ${LANG_ENGLISH} "into start menu"
LangString SEC_STARTMENU ${LANG_GERMAN}  "Eintr�ge ins Startmen�"

LangString SEC_AUTOSTART ${LANG_ENGLISH} "into startup folder"
LangString SEC_AUTOSTART ${LANG_GERMAN}  "Eintrag in Autostart"

LangString SEC_DESKTOP   ${LANG_ENGLISH} "onto desktop"
LangString SEC_DESKTOP   ${LANG_GERMAN}  "Link auf Desktop"

; Section descriptions

LangString DESC_W32LOS    ${LANG_ENGLISH} "Installs the program and the text of The Word"
LangString DESC_W32LOS    ${LANG_GERMAN}  "Installiert das Programm und den Text von Das Wort"

LangString DESC_STARTMENU ${LANG_ENGLISH} "Puts Logoskop into start menu"
LangString DESC_STARTMENU ${LANG_GERMAN}  "Tr�gt Logoskop ins Startmen� ein"

LangString DESC_AUTOSTART ${LANG_ENGLISH} "Puts Logoskop into the startup folder, so that it starts automatically every time you start Windows"
LangString DESC_AUTOSTART ${LANG_GERMAN}  "Tr�gt Logoskop in den Autostart-Order ein, so dass es mit jedem Windows-Start automatisch ge�ffnet wird"

LangString DESC_DESKTOP ${LANG_ENGLISH} "Puts a link to Logoskop onto your desktop"
LangString DESC_DESKTOP ${LANG_GERMAN}  "Tr�gt einen Link auf Logoskop auf dem Desktop ein"




; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Logoskop_2011_Installer.exe"; ADAPT
InstallDir "$PROGRAMFILES\bible2.net Logoskop"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

  ; Version Information
  ; HS 2004-11-01: must add ".0" since VIProductVersion requires 4 numbers
  VIProductVersion "${PRODUCT_VERSION}.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${PRODUCT_NAME}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "${LEGAL_TRADEMARKS}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${PRODUCT_PUBLISHER}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "${LEGAL_COPYRIGHT}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${FILE_DESCRIPTION}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${PRODUCT_VERSION}"

Function .onInit
  ; Contrib\Modern UI\Readme.html:
  ; MUI_LANGDLL_ALWAYSSHOW
  ; Always show the language selection dialog, even if a language has been stored in the registry.
  ; The language stored in the registry will be selected by default.
  ; !define MUI_LANGDLL_ALWAYSSHOW

  ; Contrib\Modern UI\Readme.html:
  ; If you want the installer to display a language selection dialog (have a look at the MultiLanguage.nsi example),
  ; insert the MUI_LANGDLL_DISPLAY macro in the .onInit function
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "${PRODUCT_NAME}" SEC01
  SectionIn RO
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer

  ; HS 2004-12-07:
  ; Put files that may be locked (because already running) first,
  ; so that no other files have been overwritten if user leaves installation.
  ; (hope that works)

  File "exe\ENreadme.htm"
  File "exe\DEreadme.htm"
  File "exe\af_AfrikaanseNuweVertaling_2011.twd"; ADAPT
  File "exe\de_Schlachter2000_2011.twd"; ADAPT
  File "exe\en_EnglishStandardVersion_2011.twd"; ADAPT
  File "exe\es_ReinaValera1995_2011.twd"; ADAPT
  File "exe\fr_Segond21_2011.twd"; ADAPT
  File "exe\he_ModernHebrew2004_2011.twd"; ADAPT
  File "exe\hu_Karoli1990_2011.twd"; ADAPT
  File "exe\it_NuovaRiveduta1994_2011.twd"; ADAPT
  File "exe\ru_JubilaeumsBibel_2011.twd"; ADAPT
  File "exe\th_ThaiHolyBible1971_2011.twd"; ADAPT
  File "exe\sw_KiswahiliContemporaryVersion2006_2011.twd"; ADAPT
  File "exe\zh-Hant_ChineseUnionVersionTraditional_2011.twd"; ADAPT
  File "exe\pub.css"
  File "exe\Logoskop.exe"
  File "exe\Logoskop_ger.dll"
SectionEnd

Section "$(SEC_STARTMENU)" SEC02
  SetOverwrite ifnewer
  CreateDirectory "${PRODUCT_SMFOLDER}"
  CreateShortCut  "${PRODUCT_SMFOLDER}\${PRODUCT_LINK_NAME}.lnk" "$INSTDIR\Logoskop.exe"
  CreateShortCut  "${PRODUCT_SMFOLDER}\DEreadme.lnk" "$INSTDIR\DEreadme.htm"
  CreateShortCut  "${PRODUCT_SMFOLDER}\ENreadme.lnk" "$INSTDIR\ENreadme.htm"
  CreateShortCut  "${PRODUCT_SMFOLDER}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd


Section "$(SEC_AUTOSTART)" SEC03
  ; HS 2004-11-03:
  ; CR 005 "w32los starts twice on WindowsXP.sxw" -> pass "-autostart"
  CreateShortCut "$SMSTARTUP\${PRODUCT_LINK_NAME}.lnk" "$INSTDIR\Logoskop.exe" "-autostart" "$INSTDIR\Logoskop.exe" 0
SectionEnd

Section "$(SEC_DESKTOP)" SEC04
  SetOverwrite ifnewer
  CreateShortCut "$DESKTOP\${PRODUCT_LINK_NAME}.lnk" "$INSTDIR\Logoskop.exe"
SectionEnd


Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\Logoskop.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Logoskop.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; --- assign descriptions ---
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(DESC_W32LOS)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} $(DESC_STARTMENU)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} $(DESC_AUTOSTART)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} $(DESC_DESKTOP)
!insertmacro MUI_FUNCTION_DESCRIPTION_END


;Function un.onUninstSuccess
;  HideWindow
;  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) wurde erfolgreich deinstalliert."
;FunctionEnd

;Function un.onInit
;!insertmacro MUI_UNGETLANGUAGE
;  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "M�chten Sie $(^Name) und alle seine Komponenten deinstallieren?" IDYES +2
;  Abort
;FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\DEreadme.htm"
  Delete "$INSTDIR\ENreadme.htm"
  ; note: the following leaves the copy in APPDATA
  Delete "$INSTDIR\af_AfrikaanseNuweVertaling_2011.twd"; ADAPT
  Delete "$INSTDIR\de_Schlachter2000_2011.twd"; ADAPT
  Delete "$INSTDIR\en_EnglishStandardVersion_2011.twd"; ADAPT
  Delete "$INSTDIR\es_ReinaValera1995_2011.twd"; ADAPT
  Delete "$INSTDIR\fr_Segond21_2011.twd"; ADAPT
  Delete "$INSTDIR\he_ModernHebrew2004_2011.twd"; ADAPT
  Delete "$INSTDIR\hu_Karoli1990_2011.twd"; ADAPT
  Delete "$INSTDIR\it_NuovaRiveduta1994_2011.twd"; ADAPT
  Delete "$INSTDIR\ru_JubilaeumsBibel_2011.twd"; ADAPT
  Delete "$INSTDIR\th_ThaiHolyBible1971_2011.twd"; ADAPT
  Delete "$INSTDIR\sw_KiswahiliContemporaryVersion2006_2011.twd"; ADAPT
  Delete "$INSTDIR\zh-Hant_ChineseUnionVersionTraditional_2011.twd"; ADAPT
  Delete "$INSTDIR\pub.css"
  Delete "$INSTDIR\Logoskop.exe"
  Delete "$INSTDIR\Logoskop_ger.dll"

  Delete "${PRODUCT_SMFOLDER}\Uninstall.lnk"
  Delete "${PRODUCT_SMFOLDER}\${PRODUCT_LINK_NAME}.lnk"
  Delete "${PRODUCT_SMFOLDER}\DEreadme.lnk"
  Delete "${PRODUCT_SMFOLDER}\ENreadme.lnk"
  Delete "$SMSTARTUP\${PRODUCT_LINK_NAME}.lnk"
  Delete "$DESKTOP\${PRODUCT_LINK_NAME}.lnk"

  RMDir "${PRODUCT_SMFOLDER}"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  ; HS 2004-11-01:
  ; HM NIS Edit creates the following line (activated):
  ;  SetAutoClose true
  ; but this prevents uninstaller from showing final window -> commented it out.
SectionEnd
