/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for language names
  $Id: LangNames.cpp 3853 2009-08-26 13:50:27Z helmut $

The contents of this file are subject to the Mozilla Public License Version 1.1
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

The Original Code is Logoskop.

The Initial Developer of the Original Code is Helmut Steeb.
Portions created by Helmut Steeb are Copyright (C) 2008 Helmut Steeb.
All Rights Reserved.

Contributor(s):

Alternatively, the contents of this file may be used under the terms of the
GNU General Public License (the "GPL"), in which case the provisions of
GPL are applicable instead of those above. If you wish to allow use
of your version of this file only under the terms of the GPL and not
to allow others to use your version of this file under the MPL, indicate your
decision by deleting the provisions above and replace them with the notice and
other provisions required by the GPL. If you do not delete the
provisions above, a recipient may use your version of this file under either the
MPL or the GPL.
====================================================================== */

#include "stdafx.h"
#include "w32los.h"
#include "LangNames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// http://www.oasis-open.org/cover/iso639a.html
// http://www.oasis-open.org/cover/nisoLang3-1994.html

// Language names:
//   2-/5-char form (ISO 639)
//   3-char form (NISO)
//   long name (English)
//   Windows LANGID (primary langID with optional secondary langID; LANG_NEUTRAL where no Windows LANGID known)
// stored consecutively in one plain array, terminated by 0.
#define OFFSET_ISO  0
#define OFFSET_NISO 1
#define OFFSET_LONG 2
#define OFFSET_LANGID 3

struct LANGRECORD {
  char *pIso;
  char *pNiso;
  char *pLong;
  LANGID langID;
};
#define RECSIZE (sizeof(LANGRECORD));

LANGRECORD langRecords[] = {
// "ACE", "Achinese",
// "ACH", "Acoli",
// "ADA", "Adangme",
// "AFA", "Afro-Asiatic (Other)",
// "AFH", "Afrihili (Artificial language)",
  {"af", "AFR", "Afrikaans", LANG_AFRIKAANS},
// "AJM", "Aljamia",
// "AKK", "Akkadian",
  { "sq", "ALB", "Albanian", LANG_ALBANIAN},
// "ALE", "Aleut",
// "ALG", "Algonquian languages",
// "AMH", "Amharic",
// "ANG", "English, Old (ca. 450-1100)",
// "APA", "Apache languages",
  { "ar", "ARA", "Arabic", LANG_ARABIC},
// "ARC", "Aramaic",
// "ARM", "Armenian",
// "ARN", "Araucanian",
// "ARP", "Arapaho",
// "ART", "Artificial (Other)",
// "ARW", "Arawak",
// "ASM", "Assamese", LANG_ASSAMESE,
// "ATH", "Athapascan languages",
// "AVA", "Avaric ",
// "AVE", "Avestan",
// "AWA", "Awadhi",
// "AYM", "Aymara",
// "AZE", "Azerbaijani", LANG_AZERI, ???
// "BAD", "Banda",
// "BAI", "Bamileke languages",
// "BAK", "Bashkir",
// "BAL", "Baluchi",
// "BAM", "Bambara",
// "BAN", "Balinese",
// "BAQ", "Basque", LANG_BASQUE,
// "BAS", "Basa",
// "BAT", "Baltic (Other)",
// "BEJ", "Beja",
// "BEL", "Byelorussian", LANG_BELARUSIAN, ???
// "BEM", "Bemba",
// "BEN", "Bengali", LANG_BENGALI,
// "BER", "Berber languages",
// "BHO", "Bhojpuri",
// "BIK", "Bikol",
// "BIN", "Bini",
// "BLA", "Siksika",
// "BRA", "Braj",
// "BRE", "Breton",
// "BUG", "Buginese",
// "BUL", "Bulgarian", LANG_BULGARIAN,
// "BUR", "Burmese",
// "CAD", "Caddo",
// "CAI", "Central American Indian (Other)",
// "CAM", "Khmer",
// "CAR", "Carib",
// "CAT", "Catalan", LANG_CATALAN,
// "CAU", "Caucasian (Other)",
// "CEB", "Cebuano",
// "CEL", "Celtic languages",
// "CHA", "Chamorro",
// "CHB", "Chibcha",
// "CHE", "Chechen",
// "CHG", "Chagatai",
  { "zh", "CHI", "Chinese", LANG_CHINESE},
// 2008-11-01 HS: for Chinese, cf. 
// - http://www.w3.org/International/questions/qa-css-lang "by the way"
// - http://www.iana.org/assignments/language-subtag-registry
// - http://publib.boulder.ibm.com/infocenter/wf/v2r7m0/topic/com.ibm.help.wf.doc/locale_spec/i_xfdl_r_formats_zh_Hant_TW.html
  { "zh-Hans", "@", "Simplified Chinese", MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)},
  { "zh-Hant", "@", "Traditional Chinese", MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)},
// "CHN", "Chinook jargon",
// "CHO", "Choctaw",
// "CHR", "Cherokee",
// "CHU", "Church Slavic",
// "CHV", "Chuvash",
// "CHY", "Cheyenne",
// "COP", "Coptic",
// "COR", "Cornish",
// "CPE", "Creoles and Pidgins, English-based (Other)",
// "CPF", "Creoles and Pidgins, French-based (Other)",
// "CPP", "Creoles and Pidgins, Portuguese-based (Other)",
// "CRE", "Cree",
// "CRP", "Creoles and Pidgins (Other)",
// "CUS", "Cushitic (Other)",
  { "cs", "CZE", "Czech", LANG_CZECH},
// "DAK", "Dakota",
  { "da", "DAN", "Danish", LANG_DANISH},
// "DEL", "Delaware",
// "DIN", "Dinka",
// "DOI", "Dogri",
// "DRA", "Dravidian (Other)",
// "DUA", "Duala",
// "DUM", "Dutch, Middle (ca. 1050-1350)",
  { "nl", "DUT", "Dutch", LANG_DUTCH},
// "DYU", "Dyula",
// "EFI", "Efik",
// "EGY", "Egyptian",
// "EKA", "Ekajuk",
// "ELX", "Elamite",
  { "en", "ENG", "English", LANG_ENGLISH},
// "ENM", "English, Middle (1100-1500)",
// "ESK", "Eskimo",
// "ESP", "Esperanto",
// "EST", "Estonian", LANG_ESTONIAN,
// "ETH", "Ethiopic",
// "EWE", "Ewe",
// "EWO", "Ewondo",
// "FAN", "Fang",
// "FAR", "Faroese", LANG_FAEROESE, ???
// "FAT", "Fanti",
// "FIJ", "Fijian",
  { "fi", "FIN", "Finnish", LANG_FINNISH},
// "FIU", "Finno-Ugrian (Other)",
// "FON", "Fon",
  { "fr", "FRE", "French", LANG_FRENCH},
 "fy", "FRI", "Friesian", LANG_NEUTRAL,
// "FRM", "French, Middle (ca. 1400-1600)",
// "FRO", "French, Old (ca. 842-1400)",
// "FUL", "Fula",
// "GAA", "G?", ????????????
// "GAE", "Gaelic (Scots)",
// "GAG", "Gallegan",
// "GAL", "Oromo",
// "GAY", "Gayo",
// "GEM", "Germanic (Other)",
// "GEO", "Georgian", LANG_GEORGIAN,
  { "de", "GER", "German", LANG_GERMAN},
// "GIL", "Gilbertese",
// "GMH", "German, Middle High (ca. 1050-1500)",
// "GOH", "German, Old High (ca. 750-1050)",
// "GON", "Gondi",
// "GOT", "Gothic",
// "GRB", "Grebo",
// "GRC", "Greek, Ancient (to 1453)",
  { "el", "GRE", "Greek, Modern (1453- )", LANG_GREEK},
 "gn", "GUA", "Guarani", LANG_NEUTRAL,
// "GUJ", "Gujarati", LANG_GUJARATI,
// "HAI", "Haida",
// "HAU", "Hausa",
// "HAW", "Hawaiian",
  { "he", "HEB", "Hebrew", LANG_HEBREW},
// "HER", "Herero",
// "HIL", "Hiligaynon",
// "HIM", "Himachali",
// "HIN", "Hindi", LANG_HINDI,
// "HMO", "Hiri Motu",
  { "hu", "HUN", "Hungarian", LANG_HUNGARIAN},
// "HUP", "Hupa",
// "IBA", "Iban",
// "IBO", "Igbo",
// "ICE", "Icelandic", LANG_ICELANDIC,
// "IJO", "Ijo",
// "ILO", "Iloko",
// "INC", "Indic (Other)",
  { "id", "IND", "Indonesian", LANG_INDONESIAN},
// "INE", "Indo-European (Other)",
// "INT", "Interlingua (International Auxiliary Language Association)",
// "IRA", "Iranian (Other)",
// "IRI", "Irish",
// "IRO", "Iroquoian languages",
  { "it", "ITA", "Italian", LANG_ITALIAN},
// "JAV", "Javanese",
  { "ja", "JPN", "Japanese", LANG_JAPANESE},
// "JPR", "Judeo-Persian",
// "JRB", "Judeo-Arabic",
// "KAA", "Kara-Kalpak",
// "KAB", "Kabyle",
// "KAC", "Kachin",
// "KAM", "Kamba",
// "KAN", "Kannada", LANG_KANNADA,
// "KAR", "Karen",
// "KAS", "Kashmiri", LANG_KASHMIRI,
// "KAU", "Kanuri",
// "KAW", "Kawi",
// "KAZ", "Kazakh", LANG_KAZAK,
// "KHA", "Khasi",
// "KHI", "Khoisan (Other)",
// "KHO", "Khotanese",
// "KIK", "Kikuyu",
// "KIN", "Kinyarwanda",
// "KIR", "Kirghiz",
// "KOK", "Konkani", LANG_KONKANI,
// "KON", "Kongo",
  { "ko", "KOR", "Korean", LANG_KOREAN},
// "KPE", "Kpelle",
// "KRO", "Kru",
// "KRU", "Kurukh",
// "KUA", "Kuanyama",
  { "ku", "KUR", "Kurdish", LANG_NEUTRAL},
// "KUS", "Kusaie",
// "KUT", "Kutenai",
// "LAD", "Ladino",
// "LAH", "Lahnd",
// "LAM", "Lamba",
// "LAN", "Langue d'oc (post-1500)",
// "LAO", "Lao",
// "LAP", "Lapp",
  { "la", "LAT", "Latin", LANG_NEUTRAL},
// "LAV", "Latvian", LANG_LATVIAN,
// "LIN", "Lingala",
// "LIT", "Lithuanian", LANG_LITHUANIAN,
// "LOL", "Mongo",
// "LOZ", "Lozi",
// "LUB", "Luba-Katanga",
// "LUG", "Ganda",
// "LUI", "Luiseno",
// "LUN", "Lunda",
// "LUO", "Luo (Kenya and Tanzania)",
// "MAC", "Macedonian", LANG_MACEDONIAN,
// "MAD", "Madurese",
// "MAG", "Magahi",
// "MAH", "Marshall",
// "MAI", "Maithili",
// "MAK", "Makasar",
// "MAL", "Malayalam", LANG_MALAY???, LANG_MALAYALAM,
// "MAN", "Mandingo",
// "MAO", "Maori",
// "MAP", "Austronesian (Other)",
// "MAR", "Marathi", LANG_MARATHI,
// "MAS", "Masai",
// "MAX", "Manx",
// "MAY", "Malay",
// "MEN", "Mende",
// "MIC", "Micmac",
// "MIN", "Minangkabau",
// "MIS", "Miscellaneous (Other)",
// "MKH", "Mon-Khmer (Other)",
// "MLA", "Malagasy",
// "MLT", "Maltese",
// "MNI", "Manipuri", LANG_MANIPURI,
// "MNO", "Manobo languages",
// "MOH", "Mohawk",
  { "mo", "MOL", "Moldavian", LANG_NEUTRAL},
// "MON", "Mongolian",
// "MOS",s"Mossi",
// "MUL", "Multiple languages",
// "MUN", "Munda (Other)",
// "MUS", "Creek",
// "MWR", "Marwari",
// "MYN", "Mayan languages",
// "NAH", "Aztec",
// "NAI", "North American Indian (Other)",
// "NAV", "Navajo",
// "NDE", "Ndebele (Zimbabwe)  ",
// "NDO", "Ndonga",
// "NEP", "Nepali", LANG_NEPALI,
// "NEW", "Newari",
// "NIC", "Niger-Kordofanian (Other)",
// "NIU", "Niuean",
  { "no", "NOR", "Norwegian", LANG_NORWEGIAN},
// "NSO", "Northern Sotho",
// "NUB", "Nubian languages",
// "NYA", "Nyanja",
// "NYM", "Nyamwezi",
// "NYN", "Nyankole",
// "NYO", "Nyoro",
// "NZI", "Nzima",
// "OJI", "Ojibwa",
// "ORI", "Oriya", LANG_ORIYA,
// "OSA", "Osage",
// "OSS", "Ossetic",
// "OTA", "Turkish, Ottoman",
// "OTO", "Otomian languages",
// "PAA", "Papuan-Australian (Other)",
// "PAG", "Pangasinan",
// "PAL", "Pahlavi",
// "PAM", "Pampanga",
// "PAN", "Panjabi", LANG_PUNJABI, ???
// "PAP", "Papiamento",
// "PAU", "Palauan",
// "PEO", "Old Persian (ca. 600-400 B.C.)",
// "PER", "Persian", LANG_FARSI???
// "PLI", "Pali",
  { "pl", "POL", "Polish", LANG_POLISH},
// "PON", "Ponape",
  { "pt", "POR", "Portuguese", LANG_PORTUGUESE},
// "PRA", "Prakrit languages",
// "PRO", "Provencal, Old (to 1500)",
// "PUS", "Pushto",
  { "qu", "QUE", "Quechua", LANG_NEUTRAL},
// "RAJ", "Rajasthani",
// "RAR", "Rarotongan",
// "ROA", "Romance (Other)",
// "ROH", "Raeto-Romance",
// "ROM", "Romany",
  { "ro", "RUM", "Romanian", LANG_ROMANIAN},
// "RUN", "Rundi",
  { "ru", "RUS", "Russian", LANG_RUSSIAN},
// "SAD", "Sandawe",
// "SAG", "Sango",
// "SAI", "South American Indian (Other)",
// "SAL", "Salishan languages",
// "SAM", "Samaritan Aramaic",
// "SAN", "Sanskrit", LANG_SANSKRIT,
// "SAO", "Samoan",
// "SCC", "Serbo-Croatian (Cyrillic)",  LANG_CROATIAN,LANG_SERBIAN???
// "SCO", "Scots",
// "SCR", "Serbo-Croatian (Roman)", LANG_CROATIAN,???
// "SEL", "Selkup",
// "SEM", "Semitic (Other)",
// "SHN", "Shan",
// "SHO", "Shona",
// "SID", "Sidamo",
// "SIO", "Siouan languages",
// "SIT", "Sino-Tibetan (Other)",
// "SLA", "Slavic (Other)",
// "SLO", "Slovak", LANG_SLOVAK,
// "SLV", "Slovenian", LANG_SLOVENIAN,
// "SND", "Sindhi", LANG_SINDHI,
// "SNH", "Sinhalese",
// "SOM", "Somali",
// "SON", "Songhai",
  { "es", "SPA", "Spanish", LANG_SPANISH},
// "SRR", "Serer",
// "SSO", "Sotho",
// "SUK", "Sukuma",
// "SUN", "Sundanese",
// "SUS", "Susu",
// "SUX", "Sumerian",
  { "sw", "SWA", "Swahili", LANG_SWAHILI},
// "SWZ", "Swazi",
// "SYR", "Syriac",
// "TAG", "Tagalog",
// "TAH", "Tahitian",
// "TAJ", "Tajik",
  { "ta", "TAM", "Tamil", LANG_TAMIL},
// "TAR", "Tatar", LANG_TATAR,
// "TEL", "Telugu", LANG_TELUGU,
// "TEM", "Timne",
// "TER", "Tereno",
  { "th", "THA", "Thai", LANG_THAI},
// "TIB", "Tibetan",
// "TIG", "Tigre",
// "TIR", "Tigrinya",
// "TIV", "Tivi",
// "TLI", "Tlingit",
// "TOG", "Tonga (Nyasa)",
// "TON", "Tonga (Tonga Islands)",
// "TRU", "Truk",
// "TSI", "Tsimshian",
// "TSO", "Tsonga",
// "TSW", "Tswana",
// "TUK", "Turkmen",
// "TUM", "Tumbuka",
  { "tr", "TUR", "Turkish", LANG_TURKISH},
// "TUT", "Altaic (Other)",
// "TWI", "Twi",
// "UGA", "Ugaritic",
// "UIG", "Uighur",
// "UKR", "Ukrainian", LANG_UKRANIAN, ???
// "UMB", "Umbundu",
// "UND", "Undetermined",
// "URD", "Urdu", LANG_URDU,
// "UZB", "Uzbek", LANG_UZBEK,
// "VAI", "Vai",
// "VEN", "Venda",
  { "vi", "VIE", "Vietnamese", LANG_VIETNAMESE},
// "VOT", "Votic",
// "WAK", "Wakashan languages",
// "WAL", "Walamo",
// "WAR", "Waray",
// "WAS", "Washo",
// "WEL", "Welsh",
// "WEN", "Sorbian languages",
// "WOL", "Wolof",
// "XHO", "Xhosa",
// "YAO", "Yao",
// "YAP", "Yap",
// "YID", "Yiddish",
// "YOR", "Yoruba",
// "ZAP", "Zapotec",
// "ZEN", "Zenaga",
// "ZUL", "Zulu",
// "ZUN", "Zuni",
  { 0, 0, 0, LANG_NEUTRAL },
 };

const char *NLangNames::GetLongForNiso(const char *nisoName)
{
  for (int i = 0; langRecords[i].pIso && stricmp(nisoName, langRecords[i].pNiso); ++i) {
    ;
  }
  return langRecords[i].pIso ? langRecords[i].pLong : 0;
}

const char *NLangNames::GetNisoForLong(const char *longName)
{
  for (int i = 0; langRecords[i].pIso && strcmp(longName, langRecords[i].pLong); ++i) {
    ;
  }
  return langRecords[i].pIso ? langRecords[i].pNiso : 0;
}

const char *NLangNames::GetLongForIso(const char *nisoName)
{
  for (int i = 0; langRecords[i].pIso && stricmp(nisoName, langRecords[i].pIso); ++i) {
    ;
  }
  return langRecords[i].pIso ? langRecords[i].pLong : 0;
}

const char *NLangNames::GetIsoForLong(const char *longName)
{
  for (int i = 0; langRecords[i].pIso && strcmp(longName, langRecords[i].pLong); ++i) {
    ;
  }
  return langRecords[i].pIso ? langRecords[i].pIso : 0;
}

const char *NLangNames::GetIsoForLangID(LANGID langID)
{
  WORD primaryLangID = PRIMARYLANGID(langID);
  WORD subLangID = SUBLANGID(langID);

  for (int i = 0; langRecords[i].pIso; ++i) {
    LANGID L = langRecords[i].langID;
    WORD p = PRIMARYLANGID(L);
    WORD s = SUBLANGID(L);
    // primaryLangID must match
    // + if subLangID is given in table above (= not SUBLANG_NEUTRAL), it must match, too
    if (p == primaryLangID && (s == SUBLANG_NEUTRAL || s == subLangID)) {
      break;
    }
  }
  return langRecords[i].pIso ? langRecords[i].pIso : 0;
}

#define SEP " "

string NLangNames::MakeNisoDisplayName(const string &shortName)
{
  string res;
  const char *longName = GetLongForNiso(shortName.c_str());
  // write "[longName ]shortName"
  if (longName) {
    res = longName;
    res += SEP;
  }
  res += shortName;
  return res;
}

string NLangNames::MakeIsoDisplayName(const string &shortName)
{
  string res;
  const char *longName = GetLongForIso(shortName.c_str());
  // write "[longName ]shortName"
  if (longName) {
    res = longName;
    res += SEP;
  }
  res += shortName;
  return res;
}


string NLangNames::ExtractNisoFromNisoDisplayName(const string &nisoDisplayName)
{
  string::size_type pos = nisoDisplayName.rfind(SEP);
  if (pos == string::npos) {
    return nisoDisplayName;
  }
  return nisoDisplayName.substr(pos+1, string::npos);
}

string NLangNames::ExtractIsoFromIsoDisplayName(const string &isoDisplayName)
{
  string::size_type pos = isoDisplayName.rfind(SEP);
  if (pos == string::npos) {
    return isoDisplayName;
  }
  return isoDisplayName.substr(pos+1, string::npos);
}

const string NLangNames::GetDefaultLangNisoName()
{
  return string("eng");
}


