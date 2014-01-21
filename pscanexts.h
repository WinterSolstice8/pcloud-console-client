#define PSYNC_SCAN_EXTENSIONS_CNT 166

static const char *psync_scan_typenames[4]={"pictures", "videos", "music files", "documents"};

static const uint32_t psync_scan_extensions[PSYNC_SCAN_EXTENSIONS_CNT]={
  438 /* au */,
  540 /* dl */,
  550 /* dv */,
  651 /* gl */,
  1029 /* qt */,
  1047 /* ra */,
  1059 /* rm */,
  1139 /* ts */,
  1244 /* wm */,
  6131 /* 3gp */,
  15536 /* abw */,
  15778 /* aif */,
  15938 /* amr */,
  16036 /* ape */,
  16125 /* art */,
  16145 /* asc */,
  16148 /* asf */,
  16166 /* asx */,
  16262 /* avi */,
  16292 /* awb */,
  16328 /* axa */,
  16349 /* axv */,
  17305 /* bmp */,
  17480 /* brf */,
  18220 /* caf */,
  18343 /* cdr */,
  18345 /* cdt */,
  18789 /* cpt */,
  18836 /* cr2 */,
  18866 /* crw */,
  18884 /* csd */,
  19885 /* dif */,
  19938 /* djv */,
  20104 /* doc */,
  20121 /* dot */,
  21587 /* erf */,
  22737 /* fli */,
  22750 /* flv */,
  23992 /* gif */,
  24369 /* gsm */,
  26517 /* ico */,
  26582 /* ief */,
  28285 /* jng */,
  28357 /* jpe */,
  28359 /* jpg */,
  29184 /* kar */,
  29739 /* kpr */,
  29741 /* kpt */,
  29848 /* ksp */,
  29984 /* kwd */,
  30000 /* kwt */,
  31207 /* lsf */,
  31225 /* lsx */,
  31666 /* m3u */,
  31683 /* m4a */,
  32017 /* mdb */,
  32204 /* mid */,
  32296 /* mkv */,
  32392 /* mng */,
  32444 /* mov */,
  32452 /* mp2 */,
  32453 /* mp3 */,
  32454 /* mp4 */,
  32464 /* mpe */,
  32466 /* mpg */,
  32481 /* mpv */,
  32626 /* mts */,
  32776 /* mxu */,
  33427 /* nef */,
  34755 /* odb */,
  34756 /* odc */,
  34762 /* odi */,
  34766 /* odm */,
  34769 /* odp */,
  34772 /* ods */,
  34773 /* odt */,
  34865 /* oga */,
  34871 /* ogg */,
  34886 /* ogv */,
  35274 /* orc */,
  35277 /* orf */,
  35353 /* oth */,
  35361 /* otp */,
  35364 /* ots */,
  35365 /* ott */,
  36031 /* pat */,
  36061 /* pbm */,
  36109 /* pcx */,
  36128 /* pdf */,
  36246 /* pgm */,
  36437 /* pls */,
  36499 /* png */,
  36505 /* pnm */,
  36549 /* pot */,
  36579 /* ppm */,
  36585 /* pps */,
  36586 /* ppt */,
  36681 /* psd */,
  38762 /* ram */,
  38768 /* ras */,
  38973 /* rgb */,
  39458 /* rtf */,
  40207 /* sco */,
  40222 /* sd2 */,
  40252 /* sdw */,
  40352 /* sgl */,
  40418 /* sid */,
  40603 /* snd */,
  40697 /* spx */,
  40767 /* srt */,
  40844 /* stw */,
  40902 /* svg */,
  40976 /* sxg */,
  40992 /* sxw */,
  41789 /* tif */,
  42154 /* tsa */,
  42175 /* tsv */,
  42358 /* txt */,
  45616 /* wav */,
  45618 /* wax */,
  46039 /* wma */,
  46060 /* wmv */,
  46062 /* wmx */,
  46395 /* wvx */,
  47013 /* xbm */,
  47372 /* xlb */,
  47389 /* xls */,
  47390 /* xlt */,
  47531 /* xpm */,
  47781 /* xwd */,
  583799 /* aifc */,
  583802 /* aiff */,
  684197 /* chrt */,
  737737 /* djvu */,
  743871 /* docm */,
  743882 /* docx */,
  744500 /* dotm */,
  744511 /* dotx */,
  840986 /* flac */,
  1049226 /* jpeg */,
  1170265 /* m2ts */,
  1191567 /* midi */,
  1201185 /* mpeg */,
  1201253 /* mpga */,
  1352336 /* potm */,
  1352347 /* potx */,
  1353002 /* ppam */,
  1353668 /* ppsm */,
  1353679 /* ppsx */,
  1353705 /* pptm */,
  1353716 /* pptx */,
  1499596 /* sldm */,
  1499607 /* sldx */,
  1513410 /* svgz */,
  1541413 /* text */,
  1546209 /* tiff */,
  1688854 /* wbmp */,
  1692551 /* webm */,
  1752750 /* xlam */,
  1753405 /* xlsb */,
  1753416 /* xlsm */,
  1753427 /* xlsx */,
  1753453 /* xltm */,
  1753464 /* xltx */,
  44416554 /* movie */,
  44443856 /* mpega */
};

static const uint8_t psync_scan_types[PSYNC_SCAN_EXTENSIONS_CNT]={
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  3,
  2,
  2,
  2,
  0,
  3,
  1,
  1,
  1,
  2,
  2,
  1,
  0,
  3,
  2,
  0,
  0,
  0,
  0,
  0,
  2,
  1,
  0,
  3,
  3,
  0,
  1,
  1,
  0,
  2,
  0,
  0,
  0,
  0,
  0,
  2,
  3,
  3,
  3,
  3,
  3,
  1,
  1,
  2,
  2,
  3,
  2,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  3,
  3,
  0,
  3,
  3,
  3,
  3,
  2,
  2,
  1,
  2,
  0,
  3,
  3,
  3,
  3,
  0,
  0,
  0,
  3,
  0,
  2,
  0,
  0,
  3,
  0,
  3,
  3,
  0,
  2,
  0,
  0,
  3,
  2,
  2,
  3,
  3,
  2,
  2,
  2,
  3,
  3,
  0,
  3,
  3,
  0,
  1,
  1,
  3,
  2,
  2,
  2,
  1,
  1,
  1,
  0,
  3,
  3,
  3,
  0,
  0,
  2,
  2,
  3,
  0,
  3,
  3,
  3,
  3,
  2,
  0,
  1,
  2,
  1,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  3,
  0,
  0,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  1,
  2
};

