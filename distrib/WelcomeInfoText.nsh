;  $Id: $
; HS 2004-11-01:
; This file defines WELCOME_INFO_TEXT strings, taken from the files "Modern UI/Language files/*.nsh",
;   with the text between the first and second \r\n\r\n sequence ("close all apps...") dropped,
;   because w32los does not overwrite any system data.
;
; We cannot simply define all LANGSTRING for any language here,
; because for languages XX for which !insertmacro MUI_LANGUAGE "XX" has not been issued
; at the location where this file is !included,
; the ${LANG_XX} below will yield the value 0 and lead to error messages.
; To avoid such error messages,
; we must check here whether the respective language has been included.
; How this is done: check !define set in the *.nsh files.
;
; E.g. Albanian.nsh contains 
;
;   !insertmacro MUI_LANGUAGEFILE_BEGIN "Albanian"
;
; and System.nsh contains
;
;  !macro MUI_LANGUAGEFILE_BEGIN LANGUAGE
;    ...
;    !ifndef "MUI_LANGUAGEFILE_${LANGUAGE}_USED"
;      !define "MUI_LANGUAGEFILE_${LANGUAGE}_USED"
;    !endif
;
; Note: obviously, !define and !ifdef are case insensitive, i.e. after 
;   !define "MUI_LANGUAGEFILE_Albanian_USED"
; the condition
;   !ifdef MUI_LANGUAGEFILE_ALBANIAN_USED
; matches.


!ifdef MUI_LANGUAGEFILE_ALBANIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ALBANIAN} "Ky do t'ju udhлheqл gjatл instalimit tл $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ARABIC_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ARABIC} "УнУЗЪПЯ еРЗ ЗбгСФП Эн КдХнИ $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_BULGARIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_BULGARIAN} "Той ще инсталира $(^NameDA) на вашия компютър.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CATALAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CATALAN} "Aquest assistent el guiarа durant el procйs d'instal·laciу de $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CROATIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CROATIAN} "Instalacija programa $(^NameDA) na Vaљe raиunalo sastoji se od nekoliko jednostavnih koraka kroz koje жe Vas provesti ovaj иarobnjak.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CZECH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CZECH} "Tento prщvodce Vбs bude provбzet skrz instalaci $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_DANISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_DANISH} "Denne guide vil installere $(^NameDA) pе din computer.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_DEFAULT_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_DEFAULT} "This wizard will guide you through the installation of $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_DUTCH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_DUTCH} "Deze wizard zal $(^NameDA) op uw systeem installeren.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ENGLISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ENGLISH} "This wizard will guide you through the installation of $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ESTONIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ESTONIAN} "See abiline aitab paigaldada $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_FARSI_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_FARSI} "Знд ИСдЗге ФгЗ СЗ ПС дХИ $(^NameDA) СЗедгЗнн ОжЗеП СП.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_FINNISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_FINNISH} "Tдmд avustaja ohjaa sinut ohjelman $(^NameDA) asennuksen lдpi.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_FRENCH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_FRENCH} "Vous кtes sur le point d'installer $(^NameDA) sur votre ordinateur.\r\n\r\n"
!endif

!ifdef MUI_LANGUAGEFILE_GERMAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_GERMAN} "Dieser Assistent wird Sie durch die Installation von $(^NameDA) begleiten.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_GREEK_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_GREEK} "П пдзгьт бхфьт иб убт кбипдзгЮуей кбфЬ фз дйЬскейб фзт егкбфЬуфбузт фпх '$(^NameDA)'.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_HEBREW_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_HEBREW} "ащу жд йрзд аълн бодмк ддъчрд щм $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_HUNGARIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_HUNGARIAN} "A(z) $(^NameDA) telepнtйse kцvetkezik a szбmнtуgйpre.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_INDONESIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_INDONESIAN} "Setup Wizard akan membantu anda pada proses instalasi $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ITALIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ITALIAN} "Questo programma installerа $(^NameDA) nel vostro computer.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_JAPANESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_JAPANESE} "‚±‚МѓEѓBѓUЃ[ѓh‚НЃA$(^NameDA)‚МѓCѓ“ѓXѓgЃ[ѓ‹‚рѓKѓCѓh‚µ‚Д‚ў‚«‚Ь‚·ЃB\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_KOREAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_KOREAN} "АМ ЗБ·О±Ч·ҐАє ґзЅЕАЗ ДДЗ»ЕНїЎ $(^NameDA)${EUL_RUL} јіДЎЗТ °НАФґПґЩ.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_LATVIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LATVIAN} "Рis uzstвdорanas vednis jums palоdzзs veikt '$(^NameDA)' uzstвdорanu.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_LITHUANIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LITHUANIAN} "Рi programa jums padлs lengvai бdiegti $(^NameDA).\r\n\r\n"
!endif

!ifdef MUI_LANGUAGEFILE_LUXEMBOURGISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LUXEMBOURGISH} "Desen Assistent wдrt dech duech d'Installatioun vun $(^NameDA) begleeden.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_MACEDONIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_MACEDONIAN} "Овој програм ќе ве води низ инсталацијата на $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_MONGOLIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_MONGOLIAN} "$(^NameDA) суулгацын илбэчинг та шууд ашиглаж болно.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_NORWEGIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_NORWEGIAN} "Denne veiviseren vil lede deg gjennom installasjonen av $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_POLISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_POLISH} "Ten kreator pomoїe Ci zainstalowaж program $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_PORTUGUESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_PORTUGUESE} "Este assistente ajudб-lo-б durante a instalaзгo do $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_PORTUGUESEBR_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_PORTUGUESEBR} "Este assistente o guiarб durante a instalaзгo do $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ROMANIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ROMANIAN} "Aceasta aplicatie va instala produsului $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_RUSSIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_RUSSIAN} "Эта программа установит $(^NameDA) на ваш компьютер.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SERBIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SERBIAN} "Бићете вођени кроз процес инсталирања програма $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SERBIANLATIN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SERBIANLATIN} "Biжete voрeni kroz proces instaliranja programa $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SIMPCHINESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SIMPCHINESE} "ХвёцПтµјЅ«ЦёТэДгНкіЙ $(^NameDA) µД°ІЧ°ЅшіМЎЈ\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SLOVAK_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SLOVAK} "Tento sprievodca Vбs prevedie inљtalбciou $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SLOVENIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SLOVENIAN} "Ta иarovnik vam bo pomagal pri namestitvi $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SPANISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SPANISH} "Este programa instalarб $(^NameDA) en su ordenador.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SWEDISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SWEDISH} "Denna guide tar dig igenom installationen av $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_THAI_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_THAI} "µСЗµФґµСй§№ХйЁРµФґµСй§в»ГбЎГБ $(^NameDA) Е§є№¤НБѕФЗаµНГмўН§¤Ші\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_TRADCHINESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_TRADCHINESE} "іo­УєлЖF±N«ь¤Ю§A§№¦Ё $(^NameDA) Єє¦wёЛ¶iµ{ЎC\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_TURKISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_TURKISH} "Bu sihirbaz size $(^NameDA) kurulumu boyunca rehberlik edecektir.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_UKRAINIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_UKRAINIAN} "Дана установка встановить $(^NameDA) на Ваш комп'ютер.\r\n\r\n$_CLICK"
!endif
