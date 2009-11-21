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
LANGSTRING WELCOME_INFO_TEXT ${LANG_ALBANIAN} "Ky do t'ju udh�heq� gjat� instalimit t� $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ARABIC_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ARABIC} "������� ��� ������ �� ����� $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_BULGARIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_BULGARIAN} "��� �� ��������� $(^NameDA) �� ����� ��������.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CATALAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CATALAN} "Aquest assistent el guiar� durant el proc�s d'instal�laci� de $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CROATIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CROATIAN} "Instalacija programa $(^NameDA) na Va�e ra�unalo sastoji se od nekoliko jednostavnih koraka kroz koje �e Vas provesti ovaj �arobnjak.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_CZECH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_CZECH} "Tento pr�vodce V�s bude prov�zet skrz instalaci $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_DANISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_DANISH} "Denne guide vil installere $(^NameDA) p� din computer.\r\n\r\n$_CLICK"
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
LANGSTRING WELCOME_INFO_TEXT ${LANG_FARSI} "��� ������ ��� �� �� ��� $(^NameDA) �������� ����� ���.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_FINNISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_FINNISH} "T�m� avustaja ohjaa sinut ohjelman $(^NameDA) asennuksen l�pi.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_FRENCH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_FRENCH} "Vous �tes sur le point d'installer $(^NameDA) sur votre ordinateur.\r\n\r\n"
!endif

!ifdef MUI_LANGUAGEFILE_GERMAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_GERMAN} "Dieser Assistent wird Sie durch die Installation von $(^NameDA) begleiten.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_GREEK_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_GREEK} "� ������ ����� �� ��� ����������� ���� �� �������� ��� ������������ ��� '$(^NameDA)'.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_HEBREW_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_HEBREW} "��� �� ���� ���� ����� ������ �� $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_HUNGARIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_HUNGARIAN} "A(z) $(^NameDA) telep�t�se k�vetkezik a sz�m�t�g�pre.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_INDONESIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_INDONESIAN} "Setup Wizard akan membantu anda pada proses instalasi $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ITALIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ITALIAN} "Questo programma installer� $(^NameDA) nel vostro computer.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_JAPANESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_JAPANESE} "���̃E�B�U�[�h�́A$(^NameDA)�̃C���X�g�[�����K�C�h���Ă����܂��B\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_KOREAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_KOREAN} "�� ���α׷��� ����� ��ǻ�Ϳ� $(^NameDA)${EUL_RUL} ��ġ�� ���Դϴ�.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_LATVIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LATVIAN} "�is uzst�d��anas vednis jums pal�dz�s veikt '$(^NameDA)' uzst�d��anu.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_LITHUANIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LITHUANIAN} "�i programa jums pad�s lengvai �diegti $(^NameDA).\r\n\r\n"
!endif

!ifdef MUI_LANGUAGEFILE_LUXEMBOURGISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_LUXEMBOURGISH} "Desen Assistent w�rt dech duech d'Installatioun vun $(^NameDA) begleeden.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_MACEDONIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_MACEDONIAN} "��� ������� �� �� ���� ��� ������������ �� $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_MONGOLIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_MONGOLIAN} "$(^NameDA) ��������� �������� �� ���� ������� �����.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_NORWEGIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_NORWEGIAN} "Denne veiviseren vil lede deg gjennom installasjonen av $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_POLISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_POLISH} "Ten kreator pomo�e Ci zainstalowa� program $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_PORTUGUESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_PORTUGUESE} "Este assistente ajud�-lo-� durante a instala��o do $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_PORTUGUESEBR_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_PORTUGUESEBR} "Este assistente o guiar� durante a instala��o do $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_ROMANIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_ROMANIAN} "Aceasta aplicatie va instala produsului $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_RUSSIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_RUSSIAN} "��� ��������� ��������� $(^NameDA) �� ��� ���������.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SERBIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SERBIAN} "����� ����� ���� ������ ����������� �������� $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SERBIANLATIN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SERBIANLATIN} "Bi�ete vo�eni kroz proces instaliranja programa $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SIMPCHINESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SIMPCHINESE} "����򵼽�ָ������� $(^NameDA) �İ�װ���̡�\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SLOVAK_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SLOVAK} "Tento sprievodca V�s prevedie in�tal�ciou $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SLOVENIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SLOVENIAN} "Ta �arovnik vam bo pomagal pri namestitvi $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SPANISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SPANISH} "Este programa instalar� $(^NameDA) en su ordenador.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_SWEDISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_SWEDISH} "Denna guide tar dig igenom installationen av $(^NameDA).\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_THAI_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_THAI} "��ǵԴ��駹��еԴ�������� $(^NameDA) ŧ������������ͧ�س\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_TRADCHINESE_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_TRADCHINESE} "�o�Ӻ��F�N���ާA���� $(^NameDA) ���w�˶i�{�C\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_TURKISH_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_TURKISH} "Bu sihirbaz size $(^NameDA) kurulumu boyunca rehberlik edecektir.\r\n\r\n$_CLICK"
!endif

!ifdef MUI_LANGUAGEFILE_UKRAINIAN_USED
LANGSTRING WELCOME_INFO_TEXT ${LANG_UKRAINIAN} "���� ��������� ���������� $(^NameDA) �� ��� ����'����.\r\n\r\n$_CLICK"
!endif
