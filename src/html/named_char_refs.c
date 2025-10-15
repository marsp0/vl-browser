#include "named_char_refs.h"

#define MAX_SIZE 2231

static uint32_t keys[MAX_SIZE] = { 0 };
static uint32_t values[MAX_SIZE] = { 0 };

uint32_t html_get_named_char_ref(hash_str_t key)
{
    int32_t idx = -1;
    for (uint32_t i = 0; i < MAX_SIZE; i++)
    {
        if (keys[i] == key) { idx = (int32_t)i; }
    }

    if (idx < 0) { return 0; }

    return values[idx];
}

void html_named_char_ref_map_init()
{
    keys[0]   = hash_str_new("&AElig", 6);
    values[0] = 198;

    keys[1]   = hash_str_new("&AElig;", 7);
    values[1] = 198;

    keys[2]   = hash_str_new("&AMP", 4);
    values[2] = 38;

    keys[3]   = hash_str_new("&AMP;", 5);
    values[3] = 38;

    keys[4]   = hash_str_new("&Aacute", 7);
    values[4] = 193;

    keys[5]   = hash_str_new("&Aacute;", 8);
    values[5] = 193;

    keys[6]   = hash_str_new("&Abreve;", 8);
    values[6] = 258;

    keys[7]   = hash_str_new("&Acirc", 6);
    values[7] = 194;

    keys[8]   = hash_str_new("&Acirc;", 7);
    values[8] = 194;

    keys[9]   = hash_str_new("&Acy;", 5);
    values[9] = 1040;

    keys[10]   = hash_str_new("&Afr;", 5);
    values[10] = 120068;

    keys[11]   = hash_str_new("&Agrave", 7);
    values[11] = 192;

    keys[12]   = hash_str_new("&Agrave;", 8);
    values[12] = 192;

    keys[13]   = hash_str_new("&Alpha;", 7);
    values[13] = 913;

    keys[14]   = hash_str_new("&Amacr;", 7);
    values[14] = 256;

    keys[15]   = hash_str_new("&And;", 5);
    values[15] = 10835;

    keys[16]   = hash_str_new("&Aogon;", 7);
    values[16] = 260;

    keys[17]   = hash_str_new("&Aopf;", 6);
    values[17] = 120120;

    keys[18]   = hash_str_new("&ApplyFunction;", 15);
    values[18] = 8289;

    keys[19]   = hash_str_new("&Aring", 6);
    values[19] = 197;

    keys[20]   = hash_str_new("&Aring;", 7);
    values[20] = 197;

    keys[21]   = hash_str_new("&Ascr;", 6);
    values[21] = 119964;

    keys[22]   = hash_str_new("&Assign;", 8);
    values[22] = 8788;

    keys[23]   = hash_str_new("&Atilde", 7);
    values[23] = 195;

    keys[24]   = hash_str_new("&Atilde;", 8);
    values[24] = 195;

    keys[25]   = hash_str_new("&Auml", 5);
    values[25] = 196;

    keys[26]   = hash_str_new("&Auml;", 6);
    values[26] = 196;

    keys[27]   = hash_str_new("&Backslash;", 11);
    values[27] = 8726;

    keys[28]   = hash_str_new("&Barv;", 6);
    values[28] = 10983;

    keys[29]   = hash_str_new("&Barwed;", 8);
    values[29] = 8966;

    keys[30]   = hash_str_new("&Bcy;", 5);
    values[30] = 1041;

    keys[31]   = hash_str_new("&Because;", 9);
    values[31] = 8757;

    keys[32]   = hash_str_new("&Bernoullis;", 12);
    values[32] = 8492;

    keys[33]   = hash_str_new("&Beta;", 6);
    values[33] = 914;

    keys[34]   = hash_str_new("&Bfr;", 5);
    values[34] = 120069;

    keys[35]   = hash_str_new("&Bopf;", 6);
    values[35] = 120121;

    keys[36]   = hash_str_new("&Breve;", 7);
    values[36] = 728;

    keys[37]   = hash_str_new("&Bscr;", 6);
    values[37] = 8492;

    keys[38]   = hash_str_new("&Bumpeq;", 8);
    values[38] = 8782;

    keys[39]   = hash_str_new("&CHcy;", 6);
    values[39] = 1063;

    keys[40]   = hash_str_new("&COPY", 5);
    values[40] = 169;

    keys[41]   = hash_str_new("&COPY;", 6);
    values[41] = 169;

    keys[42]   = hash_str_new("&Cacute;", 8);
    values[42] = 262;

    keys[43]   = hash_str_new("&Cap;", 5);
    values[43] = 8914;

    keys[44]   = hash_str_new("&CapitalDifferentialD;", 22);
    values[44] = 8517;

    keys[45]   = hash_str_new("&Cayleys;", 9);
    values[45] = 8493;

    keys[46]   = hash_str_new("&Ccaron;", 8);
    values[46] = 268;

    keys[47]   = hash_str_new("&Ccedil", 7);
    values[47] = 199;

    keys[48]   = hash_str_new("&Ccedil;", 8);
    values[48] = 199;

    keys[49]   = hash_str_new("&Ccirc;", 7);
    values[49] = 264;

    keys[50]   = hash_str_new("&Cconint;", 9);
    values[50] = 8752;

    keys[51]   = hash_str_new("&Cdot;", 6);
    values[51] = 266;

    keys[52]   = hash_str_new("&Cedilla;", 9);
    values[52] = 184;

    keys[53]   = hash_str_new("&CenterDot;", 11);
    values[53] = 183;

    keys[54]   = hash_str_new("&Cfr;", 5);
    values[54] = 8493;

    keys[55]   = hash_str_new("&Chi;", 5);
    values[55] = 935;

    keys[56]   = hash_str_new("&CircleDot;", 11);
    values[56] = 8857;

    keys[57]   = hash_str_new("&CircleMinus;", 13);
    values[57] = 8854;

    keys[58]   = hash_str_new("&CirclePlus;", 12);
    values[58] = 8853;

    keys[59]   = hash_str_new("&CircleTimes;", 13);
    values[59] = 8855;

    keys[60]   = hash_str_new("&ClockwiseContourIntegral;", 26);
    values[60] = 8754;

    keys[61]   = hash_str_new("&CloseCurlyDoubleQuote;", 23);
    values[61] = 8221;

    keys[62]   = hash_str_new("&CloseCurlyQuote;", 17);
    values[62] = 8217;

    keys[63]   = hash_str_new("&Colon;", 7);
    values[63] = 8759;

    keys[64]   = hash_str_new("&Colone;", 8);
    values[64] = 10868;

    keys[65]   = hash_str_new("&Congruent;", 11);
    values[65] = 8801;

    keys[66]   = hash_str_new("&Conint;", 8);
    values[66] = 8751;

    keys[67]   = hash_str_new("&ContourIntegral;", 17);
    values[67] = 8750;

    keys[68]   = hash_str_new("&Copf;", 6);
    values[68] = 8450;

    keys[69]   = hash_str_new("&Coproduct;", 11);
    values[69] = 8720;

    keys[70]   = hash_str_new("&CounterClockwiseContourIntegral;", 33);
    values[70] = 8755;

    keys[71]   = hash_str_new("&Cross;", 7);
    values[71] = 10799;

    keys[72]   = hash_str_new("&Cscr;", 6);
    values[72] = 119966;

    keys[73]   = hash_str_new("&Cup;", 5);
    values[73] = 8915;

    keys[74]   = hash_str_new("&CupCap;", 8);
    values[74] = 8781;

    keys[75]   = hash_str_new("&DD;", 4);
    values[75] = 8517;

    keys[76]   = hash_str_new("&DDotrahd;", 10);
    values[76] = 10513;

    keys[77]   = hash_str_new("&DJcy;", 6);
    values[77] = 1026;

    keys[78]   = hash_str_new("&DScy;", 6);
    values[78] = 1029;

    keys[79]   = hash_str_new("&DZcy;", 6);
    values[79] = 1039;

    keys[80]   = hash_str_new("&Dagger;", 8);
    values[80] = 8225;

    keys[81]   = hash_str_new("&Darr;", 6);
    values[81] = 8609;

    keys[82]   = hash_str_new("&Dashv;", 7);
    values[82] = 10980;

    keys[83]   = hash_str_new("&Dcaron;", 8);
    values[83] = 270;

    keys[84]   = hash_str_new("&Dcy;", 5);
    values[84] = 1044;

    keys[85]   = hash_str_new("&Del;", 5);
    values[85] = 8711;

    keys[86]   = hash_str_new("&Delta;", 7);
    values[86] = 916;

    keys[87]   = hash_str_new("&Dfr;", 5);
    values[87] = 120071;

    keys[88]   = hash_str_new("&DiacriticalAcute;", 18);
    values[88] = 180;

    keys[89]   = hash_str_new("&DiacriticalDot;", 16);
    values[89] = 729;

    keys[90]   = hash_str_new("&DiacriticalDoubleAcute;", 24);
    values[90] = 733;

    keys[91]   = hash_str_new("&DiacriticalGrave;", 18);
    values[91] = 96;

    keys[92]   = hash_str_new("&DiacriticalTilde;", 18);
    values[92] = 732;

    keys[93]   = hash_str_new("&Diamond;", 9);
    values[93] = 8900;

    keys[94]   = hash_str_new("&DifferentialD;", 15);
    values[94] = 8518;

    keys[95]   = hash_str_new("&Dopf;", 6);
    values[95] = 120123;

    keys[96]   = hash_str_new("&Dot;", 5);
    values[96] = 168;

    keys[97]   = hash_str_new("&DotDot;", 8);
    values[97] = 8412;

    keys[98]   = hash_str_new("&DotEqual;", 10);
    values[98] = 8784;

    keys[99]   = hash_str_new("&DoubleContourIntegral;", 23);
    values[99] = 8751;

    keys[100]   = hash_str_new("&DoubleDot;", 11);
    values[100] = 168;

    keys[101]   = hash_str_new("&DoubleDownArrow;", 17);
    values[101] = 8659;

    keys[102]   = hash_str_new("&DoubleLeftArrow;", 17);
    values[102] = 8656;

    keys[103]   = hash_str_new("&DoubleLeftRightArrow;", 22);
    values[103] = 8660;

    keys[104]   = hash_str_new("&DoubleLeftTee;", 15);
    values[104] = 10980;

    keys[105]   = hash_str_new("&DoubleLongLeftArrow;", 21);
    values[105] = 10232;

    keys[106]   = hash_str_new("&DoubleLongLeftRightArrow;", 26);
    values[106] = 10234;

    keys[107]   = hash_str_new("&DoubleLongRightArrow;", 22);
    values[107] = 10233;

    keys[108]   = hash_str_new("&DoubleRightArrow;", 18);
    values[108] = 8658;

    keys[109]   = hash_str_new("&DoubleRightTee;", 16);
    values[109] = 8872;

    keys[110]   = hash_str_new("&DoubleUpArrow;", 15);
    values[110] = 8657;

    keys[111]   = hash_str_new("&DoubleUpDownArrow;", 19);
    values[111] = 8661;

    keys[112]   = hash_str_new("&DoubleVerticalBar;", 19);
    values[112] = 8741;

    keys[113]   = hash_str_new("&DownArrow;", 11);
    values[113] = 8595;

    keys[114]   = hash_str_new("&DownArrowBar;", 14);
    values[114] = 10515;

    keys[115]   = hash_str_new("&DownArrowUpArrow;", 18);
    values[115] = 8693;

    keys[116]   = hash_str_new("&DownBreve;", 11);
    values[116] = 785;

    keys[117]   = hash_str_new("&DownLeftRightVector;", 21);
    values[117] = 10576;

    keys[118]   = hash_str_new("&DownLeftTeeVector;", 19);
    values[118] = 10590;

    keys[119]   = hash_str_new("&DownLeftVector;", 16);
    values[119] = 8637;

    keys[120]   = hash_str_new("&DownLeftVectorBar;", 19);
    values[120] = 10582;

    keys[121]   = hash_str_new("&DownRightTeeVector;", 20);
    values[121] = 10591;

    keys[122]   = hash_str_new("&DownRightVector;", 17);
    values[122] = 8641;

    keys[123]   = hash_str_new("&DownRightVectorBar;", 20);
    values[123] = 10583;

    keys[124]   = hash_str_new("&DownTee;", 9);
    values[124] = 8868;

    keys[125]   = hash_str_new("&DownTeeArrow;", 14);
    values[125] = 8615;

    keys[126]   = hash_str_new("&Downarrow;", 11);
    values[126] = 8659;

    keys[127]   = hash_str_new("&Dscr;", 6);
    values[127] = 119967;

    keys[128]   = hash_str_new("&Dstrok;", 8);
    values[128] = 272;

    keys[129]   = hash_str_new("&ENG;", 5);
    values[129] = 330;

    keys[130]   = hash_str_new("&ETH", 4);
    values[130] = 208;

    keys[131]   = hash_str_new("&ETH;", 5);
    values[131] = 208;

    keys[132]   = hash_str_new("&Eacute", 7);
    values[132] = 201;

    keys[133]   = hash_str_new("&Eacute;", 8);
    values[133] = 201;

    keys[134]   = hash_str_new("&Ecaron;", 8);
    values[134] = 282;

    keys[135]   = hash_str_new("&Ecirc", 6);
    values[135] = 202;

    keys[136]   = hash_str_new("&Ecirc;", 7);
    values[136] = 202;

    keys[137]   = hash_str_new("&Ecy;", 5);
    values[137] = 1069;

    keys[138]   = hash_str_new("&Edot;", 6);
    values[138] = 278;

    keys[139]   = hash_str_new("&Efr;", 5);
    values[139] = 120072;

    keys[140]   = hash_str_new("&Egrave", 7);
    values[140] = 200;

    keys[141]   = hash_str_new("&Egrave;", 8);
    values[141] = 200;

    keys[142]   = hash_str_new("&Element;", 9);
    values[142] = 8712;

    keys[143]   = hash_str_new("&Emacr;", 7);
    values[143] = 274;

    keys[144]   = hash_str_new("&EmptySmallSquare;", 18);
    values[144] = 9723;

    keys[145]   = hash_str_new("&EmptyVerySmallSquare;", 22);
    values[145] = 9643;

    keys[146]   = hash_str_new("&Eogon;", 7);
    values[146] = 280;

    keys[147]   = hash_str_new("&Eopf;", 6);
    values[147] = 120124;

    keys[148]   = hash_str_new("&Epsilon;", 9);
    values[148] = 917;

    keys[149]   = hash_str_new("&Equal;", 7);
    values[149] = 10869;

    keys[150]   = hash_str_new("&EqualTilde;", 12);
    values[150] = 8770;

    keys[151]   = hash_str_new("&Equilibrium;", 13);
    values[151] = 8652;

    keys[152]   = hash_str_new("&Escr;", 6);
    values[152] = 8496;

    keys[153]   = hash_str_new("&Esim;", 6);
    values[153] = 10867;

    keys[154]   = hash_str_new("&Eta;", 5);
    values[154] = 919;

    keys[155]   = hash_str_new("&Euml", 5);
    values[155] = 203;

    keys[156]   = hash_str_new("&Euml;", 6);
    values[156] = 203;

    keys[157]   = hash_str_new("&Exists;", 8);
    values[157] = 8707;

    keys[158]   = hash_str_new("&ExponentialE;", 14);
    values[158] = 8519;

    keys[159]   = hash_str_new("&Fcy;", 5);
    values[159] = 1060;

    keys[160]   = hash_str_new("&Ffr;", 5);
    values[160] = 120073;

    keys[161]   = hash_str_new("&FilledSmallSquare;", 19);
    values[161] = 9724;

    keys[162]   = hash_str_new("&FilledVerySmallSquare;", 23);
    values[162] = 9642;

    keys[163]   = hash_str_new("&Fopf;", 6);
    values[163] = 120125;

    keys[164]   = hash_str_new("&ForAll;", 8);
    values[164] = 8704;

    keys[165]   = hash_str_new("&Fouriertrf;", 12);
    values[165] = 8497;

    keys[166]   = hash_str_new("&Fscr;", 6);
    values[166] = 8497;

    keys[167]   = hash_str_new("&GJcy;", 6);
    values[167] = 1027;

    keys[168]   = hash_str_new("&GT", 3);
    values[168] = 62;

    keys[169]   = hash_str_new("&GT;", 4);
    values[169] = 62;

    keys[170]   = hash_str_new("&Gamma;", 7);
    values[170] = 915;

    keys[171]   = hash_str_new("&Gammad;", 8);
    values[171] = 988;

    keys[172]   = hash_str_new("&Gbreve;", 8);
    values[172] = 286;

    keys[173]   = hash_str_new("&Gcedil;", 8);
    values[173] = 290;

    keys[174]   = hash_str_new("&Gcirc;", 7);
    values[174] = 284;

    keys[175]   = hash_str_new("&Gcy;", 5);
    values[175] = 1043;

    keys[176]   = hash_str_new("&Gdot;", 6);
    values[176] = 288;

    keys[177]   = hash_str_new("&Gfr;", 5);
    values[177] = 120074;

    keys[178]   = hash_str_new("&Gg;", 4);
    values[178] = 8921;

    keys[179]   = hash_str_new("&Gopf;", 6);
    values[179] = 120126;

    keys[180]   = hash_str_new("&GreaterEqual;", 14);
    values[180] = 8805;

    keys[181]   = hash_str_new("&GreaterEqualLess;", 18);
    values[181] = 8923;

    keys[182]   = hash_str_new("&GreaterFullEqual;", 18);
    values[182] = 8807;

    keys[183]   = hash_str_new("&GreaterGreater;", 16);
    values[183] = 10914;

    keys[184]   = hash_str_new("&GreaterLess;", 13);
    values[184] = 8823;

    keys[185]   = hash_str_new("&GreaterSlantEqual;", 19);
    values[185] = 10878;

    keys[186]   = hash_str_new("&GreaterTilde;", 14);
    values[186] = 8819;

    keys[187]   = hash_str_new("&Gscr;", 6);
    values[187] = 119970;

    keys[188]   = hash_str_new("&Gt;", 4);
    values[188] = 8811;

    keys[189]   = hash_str_new("&HARDcy;", 8);
    values[189] = 1066;

    keys[190]   = hash_str_new("&Hacek;", 7);
    values[190] = 711;

    keys[191]   = hash_str_new("&Hat;", 5);
    values[191] = 94;

    keys[192]   = hash_str_new("&Hcirc;", 7);
    values[192] = 292;

    keys[193]   = hash_str_new("&Hfr;", 5);
    values[193] = 8460;

    keys[194]   = hash_str_new("&HilbertSpace;", 14);
    values[194] = 8459;

    keys[195]   = hash_str_new("&Hopf;", 6);
    values[195] = 8461;

    keys[196]   = hash_str_new("&HorizontalLine;", 16);
    values[196] = 9472;

    keys[197]   = hash_str_new("&Hscr;", 6);
    values[197] = 8459;

    keys[198]   = hash_str_new("&Hstrok;", 8);
    values[198] = 294;

    keys[199]   = hash_str_new("&HumpDownHump;", 14);
    values[199] = 8782;

    keys[200]   = hash_str_new("&HumpEqual;", 11);
    values[200] = 8783;

    keys[201]   = hash_str_new("&IEcy;", 6);
    values[201] = 1045;

    keys[202]   = hash_str_new("&IJlig;", 7);
    values[202] = 306;

    keys[203]   = hash_str_new("&IOcy;", 6);
    values[203] = 1025;

    keys[204]   = hash_str_new("&Iacute", 7);
    values[204] = 205;

    keys[205]   = hash_str_new("&Iacute;", 8);
    values[205] = 205;

    keys[206]   = hash_str_new("&Icirc", 6);
    values[206] = 206;

    keys[207]   = hash_str_new("&Icirc;", 7);
    values[207] = 206;

    keys[208]   = hash_str_new("&Icy;", 5);
    values[208] = 1048;

    keys[209]   = hash_str_new("&Idot;", 6);
    values[209] = 304;

    keys[210]   = hash_str_new("&Ifr;", 5);
    values[210] = 8465;

    keys[211]   = hash_str_new("&Igrave", 7);
    values[211] = 204;

    keys[212]   = hash_str_new("&Igrave;", 8);
    values[212] = 204;

    keys[213]   = hash_str_new("&Im;", 4);
    values[213] = 8465;

    keys[214]   = hash_str_new("&Imacr;", 7);
    values[214] = 298;

    keys[215]   = hash_str_new("&ImaginaryI;", 12);
    values[215] = 8520;

    keys[216]   = hash_str_new("&Implies;", 9);
    values[216] = 8658;

    keys[217]   = hash_str_new("&Int;", 5);
    values[217] = 8748;

    keys[218]   = hash_str_new("&Integral;", 10);
    values[218] = 8747;

    keys[219]   = hash_str_new("&Intersection;", 14);
    values[219] = 8898;

    keys[220]   = hash_str_new("&InvisibleComma;", 16);
    values[220] = 8291;

    keys[221]   = hash_str_new("&InvisibleTimes;", 16);
    values[221] = 8290;

    keys[222]   = hash_str_new("&Iogon;", 7);
    values[222] = 302;

    keys[223]   = hash_str_new("&Iopf;", 6);
    values[223] = 120128;

    keys[224]   = hash_str_new("&Iota;", 6);
    values[224] = 921;

    keys[225]   = hash_str_new("&Iscr;", 6);
    values[225] = 8464;

    keys[226]   = hash_str_new("&Itilde;", 8);
    values[226] = 296;

    keys[227]   = hash_str_new("&Iukcy;", 7);
    values[227] = 1030;

    keys[228]   = hash_str_new("&Iuml", 5);
    values[228] = 207;

    keys[229]   = hash_str_new("&Iuml;", 6);
    values[229] = 207;

    keys[230]   = hash_str_new("&Jcirc;", 7);
    values[230] = 308;

    keys[231]   = hash_str_new("&Jcy;", 5);
    values[231] = 1049;

    keys[232]   = hash_str_new("&Jfr;", 5);
    values[232] = 120077;

    keys[233]   = hash_str_new("&Jopf;", 6);
    values[233] = 120129;

    keys[234]   = hash_str_new("&Jscr;", 6);
    values[234] = 119973;

    keys[235]   = hash_str_new("&Jsercy;", 8);
    values[235] = 1032;

    keys[236]   = hash_str_new("&Jukcy;", 7);
    values[236] = 1028;

    keys[237]   = hash_str_new("&KHcy;", 6);
    values[237] = 1061;

    keys[238]   = hash_str_new("&KJcy;", 6);
    values[238] = 1036;

    keys[239]   = hash_str_new("&Kappa;", 7);
    values[239] = 922;

    keys[240]   = hash_str_new("&Kcedil;", 8);
    values[240] = 310;

    keys[241]   = hash_str_new("&Kcy;", 5);
    values[241] = 1050;

    keys[242]   = hash_str_new("&Kfr;", 5);
    values[242] = 120078;

    keys[243]   = hash_str_new("&Kopf;", 6);
    values[243] = 120130;

    keys[244]   = hash_str_new("&Kscr;", 6);
    values[244] = 119974;

    keys[245]   = hash_str_new("&LJcy;", 6);
    values[245] = 1033;

    keys[246]   = hash_str_new("&LT", 3);
    values[246] = 60;

    keys[247]   = hash_str_new("&LT;", 4);
    values[247] = 60;

    keys[248]   = hash_str_new("&Lacute;", 8);
    values[248] = 313;

    keys[249]   = hash_str_new("&Lambda;", 8);
    values[249] = 923;

    keys[250]   = hash_str_new("&Lang;", 6);
    values[250] = 10218;

    keys[251]   = hash_str_new("&Laplacetrf;", 12);
    values[251] = 8466;

    keys[252]   = hash_str_new("&Larr;", 6);
    values[252] = 8606;

    keys[253]   = hash_str_new("&Lcaron;", 8);
    values[253] = 317;

    keys[254]   = hash_str_new("&Lcedil;", 8);
    values[254] = 315;

    keys[255]   = hash_str_new("&Lcy;", 5);
    values[255] = 1051;

    keys[256]   = hash_str_new("&LeftAngleBracket;", 18);
    values[256] = 10216;

    keys[257]   = hash_str_new("&LeftArrow;", 11);
    values[257] = 8592;

    keys[258]   = hash_str_new("&LeftArrowBar;", 14);
    values[258] = 8676;

    keys[259]   = hash_str_new("&LeftArrowRightArrow;", 21);
    values[259] = 8646;

    keys[260]   = hash_str_new("&LeftCeiling;", 13);
    values[260] = 8968;

    keys[261]   = hash_str_new("&LeftDoubleBracket;", 19);
    values[261] = 10214;

    keys[262]   = hash_str_new("&LeftDownTeeVector;", 19);
    values[262] = 10593;

    keys[263]   = hash_str_new("&LeftDownVector;", 16);
    values[263] = 8643;

    keys[264]   = hash_str_new("&LeftDownVectorBar;", 19);
    values[264] = 10585;

    keys[265]   = hash_str_new("&LeftFloor;", 11);
    values[265] = 8970;

    keys[266]   = hash_str_new("&LeftRightArrow;", 16);
    values[266] = 8596;

    keys[267]   = hash_str_new("&LeftRightVector;", 17);
    values[267] = 10574;

    keys[268]   = hash_str_new("&LeftTee;", 9);
    values[268] = 8867;

    keys[269]   = hash_str_new("&LeftTeeArrow;", 14);
    values[269] = 8612;

    keys[270]   = hash_str_new("&LeftTeeVector;", 15);
    values[270] = 10586;

    keys[271]   = hash_str_new("&LeftTriangle;", 14);
    values[271] = 8882;

    keys[272]   = hash_str_new("&LeftTriangleBar;", 17);
    values[272] = 10703;

    keys[273]   = hash_str_new("&LeftTriangleEqual;", 19);
    values[273] = 8884;

    keys[274]   = hash_str_new("&LeftUpDownVector;", 18);
    values[274] = 10577;

    keys[275]   = hash_str_new("&LeftUpTeeVector;", 17);
    values[275] = 10592;

    keys[276]   = hash_str_new("&LeftUpVector;", 14);
    values[276] = 8639;

    keys[277]   = hash_str_new("&LeftUpVectorBar;", 17);
    values[277] = 10584;

    keys[278]   = hash_str_new("&LeftVector;", 12);
    values[278] = 8636;

    keys[279]   = hash_str_new("&LeftVectorBar;", 15);
    values[279] = 10578;

    keys[280]   = hash_str_new("&Leftarrow;", 11);
    values[280] = 8656;

    keys[281]   = hash_str_new("&Leftrightarrow;", 16);
    values[281] = 8660;

    keys[282]   = hash_str_new("&LessEqualGreater;", 18);
    values[282] = 8922;

    keys[283]   = hash_str_new("&LessFullEqual;", 15);
    values[283] = 8806;

    keys[284]   = hash_str_new("&LessGreater;", 13);
    values[284] = 8822;

    keys[285]   = hash_str_new("&LessLess;", 10);
    values[285] = 10913;

    keys[286]   = hash_str_new("&LessSlantEqual;", 16);
    values[286] = 10877;

    keys[287]   = hash_str_new("&LessTilde;", 11);
    values[287] = 8818;

    keys[288]   = hash_str_new("&Lfr;", 5);
    values[288] = 120079;

    keys[289]   = hash_str_new("&Ll;", 4);
    values[289] = 8920;

    keys[290]   = hash_str_new("&Lleftarrow;", 12);
    values[290] = 8666;

    keys[291]   = hash_str_new("&Lmidot;", 8);
    values[291] = 319;

    keys[292]   = hash_str_new("&LongLeftArrow;", 15);
    values[292] = 10229;

    keys[293]   = hash_str_new("&LongLeftRightArrow;", 20);
    values[293] = 10231;

    keys[294]   = hash_str_new("&LongRightArrow;", 16);
    values[294] = 10230;

    keys[295]   = hash_str_new("&Longleftarrow;", 15);
    values[295] = 10232;

    keys[296]   = hash_str_new("&Longleftrightarrow;", 20);
    values[296] = 10234;

    keys[297]   = hash_str_new("&Longrightarrow;", 16);
    values[297] = 10233;

    keys[298]   = hash_str_new("&Lopf;", 6);
    values[298] = 120131;

    keys[299]   = hash_str_new("&LowerLeftArrow;", 16);
    values[299] = 8601;

    keys[300]   = hash_str_new("&LowerRightArrow;", 17);
    values[300] = 8600;

    keys[301]   = hash_str_new("&Lscr;", 6);
    values[301] = 8466;

    keys[302]   = hash_str_new("&Lsh;", 5);
    values[302] = 8624;

    keys[303]   = hash_str_new("&Lstrok;", 8);
    values[303] = 321;

    keys[304]   = hash_str_new("&Lt;", 4);
    values[304] = 8810;

    keys[305]   = hash_str_new("&Map;", 5);
    values[305] = 10501;

    keys[306]   = hash_str_new("&Mcy;", 5);
    values[306] = 1052;

    keys[307]   = hash_str_new("&MediumSpace;", 13);
    values[307] = 8287;

    keys[308]   = hash_str_new("&Mellintrf;", 11);
    values[308] = 8499;

    keys[309]   = hash_str_new("&Mfr;", 5);
    values[309] = 120080;

    keys[310]   = hash_str_new("&MinusPlus;", 11);
    values[310] = 8723;

    keys[311]   = hash_str_new("&Mopf;", 6);
    values[311] = 120132;

    keys[312]   = hash_str_new("&Mscr;", 6);
    values[312] = 8499;

    keys[313]   = hash_str_new("&Mu;", 4);
    values[313] = 924;

    keys[314]   = hash_str_new("&NJcy;", 6);
    values[314] = 1034;

    keys[315]   = hash_str_new("&Nacute;", 8);
    values[315] = 323;

    keys[316]   = hash_str_new("&Ncaron;", 8);
    values[316] = 327;

    keys[317]   = hash_str_new("&Ncedil;", 8);
    values[317] = 325;

    keys[318]   = hash_str_new("&Ncy;", 5);
    values[318] = 1053;

    keys[319]   = hash_str_new("&NegativeMediumSpace;", 21);
    values[319] = 8203;

    keys[320]   = hash_str_new("&NegativeThickSpace;", 20);
    values[320] = 8203;

    keys[321]   = hash_str_new("&NegativeThinSpace;", 19);
    values[321] = 8203;

    keys[322]   = hash_str_new("&NegativeVeryThinSpace;", 23);
    values[322] = 8203;

    keys[323]   = hash_str_new("&NestedGreaterGreater;", 22);
    values[323] = 8811;

    keys[324]   = hash_str_new("&NestedLessLess;", 16);
    values[324] = 8810;

    keys[325]   = hash_str_new("&NewLine;", 9);
    values[325] = 10;

    keys[326]   = hash_str_new("&Nfr;", 5);
    values[326] = 120081;

    keys[327]   = hash_str_new("&NoBreak;", 9);
    values[327] = 8288;

    keys[328]   = hash_str_new("&NonBreakingSpace;", 18);
    values[328] = 160;

    keys[329]   = hash_str_new("&Nopf;", 6);
    values[329] = 8469;

    keys[330]   = hash_str_new("&Not;", 5);
    values[330] = 10988;

    keys[331]   = hash_str_new("&NotCongruent;", 14);
    values[331] = 8802;

    keys[332]   = hash_str_new("&NotCupCap;", 11);
    values[332] = 8813;

    keys[333]   = hash_str_new("&NotDoubleVerticalBar;", 22);
    values[333] = 8742;

    keys[334]   = hash_str_new("&NotElement;", 12);
    values[334] = 8713;

    keys[335]   = hash_str_new("&NotEqual;", 10);
    values[335] = 8800;

    keys[336]   = hash_str_new("&NotEqualTilde;", 15);
    values[336] = 8770;

    keys[337]   = hash_str_new("&NotExists;", 11);
    values[337] = 8708;

    keys[338]   = hash_str_new("&NotGreater;", 12);
    values[338] = 8815;

    keys[339]   = hash_str_new("&NotGreaterEqual;", 17);
    values[339] = 8817;

    keys[340]   = hash_str_new("&NotGreaterFullEqual;", 21);
    values[340] = 8807;

    keys[341]   = hash_str_new("&NotGreaterGreater;", 19);
    values[341] = 8811;

    keys[342]   = hash_str_new("&NotGreaterLess;", 16);
    values[342] = 8825;

    keys[343]   = hash_str_new("&NotGreaterSlantEqual;", 22);
    values[343] = 10878;

    keys[344]   = hash_str_new("&NotGreaterTilde;", 17);
    values[344] = 8821;

    keys[345]   = hash_str_new("&NotHumpDownHump;", 17);
    values[345] = 8782;

    keys[346]   = hash_str_new("&NotHumpEqual;", 14);
    values[346] = 8783;

    keys[347]   = hash_str_new("&NotLeftTriangle;", 17);
    values[347] = 8938;

    keys[348]   = hash_str_new("&NotLeftTriangleBar;", 20);
    values[348] = 10703;

    keys[349]   = hash_str_new("&NotLeftTriangleEqual;", 22);
    values[349] = 8940;

    keys[350]   = hash_str_new("&NotLess;", 9);
    values[350] = 8814;

    keys[351]   = hash_str_new("&NotLessEqual;", 14);
    values[351] = 8816;

    keys[352]   = hash_str_new("&NotLessGreater;", 16);
    values[352] = 8824;

    keys[353]   = hash_str_new("&NotLessLess;", 13);
    values[353] = 8810;

    keys[354]   = hash_str_new("&NotLessSlantEqual;", 19);
    values[354] = 10877;

    keys[355]   = hash_str_new("&NotLessTilde;", 14);
    values[355] = 8820;

    keys[356]   = hash_str_new("&NotNestedGreaterGreater;", 25);
    values[356] = 10914;

    keys[357]   = hash_str_new("&NotNestedLessLess;", 19);
    values[357] = 10913;

    keys[358]   = hash_str_new("&NotPrecedes;", 13);
    values[358] = 8832;

    keys[359]   = hash_str_new("&NotPrecedesEqual;", 18);
    values[359] = 10927;

    keys[360]   = hash_str_new("&NotPrecedesSlantEqual;", 23);
    values[360] = 8928;

    keys[361]   = hash_str_new("&NotReverseElement;", 19);
    values[361] = 8716;

    keys[362]   = hash_str_new("&NotRightTriangle;", 18);
    values[362] = 8939;

    keys[363]   = hash_str_new("&NotRightTriangleBar;", 21);
    values[363] = 10704;

    keys[364]   = hash_str_new("&NotRightTriangleEqual;", 23);
    values[364] = 8941;

    keys[365]   = hash_str_new("&NotSquareSubset;", 17);
    values[365] = 8847;

    keys[366]   = hash_str_new("&NotSquareSubsetEqual;", 22);
    values[366] = 8930;

    keys[367]   = hash_str_new("&NotSquareSuperset;", 19);
    values[367] = 8848;

    keys[368]   = hash_str_new("&NotSquareSupersetEqual;", 24);
    values[368] = 8931;

    keys[369]   = hash_str_new("&NotSubset;", 11);
    values[369] = 8834;

    keys[370]   = hash_str_new("&NotSubsetEqual;", 16);
    values[370] = 8840;

    keys[371]   = hash_str_new("&NotSucceeds;", 13);
    values[371] = 8833;

    keys[372]   = hash_str_new("&NotSucceedsEqual;", 18);
    values[372] = 10928;

    keys[373]   = hash_str_new("&NotSucceedsSlantEqual;", 23);
    values[373] = 8929;

    keys[374]   = hash_str_new("&NotSucceedsTilde;", 18);
    values[374] = 8831;

    keys[375]   = hash_str_new("&NotSuperset;", 13);
    values[375] = 8835;

    keys[376]   = hash_str_new("&NotSupersetEqual;", 18);
    values[376] = 8841;

    keys[377]   = hash_str_new("&NotTilde;", 10);
    values[377] = 8769;

    keys[378]   = hash_str_new("&NotTildeEqual;", 15);
    values[378] = 8772;

    keys[379]   = hash_str_new("&NotTildeFullEqual;", 19);
    values[379] = 8775;

    keys[380]   = hash_str_new("&NotTildeTilde;", 15);
    values[380] = 8777;

    keys[381]   = hash_str_new("&NotVerticalBar;", 16);
    values[381] = 8740;

    keys[382]   = hash_str_new("&Nscr;", 6);
    values[382] = 119977;

    keys[383]   = hash_str_new("&Ntilde", 7);
    values[383] = 209;

    keys[384]   = hash_str_new("&Ntilde;", 8);
    values[384] = 209;

    keys[385]   = hash_str_new("&Nu;", 4);
    values[385] = 925;

    keys[386]   = hash_str_new("&OElig;", 7);
    values[386] = 338;

    keys[387]   = hash_str_new("&Oacute", 7);
    values[387] = 211;

    keys[388]   = hash_str_new("&Oacute;", 8);
    values[388] = 211;

    keys[389]   = hash_str_new("&Ocirc", 6);
    values[389] = 212;

    keys[390]   = hash_str_new("&Ocirc;", 7);
    values[390] = 212;

    keys[391]   = hash_str_new("&Ocy;", 5);
    values[391] = 1054;

    keys[392]   = hash_str_new("&Odblac;", 8);
    values[392] = 336;

    keys[393]   = hash_str_new("&Ofr;", 5);
    values[393] = 120082;

    keys[394]   = hash_str_new("&Ograve", 7);
    values[394] = 210;

    keys[395]   = hash_str_new("&Ograve;", 8);
    values[395] = 210;

    keys[396]   = hash_str_new("&Omacr;", 7);
    values[396] = 332;

    keys[397]   = hash_str_new("&Omega;", 7);
    values[397] = 937;

    keys[398]   = hash_str_new("&Omicron;", 9);
    values[398] = 927;

    keys[399]   = hash_str_new("&Oopf;", 6);
    values[399] = 120134;

    keys[400]   = hash_str_new("&OpenCurlyDoubleQuote;", 22);
    values[400] = 8220;

    keys[401]   = hash_str_new("&OpenCurlyQuote;", 16);
    values[401] = 8216;

    keys[402]   = hash_str_new("&Or;", 4);
    values[402] = 10836;

    keys[403]   = hash_str_new("&Oscr;", 6);
    values[403] = 119978;

    keys[404]   = hash_str_new("&Oslash", 7);
    values[404] = 216;

    keys[405]   = hash_str_new("&Oslash;", 8);
    values[405] = 216;

    keys[406]   = hash_str_new("&Otilde", 7);
    values[406] = 213;

    keys[407]   = hash_str_new("&Otilde;", 8);
    values[407] = 213;

    keys[408]   = hash_str_new("&Otimes;", 8);
    values[408] = 10807;

    keys[409]   = hash_str_new("&Ouml", 5);
    values[409] = 214;

    keys[410]   = hash_str_new("&Ouml;", 6);
    values[410] = 214;

    keys[411]   = hash_str_new("&OverBar;", 9);
    values[411] = 8254;

    keys[412]   = hash_str_new("&OverBrace;", 11);
    values[412] = 9182;

    keys[413]   = hash_str_new("&OverBracket;", 13);
    values[413] = 9140;

    keys[414]   = hash_str_new("&OverParenthesis;", 17);
    values[414] = 9180;

    keys[415]   = hash_str_new("&PartialD;", 10);
    values[415] = 8706;

    keys[416]   = hash_str_new("&Pcy;", 5);
    values[416] = 1055;

    keys[417]   = hash_str_new("&Pfr;", 5);
    values[417] = 120083;

    keys[418]   = hash_str_new("&Phi;", 5);
    values[418] = 934;

    keys[419]   = hash_str_new("&Pi;", 4);
    values[419] = 928;

    keys[420]   = hash_str_new("&PlusMinus;", 11);
    values[420] = 177;

    keys[421]   = hash_str_new("&Poincareplane;", 15);
    values[421] = 8460;

    keys[422]   = hash_str_new("&Popf;", 6);
    values[422] = 8473;

    keys[423]   = hash_str_new("&Pr;", 4);
    values[423] = 10939;

    keys[424]   = hash_str_new("&Precedes;", 10);
    values[424] = 8826;

    keys[425]   = hash_str_new("&PrecedesEqual;", 15);
    values[425] = 10927;

    keys[426]   = hash_str_new("&PrecedesSlantEqual;", 20);
    values[426] = 8828;

    keys[427]   = hash_str_new("&PrecedesTilde;", 15);
    values[427] = 8830;

    keys[428]   = hash_str_new("&Prime;", 7);
    values[428] = 8243;

    keys[429]   = hash_str_new("&Product;", 9);
    values[429] = 8719;

    keys[430]   = hash_str_new("&Proportion;", 12);
    values[430] = 8759;

    keys[431]   = hash_str_new("&Proportional;", 14);
    values[431] = 8733;

    keys[432]   = hash_str_new("&Pscr;", 6);
    values[432] = 119979;

    keys[433]   = hash_str_new("&Psi;", 5);
    values[433] = 936;

    keys[434]   = hash_str_new("&QUOT", 5);
    values[434] = 34;

    keys[435]   = hash_str_new("&QUOT;", 6);
    values[435] = 34;

    keys[436]   = hash_str_new("&Qfr;", 5);
    values[436] = 120084;

    keys[437]   = hash_str_new("&Qopf;", 6);
    values[437] = 8474;

    keys[438]   = hash_str_new("&Qscr;", 6);
    values[438] = 119980;

    keys[439]   = hash_str_new("&RBarr;", 7);
    values[439] = 10512;

    keys[440]   = hash_str_new("&REG", 4);
    values[440] = 174;

    keys[441]   = hash_str_new("&REG;", 5);
    values[441] = 174;

    keys[442]   = hash_str_new("&Racute;", 8);
    values[442] = 340;

    keys[443]   = hash_str_new("&Rang;", 6);
    values[443] = 10219;

    keys[444]   = hash_str_new("&Rarr;", 6);
    values[444] = 8608;

    keys[445]   = hash_str_new("&Rarrtl;", 8);
    values[445] = 10518;

    keys[446]   = hash_str_new("&Rcaron;", 8);
    values[446] = 344;

    keys[447]   = hash_str_new("&Rcedil;", 8);
    values[447] = 342;

    keys[448]   = hash_str_new("&Rcy;", 5);
    values[448] = 1056;

    keys[449]   = hash_str_new("&Re;", 4);
    values[449] = 8476;

    keys[450]   = hash_str_new("&ReverseElement;", 16);
    values[450] = 8715;

    keys[451]   = hash_str_new("&ReverseEquilibrium;", 20);
    values[451] = 8651;

    keys[452]   = hash_str_new("&ReverseUpEquilibrium;", 22);
    values[452] = 10607;

    keys[453]   = hash_str_new("&Rfr;", 5);
    values[453] = 8476;

    keys[454]   = hash_str_new("&Rho;", 5);
    values[454] = 929;

    keys[455]   = hash_str_new("&RightAngleBracket;", 19);
    values[455] = 10217;

    keys[456]   = hash_str_new("&RightArrow;", 12);
    values[456] = 8594;

    keys[457]   = hash_str_new("&RightArrowBar;", 15);
    values[457] = 8677;

    keys[458]   = hash_str_new("&RightArrowLeftArrow;", 21);
    values[458] = 8644;

    keys[459]   = hash_str_new("&RightCeiling;", 14);
    values[459] = 8969;

    keys[460]   = hash_str_new("&RightDoubleBracket;", 20);
    values[460] = 10215;

    keys[461]   = hash_str_new("&RightDownTeeVector;", 20);
    values[461] = 10589;

    keys[462]   = hash_str_new("&RightDownVector;", 17);
    values[462] = 8642;

    keys[463]   = hash_str_new("&RightDownVectorBar;", 20);
    values[463] = 10581;

    keys[464]   = hash_str_new("&RightFloor;", 12);
    values[464] = 8971;

    keys[465]   = hash_str_new("&RightTee;", 10);
    values[465] = 8866;

    keys[466]   = hash_str_new("&RightTeeArrow;", 15);
    values[466] = 8614;

    keys[467]   = hash_str_new("&RightTeeVector;", 16);
    values[467] = 10587;

    keys[468]   = hash_str_new("&RightTriangle;", 15);
    values[468] = 8883;

    keys[469]   = hash_str_new("&RightTriangleBar;", 18);
    values[469] = 10704;

    keys[470]   = hash_str_new("&RightTriangleEqual;", 20);
    values[470] = 8885;

    keys[471]   = hash_str_new("&RightUpDownVector;", 19);
    values[471] = 10575;

    keys[472]   = hash_str_new("&RightUpTeeVector;", 18);
    values[472] = 10588;

    keys[473]   = hash_str_new("&RightUpVector;", 15);
    values[473] = 8638;

    keys[474]   = hash_str_new("&RightUpVectorBar;", 18);
    values[474] = 10580;

    keys[475]   = hash_str_new("&RightVector;", 13);
    values[475] = 8640;

    keys[476]   = hash_str_new("&RightVectorBar;", 16);
    values[476] = 10579;

    keys[477]   = hash_str_new("&Rightarrow;", 12);
    values[477] = 8658;

    keys[478]   = hash_str_new("&Ropf;", 6);
    values[478] = 8477;

    keys[479]   = hash_str_new("&RoundImplies;", 14);
    values[479] = 10608;

    keys[480]   = hash_str_new("&Rrightarrow;", 13);
    values[480] = 8667;

    keys[481]   = hash_str_new("&Rscr;", 6);
    values[481] = 8475;

    keys[482]   = hash_str_new("&Rsh;", 5);
    values[482] = 8625;

    keys[483]   = hash_str_new("&RuleDelayed;", 13);
    values[483] = 10740;

    keys[484]   = hash_str_new("&SHCHcy;", 8);
    values[484] = 1065;

    keys[485]   = hash_str_new("&SHcy;", 6);
    values[485] = 1064;

    keys[486]   = hash_str_new("&SOFTcy;", 8);
    values[486] = 1068;

    keys[487]   = hash_str_new("&Sacute;", 8);
    values[487] = 346;

    keys[488]   = hash_str_new("&Sc;", 4);
    values[488] = 10940;

    keys[489]   = hash_str_new("&Scaron;", 8);
    values[489] = 352;

    keys[490]   = hash_str_new("&Scedil;", 8);
    values[490] = 350;

    keys[491]   = hash_str_new("&Scirc;", 7);
    values[491] = 348;

    keys[492]   = hash_str_new("&Scy;", 5);
    values[492] = 1057;

    keys[493]   = hash_str_new("&Sfr;", 5);
    values[493] = 120086;

    keys[494]   = hash_str_new("&ShortDownArrow;", 16);
    values[494] = 8595;

    keys[495]   = hash_str_new("&ShortLeftArrow;", 16);
    values[495] = 8592;

    keys[496]   = hash_str_new("&ShortRightArrow;", 17);
    values[496] = 8594;

    keys[497]   = hash_str_new("&ShortUpArrow;", 14);
    values[497] = 8593;

    keys[498]   = hash_str_new("&Sigma;", 7);
    values[498] = 931;

    keys[499]   = hash_str_new("&SmallCircle;", 13);
    values[499] = 8728;

    keys[500]   = hash_str_new("&Sopf;", 6);
    values[500] = 120138;

    keys[501]   = hash_str_new("&Sqrt;", 6);
    values[501] = 8730;

    keys[502]   = hash_str_new("&Square;", 8);
    values[502] = 9633;

    keys[503]   = hash_str_new("&SquareIntersection;", 20);
    values[503] = 8851;

    keys[504]   = hash_str_new("&SquareSubset;", 14);
    values[504] = 8847;

    keys[505]   = hash_str_new("&SquareSubsetEqual;", 19);
    values[505] = 8849;

    keys[506]   = hash_str_new("&SquareSuperset;", 16);
    values[506] = 8848;

    keys[507]   = hash_str_new("&SquareSupersetEqual;", 21);
    values[507] = 8850;

    keys[508]   = hash_str_new("&SquareUnion;", 13);
    values[508] = 8852;

    keys[509]   = hash_str_new("&Sscr;", 6);
    values[509] = 119982;

    keys[510]   = hash_str_new("&Star;", 6);
    values[510] = 8902;

    keys[511]   = hash_str_new("&Sub;", 5);
    values[511] = 8912;

    keys[512]   = hash_str_new("&Subset;", 8);
    values[512] = 8912;

    keys[513]   = hash_str_new("&SubsetEqual;", 13);
    values[513] = 8838;

    keys[514]   = hash_str_new("&Succeeds;", 10);
    values[514] = 8827;

    keys[515]   = hash_str_new("&SucceedsEqual;", 15);
    values[515] = 10928;

    keys[516]   = hash_str_new("&SucceedsSlantEqual;", 20);
    values[516] = 8829;

    keys[517]   = hash_str_new("&SucceedsTilde;", 15);
    values[517] = 8831;

    keys[518]   = hash_str_new("&SuchThat;", 10);
    values[518] = 8715;

    keys[519]   = hash_str_new("&Sum;", 5);
    values[519] = 8721;

    keys[520]   = hash_str_new("&Sup;", 5);
    values[520] = 8913;

    keys[521]   = hash_str_new("&Superset;", 10);
    values[521] = 8835;

    keys[522]   = hash_str_new("&SupersetEqual;", 15);
    values[522] = 8839;

    keys[523]   = hash_str_new("&Supset;", 8);
    values[523] = 8913;

    keys[524]   = hash_str_new("&THORN", 6);
    values[524] = 222;

    keys[525]   = hash_str_new("&THORN;", 7);
    values[525] = 222;

    keys[526]   = hash_str_new("&TRADE;", 7);
    values[526] = 8482;

    keys[527]   = hash_str_new("&TSHcy;", 7);
    values[527] = 1035;

    keys[528]   = hash_str_new("&TScy;", 6);
    values[528] = 1062;

    keys[529]   = hash_str_new("&Tab;", 5);
    values[529] = 9;

    keys[530]   = hash_str_new("&Tau;", 5);
    values[530] = 932;

    keys[531]   = hash_str_new("&Tcaron;", 8);
    values[531] = 356;

    keys[532]   = hash_str_new("&Tcedil;", 8);
    values[532] = 354;

    keys[533]   = hash_str_new("&Tcy;", 5);
    values[533] = 1058;

    keys[534]   = hash_str_new("&Tfr;", 5);
    values[534] = 120087;

    keys[535]   = hash_str_new("&Therefore;", 11);
    values[535] = 8756;

    keys[536]   = hash_str_new("&Theta;", 7);
    values[536] = 920;

    keys[537]   = hash_str_new("&ThickSpace;", 12);
    values[537] = 8287;

    keys[538]   = hash_str_new("&ThinSpace;", 11);
    values[538] = 8201;

    keys[539]   = hash_str_new("&Tilde;", 7);
    values[539] = 8764;

    keys[540]   = hash_str_new("&TildeEqual;", 12);
    values[540] = 8771;

    keys[541]   = hash_str_new("&TildeFullEqual;", 16);
    values[541] = 8773;

    keys[542]   = hash_str_new("&TildeTilde;", 12);
    values[542] = 8776;

    keys[543]   = hash_str_new("&Topf;", 6);
    values[543] = 120139;

    keys[544]   = hash_str_new("&TripleDot;", 11);
    values[544] = 8411;

    keys[545]   = hash_str_new("&Tscr;", 6);
    values[545] = 119983;

    keys[546]   = hash_str_new("&Tstrok;", 8);
    values[546] = 358;

    keys[547]   = hash_str_new("&Uacute", 7);
    values[547] = 218;

    keys[548]   = hash_str_new("&Uacute;", 8);
    values[548] = 218;

    keys[549]   = hash_str_new("&Uarr;", 6);
    values[549] = 8607;

    keys[550]   = hash_str_new("&Uarrocir;", 10);
    values[550] = 10569;

    keys[551]   = hash_str_new("&Ubrcy;", 7);
    values[551] = 1038;

    keys[552]   = hash_str_new("&Ubreve;", 8);
    values[552] = 364;

    keys[553]   = hash_str_new("&Ucirc", 6);
    values[553] = 219;

    keys[554]   = hash_str_new("&Ucirc;", 7);
    values[554] = 219;

    keys[555]   = hash_str_new("&Ucy;", 5);
    values[555] = 1059;

    keys[556]   = hash_str_new("&Udblac;", 8);
    values[556] = 368;

    keys[557]   = hash_str_new("&Ufr;", 5);
    values[557] = 120088;

    keys[558]   = hash_str_new("&Ugrave", 7);
    values[558] = 217;

    keys[559]   = hash_str_new("&Ugrave;", 8);
    values[559] = 217;

    keys[560]   = hash_str_new("&Umacr;", 7);
    values[560] = 362;

    keys[561]   = hash_str_new("&UnderBar;", 10);
    values[561] = 95;

    keys[562]   = hash_str_new("&UnderBrace;", 12);
    values[562] = 9183;

    keys[563]   = hash_str_new("&UnderBracket;", 14);
    values[563] = 9141;

    keys[564]   = hash_str_new("&UnderParenthesis;", 18);
    values[564] = 9181;

    keys[565]   = hash_str_new("&Union;", 7);
    values[565] = 8899;

    keys[566]   = hash_str_new("&UnionPlus;", 11);
    values[566] = 8846;

    keys[567]   = hash_str_new("&Uogon;", 7);
    values[567] = 370;

    keys[568]   = hash_str_new("&Uopf;", 6);
    values[568] = 120140;

    keys[569]   = hash_str_new("&UpArrow;", 9);
    values[569] = 8593;

    keys[570]   = hash_str_new("&UpArrowBar;", 12);
    values[570] = 10514;

    keys[571]   = hash_str_new("&UpArrowDownArrow;", 18);
    values[571] = 8645;

    keys[572]   = hash_str_new("&UpDownArrow;", 13);
    values[572] = 8597;

    keys[573]   = hash_str_new("&UpEquilibrium;", 15);
    values[573] = 10606;

    keys[574]   = hash_str_new("&UpTee;", 7);
    values[574] = 8869;

    keys[575]   = hash_str_new("&UpTeeArrow;", 12);
    values[575] = 8613;

    keys[576]   = hash_str_new("&Uparrow;", 9);
    values[576] = 8657;

    keys[577]   = hash_str_new("&Updownarrow;", 13);
    values[577] = 8661;

    keys[578]   = hash_str_new("&UpperLeftArrow;", 16);
    values[578] = 8598;

    keys[579]   = hash_str_new("&UpperRightArrow;", 17);
    values[579] = 8599;

    keys[580]   = hash_str_new("&Upsi;", 6);
    values[580] = 978;

    keys[581]   = hash_str_new("&Upsilon;", 9);
    values[581] = 933;

    keys[582]   = hash_str_new("&Uring;", 7);
    values[582] = 366;

    keys[583]   = hash_str_new("&Uscr;", 6);
    values[583] = 119984;

    keys[584]   = hash_str_new("&Utilde;", 8);
    values[584] = 360;

    keys[585]   = hash_str_new("&Uuml", 5);
    values[585] = 220;

    keys[586]   = hash_str_new("&Uuml;", 6);
    values[586] = 220;

    keys[587]   = hash_str_new("&VDash;", 7);
    values[587] = 8875;

    keys[588]   = hash_str_new("&Vbar;", 6);
    values[588] = 10987;

    keys[589]   = hash_str_new("&Vcy;", 5);
    values[589] = 1042;

    keys[590]   = hash_str_new("&Vdash;", 7);
    values[590] = 8873;

    keys[591]   = hash_str_new("&Vdashl;", 8);
    values[591] = 10982;

    keys[592]   = hash_str_new("&Vee;", 5);
    values[592] = 8897;

    keys[593]   = hash_str_new("&Verbar;", 8);
    values[593] = 8214;

    keys[594]   = hash_str_new("&Vert;", 6);
    values[594] = 8214;

    keys[595]   = hash_str_new("&VerticalBar;", 13);
    values[595] = 8739;

    keys[596]   = hash_str_new("&VerticalLine;", 14);
    values[596] = 124;

    keys[597]   = hash_str_new("&VerticalSeparator;", 19);
    values[597] = 10072;

    keys[598]   = hash_str_new("&VerticalTilde;", 15);
    values[598] = 8768;

    keys[599]   = hash_str_new("&VeryThinSpace;", 15);
    values[599] = 8202;

    keys[600]   = hash_str_new("&Vfr;", 5);
    values[600] = 120089;

    keys[601]   = hash_str_new("&Vopf;", 6);
    values[601] = 120141;

    keys[602]   = hash_str_new("&Vscr;", 6);
    values[602] = 119985;

    keys[603]   = hash_str_new("&Vvdash;", 8);
    values[603] = 8874;

    keys[604]   = hash_str_new("&Wcirc;", 7);
    values[604] = 372;

    keys[605]   = hash_str_new("&Wedge;", 7);
    values[605] = 8896;

    keys[606]   = hash_str_new("&Wfr;", 5);
    values[606] = 120090;

    keys[607]   = hash_str_new("&Wopf;", 6);
    values[607] = 120142;

    keys[608]   = hash_str_new("&Wscr;", 6);
    values[608] = 119986;

    keys[609]   = hash_str_new("&Xfr;", 5);
    values[609] = 120091;

    keys[610]   = hash_str_new("&Xi;", 4);
    values[610] = 926;

    keys[611]   = hash_str_new("&Xopf;", 6);
    values[611] = 120143;

    keys[612]   = hash_str_new("&Xscr;", 6);
    values[612] = 119987;

    keys[613]   = hash_str_new("&YAcy;", 6);
    values[613] = 1071;

    keys[614]   = hash_str_new("&YIcy;", 6);
    values[614] = 1031;

    keys[615]   = hash_str_new("&YUcy;", 6);
    values[615] = 1070;

    keys[616]   = hash_str_new("&Yacute", 7);
    values[616] = 221;

    keys[617]   = hash_str_new("&Yacute;", 8);
    values[617] = 221;

    keys[618]   = hash_str_new("&Ycirc;", 7);
    values[618] = 374;

    keys[619]   = hash_str_new("&Ycy;", 5);
    values[619] = 1067;

    keys[620]   = hash_str_new("&Yfr;", 5);
    values[620] = 120092;

    keys[621]   = hash_str_new("&Yopf;", 6);
    values[621] = 120144;

    keys[622]   = hash_str_new("&Yscr;", 6);
    values[622] = 119988;

    keys[623]   = hash_str_new("&Yuml;", 6);
    values[623] = 376;

    keys[624]   = hash_str_new("&ZHcy;", 6);
    values[624] = 1046;

    keys[625]   = hash_str_new("&Zacute;", 8);
    values[625] = 377;

    keys[626]   = hash_str_new("&Zcaron;", 8);
    values[626] = 381;

    keys[627]   = hash_str_new("&Zcy;", 5);
    values[627] = 1047;

    keys[628]   = hash_str_new("&Zdot;", 6);
    values[628] = 379;

    keys[629]   = hash_str_new("&ZeroWidthSpace;", 16);
    values[629] = 8203;

    keys[630]   = hash_str_new("&Zeta;", 6);
    values[630] = 918;

    keys[631]   = hash_str_new("&Zfr;", 5);
    values[631] = 8488;

    keys[632]   = hash_str_new("&Zopf;", 6);
    values[632] = 8484;

    keys[633]   = hash_str_new("&Zscr;", 6);
    values[633] = 119989;

    keys[634]   = hash_str_new("&aacute", 7);
    values[634] = 225;

    keys[635]   = hash_str_new("&aacute;", 8);
    values[635] = 225;

    keys[636]   = hash_str_new("&abreve;", 8);
    values[636] = 259;

    keys[637]   = hash_str_new("&ac;", 4);
    values[637] = 8766;

    keys[638]   = hash_str_new("&acE;", 5);
    values[638] = 8766;

    keys[639]   = hash_str_new("&acd;", 5);
    values[639] = 8767;

    keys[640]   = hash_str_new("&acirc", 6);
    values[640] = 226;

    keys[641]   = hash_str_new("&acirc;", 7);
    values[641] = 226;

    keys[642]   = hash_str_new("&acute", 6);
    values[642] = 180;

    keys[643]   = hash_str_new("&acute;", 7);
    values[643] = 180;

    keys[644]   = hash_str_new("&acy;", 5);
    values[644] = 1072;

    keys[645]   = hash_str_new("&aelig", 6);
    values[645] = 230;

    keys[646]   = hash_str_new("&aelig;", 7);
    values[646] = 230;

    keys[647]   = hash_str_new("&af;", 4);
    values[647] = 8289;

    keys[648]   = hash_str_new("&afr;", 5);
    values[648] = 120094;

    keys[649]   = hash_str_new("&agrave", 7);
    values[649] = 224;

    keys[650]   = hash_str_new("&agrave;", 8);
    values[650] = 224;

    keys[651]   = hash_str_new("&alefsym;", 9);
    values[651] = 8501;

    keys[652]   = hash_str_new("&aleph;", 7);
    values[652] = 8501;

    keys[653]   = hash_str_new("&alpha;", 7);
    values[653] = 945;

    keys[654]   = hash_str_new("&amacr;", 7);
    values[654] = 257;

    keys[655]   = hash_str_new("&amalg;", 7);
    values[655] = 10815;

    keys[656]   = hash_str_new("&amp", 4);
    values[656] = 38;

    keys[657]   = hash_str_new("&amp;", 5);
    values[657] = 38;

    keys[658]   = hash_str_new("&and;", 5);
    values[658] = 8743;

    keys[659]   = hash_str_new("&andand;", 8);
    values[659] = 10837;

    keys[660]   = hash_str_new("&andd;", 6);
    values[660] = 10844;

    keys[661]   = hash_str_new("&andslope;", 10);
    values[661] = 10840;

    keys[662]   = hash_str_new("&andv;", 6);
    values[662] = 10842;

    keys[663]   = hash_str_new("&ang;", 5);
    values[663] = 8736;

    keys[664]   = hash_str_new("&ange;", 6);
    values[664] = 10660;

    keys[665]   = hash_str_new("&angle;", 7);
    values[665] = 8736;

    keys[666]   = hash_str_new("&angmsd;", 8);
    values[666] = 8737;

    keys[667]   = hash_str_new("&angmsdaa;", 10);
    values[667] = 10664;

    keys[668]   = hash_str_new("&angmsdab;", 10);
    values[668] = 10665;

    keys[669]   = hash_str_new("&angmsdac;", 10);
    values[669] = 10666;

    keys[670]   = hash_str_new("&angmsdad;", 10);
    values[670] = 10667;

    keys[671]   = hash_str_new("&angmsdae;", 10);
    values[671] = 10668;

    keys[672]   = hash_str_new("&angmsdaf;", 10);
    values[672] = 10669;

    keys[673]   = hash_str_new("&angmsdag;", 10);
    values[673] = 10670;

    keys[674]   = hash_str_new("&angmsdah;", 10);
    values[674] = 10671;

    keys[675]   = hash_str_new("&angrt;", 7);
    values[675] = 8735;

    keys[676]   = hash_str_new("&angrtvb;", 9);
    values[676] = 8894;

    keys[677]   = hash_str_new("&angrtvbd;", 10);
    values[677] = 10653;

    keys[678]   = hash_str_new("&angsph;", 8);
    values[678] = 8738;

    keys[679]   = hash_str_new("&angst;", 7);
    values[679] = 197;

    keys[680]   = hash_str_new("&angzarr;", 9);
    values[680] = 9084;

    keys[681]   = hash_str_new("&aogon;", 7);
    values[681] = 261;

    keys[682]   = hash_str_new("&aopf;", 6);
    values[682] = 120146;

    keys[683]   = hash_str_new("&ap;", 4);
    values[683] = 8776;

    keys[684]   = hash_str_new("&apE;", 5);
    values[684] = 10864;

    keys[685]   = hash_str_new("&apacir;", 8);
    values[685] = 10863;

    keys[686]   = hash_str_new("&ape;", 5);
    values[686] = 8778;

    keys[687]   = hash_str_new("&apid;", 6);
    values[687] = 8779;

    keys[688]   = hash_str_new("&apos;", 6);
    values[688] = 39;

    keys[689]   = hash_str_new("&approx;", 8);
    values[689] = 8776;

    keys[690]   = hash_str_new("&approxeq;", 10);
    values[690] = 8778;

    keys[691]   = hash_str_new("&aring", 6);
    values[691] = 229;

    keys[692]   = hash_str_new("&aring;", 7);
    values[692] = 229;

    keys[693]   = hash_str_new("&ascr;", 6);
    values[693] = 119990;

    keys[694]   = hash_str_new("&ast;", 5);
    values[694] = 42;

    keys[695]   = hash_str_new("&asymp;", 7);
    values[695] = 8776;

    keys[696]   = hash_str_new("&asympeq;", 9);
    values[696] = 8781;

    keys[697]   = hash_str_new("&atilde", 7);
    values[697] = 227;

    keys[698]   = hash_str_new("&atilde;", 8);
    values[698] = 227;

    keys[699]   = hash_str_new("&auml", 5);
    values[699] = 228;

    keys[700]   = hash_str_new("&auml;", 6);
    values[700] = 228;

    keys[701]   = hash_str_new("&awconint;", 10);
    values[701] = 8755;

    keys[702]   = hash_str_new("&awint;", 7);
    values[702] = 10769;

    keys[703]   = hash_str_new("&bNot;", 6);
    values[703] = 10989;

    keys[704]   = hash_str_new("&backcong;", 10);
    values[704] = 8780;

    keys[705]   = hash_str_new("&backepsilon;", 13);
    values[705] = 1014;

    keys[706]   = hash_str_new("&backprime;", 11);
    values[706] = 8245;

    keys[707]   = hash_str_new("&backsim;", 9);
    values[707] = 8765;

    keys[708]   = hash_str_new("&backsimeq;", 11);
    values[708] = 8909;

    keys[709]   = hash_str_new("&barvee;", 8);
    values[709] = 8893;

    keys[710]   = hash_str_new("&barwed;", 8);
    values[710] = 8965;

    keys[711]   = hash_str_new("&barwedge;", 10);
    values[711] = 8965;

    keys[712]   = hash_str_new("&bbrk;", 6);
    values[712] = 9141;

    keys[713]   = hash_str_new("&bbrktbrk;", 10);
    values[713] = 9142;

    keys[714]   = hash_str_new("&bcong;", 7);
    values[714] = 8780;

    keys[715]   = hash_str_new("&bcy;", 5);
    values[715] = 1073;

    keys[716]   = hash_str_new("&bdquo;", 7);
    values[716] = 8222;

    keys[717]   = hash_str_new("&becaus;", 8);
    values[717] = 8757;

    keys[718]   = hash_str_new("&because;", 9);
    values[718] = 8757;

    keys[719]   = hash_str_new("&bemptyv;", 9);
    values[719] = 10672;

    keys[720]   = hash_str_new("&bepsi;", 7);
    values[720] = 1014;

    keys[721]   = hash_str_new("&bernou;", 8);
    values[721] = 8492;

    keys[722]   = hash_str_new("&beta;", 6);
    values[722] = 946;

    keys[723]   = hash_str_new("&beth;", 6);
    values[723] = 8502;

    keys[724]   = hash_str_new("&between;", 9);
    values[724] = 8812;

    keys[725]   = hash_str_new("&bfr;", 5);
    values[725] = 120095;

    keys[726]   = hash_str_new("&bigcap;", 8);
    values[726] = 8898;

    keys[727]   = hash_str_new("&bigcirc;", 9);
    values[727] = 9711;

    keys[728]   = hash_str_new("&bigcup;", 8);
    values[728] = 8899;

    keys[729]   = hash_str_new("&bigodot;", 9);
    values[729] = 10752;

    keys[730]   = hash_str_new("&bigoplus;", 10);
    values[730] = 10753;

    keys[731]   = hash_str_new("&bigotimes;", 11);
    values[731] = 10754;

    keys[732]   = hash_str_new("&bigsqcup;", 10);
    values[732] = 10758;

    keys[733]   = hash_str_new("&bigstar;", 9);
    values[733] = 9733;

    keys[734]   = hash_str_new("&bigtriangledown;", 17);
    values[734] = 9661;

    keys[735]   = hash_str_new("&bigtriangleup;", 15);
    values[735] = 9651;

    keys[736]   = hash_str_new("&biguplus;", 10);
    values[736] = 10756;

    keys[737]   = hash_str_new("&bigvee;", 8);
    values[737] = 8897;

    keys[738]   = hash_str_new("&bigwedge;", 10);
    values[738] = 8896;

    keys[739]   = hash_str_new("&bkarow;", 8);
    values[739] = 10509;

    keys[740]   = hash_str_new("&blacklozenge;", 14);
    values[740] = 10731;

    keys[741]   = hash_str_new("&blacksquare;", 13);
    values[741] = 9642;

    keys[742]   = hash_str_new("&blacktriangle;", 15);
    values[742] = 9652;

    keys[743]   = hash_str_new("&blacktriangledown;", 19);
    values[743] = 9662;

    keys[744]   = hash_str_new("&blacktriangleleft;", 19);
    values[744] = 9666;

    keys[745]   = hash_str_new("&blacktriangleright;", 20);
    values[745] = 9656;

    keys[746]   = hash_str_new("&blank;", 7);
    values[746] = 9251;

    keys[747]   = hash_str_new("&blk12;", 7);
    values[747] = 9618;

    keys[748]   = hash_str_new("&blk14;", 7);
    values[748] = 9617;

    keys[749]   = hash_str_new("&blk34;", 7);
    values[749] = 9619;

    keys[750]   = hash_str_new("&block;", 7);
    values[750] = 9608;

    keys[751]   = hash_str_new("&bne;", 5);
    values[751] = 61;

    keys[752]   = hash_str_new("&bnequiv;", 9);
    values[752] = 8801;

    keys[753]   = hash_str_new("&bnot;", 6);
    values[753] = 8976;

    keys[754]   = hash_str_new("&bopf;", 6);
    values[754] = 120147;

    keys[755]   = hash_str_new("&bot;", 5);
    values[755] = 8869;

    keys[756]   = hash_str_new("&bottom;", 8);
    values[756] = 8869;

    keys[757]   = hash_str_new("&bowtie;", 8);
    values[757] = 8904;

    keys[758]   = hash_str_new("&boxDL;", 7);
    values[758] = 9559;

    keys[759]   = hash_str_new("&boxDR;", 7);
    values[759] = 9556;

    keys[760]   = hash_str_new("&boxDl;", 7);
    values[760] = 9558;

    keys[761]   = hash_str_new("&boxDr;", 7);
    values[761] = 9555;

    keys[762]   = hash_str_new("&boxH;", 6);
    values[762] = 9552;

    keys[763]   = hash_str_new("&boxHD;", 7);
    values[763] = 9574;

    keys[764]   = hash_str_new("&boxHU;", 7);
    values[764] = 9577;

    keys[765]   = hash_str_new("&boxHd;", 7);
    values[765] = 9572;

    keys[766]   = hash_str_new("&boxHu;", 7);
    values[766] = 9575;

    keys[767]   = hash_str_new("&boxUL;", 7);
    values[767] = 9565;

    keys[768]   = hash_str_new("&boxUR;", 7);
    values[768] = 9562;

    keys[769]   = hash_str_new("&boxUl;", 7);
    values[769] = 9564;

    keys[770]   = hash_str_new("&boxUr;", 7);
    values[770] = 9561;

    keys[771]   = hash_str_new("&boxV;", 6);
    values[771] = 9553;

    keys[772]   = hash_str_new("&boxVH;", 7);
    values[772] = 9580;

    keys[773]   = hash_str_new("&boxVL;", 7);
    values[773] = 9571;

    keys[774]   = hash_str_new("&boxVR;", 7);
    values[774] = 9568;

    keys[775]   = hash_str_new("&boxVh;", 7);
    values[775] = 9579;

    keys[776]   = hash_str_new("&boxVl;", 7);
    values[776] = 9570;

    keys[777]   = hash_str_new("&boxVr;", 7);
    values[777] = 9567;

    keys[778]   = hash_str_new("&boxbox;", 8);
    values[778] = 10697;

    keys[779]   = hash_str_new("&boxdL;", 7);
    values[779] = 9557;

    keys[780]   = hash_str_new("&boxdR;", 7);
    values[780] = 9554;

    keys[781]   = hash_str_new("&boxdl;", 7);
    values[781] = 9488;

    keys[782]   = hash_str_new("&boxdr;", 7);
    values[782] = 9484;

    keys[783]   = hash_str_new("&boxh;", 6);
    values[783] = 9472;

    keys[784]   = hash_str_new("&boxhD;", 7);
    values[784] = 9573;

    keys[785]   = hash_str_new("&boxhU;", 7);
    values[785] = 9576;

    keys[786]   = hash_str_new("&boxhd;", 7);
    values[786] = 9516;

    keys[787]   = hash_str_new("&boxhu;", 7);
    values[787] = 9524;

    keys[788]   = hash_str_new("&boxminus;", 10);
    values[788] = 8863;

    keys[789]   = hash_str_new("&boxplus;", 9);
    values[789] = 8862;

    keys[790]   = hash_str_new("&boxtimes;", 10);
    values[790] = 8864;

    keys[791]   = hash_str_new("&boxuL;", 7);
    values[791] = 9563;

    keys[792]   = hash_str_new("&boxuR;", 7);
    values[792] = 9560;

    keys[793]   = hash_str_new("&boxul;", 7);
    values[793] = 9496;

    keys[794]   = hash_str_new("&boxur;", 7);
    values[794] = 9492;

    keys[795]   = hash_str_new("&boxv;", 6);
    values[795] = 9474;

    keys[796]   = hash_str_new("&boxvH;", 7);
    values[796] = 9578;

    keys[797]   = hash_str_new("&boxvL;", 7);
    values[797] = 9569;

    keys[798]   = hash_str_new("&boxvR;", 7);
    values[798] = 9566;

    keys[799]   = hash_str_new("&boxvh;", 7);
    values[799] = 9532;

    keys[800]   = hash_str_new("&boxvl;", 7);
    values[800] = 9508;

    keys[801]   = hash_str_new("&boxvr;", 7);
    values[801] = 9500;

    keys[802]   = hash_str_new("&bprime;", 8);
    values[802] = 8245;

    keys[803]   = hash_str_new("&breve;", 7);
    values[803] = 728;

    keys[804]   = hash_str_new("&brvbar", 7);
    values[804] = 166;

    keys[805]   = hash_str_new("&brvbar;", 8);
    values[805] = 166;

    keys[806]   = hash_str_new("&bscr;", 6);
    values[806] = 119991;

    keys[807]   = hash_str_new("&bsemi;", 7);
    values[807] = 8271;

    keys[808]   = hash_str_new("&bsim;", 6);
    values[808] = 8765;

    keys[809]   = hash_str_new("&bsime;", 7);
    values[809] = 8909;

    keys[810]   = hash_str_new("&bsol;", 6);
    values[810] = 92;

    keys[811]   = hash_str_new("&bsolb;", 7);
    values[811] = 10693;

    keys[812]   = hash_str_new("&bsolhsub;", 10);
    values[812] = 10184;

    keys[813]   = hash_str_new("&bull;", 6);
    values[813] = 8226;

    keys[814]   = hash_str_new("&bullet;", 8);
    values[814] = 8226;

    keys[815]   = hash_str_new("&bump;", 6);
    values[815] = 8782;

    keys[816]   = hash_str_new("&bumpE;", 7);
    values[816] = 10926;

    keys[817]   = hash_str_new("&bumpe;", 7);
    values[817] = 8783;

    keys[818]   = hash_str_new("&bumpeq;", 8);
    values[818] = 8783;

    keys[819]   = hash_str_new("&cacute;", 8);
    values[819] = 263;

    keys[820]   = hash_str_new("&cap;", 5);
    values[820] = 8745;

    keys[821]   = hash_str_new("&capand;", 8);
    values[821] = 10820;

    keys[822]   = hash_str_new("&capbrcup;", 10);
    values[822] = 10825;

    keys[823]   = hash_str_new("&capcap;", 8);
    values[823] = 10827;

    keys[824]   = hash_str_new("&capcup;", 8);
    values[824] = 10823;

    keys[825]   = hash_str_new("&capdot;", 8);
    values[825] = 10816;

    keys[826]   = hash_str_new("&caps;", 6);
    values[826] = 8745;

    keys[827]   = hash_str_new("&caret;", 7);
    values[827] = 8257;

    keys[828]   = hash_str_new("&caron;", 7);
    values[828] = 711;

    keys[829]   = hash_str_new("&ccaps;", 7);
    values[829] = 10829;

    keys[830]   = hash_str_new("&ccaron;", 8);
    values[830] = 269;

    keys[831]   = hash_str_new("&ccedil", 7);
    values[831] = 231;

    keys[832]   = hash_str_new("&ccedil;", 8);
    values[832] = 231;

    keys[833]   = hash_str_new("&ccirc;", 7);
    values[833] = 265;

    keys[834]   = hash_str_new("&ccups;", 7);
    values[834] = 10828;

    keys[835]   = hash_str_new("&ccupssm;", 9);
    values[835] = 10832;

    keys[836]   = hash_str_new("&cdot;", 6);
    values[836] = 267;

    keys[837]   = hash_str_new("&cedil", 6);
    values[837] = 184;

    keys[838]   = hash_str_new("&cedil;", 7);
    values[838] = 184;

    keys[839]   = hash_str_new("&cemptyv;", 9);
    values[839] = 10674;

    keys[840]   = hash_str_new("&cent", 5);
    values[840] = 162;

    keys[841]   = hash_str_new("&cent;", 6);
    values[841] = 162;

    keys[842]   = hash_str_new("&centerdot;", 11);
    values[842] = 183;

    keys[843]   = hash_str_new("&cfr;", 5);
    values[843] = 120096;

    keys[844]   = hash_str_new("&chcy;", 6);
    values[844] = 1095;

    keys[845]   = hash_str_new("&check;", 7);
    values[845] = 10003;

    keys[846]   = hash_str_new("&checkmark;", 11);
    values[846] = 10003;

    keys[847]   = hash_str_new("&chi;", 5);
    values[847] = 967;

    keys[848]   = hash_str_new("&cir;", 5);
    values[848] = 9675;

    keys[849]   = hash_str_new("&cirE;", 6);
    values[849] = 10691;

    keys[850]   = hash_str_new("&circ;", 6);
    values[850] = 710;

    keys[851]   = hash_str_new("&circeq;", 8);
    values[851] = 8791;

    keys[852]   = hash_str_new("&circlearrowleft;", 17);
    values[852] = 8634;

    keys[853]   = hash_str_new("&circlearrowright;", 18);
    values[853] = 8635;

    keys[854]   = hash_str_new("&circledR;", 10);
    values[854] = 174;

    keys[855]   = hash_str_new("&circledS;", 10);
    values[855] = 9416;

    keys[856]   = hash_str_new("&circledast;", 12);
    values[856] = 8859;

    keys[857]   = hash_str_new("&circledcirc;", 13);
    values[857] = 8858;

    keys[858]   = hash_str_new("&circleddash;", 13);
    values[858] = 8861;

    keys[859]   = hash_str_new("&cire;", 6);
    values[859] = 8791;

    keys[860]   = hash_str_new("&cirfnint;", 10);
    values[860] = 10768;

    keys[861]   = hash_str_new("&cirmid;", 8);
    values[861] = 10991;

    keys[862]   = hash_str_new("&cirscir;", 9);
    values[862] = 10690;

    keys[863]   = hash_str_new("&clubs;", 7);
    values[863] = 9827;

    keys[864]   = hash_str_new("&clubsuit;", 10);
    values[864] = 9827;

    keys[865]   = hash_str_new("&colon;", 7);
    values[865] = 58;

    keys[866]   = hash_str_new("&colone;", 8);
    values[866] = 8788;

    keys[867]   = hash_str_new("&coloneq;", 9);
    values[867] = 8788;

    keys[868]   = hash_str_new("&comma;", 7);
    values[868] = 44;

    keys[869]   = hash_str_new("&commat;", 8);
    values[869] = 64;

    keys[870]   = hash_str_new("&comp;", 6);
    values[870] = 8705;

    keys[871]   = hash_str_new("&compfn;", 8);
    values[871] = 8728;

    keys[872]   = hash_str_new("&complement;", 12);
    values[872] = 8705;

    keys[873]   = hash_str_new("&complexes;", 11);
    values[873] = 8450;

    keys[874]   = hash_str_new("&cong;", 6);
    values[874] = 8773;

    keys[875]   = hash_str_new("&congdot;", 9);
    values[875] = 10861;

    keys[876]   = hash_str_new("&conint;", 8);
    values[876] = 8750;

    keys[877]   = hash_str_new("&copf;", 6);
    values[877] = 120148;

    keys[878]   = hash_str_new("&coprod;", 8);
    values[878] = 8720;

    keys[879]   = hash_str_new("&copy", 5);
    values[879] = 169;

    keys[880]   = hash_str_new("&copy;", 6);
    values[880] = 169;

    keys[881]   = hash_str_new("&copysr;", 8);
    values[881] = 8471;

    keys[882]   = hash_str_new("&crarr;", 7);
    values[882] = 8629;

    keys[883]   = hash_str_new("&cross;", 7);
    values[883] = 10007;

    keys[884]   = hash_str_new("&cscr;", 6);
    values[884] = 119992;

    keys[885]   = hash_str_new("&csub;", 6);
    values[885] = 10959;

    keys[886]   = hash_str_new("&csube;", 7);
    values[886] = 10961;

    keys[887]   = hash_str_new("&csup;", 6);
    values[887] = 10960;

    keys[888]   = hash_str_new("&csupe;", 7);
    values[888] = 10962;

    keys[889]   = hash_str_new("&ctdot;", 7);
    values[889] = 8943;

    keys[890]   = hash_str_new("&cudarrl;", 9);
    values[890] = 10552;

    keys[891]   = hash_str_new("&cudarrr;", 9);
    values[891] = 10549;

    keys[892]   = hash_str_new("&cuepr;", 7);
    values[892] = 8926;

    keys[893]   = hash_str_new("&cuesc;", 7);
    values[893] = 8927;

    keys[894]   = hash_str_new("&cularr;", 8);
    values[894] = 8630;

    keys[895]   = hash_str_new("&cularrp;", 9);
    values[895] = 10557;

    keys[896]   = hash_str_new("&cup;", 5);
    values[896] = 8746;

    keys[897]   = hash_str_new("&cupbrcap;", 10);
    values[897] = 10824;

    keys[898]   = hash_str_new("&cupcap;", 8);
    values[898] = 10822;

    keys[899]   = hash_str_new("&cupcup;", 8);
    values[899] = 10826;

    keys[900]   = hash_str_new("&cupdot;", 8);
    values[900] = 8845;

    keys[901]   = hash_str_new("&cupor;", 7);
    values[901] = 10821;

    keys[902]   = hash_str_new("&cups;", 6);
    values[902] = 8746;

    keys[903]   = hash_str_new("&curarr;", 8);
    values[903] = 8631;

    keys[904]   = hash_str_new("&curarrm;", 9);
    values[904] = 10556;

    keys[905]   = hash_str_new("&curlyeqprec;", 13);
    values[905] = 8926;

    keys[906]   = hash_str_new("&curlyeqsucc;", 13);
    values[906] = 8927;

    keys[907]   = hash_str_new("&curlyvee;", 10);
    values[907] = 8910;

    keys[908]   = hash_str_new("&curlywedge;", 12);
    values[908] = 8911;

    keys[909]   = hash_str_new("&curren", 7);
    values[909] = 164;

    keys[910]   = hash_str_new("&curren;", 8);
    values[910] = 164;

    keys[911]   = hash_str_new("&curvearrowleft;", 16);
    values[911] = 8630;

    keys[912]   = hash_str_new("&curvearrowright;", 17);
    values[912] = 8631;

    keys[913]   = hash_str_new("&cuvee;", 7);
    values[913] = 8910;

    keys[914]   = hash_str_new("&cuwed;", 7);
    values[914] = 8911;

    keys[915]   = hash_str_new("&cwconint;", 10);
    values[915] = 8754;

    keys[916]   = hash_str_new("&cwint;", 7);
    values[916] = 8753;

    keys[917]   = hash_str_new("&cylcty;", 8);
    values[917] = 9005;

    keys[918]   = hash_str_new("&dArr;", 6);
    values[918] = 8659;

    keys[919]   = hash_str_new("&dHar;", 6);
    values[919] = 10597;

    keys[920]   = hash_str_new("&dagger;", 8);
    values[920] = 8224;

    keys[921]   = hash_str_new("&daleth;", 8);
    values[921] = 8504;

    keys[922]   = hash_str_new("&darr;", 6);
    values[922] = 8595;

    keys[923]   = hash_str_new("&dash;", 6);
    values[923] = 8208;

    keys[924]   = hash_str_new("&dashv;", 7);
    values[924] = 8867;

    keys[925]   = hash_str_new("&dbkarow;", 9);
    values[925] = 10511;

    keys[926]   = hash_str_new("&dblac;", 7);
    values[926] = 733;

    keys[927]   = hash_str_new("&dcaron;", 8);
    values[927] = 271;

    keys[928]   = hash_str_new("&dcy;", 5);
    values[928] = 1076;

    keys[929]   = hash_str_new("&dd;", 4);
    values[929] = 8518;

    keys[930]   = hash_str_new("&ddagger;", 9);
    values[930] = 8225;

    keys[931]   = hash_str_new("&ddarr;", 7);
    values[931] = 8650;

    keys[932]   = hash_str_new("&ddotseq;", 9);
    values[932] = 10871;

    keys[933]   = hash_str_new("&deg", 4);
    values[933] = 176;

    keys[934]   = hash_str_new("&deg;", 5);
    values[934] = 176;

    keys[935]   = hash_str_new("&delta;", 7);
    values[935] = 948;

    keys[936]   = hash_str_new("&demptyv;", 9);
    values[936] = 10673;

    keys[937]   = hash_str_new("&dfisht;", 8);
    values[937] = 10623;

    keys[938]   = hash_str_new("&dfr;", 5);
    values[938] = 120097;

    keys[939]   = hash_str_new("&dharl;", 7);
    values[939] = 8643;

    keys[940]   = hash_str_new("&dharr;", 7);
    values[940] = 8642;

    keys[941]   = hash_str_new("&diam;", 6);
    values[941] = 8900;

    keys[942]   = hash_str_new("&diamond;", 9);
    values[942] = 8900;

    keys[943]   = hash_str_new("&diamondsuit;", 13);
    values[943] = 9830;

    keys[944]   = hash_str_new("&diams;", 7);
    values[944] = 9830;

    keys[945]   = hash_str_new("&die;", 5);
    values[945] = 168;

    keys[946]   = hash_str_new("&digamma;", 9);
    values[946] = 989;

    keys[947]   = hash_str_new("&disin;", 7);
    values[947] = 8946;

    keys[948]   = hash_str_new("&div;", 5);
    values[948] = 247;

    keys[949]   = hash_str_new("&divide", 7);
    values[949] = 247;

    keys[950]   = hash_str_new("&divide;", 8);
    values[950] = 247;

    keys[951]   = hash_str_new("&divideontimes;", 15);
    values[951] = 8903;

    keys[952]   = hash_str_new("&divonx;", 8);
    values[952] = 8903;

    keys[953]   = hash_str_new("&djcy;", 6);
    values[953] = 1106;

    keys[954]   = hash_str_new("&dlcorn;", 8);
    values[954] = 8990;

    keys[955]   = hash_str_new("&dlcrop;", 8);
    values[955] = 8973;

    keys[956]   = hash_str_new("&dollar;", 8);
    values[956] = 36;

    keys[957]   = hash_str_new("&dopf;", 6);
    values[957] = 120149;

    keys[958]   = hash_str_new("&dot;", 5);
    values[958] = 729;

    keys[959]   = hash_str_new("&doteq;", 7);
    values[959] = 8784;

    keys[960]   = hash_str_new("&doteqdot;", 10);
    values[960] = 8785;

    keys[961]   = hash_str_new("&dotminus;", 10);
    values[961] = 8760;

    keys[962]   = hash_str_new("&dotplus;", 9);
    values[962] = 8724;

    keys[963]   = hash_str_new("&dotsquare;", 11);
    values[963] = 8865;

    keys[964]   = hash_str_new("&doublebarwedge;", 16);
    values[964] = 8966;

    keys[965]   = hash_str_new("&downarrow;", 11);
    values[965] = 8595;

    keys[966]   = hash_str_new("&downdownarrows;", 16);
    values[966] = 8650;

    keys[967]   = hash_str_new("&downharpoonleft;", 17);
    values[967] = 8643;

    keys[968]   = hash_str_new("&downharpoonright;", 18);
    values[968] = 8642;

    keys[969]   = hash_str_new("&drbkarow;", 10);
    values[969] = 10512;

    keys[970]   = hash_str_new("&drcorn;", 8);
    values[970] = 8991;

    keys[971]   = hash_str_new("&drcrop;", 8);
    values[971] = 8972;

    keys[972]   = hash_str_new("&dscr;", 6);
    values[972] = 119993;

    keys[973]   = hash_str_new("&dscy;", 6);
    values[973] = 1109;

    keys[974]   = hash_str_new("&dsol;", 6);
    values[974] = 10742;

    keys[975]   = hash_str_new("&dstrok;", 8);
    values[975] = 273;

    keys[976]   = hash_str_new("&dtdot;", 7);
    values[976] = 8945;

    keys[977]   = hash_str_new("&dtri;", 6);
    values[977] = 9663;

    keys[978]   = hash_str_new("&dtrif;", 7);
    values[978] = 9662;

    keys[979]   = hash_str_new("&duarr;", 7);
    values[979] = 8693;

    keys[980]   = hash_str_new("&duhar;", 7);
    values[980] = 10607;

    keys[981]   = hash_str_new("&dwangle;", 9);
    values[981] = 10662;

    keys[982]   = hash_str_new("&dzcy;", 6);
    values[982] = 1119;

    keys[983]   = hash_str_new("&dzigrarr;", 10);
    values[983] = 10239;

    keys[984]   = hash_str_new("&eDDot;", 7);
    values[984] = 10871;

    keys[985]   = hash_str_new("&eDot;", 6);
    values[985] = 8785;

    keys[986]   = hash_str_new("&eacute", 7);
    values[986] = 233;

    keys[987]   = hash_str_new("&eacute;", 8);
    values[987] = 233;

    keys[988]   = hash_str_new("&easter;", 8);
    values[988] = 10862;

    keys[989]   = hash_str_new("&ecaron;", 8);
    values[989] = 283;

    keys[990]   = hash_str_new("&ecir;", 6);
    values[990] = 8790;

    keys[991]   = hash_str_new("&ecirc", 6);
    values[991] = 234;

    keys[992]   = hash_str_new("&ecirc;", 7);
    values[992] = 234;

    keys[993]   = hash_str_new("&ecolon;", 8);
    values[993] = 8789;

    keys[994]   = hash_str_new("&ecy;", 5);
    values[994] = 1101;

    keys[995]   = hash_str_new("&edot;", 6);
    values[995] = 279;

    keys[996]   = hash_str_new("&ee;", 4);
    values[996] = 8519;

    keys[997]   = hash_str_new("&efDot;", 7);
    values[997] = 8786;

    keys[998]   = hash_str_new("&efr;", 5);
    values[998] = 120098;

    keys[999]   = hash_str_new("&eg;", 4);
    values[999] = 10906;

    keys[1000]   = hash_str_new("&egrave", 7);
    values[1000] = 232;

    keys[1001]   = hash_str_new("&egrave;", 8);
    values[1001] = 232;

    keys[1002]   = hash_str_new("&egs;", 5);
    values[1002] = 10902;

    keys[1003]   = hash_str_new("&egsdot;", 8);
    values[1003] = 10904;

    keys[1004]   = hash_str_new("&el;", 4);
    values[1004] = 10905;

    keys[1005]   = hash_str_new("&elinters;", 10);
    values[1005] = 9191;

    keys[1006]   = hash_str_new("&ell;", 5);
    values[1006] = 8467;

    keys[1007]   = hash_str_new("&els;", 5);
    values[1007] = 10901;

    keys[1008]   = hash_str_new("&elsdot;", 8);
    values[1008] = 10903;

    keys[1009]   = hash_str_new("&emacr;", 7);
    values[1009] = 275;

    keys[1010]   = hash_str_new("&empty;", 7);
    values[1010] = 8709;

    keys[1011]   = hash_str_new("&emptyset;", 10);
    values[1011] = 8709;

    keys[1012]   = hash_str_new("&emptyv;", 8);
    values[1012] = 8709;

    keys[1013]   = hash_str_new("&emsp13;", 8);
    values[1013] = 8196;

    keys[1014]   = hash_str_new("&emsp14;", 8);
    values[1014] = 8197;

    keys[1015]   = hash_str_new("&emsp;", 6);
    values[1015] = 8195;

    keys[1016]   = hash_str_new("&eng;", 5);
    values[1016] = 331;

    keys[1017]   = hash_str_new("&ensp;", 6);
    values[1017] = 8194;

    keys[1018]   = hash_str_new("&eogon;", 7);
    values[1018] = 281;

    keys[1019]   = hash_str_new("&eopf;", 6);
    values[1019] = 120150;

    keys[1020]   = hash_str_new("&epar;", 6);
    values[1020] = 8917;

    keys[1021]   = hash_str_new("&eparsl;", 8);
    values[1021] = 10723;

    keys[1022]   = hash_str_new("&eplus;", 7);
    values[1022] = 10865;

    keys[1023]   = hash_str_new("&epsi;", 6);
    values[1023] = 949;

    keys[1024]   = hash_str_new("&epsilon;", 9);
    values[1024] = 949;

    keys[1025]   = hash_str_new("&epsiv;", 7);
    values[1025] = 1013;

    keys[1026]   = hash_str_new("&eqcirc;", 8);
    values[1026] = 8790;

    keys[1027]   = hash_str_new("&eqcolon;", 9);
    values[1027] = 8789;

    keys[1028]   = hash_str_new("&eqsim;", 7);
    values[1028] = 8770;

    keys[1029]   = hash_str_new("&eqslantgtr;", 12);
    values[1029] = 10902;

    keys[1030]   = hash_str_new("&eqslantless;", 13);
    values[1030] = 10901;

    keys[1031]   = hash_str_new("&equals;", 8);
    values[1031] = 61;

    keys[1032]   = hash_str_new("&equest;", 8);
    values[1032] = 8799;

    keys[1033]   = hash_str_new("&equiv;", 7);
    values[1033] = 8801;

    keys[1034]   = hash_str_new("&equivDD;", 9);
    values[1034] = 10872;

    keys[1035]   = hash_str_new("&eqvparsl;", 10);
    values[1035] = 10725;

    keys[1036]   = hash_str_new("&erDot;", 7);
    values[1036] = 8787;

    keys[1037]   = hash_str_new("&erarr;", 7);
    values[1037] = 10609;

    keys[1038]   = hash_str_new("&escr;", 6);
    values[1038] = 8495;

    keys[1039]   = hash_str_new("&esdot;", 7);
    values[1039] = 8784;

    keys[1040]   = hash_str_new("&esim;", 6);
    values[1040] = 8770;

    keys[1041]   = hash_str_new("&eta;", 5);
    values[1041] = 951;

    keys[1042]   = hash_str_new("&eth", 4);
    values[1042] = 240;

    keys[1043]   = hash_str_new("&eth;", 5);
    values[1043] = 240;

    keys[1044]   = hash_str_new("&euml", 5);
    values[1044] = 235;

    keys[1045]   = hash_str_new("&euml;", 6);
    values[1045] = 235;

    keys[1046]   = hash_str_new("&euro;", 6);
    values[1046] = 8364;

    keys[1047]   = hash_str_new("&excl;", 6);
    values[1047] = 33;

    keys[1048]   = hash_str_new("&exist;", 7);
    values[1048] = 8707;

    keys[1049]   = hash_str_new("&expectation;", 13);
    values[1049] = 8496;

    keys[1050]   = hash_str_new("&exponentiale;", 14);
    values[1050] = 8519;

    keys[1051]   = hash_str_new("&fallingdotseq;", 15);
    values[1051] = 8786;

    keys[1052]   = hash_str_new("&fcy;", 5);
    values[1052] = 1092;

    keys[1053]   = hash_str_new("&female;", 8);
    values[1053] = 9792;

    keys[1054]   = hash_str_new("&ffilig;", 8);
    values[1054] = 64259;

    keys[1055]   = hash_str_new("&fflig;", 7);
    values[1055] = 64256;

    keys[1056]   = hash_str_new("&ffllig;", 8);
    values[1056] = 64260;

    keys[1057]   = hash_str_new("&ffr;", 5);
    values[1057] = 120099;

    keys[1058]   = hash_str_new("&filig;", 7);
    values[1058] = 64257;

    keys[1059]   = hash_str_new("&fjlig;", 7);
    values[1059] = 102;

    keys[1060]   = hash_str_new("&flat;", 6);
    values[1060] = 9837;

    keys[1061]   = hash_str_new("&fllig;", 7);
    values[1061] = 64258;

    keys[1062]   = hash_str_new("&fltns;", 7);
    values[1062] = 9649;

    keys[1063]   = hash_str_new("&fnof;", 6);
    values[1063] = 402;

    keys[1064]   = hash_str_new("&fopf;", 6);
    values[1064] = 120151;

    keys[1065]   = hash_str_new("&forall;", 8);
    values[1065] = 8704;

    keys[1066]   = hash_str_new("&fork;", 6);
    values[1066] = 8916;

    keys[1067]   = hash_str_new("&forkv;", 7);
    values[1067] = 10969;

    keys[1068]   = hash_str_new("&fpartint;", 10);
    values[1068] = 10765;

    keys[1069]   = hash_str_new("&frac12", 7);
    values[1069] = 189;

    keys[1070]   = hash_str_new("&frac12;", 8);
    values[1070] = 189;

    keys[1071]   = hash_str_new("&frac13;", 8);
    values[1071] = 8531;

    keys[1072]   = hash_str_new("&frac14", 7);
    values[1072] = 188;

    keys[1073]   = hash_str_new("&frac14;", 8);
    values[1073] = 188;

    keys[1074]   = hash_str_new("&frac15;", 8);
    values[1074] = 8533;

    keys[1075]   = hash_str_new("&frac16;", 8);
    values[1075] = 8537;

    keys[1076]   = hash_str_new("&frac18;", 8);
    values[1076] = 8539;

    keys[1077]   = hash_str_new("&frac23;", 8);
    values[1077] = 8532;

    keys[1078]   = hash_str_new("&frac25;", 8);
    values[1078] = 8534;

    keys[1079]   = hash_str_new("&frac34", 7);
    values[1079] = 190;

    keys[1080]   = hash_str_new("&frac34;", 8);
    values[1080] = 190;

    keys[1081]   = hash_str_new("&frac35;", 8);
    values[1081] = 8535;

    keys[1082]   = hash_str_new("&frac38;", 8);
    values[1082] = 8540;

    keys[1083]   = hash_str_new("&frac45;", 8);
    values[1083] = 8536;

    keys[1084]   = hash_str_new("&frac56;", 8);
    values[1084] = 8538;

    keys[1085]   = hash_str_new("&frac58;", 8);
    values[1085] = 8541;

    keys[1086]   = hash_str_new("&frac78;", 8);
    values[1086] = 8542;

    keys[1087]   = hash_str_new("&frasl;", 7);
    values[1087] = 8260;

    keys[1088]   = hash_str_new("&frown;", 7);
    values[1088] = 8994;

    keys[1089]   = hash_str_new("&fscr;", 6);
    values[1089] = 119995;

    keys[1090]   = hash_str_new("&gE;", 4);
    values[1090] = 8807;

    keys[1091]   = hash_str_new("&gEl;", 5);
    values[1091] = 10892;

    keys[1092]   = hash_str_new("&gacute;", 8);
    values[1092] = 501;

    keys[1093]   = hash_str_new("&gamma;", 7);
    values[1093] = 947;

    keys[1094]   = hash_str_new("&gammad;", 8);
    values[1094] = 989;

    keys[1095]   = hash_str_new("&gap;", 5);
    values[1095] = 10886;

    keys[1096]   = hash_str_new("&gbreve;", 8);
    values[1096] = 287;

    keys[1097]   = hash_str_new("&gcirc;", 7);
    values[1097] = 285;

    keys[1098]   = hash_str_new("&gcy;", 5);
    values[1098] = 1075;

    keys[1099]   = hash_str_new("&gdot;", 6);
    values[1099] = 289;

    keys[1100]   = hash_str_new("&ge;", 4);
    values[1100] = 8805;

    keys[1101]   = hash_str_new("&gel;", 5);
    values[1101] = 8923;

    keys[1102]   = hash_str_new("&geq;", 5);
    values[1102] = 8805;

    keys[1103]   = hash_str_new("&geqq;", 6);
    values[1103] = 8807;

    keys[1104]   = hash_str_new("&geqslant;", 10);
    values[1104] = 10878;

    keys[1105]   = hash_str_new("&ges;", 5);
    values[1105] = 10878;

    keys[1106]   = hash_str_new("&gescc;", 7);
    values[1106] = 10921;

    keys[1107]   = hash_str_new("&gesdot;", 8);
    values[1107] = 10880;

    keys[1108]   = hash_str_new("&gesdoto;", 9);
    values[1108] = 10882;

    keys[1109]   = hash_str_new("&gesdotol;", 10);
    values[1109] = 10884;

    keys[1110]   = hash_str_new("&gesl;", 6);
    values[1110] = 8923;

    keys[1111]   = hash_str_new("&gesles;", 8);
    values[1111] = 10900;

    keys[1112]   = hash_str_new("&gfr;", 5);
    values[1112] = 120100;

    keys[1113]   = hash_str_new("&gg;", 4);
    values[1113] = 8811;

    keys[1114]   = hash_str_new("&ggg;", 5);
    values[1114] = 8921;

    keys[1115]   = hash_str_new("&gimel;", 7);
    values[1115] = 8503;

    keys[1116]   = hash_str_new("&gjcy;", 6);
    values[1116] = 1107;

    keys[1117]   = hash_str_new("&gl;", 4);
    values[1117] = 8823;

    keys[1118]   = hash_str_new("&glE;", 5);
    values[1118] = 10898;

    keys[1119]   = hash_str_new("&gla;", 5);
    values[1119] = 10917;

    keys[1120]   = hash_str_new("&glj;", 5);
    values[1120] = 10916;

    keys[1121]   = hash_str_new("&gnE;", 5);
    values[1121] = 8809;

    keys[1122]   = hash_str_new("&gnap;", 6);
    values[1122] = 10890;

    keys[1123]   = hash_str_new("&gnapprox;", 10);
    values[1123] = 10890;

    keys[1124]   = hash_str_new("&gne;", 5);
    values[1124] = 10888;

    keys[1125]   = hash_str_new("&gneq;", 6);
    values[1125] = 10888;

    keys[1126]   = hash_str_new("&gneqq;", 7);
    values[1126] = 8809;

    keys[1127]   = hash_str_new("&gnsim;", 7);
    values[1127] = 8935;

    keys[1128]   = hash_str_new("&gopf;", 6);
    values[1128] = 120152;

    keys[1129]   = hash_str_new("&grave;", 7);
    values[1129] = 96;

    keys[1130]   = hash_str_new("&gscr;", 6);
    values[1130] = 8458;

    keys[1131]   = hash_str_new("&gsim;", 6);
    values[1131] = 8819;

    keys[1132]   = hash_str_new("&gsime;", 7);
    values[1132] = 10894;

    keys[1133]   = hash_str_new("&gsiml;", 7);
    values[1133] = 10896;

    keys[1134]   = hash_str_new("&gt", 3);
    values[1134] = 62;

    keys[1135]   = hash_str_new("&gt;", 4);
    values[1135] = 62;

    keys[1136]   = hash_str_new("&gtcc;", 6);
    values[1136] = 10919;

    keys[1137]   = hash_str_new("&gtcir;", 7);
    values[1137] = 10874;

    keys[1138]   = hash_str_new("&gtdot;", 7);
    values[1138] = 8919;

    keys[1139]   = hash_str_new("&gtlPar;", 8);
    values[1139] = 10645;

    keys[1140]   = hash_str_new("&gtquest;", 9);
    values[1140] = 10876;

    keys[1141]   = hash_str_new("&gtrapprox;", 11);
    values[1141] = 10886;

    keys[1142]   = hash_str_new("&gtrarr;", 8);
    values[1142] = 10616;

    keys[1143]   = hash_str_new("&gtrdot;", 8);
    values[1143] = 8919;

    keys[1144]   = hash_str_new("&gtreqless;", 11);
    values[1144] = 8923;

    keys[1145]   = hash_str_new("&gtreqqless;", 12);
    values[1145] = 10892;

    keys[1146]   = hash_str_new("&gtrless;", 9);
    values[1146] = 8823;

    keys[1147]   = hash_str_new("&gtrsim;", 8);
    values[1147] = 8819;

    keys[1148]   = hash_str_new("&gvertneqq;", 11);
    values[1148] = 8809;

    keys[1149]   = hash_str_new("&gvnE;", 6);
    values[1149] = 8809;

    keys[1150]   = hash_str_new("&hArr;", 6);
    values[1150] = 8660;

    keys[1151]   = hash_str_new("&hairsp;", 8);
    values[1151] = 8202;

    keys[1152]   = hash_str_new("&half;", 6);
    values[1152] = 189;

    keys[1153]   = hash_str_new("&hamilt;", 8);
    values[1153] = 8459;

    keys[1154]   = hash_str_new("&hardcy;", 8);
    values[1154] = 1098;

    keys[1155]   = hash_str_new("&harr;", 6);
    values[1155] = 8596;

    keys[1156]   = hash_str_new("&harrcir;", 9);
    values[1156] = 10568;

    keys[1157]   = hash_str_new("&harrw;", 7);
    values[1157] = 8621;

    keys[1158]   = hash_str_new("&hbar;", 6);
    values[1158] = 8463;

    keys[1159]   = hash_str_new("&hcirc;", 7);
    values[1159] = 293;

    keys[1160]   = hash_str_new("&hearts;", 8);
    values[1160] = 9829;

    keys[1161]   = hash_str_new("&heartsuit;", 11);
    values[1161] = 9829;

    keys[1162]   = hash_str_new("&hellip;", 8);
    values[1162] = 8230;

    keys[1163]   = hash_str_new("&hercon;", 8);
    values[1163] = 8889;

    keys[1164]   = hash_str_new("&hfr;", 5);
    values[1164] = 120101;

    keys[1165]   = hash_str_new("&hksearow;", 10);
    values[1165] = 10533;

    keys[1166]   = hash_str_new("&hkswarow;", 10);
    values[1166] = 10534;

    keys[1167]   = hash_str_new("&hoarr;", 7);
    values[1167] = 8703;

    keys[1168]   = hash_str_new("&homtht;", 8);
    values[1168] = 8763;

    keys[1169]   = hash_str_new("&hookleftarrow;", 15);
    values[1169] = 8617;

    keys[1170]   = hash_str_new("&hookrightarrow;", 16);
    values[1170] = 8618;

    keys[1171]   = hash_str_new("&hopf;", 6);
    values[1171] = 120153;

    keys[1172]   = hash_str_new("&horbar;", 8);
    values[1172] = 8213;

    keys[1173]   = hash_str_new("&hscr;", 6);
    values[1173] = 119997;

    keys[1174]   = hash_str_new("&hslash;", 8);
    values[1174] = 8463;

    keys[1175]   = hash_str_new("&hstrok;", 8);
    values[1175] = 295;

    keys[1176]   = hash_str_new("&hybull;", 8);
    values[1176] = 8259;

    keys[1177]   = hash_str_new("&hyphen;", 8);
    values[1177] = 8208;

    keys[1178]   = hash_str_new("&iacute", 7);
    values[1178] = 237;

    keys[1179]   = hash_str_new("&iacute;", 8);
    values[1179] = 237;

    keys[1180]   = hash_str_new("&ic;", 4);
    values[1180] = 8291;

    keys[1181]   = hash_str_new("&icirc", 6);
    values[1181] = 238;

    keys[1182]   = hash_str_new("&icirc;", 7);
    values[1182] = 238;

    keys[1183]   = hash_str_new("&icy;", 5);
    values[1183] = 1080;

    keys[1184]   = hash_str_new("&iecy;", 6);
    values[1184] = 1077;

    keys[1185]   = hash_str_new("&iexcl", 6);
    values[1185] = 161;

    keys[1186]   = hash_str_new("&iexcl;", 7);
    values[1186] = 161;

    keys[1187]   = hash_str_new("&iff;", 5);
    values[1187] = 8660;

    keys[1188]   = hash_str_new("&ifr;", 5);
    values[1188] = 120102;

    keys[1189]   = hash_str_new("&igrave", 7);
    values[1189] = 236;

    keys[1190]   = hash_str_new("&igrave;", 8);
    values[1190] = 236;

    keys[1191]   = hash_str_new("&ii;", 4);
    values[1191] = 8520;

    keys[1192]   = hash_str_new("&iiiint;", 8);
    values[1192] = 10764;

    keys[1193]   = hash_str_new("&iiint;", 7);
    values[1193] = 8749;

    keys[1194]   = hash_str_new("&iinfin;", 8);
    values[1194] = 10716;

    keys[1195]   = hash_str_new("&iiota;", 7);
    values[1195] = 8489;

    keys[1196]   = hash_str_new("&ijlig;", 7);
    values[1196] = 307;

    keys[1197]   = hash_str_new("&imacr;", 7);
    values[1197] = 299;

    keys[1198]   = hash_str_new("&image;", 7);
    values[1198] = 8465;

    keys[1199]   = hash_str_new("&imagline;", 10);
    values[1199] = 8464;

    keys[1200]   = hash_str_new("&imagpart;", 10);
    values[1200] = 8465;

    keys[1201]   = hash_str_new("&imath;", 7);
    values[1201] = 305;

    keys[1202]   = hash_str_new("&imof;", 6);
    values[1202] = 8887;

    keys[1203]   = hash_str_new("&imped;", 7);
    values[1203] = 437;

    keys[1204]   = hash_str_new("&in;", 4);
    values[1204] = 8712;

    keys[1205]   = hash_str_new("&incare;", 8);
    values[1205] = 8453;

    keys[1206]   = hash_str_new("&infin;", 7);
    values[1206] = 8734;

    keys[1207]   = hash_str_new("&infintie;", 10);
    values[1207] = 10717;

    keys[1208]   = hash_str_new("&inodot;", 8);
    values[1208] = 305;

    keys[1209]   = hash_str_new("&int;", 5);
    values[1209] = 8747;

    keys[1210]   = hash_str_new("&intcal;", 8);
    values[1210] = 8890;

    keys[1211]   = hash_str_new("&integers;", 10);
    values[1211] = 8484;

    keys[1212]   = hash_str_new("&intercal;", 10);
    values[1212] = 8890;

    keys[1213]   = hash_str_new("&intlarhk;", 10);
    values[1213] = 10775;

    keys[1214]   = hash_str_new("&intprod;", 9);
    values[1214] = 10812;

    keys[1215]   = hash_str_new("&iocy;", 6);
    values[1215] = 1105;

    keys[1216]   = hash_str_new("&iogon;", 7);
    values[1216] = 303;

    keys[1217]   = hash_str_new("&iopf;", 6);
    values[1217] = 120154;

    keys[1218]   = hash_str_new("&iota;", 6);
    values[1218] = 953;

    keys[1219]   = hash_str_new("&iprod;", 7);
    values[1219] = 10812;

    keys[1220]   = hash_str_new("&iquest", 7);
    values[1220] = 191;

    keys[1221]   = hash_str_new("&iquest;", 8);
    values[1221] = 191;

    keys[1222]   = hash_str_new("&iscr;", 6);
    values[1222] = 119998;

    keys[1223]   = hash_str_new("&isin;", 6);
    values[1223] = 8712;

    keys[1224]   = hash_str_new("&isinE;", 7);
    values[1224] = 8953;

    keys[1225]   = hash_str_new("&isindot;", 9);
    values[1225] = 8949;

    keys[1226]   = hash_str_new("&isins;", 7);
    values[1226] = 8948;

    keys[1227]   = hash_str_new("&isinsv;", 8);
    values[1227] = 8947;

    keys[1228]   = hash_str_new("&isinv;", 7);
    values[1228] = 8712;

    keys[1229]   = hash_str_new("&it;", 4);
    values[1229] = 8290;

    keys[1230]   = hash_str_new("&itilde;", 8);
    values[1230] = 297;

    keys[1231]   = hash_str_new("&iukcy;", 7);
    values[1231] = 1110;

    keys[1232]   = hash_str_new("&iuml", 5);
    values[1232] = 239;

    keys[1233]   = hash_str_new("&iuml;", 6);
    values[1233] = 239;

    keys[1234]   = hash_str_new("&jcirc;", 7);
    values[1234] = 309;

    keys[1235]   = hash_str_new("&jcy;", 5);
    values[1235] = 1081;

    keys[1236]   = hash_str_new("&jfr;", 5);
    values[1236] = 120103;

    keys[1237]   = hash_str_new("&jmath;", 7);
    values[1237] = 567;

    keys[1238]   = hash_str_new("&jopf;", 6);
    values[1238] = 120155;

    keys[1239]   = hash_str_new("&jscr;", 6);
    values[1239] = 119999;

    keys[1240]   = hash_str_new("&jsercy;", 8);
    values[1240] = 1112;

    keys[1241]   = hash_str_new("&jukcy;", 7);
    values[1241] = 1108;

    keys[1242]   = hash_str_new("&kappa;", 7);
    values[1242] = 954;

    keys[1243]   = hash_str_new("&kappav;", 8);
    values[1243] = 1008;

    keys[1244]   = hash_str_new("&kcedil;", 8);
    values[1244] = 311;

    keys[1245]   = hash_str_new("&kcy;", 5);
    values[1245] = 1082;

    keys[1246]   = hash_str_new("&kfr;", 5);
    values[1246] = 120104;

    keys[1247]   = hash_str_new("&kgreen;", 8);
    values[1247] = 312;

    keys[1248]   = hash_str_new("&khcy;", 6);
    values[1248] = 1093;

    keys[1249]   = hash_str_new("&kjcy;", 6);
    values[1249] = 1116;

    keys[1250]   = hash_str_new("&kopf;", 6);
    values[1250] = 120156;

    keys[1251]   = hash_str_new("&kscr;", 6);
    values[1251] = 120000;

    keys[1252]   = hash_str_new("&lAarr;", 7);
    values[1252] = 8666;

    keys[1253]   = hash_str_new("&lArr;", 6);
    values[1253] = 8656;

    keys[1254]   = hash_str_new("&lAtail;", 8);
    values[1254] = 10523;

    keys[1255]   = hash_str_new("&lBarr;", 7);
    values[1255] = 10510;

    keys[1256]   = hash_str_new("&lE;", 4);
    values[1256] = 8806;

    keys[1257]   = hash_str_new("&lEg;", 5);
    values[1257] = 10891;

    keys[1258]   = hash_str_new("&lHar;", 6);
    values[1258] = 10594;

    keys[1259]   = hash_str_new("&lacute;", 8);
    values[1259] = 314;

    keys[1260]   = hash_str_new("&laemptyv;", 10);
    values[1260] = 10676;

    keys[1261]   = hash_str_new("&lagran;", 8);
    values[1261] = 8466;

    keys[1262]   = hash_str_new("&lambda;", 8);
    values[1262] = 955;

    keys[1263]   = hash_str_new("&lang;", 6);
    values[1263] = 10216;

    keys[1264]   = hash_str_new("&langd;", 7);
    values[1264] = 10641;

    keys[1265]   = hash_str_new("&langle;", 8);
    values[1265] = 10216;

    keys[1266]   = hash_str_new("&lap;", 5);
    values[1266] = 10885;

    keys[1267]   = hash_str_new("&laquo", 6);
    values[1267] = 171;

    keys[1268]   = hash_str_new("&laquo;", 7);
    values[1268] = 171;

    keys[1269]   = hash_str_new("&larr;", 6);
    values[1269] = 8592;

    keys[1270]   = hash_str_new("&larrb;", 7);
    values[1270] = 8676;

    keys[1271]   = hash_str_new("&larrbfs;", 9);
    values[1271] = 10527;

    keys[1272]   = hash_str_new("&larrfs;", 8);
    values[1272] = 10525;

    keys[1273]   = hash_str_new("&larrhk;", 8);
    values[1273] = 8617;

    keys[1274]   = hash_str_new("&larrlp;", 8);
    values[1274] = 8619;

    keys[1275]   = hash_str_new("&larrpl;", 8);
    values[1275] = 10553;

    keys[1276]   = hash_str_new("&larrsim;", 9);
    values[1276] = 10611;

    keys[1277]   = hash_str_new("&larrtl;", 8);
    values[1277] = 8610;

    keys[1278]   = hash_str_new("&lat;", 5);
    values[1278] = 10923;

    keys[1279]   = hash_str_new("&latail;", 8);
    values[1279] = 10521;

    keys[1280]   = hash_str_new("&late;", 6);
    values[1280] = 10925;

    keys[1281]   = hash_str_new("&lates;", 7);
    values[1281] = 10925;

    keys[1282]   = hash_str_new("&lbarr;", 7);
    values[1282] = 10508;

    keys[1283]   = hash_str_new("&lbbrk;", 7);
    values[1283] = 10098;

    keys[1284]   = hash_str_new("&lbrace;", 8);
    values[1284] = 123;

    keys[1285]   = hash_str_new("&lbrack;", 8);
    values[1285] = 91;

    keys[1286]   = hash_str_new("&lbrke;", 7);
    values[1286] = 10635;

    keys[1287]   = hash_str_new("&lbrksld;", 9);
    values[1287] = 10639;

    keys[1288]   = hash_str_new("&lbrkslu;", 9);
    values[1288] = 10637;

    keys[1289]   = hash_str_new("&lcaron;", 8);
    values[1289] = 318;

    keys[1290]   = hash_str_new("&lcedil;", 8);
    values[1290] = 316;

    keys[1291]   = hash_str_new("&lceil;", 7);
    values[1291] = 8968;

    keys[1292]   = hash_str_new("&lcub;", 6);
    values[1292] = 123;

    keys[1293]   = hash_str_new("&lcy;", 5);
    values[1293] = 1083;

    keys[1294]   = hash_str_new("&ldca;", 6);
    values[1294] = 10550;

    keys[1295]   = hash_str_new("&ldquo;", 7);
    values[1295] = 8220;

    keys[1296]   = hash_str_new("&ldquor;", 8);
    values[1296] = 8222;

    keys[1297]   = hash_str_new("&ldrdhar;", 9);
    values[1297] = 10599;

    keys[1298]   = hash_str_new("&ldrushar;", 10);
    values[1298] = 10571;

    keys[1299]   = hash_str_new("&ldsh;", 6);
    values[1299] = 8626;

    keys[1300]   = hash_str_new("&le;", 4);
    values[1300] = 8804;

    keys[1301]   = hash_str_new("&leftarrow;", 11);
    values[1301] = 8592;

    keys[1302]   = hash_str_new("&leftarrowtail;", 15);
    values[1302] = 8610;

    keys[1303]   = hash_str_new("&leftharpoondown;", 17);
    values[1303] = 8637;

    keys[1304]   = hash_str_new("&leftharpoonup;", 15);
    values[1304] = 8636;

    keys[1305]   = hash_str_new("&leftleftarrows;", 16);
    values[1305] = 8647;

    keys[1306]   = hash_str_new("&leftrightarrow;", 16);
    values[1306] = 8596;

    keys[1307]   = hash_str_new("&leftrightarrows;", 17);
    values[1307] = 8646;

    keys[1308]   = hash_str_new("&leftrightharpoons;", 19);
    values[1308] = 8651;

    keys[1309]   = hash_str_new("&leftrightsquigarrow;", 21);
    values[1309] = 8621;

    keys[1310]   = hash_str_new("&leftthreetimes;", 16);
    values[1310] = 8907;

    keys[1311]   = hash_str_new("&leg;", 5);
    values[1311] = 8922;

    keys[1312]   = hash_str_new("&leq;", 5);
    values[1312] = 8804;

    keys[1313]   = hash_str_new("&leqq;", 6);
    values[1313] = 8806;

    keys[1314]   = hash_str_new("&leqslant;", 10);
    values[1314] = 10877;

    keys[1315]   = hash_str_new("&les;", 5);
    values[1315] = 10877;

    keys[1316]   = hash_str_new("&lescc;", 7);
    values[1316] = 10920;

    keys[1317]   = hash_str_new("&lesdot;", 8);
    values[1317] = 10879;

    keys[1318]   = hash_str_new("&lesdoto;", 9);
    values[1318] = 10881;

    keys[1319]   = hash_str_new("&lesdotor;", 10);
    values[1319] = 10883;

    keys[1320]   = hash_str_new("&lesg;", 6);
    values[1320] = 8922;

    keys[1321]   = hash_str_new("&lesges;", 8);
    values[1321] = 10899;

    keys[1322]   = hash_str_new("&lessapprox;", 12);
    values[1322] = 10885;

    keys[1323]   = hash_str_new("&lessdot;", 9);
    values[1323] = 8918;

    keys[1324]   = hash_str_new("&lesseqgtr;", 11);
    values[1324] = 8922;

    keys[1325]   = hash_str_new("&lesseqqgtr;", 12);
    values[1325] = 10891;

    keys[1326]   = hash_str_new("&lessgtr;", 9);
    values[1326] = 8822;

    keys[1327]   = hash_str_new("&lesssim;", 9);
    values[1327] = 8818;

    keys[1328]   = hash_str_new("&lfisht;", 8);
    values[1328] = 10620;

    keys[1329]   = hash_str_new("&lfloor;", 8);
    values[1329] = 8970;

    keys[1330]   = hash_str_new("&lfr;", 5);
    values[1330] = 120105;

    keys[1331]   = hash_str_new("&lg;", 4);
    values[1331] = 8822;

    keys[1332]   = hash_str_new("&lgE;", 5);
    values[1332] = 10897;

    keys[1333]   = hash_str_new("&lhard;", 7);
    values[1333] = 8637;

    keys[1334]   = hash_str_new("&lharu;", 7);
    values[1334] = 8636;

    keys[1335]   = hash_str_new("&lharul;", 8);
    values[1335] = 10602;

    keys[1336]   = hash_str_new("&lhblk;", 7);
    values[1336] = 9604;

    keys[1337]   = hash_str_new("&ljcy;", 6);
    values[1337] = 1113;

    keys[1338]   = hash_str_new("&ll;", 4);
    values[1338] = 8810;

    keys[1339]   = hash_str_new("&llarr;", 7);
    values[1339] = 8647;

    keys[1340]   = hash_str_new("&llcorner;", 10);
    values[1340] = 8990;

    keys[1341]   = hash_str_new("&llhard;", 8);
    values[1341] = 10603;

    keys[1342]   = hash_str_new("&lltri;", 7);
    values[1342] = 9722;

    keys[1343]   = hash_str_new("&lmidot;", 8);
    values[1343] = 320;

    keys[1344]   = hash_str_new("&lmoust;", 8);
    values[1344] = 9136;

    keys[1345]   = hash_str_new("&lmoustache;", 12);
    values[1345] = 9136;

    keys[1346]   = hash_str_new("&lnE;", 5);
    values[1346] = 8808;

    keys[1347]   = hash_str_new("&lnap;", 6);
    values[1347] = 10889;

    keys[1348]   = hash_str_new("&lnapprox;", 10);
    values[1348] = 10889;

    keys[1349]   = hash_str_new("&lne;", 5);
    values[1349] = 10887;

    keys[1350]   = hash_str_new("&lneq;", 6);
    values[1350] = 10887;

    keys[1351]   = hash_str_new("&lneqq;", 7);
    values[1351] = 8808;

    keys[1352]   = hash_str_new("&lnsim;", 7);
    values[1352] = 8934;

    keys[1353]   = hash_str_new("&loang;", 7);
    values[1353] = 10220;

    keys[1354]   = hash_str_new("&loarr;", 7);
    values[1354] = 8701;

    keys[1355]   = hash_str_new("&lobrk;", 7);
    values[1355] = 10214;

    keys[1356]   = hash_str_new("&longleftarrow;", 15);
    values[1356] = 10229;

    keys[1357]   = hash_str_new("&longleftrightarrow;", 20);
    values[1357] = 10231;

    keys[1358]   = hash_str_new("&longmapsto;", 12);
    values[1358] = 10236;

    keys[1359]   = hash_str_new("&longrightarrow;", 16);
    values[1359] = 10230;

    keys[1360]   = hash_str_new("&looparrowleft;", 15);
    values[1360] = 8619;

    keys[1361]   = hash_str_new("&looparrowright;", 16);
    values[1361] = 8620;

    keys[1362]   = hash_str_new("&lopar;", 7);
    values[1362] = 10629;

    keys[1363]   = hash_str_new("&lopf;", 6);
    values[1363] = 120157;

    keys[1364]   = hash_str_new("&loplus;", 8);
    values[1364] = 10797;

    keys[1365]   = hash_str_new("&lotimes;", 9);
    values[1365] = 10804;

    keys[1366]   = hash_str_new("&lowast;", 8);
    values[1366] = 8727;

    keys[1367]   = hash_str_new("&lowbar;", 8);
    values[1367] = 95;

    keys[1368]   = hash_str_new("&loz;", 5);
    values[1368] = 9674;

    keys[1369]   = hash_str_new("&lozenge;", 9);
    values[1369] = 9674;

    keys[1370]   = hash_str_new("&lozf;", 6);
    values[1370] = 10731;

    keys[1371]   = hash_str_new("&lpar;", 6);
    values[1371] = 40;

    keys[1372]   = hash_str_new("&lparlt;", 8);
    values[1372] = 10643;

    keys[1373]   = hash_str_new("&lrarr;", 7);
    values[1373] = 8646;

    keys[1374]   = hash_str_new("&lrcorner;", 10);
    values[1374] = 8991;

    keys[1375]   = hash_str_new("&lrhar;", 7);
    values[1375] = 8651;

    keys[1376]   = hash_str_new("&lrhard;", 8);
    values[1376] = 10605;

    keys[1377]   = hash_str_new("&lrm;", 5);
    values[1377] = 8206;

    keys[1378]   = hash_str_new("&lrtri;", 7);
    values[1378] = 8895;

    keys[1379]   = hash_str_new("&lsaquo;", 8);
    values[1379] = 8249;

    keys[1380]   = hash_str_new("&lscr;", 6);
    values[1380] = 120001;

    keys[1381]   = hash_str_new("&lsh;", 5);
    values[1381] = 8624;

    keys[1382]   = hash_str_new("&lsim;", 6);
    values[1382] = 8818;

    keys[1383]   = hash_str_new("&lsime;", 7);
    values[1383] = 10893;

    keys[1384]   = hash_str_new("&lsimg;", 7);
    values[1384] = 10895;

    keys[1385]   = hash_str_new("&lsqb;", 6);
    values[1385] = 91;

    keys[1386]   = hash_str_new("&lsquo;", 7);
    values[1386] = 8216;

    keys[1387]   = hash_str_new("&lsquor;", 8);
    values[1387] = 8218;

    keys[1388]   = hash_str_new("&lstrok;", 8);
    values[1388] = 322;

    keys[1389]   = hash_str_new("&lt", 3);
    values[1389] = 60;

    keys[1390]   = hash_str_new("&lt;", 4);
    values[1390] = 60;

    keys[1391]   = hash_str_new("&ltcc;", 6);
    values[1391] = 10918;

    keys[1392]   = hash_str_new("&ltcir;", 7);
    values[1392] = 10873;

    keys[1393]   = hash_str_new("&ltdot;", 7);
    values[1393] = 8918;

    keys[1394]   = hash_str_new("&lthree;", 8);
    values[1394] = 8907;

    keys[1395]   = hash_str_new("&ltimes;", 8);
    values[1395] = 8905;

    keys[1396]   = hash_str_new("&ltlarr;", 8);
    values[1396] = 10614;

    keys[1397]   = hash_str_new("&ltquest;", 9);
    values[1397] = 10875;

    keys[1398]   = hash_str_new("&ltrPar;", 8);
    values[1398] = 10646;

    keys[1399]   = hash_str_new("&ltri;", 6);
    values[1399] = 9667;

    keys[1400]   = hash_str_new("&ltrie;", 7);
    values[1400] = 8884;

    keys[1401]   = hash_str_new("&ltrif;", 7);
    values[1401] = 9666;

    keys[1402]   = hash_str_new("&lurdshar;", 10);
    values[1402] = 10570;

    keys[1403]   = hash_str_new("&luruhar;", 9);
    values[1403] = 10598;

    keys[1404]   = hash_str_new("&lvertneqq;", 11);
    values[1404] = 8808;

    keys[1405]   = hash_str_new("&lvnE;", 6);
    values[1405] = 8808;

    keys[1406]   = hash_str_new("&mDDot;", 7);
    values[1406] = 8762;

    keys[1407]   = hash_str_new("&macr", 5);
    values[1407] = 175;

    keys[1408]   = hash_str_new("&macr;", 6);
    values[1408] = 175;

    keys[1409]   = hash_str_new("&male;", 6);
    values[1409] = 9794;

    keys[1410]   = hash_str_new("&malt;", 6);
    values[1410] = 10016;

    keys[1411]   = hash_str_new("&maltese;", 9);
    values[1411] = 10016;

    keys[1412]   = hash_str_new("&map;", 5);
    values[1412] = 8614;

    keys[1413]   = hash_str_new("&mapsto;", 8);
    values[1413] = 8614;

    keys[1414]   = hash_str_new("&mapstodown;", 12);
    values[1414] = 8615;

    keys[1415]   = hash_str_new("&mapstoleft;", 12);
    values[1415] = 8612;

    keys[1416]   = hash_str_new("&mapstoup;", 10);
    values[1416] = 8613;

    keys[1417]   = hash_str_new("&marker;", 8);
    values[1417] = 9646;

    keys[1418]   = hash_str_new("&mcomma;", 8);
    values[1418] = 10793;

    keys[1419]   = hash_str_new("&mcy;", 5);
    values[1419] = 1084;

    keys[1420]   = hash_str_new("&mdash;", 7);
    values[1420] = 8212;

    keys[1421]   = hash_str_new("&measuredangle;", 15);
    values[1421] = 8737;

    keys[1422]   = hash_str_new("&mfr;", 5);
    values[1422] = 120106;

    keys[1423]   = hash_str_new("&mho;", 5);
    values[1423] = 8487;

    keys[1424]   = hash_str_new("&micro", 6);
    values[1424] = 181;

    keys[1425]   = hash_str_new("&micro;", 7);
    values[1425] = 181;

    keys[1426]   = hash_str_new("&mid;", 5);
    values[1426] = 8739;

    keys[1427]   = hash_str_new("&midast;", 8);
    values[1427] = 42;

    keys[1428]   = hash_str_new("&midcir;", 8);
    values[1428] = 10992;

    keys[1429]   = hash_str_new("&middot", 7);
    values[1429] = 183;

    keys[1430]   = hash_str_new("&middot;", 8);
    values[1430] = 183;

    keys[1431]   = hash_str_new("&minus;", 7);
    values[1431] = 8722;

    keys[1432]   = hash_str_new("&minusb;", 8);
    values[1432] = 8863;

    keys[1433]   = hash_str_new("&minusd;", 8);
    values[1433] = 8760;

    keys[1434]   = hash_str_new("&minusdu;", 9);
    values[1434] = 10794;

    keys[1435]   = hash_str_new("&mlcp;", 6);
    values[1435] = 10971;

    keys[1436]   = hash_str_new("&mldr;", 6);
    values[1436] = 8230;

    keys[1437]   = hash_str_new("&mnplus;", 8);
    values[1437] = 8723;

    keys[1438]   = hash_str_new("&models;", 8);
    values[1438] = 8871;

    keys[1439]   = hash_str_new("&mopf;", 6);
    values[1439] = 120158;

    keys[1440]   = hash_str_new("&mp;", 4);
    values[1440] = 8723;

    keys[1441]   = hash_str_new("&mscr;", 6);
    values[1441] = 120002;

    keys[1442]   = hash_str_new("&mstpos;", 8);
    values[1442] = 8766;

    keys[1443]   = hash_str_new("&mu;", 4);
    values[1443] = 956;

    keys[1444]   = hash_str_new("&multimap;", 10);
    values[1444] = 8888;

    keys[1445]   = hash_str_new("&mumap;", 7);
    values[1445] = 8888;

    keys[1446]   = hash_str_new("&nGg;", 5);
    values[1446] = 8921;

    keys[1447]   = hash_str_new("&nGt;", 5);
    values[1447] = 8811;

    keys[1448]   = hash_str_new("&nGtv;", 6);
    values[1448] = 8811;

    keys[1449]   = hash_str_new("&nLeftarrow;", 12);
    values[1449] = 8653;

    keys[1450]   = hash_str_new("&nLeftrightarrow;", 17);
    values[1450] = 8654;

    keys[1451]   = hash_str_new("&nLl;", 5);
    values[1451] = 8920;

    keys[1452]   = hash_str_new("&nLt;", 5);
    values[1452] = 8810;

    keys[1453]   = hash_str_new("&nLtv;", 6);
    values[1453] = 8810;

    keys[1454]   = hash_str_new("&nRightarrow;", 13);
    values[1454] = 8655;

    keys[1455]   = hash_str_new("&nVDash;", 8);
    values[1455] = 8879;

    keys[1456]   = hash_str_new("&nVdash;", 8);
    values[1456] = 8878;

    keys[1457]   = hash_str_new("&nabla;", 7);
    values[1457] = 8711;

    keys[1458]   = hash_str_new("&nacute;", 8);
    values[1458] = 324;

    keys[1459]   = hash_str_new("&nang;", 6);
    values[1459] = 8736;

    keys[1460]   = hash_str_new("&nap;", 5);
    values[1460] = 8777;

    keys[1461]   = hash_str_new("&napE;", 6);
    values[1461] = 10864;

    keys[1462]   = hash_str_new("&napid;", 7);
    values[1462] = 8779;

    keys[1463]   = hash_str_new("&napos;", 7);
    values[1463] = 329;

    keys[1464]   = hash_str_new("&napprox;", 9);
    values[1464] = 8777;

    keys[1465]   = hash_str_new("&natur;", 7);
    values[1465] = 9838;

    keys[1466]   = hash_str_new("&natural;", 9);
    values[1466] = 9838;

    keys[1467]   = hash_str_new("&naturals;", 10);
    values[1467] = 8469;

    keys[1468]   = hash_str_new("&nbsp", 5);
    values[1468] = 160;

    keys[1469]   = hash_str_new("&nbsp;", 6);
    values[1469] = 160;

    keys[1470]   = hash_str_new("&nbump;", 7);
    values[1470] = 8782;

    keys[1471]   = hash_str_new("&nbumpe;", 8);
    values[1471] = 8783;

    keys[1472]   = hash_str_new("&ncap;", 6);
    values[1472] = 10819;

    keys[1473]   = hash_str_new("&ncaron;", 8);
    values[1473] = 328;

    keys[1474]   = hash_str_new("&ncedil;", 8);
    values[1474] = 326;

    keys[1475]   = hash_str_new("&ncong;", 7);
    values[1475] = 8775;

    keys[1476]   = hash_str_new("&ncongdot;", 10);
    values[1476] = 10861;

    keys[1477]   = hash_str_new("&ncup;", 6);
    values[1477] = 10818;

    keys[1478]   = hash_str_new("&ncy;", 5);
    values[1478] = 1085;

    keys[1479]   = hash_str_new("&ndash;", 7);
    values[1479] = 8211;

    keys[1480]   = hash_str_new("&ne;", 4);
    values[1480] = 8800;

    keys[1481]   = hash_str_new("&neArr;", 7);
    values[1481] = 8663;

    keys[1482]   = hash_str_new("&nearhk;", 8);
    values[1482] = 10532;

    keys[1483]   = hash_str_new("&nearr;", 7);
    values[1483] = 8599;

    keys[1484]   = hash_str_new("&nearrow;", 9);
    values[1484] = 8599;

    keys[1485]   = hash_str_new("&nedot;", 7);
    values[1485] = 8784;

    keys[1486]   = hash_str_new("&nequiv;", 8);
    values[1486] = 8802;

    keys[1487]   = hash_str_new("&nesear;", 8);
    values[1487] = 10536;

    keys[1488]   = hash_str_new("&nesim;", 7);
    values[1488] = 8770;

    keys[1489]   = hash_str_new("&nexist;", 8);
    values[1489] = 8708;

    keys[1490]   = hash_str_new("&nexists;", 9);
    values[1490] = 8708;

    keys[1491]   = hash_str_new("&nfr;", 5);
    values[1491] = 120107;

    keys[1492]   = hash_str_new("&ngE;", 5);
    values[1492] = 8807;

    keys[1493]   = hash_str_new("&nge;", 5);
    values[1493] = 8817;

    keys[1494]   = hash_str_new("&ngeq;", 6);
    values[1494] = 8817;

    keys[1495]   = hash_str_new("&ngeqq;", 7);
    values[1495] = 8807;

    keys[1496]   = hash_str_new("&ngeqslant;", 11);
    values[1496] = 10878;

    keys[1497]   = hash_str_new("&nges;", 6);
    values[1497] = 10878;

    keys[1498]   = hash_str_new("&ngsim;", 7);
    values[1498] = 8821;

    keys[1499]   = hash_str_new("&ngt;", 5);
    values[1499] = 8815;

    keys[1500]   = hash_str_new("&ngtr;", 6);
    values[1500] = 8815;

    keys[1501]   = hash_str_new("&nhArr;", 7);
    values[1501] = 8654;

    keys[1502]   = hash_str_new("&nharr;", 7);
    values[1502] = 8622;

    keys[1503]   = hash_str_new("&nhpar;", 7);
    values[1503] = 10994;

    keys[1504]   = hash_str_new("&ni;", 4);
    values[1504] = 8715;

    keys[1505]   = hash_str_new("&nis;", 5);
    values[1505] = 8956;

    keys[1506]   = hash_str_new("&nisd;", 6);
    values[1506] = 8954;

    keys[1507]   = hash_str_new("&niv;", 5);
    values[1507] = 8715;

    keys[1508]   = hash_str_new("&njcy;", 6);
    values[1508] = 1114;

    keys[1509]   = hash_str_new("&nlArr;", 7);
    values[1509] = 8653;

    keys[1510]   = hash_str_new("&nlE;", 5);
    values[1510] = 8806;

    keys[1511]   = hash_str_new("&nlarr;", 7);
    values[1511] = 8602;

    keys[1512]   = hash_str_new("&nldr;", 6);
    values[1512] = 8229;

    keys[1513]   = hash_str_new("&nle;", 5);
    values[1513] = 8816;

    keys[1514]   = hash_str_new("&nleftarrow;", 12);
    values[1514] = 8602;

    keys[1515]   = hash_str_new("&nleftrightarrow;", 17);
    values[1515] = 8622;

    keys[1516]   = hash_str_new("&nleq;", 6);
    values[1516] = 8816;

    keys[1517]   = hash_str_new("&nleqq;", 7);
    values[1517] = 8806;

    keys[1518]   = hash_str_new("&nleqslant;", 11);
    values[1518] = 10877;

    keys[1519]   = hash_str_new("&nles;", 6);
    values[1519] = 10877;

    keys[1520]   = hash_str_new("&nless;", 7);
    values[1520] = 8814;

    keys[1521]   = hash_str_new("&nlsim;", 7);
    values[1521] = 8820;

    keys[1522]   = hash_str_new("&nlt;", 5);
    values[1522] = 8814;

    keys[1523]   = hash_str_new("&nltri;", 7);
    values[1523] = 8938;

    keys[1524]   = hash_str_new("&nltrie;", 8);
    values[1524] = 8940;

    keys[1525]   = hash_str_new("&nmid;", 6);
    values[1525] = 8740;

    keys[1526]   = hash_str_new("&nopf;", 6);
    values[1526] = 120159;

    keys[1527]   = hash_str_new("&not", 4);
    values[1527] = 172;

    keys[1528]   = hash_str_new("&not;", 5);
    values[1528] = 172;

    keys[1529]   = hash_str_new("&notin;", 7);
    values[1529] = 8713;

    keys[1530]   = hash_str_new("&notinE;", 8);
    values[1530] = 8953;

    keys[1531]   = hash_str_new("&notindot;", 10);
    values[1531] = 8949;

    keys[1532]   = hash_str_new("&notinva;", 9);
    values[1532] = 8713;

    keys[1533]   = hash_str_new("&notinvb;", 9);
    values[1533] = 8951;

    keys[1534]   = hash_str_new("&notinvc;", 9);
    values[1534] = 8950;

    keys[1535]   = hash_str_new("&notni;", 7);
    values[1535] = 8716;

    keys[1536]   = hash_str_new("&notniva;", 9);
    values[1536] = 8716;

    keys[1537]   = hash_str_new("&notnivb;", 9);
    values[1537] = 8958;

    keys[1538]   = hash_str_new("&notnivc;", 9);
    values[1538] = 8957;

    keys[1539]   = hash_str_new("&npar;", 6);
    values[1539] = 8742;

    keys[1540]   = hash_str_new("&nparallel;", 11);
    values[1540] = 8742;

    keys[1541]   = hash_str_new("&nparsl;", 8);
    values[1541] = 11005;

    keys[1542]   = hash_str_new("&npart;", 7);
    values[1542] = 8706;

    keys[1543]   = hash_str_new("&npolint;", 9);
    values[1543] = 10772;

    keys[1544]   = hash_str_new("&npr;", 5);
    values[1544] = 8832;

    keys[1545]   = hash_str_new("&nprcue;", 8);
    values[1545] = 8928;

    keys[1546]   = hash_str_new("&npre;", 6);
    values[1546] = 10927;

    keys[1547]   = hash_str_new("&nprec;", 7);
    values[1547] = 8832;

    keys[1548]   = hash_str_new("&npreceq;", 9);
    values[1548] = 10927;

    keys[1549]   = hash_str_new("&nrArr;", 7);
    values[1549] = 8655;

    keys[1550]   = hash_str_new("&nrarr;", 7);
    values[1550] = 8603;

    keys[1551]   = hash_str_new("&nrarrc;", 8);
    values[1551] = 10547;

    keys[1552]   = hash_str_new("&nrarrw;", 8);
    values[1552] = 8605;

    keys[1553]   = hash_str_new("&nrightarrow;", 13);
    values[1553] = 8603;

    keys[1554]   = hash_str_new("&nrtri;", 7);
    values[1554] = 8939;

    keys[1555]   = hash_str_new("&nrtrie;", 8);
    values[1555] = 8941;

    keys[1556]   = hash_str_new("&nsc;", 5);
    values[1556] = 8833;

    keys[1557]   = hash_str_new("&nsccue;", 8);
    values[1557] = 8929;

    keys[1558]   = hash_str_new("&nsce;", 6);
    values[1558] = 10928;

    keys[1559]   = hash_str_new("&nscr;", 6);
    values[1559] = 120003;

    keys[1560]   = hash_str_new("&nshortmid;", 11);
    values[1560] = 8740;

    keys[1561]   = hash_str_new("&nshortparallel;", 16);
    values[1561] = 8742;

    keys[1562]   = hash_str_new("&nsim;", 6);
    values[1562] = 8769;

    keys[1563]   = hash_str_new("&nsime;", 7);
    values[1563] = 8772;

    keys[1564]   = hash_str_new("&nsimeq;", 8);
    values[1564] = 8772;

    keys[1565]   = hash_str_new("&nsmid;", 7);
    values[1565] = 8740;

    keys[1566]   = hash_str_new("&nspar;", 7);
    values[1566] = 8742;

    keys[1567]   = hash_str_new("&nsqsube;", 9);
    values[1567] = 8930;

    keys[1568]   = hash_str_new("&nsqsupe;", 9);
    values[1568] = 8931;

    keys[1569]   = hash_str_new("&nsub;", 6);
    values[1569] = 8836;

    keys[1570]   = hash_str_new("&nsubE;", 7);
    values[1570] = 10949;

    keys[1571]   = hash_str_new("&nsube;", 7);
    values[1571] = 8840;

    keys[1572]   = hash_str_new("&nsubset;", 9);
    values[1572] = 8834;

    keys[1573]   = hash_str_new("&nsubseteq;", 11);
    values[1573] = 8840;

    keys[1574]   = hash_str_new("&nsubseteqq;", 12);
    values[1574] = 10949;

    keys[1575]   = hash_str_new("&nsucc;", 7);
    values[1575] = 8833;

    keys[1576]   = hash_str_new("&nsucceq;", 9);
    values[1576] = 10928;

    keys[1577]   = hash_str_new("&nsup;", 6);
    values[1577] = 8837;

    keys[1578]   = hash_str_new("&nsupE;", 7);
    values[1578] = 10950;

    keys[1579]   = hash_str_new("&nsupe;", 7);
    values[1579] = 8841;

    keys[1580]   = hash_str_new("&nsupset;", 9);
    values[1580] = 8835;

    keys[1581]   = hash_str_new("&nsupseteq;", 11);
    values[1581] = 8841;

    keys[1582]   = hash_str_new("&nsupseteqq;", 12);
    values[1582] = 10950;

    keys[1583]   = hash_str_new("&ntgl;", 6);
    values[1583] = 8825;

    keys[1584]   = hash_str_new("&ntilde", 7);
    values[1584] = 241;

    keys[1585]   = hash_str_new("&ntilde;", 8);
    values[1585] = 241;

    keys[1586]   = hash_str_new("&ntlg;", 6);
    values[1586] = 8824;

    keys[1587]   = hash_str_new("&ntriangleleft;", 15);
    values[1587] = 8938;

    keys[1588]   = hash_str_new("&ntrianglelefteq;", 17);
    values[1588] = 8940;

    keys[1589]   = hash_str_new("&ntriangleright;", 16);
    values[1589] = 8939;

    keys[1590]   = hash_str_new("&ntrianglerighteq;", 18);
    values[1590] = 8941;

    keys[1591]   = hash_str_new("&nu;", 4);
    values[1591] = 957;

    keys[1592]   = hash_str_new("&num;", 5);
    values[1592] = 35;

    keys[1593]   = hash_str_new("&numero;", 8);
    values[1593] = 8470;

    keys[1594]   = hash_str_new("&numsp;", 7);
    values[1594] = 8199;

    keys[1595]   = hash_str_new("&nvDash;", 8);
    values[1595] = 8877;

    keys[1596]   = hash_str_new("&nvHarr;", 8);
    values[1596] = 10500;

    keys[1597]   = hash_str_new("&nvap;", 6);
    values[1597] = 8781;

    keys[1598]   = hash_str_new("&nvdash;", 8);
    values[1598] = 8876;

    keys[1599]   = hash_str_new("&nvge;", 6);
    values[1599] = 8805;

    keys[1600]   = hash_str_new("&nvgt;", 6);
    values[1600] = 62;

    keys[1601]   = hash_str_new("&nvinfin;", 9);
    values[1601] = 10718;

    keys[1602]   = hash_str_new("&nvlArr;", 8);
    values[1602] = 10498;

    keys[1603]   = hash_str_new("&nvle;", 6);
    values[1603] = 8804;

    keys[1604]   = hash_str_new("&nvlt;", 6);
    values[1604] = 60;

    keys[1605]   = hash_str_new("&nvltrie;", 9);
    values[1605] = 8884;

    keys[1606]   = hash_str_new("&nvrArr;", 8);
    values[1606] = 10499;

    keys[1607]   = hash_str_new("&nvrtrie;", 9);
    values[1607] = 8885;

    keys[1608]   = hash_str_new("&nvsim;", 7);
    values[1608] = 8764;

    keys[1609]   = hash_str_new("&nwArr;", 7);
    values[1609] = 8662;

    keys[1610]   = hash_str_new("&nwarhk;", 8);
    values[1610] = 10531;

    keys[1611]   = hash_str_new("&nwarr;", 7);
    values[1611] = 8598;

    keys[1612]   = hash_str_new("&nwarrow;", 9);
    values[1612] = 8598;

    keys[1613]   = hash_str_new("&nwnear;", 8);
    values[1613] = 10535;

    keys[1614]   = hash_str_new("&oS;", 4);
    values[1614] = 9416;

    keys[1615]   = hash_str_new("&oacute", 7);
    values[1615] = 243;

    keys[1616]   = hash_str_new("&oacute;", 8);
    values[1616] = 243;

    keys[1617]   = hash_str_new("&oast;", 6);
    values[1617] = 8859;

    keys[1618]   = hash_str_new("&ocir;", 6);
    values[1618] = 8858;

    keys[1619]   = hash_str_new("&ocirc", 6);
    values[1619] = 244;

    keys[1620]   = hash_str_new("&ocirc;", 7);
    values[1620] = 244;

    keys[1621]   = hash_str_new("&ocy;", 5);
    values[1621] = 1086;

    keys[1622]   = hash_str_new("&odash;", 7);
    values[1622] = 8861;

    keys[1623]   = hash_str_new("&odblac;", 8);
    values[1623] = 337;

    keys[1624]   = hash_str_new("&odiv;", 6);
    values[1624] = 10808;

    keys[1625]   = hash_str_new("&odot;", 6);
    values[1625] = 8857;

    keys[1626]   = hash_str_new("&odsold;", 8);
    values[1626] = 10684;

    keys[1627]   = hash_str_new("&oelig;", 7);
    values[1627] = 339;

    keys[1628]   = hash_str_new("&ofcir;", 7);
    values[1628] = 10687;

    keys[1629]   = hash_str_new("&ofr;", 5);
    values[1629] = 120108;

    keys[1630]   = hash_str_new("&ogon;", 6);
    values[1630] = 731;

    keys[1631]   = hash_str_new("&ograve", 7);
    values[1631] = 242;

    keys[1632]   = hash_str_new("&ograve;", 8);
    values[1632] = 242;

    keys[1633]   = hash_str_new("&ogt;", 5);
    values[1633] = 10689;

    keys[1634]   = hash_str_new("&ohbar;", 7);
    values[1634] = 10677;

    keys[1635]   = hash_str_new("&ohm;", 5);
    values[1635] = 937;

    keys[1636]   = hash_str_new("&oint;", 6);
    values[1636] = 8750;

    keys[1637]   = hash_str_new("&olarr;", 7);
    values[1637] = 8634;

    keys[1638]   = hash_str_new("&olcir;", 7);
    values[1638] = 10686;

    keys[1639]   = hash_str_new("&olcross;", 9);
    values[1639] = 10683;

    keys[1640]   = hash_str_new("&oline;", 7);
    values[1640] = 8254;

    keys[1641]   = hash_str_new("&olt;", 5);
    values[1641] = 10688;

    keys[1642]   = hash_str_new("&omacr;", 7);
    values[1642] = 333;

    keys[1643]   = hash_str_new("&omega;", 7);
    values[1643] = 969;

    keys[1644]   = hash_str_new("&omicron;", 9);
    values[1644] = 959;

    keys[1645]   = hash_str_new("&omid;", 6);
    values[1645] = 10678;

    keys[1646]   = hash_str_new("&ominus;", 8);
    values[1646] = 8854;

    keys[1647]   = hash_str_new("&oopf;", 6);
    values[1647] = 120160;

    keys[1648]   = hash_str_new("&opar;", 6);
    values[1648] = 10679;

    keys[1649]   = hash_str_new("&operp;", 7);
    values[1649] = 10681;

    keys[1650]   = hash_str_new("&oplus;", 7);
    values[1650] = 8853;

    keys[1651]   = hash_str_new("&or;", 4);
    values[1651] = 8744;

    keys[1652]   = hash_str_new("&orarr;", 7);
    values[1652] = 8635;

    keys[1653]   = hash_str_new("&ord;", 5);
    values[1653] = 10845;

    keys[1654]   = hash_str_new("&order;", 7);
    values[1654] = 8500;

    keys[1655]   = hash_str_new("&orderof;", 9);
    values[1655] = 8500;

    keys[1656]   = hash_str_new("&ordf", 5);
    values[1656] = 170;

    keys[1657]   = hash_str_new("&ordf;", 6);
    values[1657] = 170;

    keys[1658]   = hash_str_new("&ordm", 5);
    values[1658] = 186;

    keys[1659]   = hash_str_new("&ordm;", 6);
    values[1659] = 186;

    keys[1660]   = hash_str_new("&origof;", 8);
    values[1660] = 8886;

    keys[1661]   = hash_str_new("&oror;", 6);
    values[1661] = 10838;

    keys[1662]   = hash_str_new("&orslope;", 9);
    values[1662] = 10839;

    keys[1663]   = hash_str_new("&orv;", 5);
    values[1663] = 10843;

    keys[1664]   = hash_str_new("&oscr;", 6);
    values[1664] = 8500;

    keys[1665]   = hash_str_new("&oslash", 7);
    values[1665] = 248;

    keys[1666]   = hash_str_new("&oslash;", 8);
    values[1666] = 248;

    keys[1667]   = hash_str_new("&osol;", 6);
    values[1667] = 8856;

    keys[1668]   = hash_str_new("&otilde", 7);
    values[1668] = 245;

    keys[1669]   = hash_str_new("&otilde;", 8);
    values[1669] = 245;

    keys[1670]   = hash_str_new("&otimes;", 8);
    values[1670] = 8855;

    keys[1671]   = hash_str_new("&otimesas;", 10);
    values[1671] = 10806;

    keys[1672]   = hash_str_new("&ouml", 5);
    values[1672] = 246;

    keys[1673]   = hash_str_new("&ouml;", 6);
    values[1673] = 246;

    keys[1674]   = hash_str_new("&ovbar;", 7);
    values[1674] = 9021;

    keys[1675]   = hash_str_new("&par;", 5);
    values[1675] = 8741;

    keys[1676]   = hash_str_new("&para", 5);
    values[1676] = 182;

    keys[1677]   = hash_str_new("&para;", 6);
    values[1677] = 182;

    keys[1678]   = hash_str_new("&parallel;", 10);
    values[1678] = 8741;

    keys[1679]   = hash_str_new("&parsim;", 8);
    values[1679] = 10995;

    keys[1680]   = hash_str_new("&parsl;", 7);
    values[1680] = 11005;

    keys[1681]   = hash_str_new("&part;", 6);
    values[1681] = 8706;

    keys[1682]   = hash_str_new("&pcy;", 5);
    values[1682] = 1087;

    keys[1683]   = hash_str_new("&percnt;", 8);
    values[1683] = 37;

    keys[1684]   = hash_str_new("&period;", 8);
    values[1684] = 46;

    keys[1685]   = hash_str_new("&permil;", 8);
    values[1685] = 8240;

    keys[1686]   = hash_str_new("&perp;", 6);
    values[1686] = 8869;

    keys[1687]   = hash_str_new("&pertenk;", 9);
    values[1687] = 8241;

    keys[1688]   = hash_str_new("&pfr;", 5);
    values[1688] = 120109;

    keys[1689]   = hash_str_new("&phi;", 5);
    values[1689] = 966;

    keys[1690]   = hash_str_new("&phiv;", 6);
    values[1690] = 981;

    keys[1691]   = hash_str_new("&phmmat;", 8);
    values[1691] = 8499;

    keys[1692]   = hash_str_new("&phone;", 7);
    values[1692] = 9742;

    keys[1693]   = hash_str_new("&pi;", 4);
    values[1693] = 960;

    keys[1694]   = hash_str_new("&pitchfork;", 11);
    values[1694] = 8916;

    keys[1695]   = hash_str_new("&piv;", 5);
    values[1695] = 982;

    keys[1696]   = hash_str_new("&planck;", 8);
    values[1696] = 8463;

    keys[1697]   = hash_str_new("&planckh;", 9);
    values[1697] = 8462;

    keys[1698]   = hash_str_new("&plankv;", 8);
    values[1698] = 8463;

    keys[1699]   = hash_str_new("&plus;", 6);
    values[1699] = 43;

    keys[1700]   = hash_str_new("&plusacir;", 10);
    values[1700] = 10787;

    keys[1701]   = hash_str_new("&plusb;", 7);
    values[1701] = 8862;

    keys[1702]   = hash_str_new("&pluscir;", 9);
    values[1702] = 10786;

    keys[1703]   = hash_str_new("&plusdo;", 8);
    values[1703] = 8724;

    keys[1704]   = hash_str_new("&plusdu;", 8);
    values[1704] = 10789;

    keys[1705]   = hash_str_new("&pluse;", 7);
    values[1705] = 10866;

    keys[1706]   = hash_str_new("&plusmn", 7);
    values[1706] = 177;

    keys[1707]   = hash_str_new("&plusmn;", 8);
    values[1707] = 177;

    keys[1708]   = hash_str_new("&plussim;", 9);
    values[1708] = 10790;

    keys[1709]   = hash_str_new("&plustwo;", 9);
    values[1709] = 10791;

    keys[1710]   = hash_str_new("&pm;", 4);
    values[1710] = 177;

    keys[1711]   = hash_str_new("&pointint;", 10);
    values[1711] = 10773;

    keys[1712]   = hash_str_new("&popf;", 6);
    values[1712] = 120161;

    keys[1713]   = hash_str_new("&pound", 6);
    values[1713] = 163;

    keys[1714]   = hash_str_new("&pound;", 7);
    values[1714] = 163;

    keys[1715]   = hash_str_new("&pr;", 4);
    values[1715] = 8826;

    keys[1716]   = hash_str_new("&prE;", 5);
    values[1716] = 10931;

    keys[1717]   = hash_str_new("&prap;", 6);
    values[1717] = 10935;

    keys[1718]   = hash_str_new("&prcue;", 7);
    values[1718] = 8828;

    keys[1719]   = hash_str_new("&pre;", 5);
    values[1719] = 10927;

    keys[1720]   = hash_str_new("&prec;", 6);
    values[1720] = 8826;

    keys[1721]   = hash_str_new("&precapprox;", 12);
    values[1721] = 10935;

    keys[1722]   = hash_str_new("&preccurlyeq;", 13);
    values[1722] = 8828;

    keys[1723]   = hash_str_new("&preceq;", 8);
    values[1723] = 10927;

    keys[1724]   = hash_str_new("&precnapprox;", 13);
    values[1724] = 10937;

    keys[1725]   = hash_str_new("&precneqq;", 10);
    values[1725] = 10933;

    keys[1726]   = hash_str_new("&precnsim;", 10);
    values[1726] = 8936;

    keys[1727]   = hash_str_new("&precsim;", 9);
    values[1727] = 8830;

    keys[1728]   = hash_str_new("&prime;", 7);
    values[1728] = 8242;

    keys[1729]   = hash_str_new("&primes;", 8);
    values[1729] = 8473;

    keys[1730]   = hash_str_new("&prnE;", 6);
    values[1730] = 10933;

    keys[1731]   = hash_str_new("&prnap;", 7);
    values[1731] = 10937;

    keys[1732]   = hash_str_new("&prnsim;", 8);
    values[1732] = 8936;

    keys[1733]   = hash_str_new("&prod;", 6);
    values[1733] = 8719;

    keys[1734]   = hash_str_new("&profalar;", 10);
    values[1734] = 9006;

    keys[1735]   = hash_str_new("&profline;", 10);
    values[1735] = 8978;

    keys[1736]   = hash_str_new("&profsurf;", 10);
    values[1736] = 8979;

    keys[1737]   = hash_str_new("&prop;", 6);
    values[1737] = 8733;

    keys[1738]   = hash_str_new("&propto;", 8);
    values[1738] = 8733;

    keys[1739]   = hash_str_new("&prsim;", 7);
    values[1739] = 8830;

    keys[1740]   = hash_str_new("&prurel;", 8);
    values[1740] = 8880;

    keys[1741]   = hash_str_new("&pscr;", 6);
    values[1741] = 120005;

    keys[1742]   = hash_str_new("&psi;", 5);
    values[1742] = 968;

    keys[1743]   = hash_str_new("&puncsp;", 8);
    values[1743] = 8200;

    keys[1744]   = hash_str_new("&qfr;", 5);
    values[1744] = 120110;

    keys[1745]   = hash_str_new("&qint;", 6);
    values[1745] = 10764;

    keys[1746]   = hash_str_new("&qopf;", 6);
    values[1746] = 120162;

    keys[1747]   = hash_str_new("&qprime;", 8);
    values[1747] = 8279;

    keys[1748]   = hash_str_new("&qscr;", 6);
    values[1748] = 120006;

    keys[1749]   = hash_str_new("&quaternions;", 13);
    values[1749] = 8461;

    keys[1750]   = hash_str_new("&quatint;", 9);
    values[1750] = 10774;

    keys[1751]   = hash_str_new("&quest;", 7);
    values[1751] = 63;

    keys[1752]   = hash_str_new("&questeq;", 9);
    values[1752] = 8799;

    keys[1753]   = hash_str_new("&quot", 5);
    values[1753] = 34;

    keys[1754]   = hash_str_new("&quot;", 6);
    values[1754] = 34;

    keys[1755]   = hash_str_new("&rAarr;", 7);
    values[1755] = 8667;

    keys[1756]   = hash_str_new("&rArr;", 6);
    values[1756] = 8658;

    keys[1757]   = hash_str_new("&rAtail;", 8);
    values[1757] = 10524;

    keys[1758]   = hash_str_new("&rBarr;", 7);
    values[1758] = 10511;

    keys[1759]   = hash_str_new("&rHar;", 6);
    values[1759] = 10596;

    keys[1760]   = hash_str_new("&race;", 6);
    values[1760] = 8765;

    keys[1761]   = hash_str_new("&racute;", 8);
    values[1761] = 341;

    keys[1762]   = hash_str_new("&radic;", 7);
    values[1762] = 8730;

    keys[1763]   = hash_str_new("&raemptyv;", 10);
    values[1763] = 10675;

    keys[1764]   = hash_str_new("&rang;", 6);
    values[1764] = 10217;

    keys[1765]   = hash_str_new("&rangd;", 7);
    values[1765] = 10642;

    keys[1766]   = hash_str_new("&range;", 7);
    values[1766] = 10661;

    keys[1767]   = hash_str_new("&rangle;", 8);
    values[1767] = 10217;

    keys[1768]   = hash_str_new("&raquo", 6);
    values[1768] = 187;

    keys[1769]   = hash_str_new("&raquo;", 7);
    values[1769] = 187;

    keys[1770]   = hash_str_new("&rarr;", 6);
    values[1770] = 8594;

    keys[1771]   = hash_str_new("&rarrap;", 8);
    values[1771] = 10613;

    keys[1772]   = hash_str_new("&rarrb;", 7);
    values[1772] = 8677;

    keys[1773]   = hash_str_new("&rarrbfs;", 9);
    values[1773] = 10528;

    keys[1774]   = hash_str_new("&rarrc;", 7);
    values[1774] = 10547;

    keys[1775]   = hash_str_new("&rarrfs;", 8);
    values[1775] = 10526;

    keys[1776]   = hash_str_new("&rarrhk;", 8);
    values[1776] = 8618;

    keys[1777]   = hash_str_new("&rarrlp;", 8);
    values[1777] = 8620;

    keys[1778]   = hash_str_new("&rarrpl;", 8);
    values[1778] = 10565;

    keys[1779]   = hash_str_new("&rarrsim;", 9);
    values[1779] = 10612;

    keys[1780]   = hash_str_new("&rarrtl;", 8);
    values[1780] = 8611;

    keys[1781]   = hash_str_new("&rarrw;", 7);
    values[1781] = 8605;

    keys[1782]   = hash_str_new("&ratail;", 8);
    values[1782] = 10522;

    keys[1783]   = hash_str_new("&ratio;", 7);
    values[1783] = 8758;

    keys[1784]   = hash_str_new("&rationals;", 11);
    values[1784] = 8474;

    keys[1785]   = hash_str_new("&rbarr;", 7);
    values[1785] = 10509;

    keys[1786]   = hash_str_new("&rbbrk;", 7);
    values[1786] = 10099;

    keys[1787]   = hash_str_new("&rbrace;", 8);
    values[1787] = 125;

    keys[1788]   = hash_str_new("&rbrack;", 8);
    values[1788] = 93;

    keys[1789]   = hash_str_new("&rbrke;", 7);
    values[1789] = 10636;

    keys[1790]   = hash_str_new("&rbrksld;", 9);
    values[1790] = 10638;

    keys[1791]   = hash_str_new("&rbrkslu;", 9);
    values[1791] = 10640;

    keys[1792]   = hash_str_new("&rcaron;", 8);
    values[1792] = 345;

    keys[1793]   = hash_str_new("&rcedil;", 8);
    values[1793] = 343;

    keys[1794]   = hash_str_new("&rceil;", 7);
    values[1794] = 8969;

    keys[1795]   = hash_str_new("&rcub;", 6);
    values[1795] = 125;

    keys[1796]   = hash_str_new("&rcy;", 5);
    values[1796] = 1088;

    keys[1797]   = hash_str_new("&rdca;", 6);
    values[1797] = 10551;

    keys[1798]   = hash_str_new("&rdldhar;", 9);
    values[1798] = 10601;

    keys[1799]   = hash_str_new("&rdquo;", 7);
    values[1799] = 8221;

    keys[1800]   = hash_str_new("&rdquor;", 8);
    values[1800] = 8221;

    keys[1801]   = hash_str_new("&rdsh;", 6);
    values[1801] = 8627;

    keys[1802]   = hash_str_new("&real;", 6);
    values[1802] = 8476;

    keys[1803]   = hash_str_new("&realine;", 9);
    values[1803] = 8475;

    keys[1804]   = hash_str_new("&realpart;", 10);
    values[1804] = 8476;

    keys[1805]   = hash_str_new("&reals;", 7);
    values[1805] = 8477;

    keys[1806]   = hash_str_new("&rect;", 6);
    values[1806] = 9645;

    keys[1807]   = hash_str_new("&reg", 4);
    values[1807] = 174;

    keys[1808]   = hash_str_new("&reg;", 5);
    values[1808] = 174;

    keys[1809]   = hash_str_new("&rfisht;", 8);
    values[1809] = 10621;

    keys[1810]   = hash_str_new("&rfloor;", 8);
    values[1810] = 8971;

    keys[1811]   = hash_str_new("&rfr;", 5);
    values[1811] = 120111;

    keys[1812]   = hash_str_new("&rhard;", 7);
    values[1812] = 8641;

    keys[1813]   = hash_str_new("&rharu;", 7);
    values[1813] = 8640;

    keys[1814]   = hash_str_new("&rharul;", 8);
    values[1814] = 10604;

    keys[1815]   = hash_str_new("&rho;", 5);
    values[1815] = 961;

    keys[1816]   = hash_str_new("&rhov;", 6);
    values[1816] = 1009;

    keys[1817]   = hash_str_new("&rightarrow;", 12);
    values[1817] = 8594;

    keys[1818]   = hash_str_new("&rightarrowtail;", 16);
    values[1818] = 8611;

    keys[1819]   = hash_str_new("&rightharpoondown;", 18);
    values[1819] = 8641;

    keys[1820]   = hash_str_new("&rightharpoonup;", 16);
    values[1820] = 8640;

    keys[1821]   = hash_str_new("&rightleftarrows;", 17);
    values[1821] = 8644;

    keys[1822]   = hash_str_new("&rightleftharpoons;", 19);
    values[1822] = 8652;

    keys[1823]   = hash_str_new("&rightrightarrows;", 18);
    values[1823] = 8649;

    keys[1824]   = hash_str_new("&rightsquigarrow;", 17);
    values[1824] = 8605;

    keys[1825]   = hash_str_new("&rightthreetimes;", 17);
    values[1825] = 8908;

    keys[1826]   = hash_str_new("&ring;", 6);
    values[1826] = 730;

    keys[1827]   = hash_str_new("&risingdotseq;", 14);
    values[1827] = 8787;

    keys[1828]   = hash_str_new("&rlarr;", 7);
    values[1828] = 8644;

    keys[1829]   = hash_str_new("&rlhar;", 7);
    values[1829] = 8652;

    keys[1830]   = hash_str_new("&rlm;", 5);
    values[1830] = 8207;

    keys[1831]   = hash_str_new("&rmoust;", 8);
    values[1831] = 9137;

    keys[1832]   = hash_str_new("&rmoustache;", 12);
    values[1832] = 9137;

    keys[1833]   = hash_str_new("&rnmid;", 7);
    values[1833] = 10990;

    keys[1834]   = hash_str_new("&roang;", 7);
    values[1834] = 10221;

    keys[1835]   = hash_str_new("&roarr;", 7);
    values[1835] = 8702;

    keys[1836]   = hash_str_new("&robrk;", 7);
    values[1836] = 10215;

    keys[1837]   = hash_str_new("&ropar;", 7);
    values[1837] = 10630;

    keys[1838]   = hash_str_new("&ropf;", 6);
    values[1838] = 120163;

    keys[1839]   = hash_str_new("&roplus;", 8);
    values[1839] = 10798;

    keys[1840]   = hash_str_new("&rotimes;", 9);
    values[1840] = 10805;

    keys[1841]   = hash_str_new("&rpar;", 6);
    values[1841] = 41;

    keys[1842]   = hash_str_new("&rpargt;", 8);
    values[1842] = 10644;

    keys[1843]   = hash_str_new("&rppolint;", 10);
    values[1843] = 10770;

    keys[1844]   = hash_str_new("&rrarr;", 7);
    values[1844] = 8649;

    keys[1845]   = hash_str_new("&rsaquo;", 8);
    values[1845] = 8250;

    keys[1846]   = hash_str_new("&rscr;", 6);
    values[1846] = 120007;

    keys[1847]   = hash_str_new("&rsh;", 5);
    values[1847] = 8625;

    keys[1848]   = hash_str_new("&rsqb;", 6);
    values[1848] = 93;

    keys[1849]   = hash_str_new("&rsquo;", 7);
    values[1849] = 8217;

    keys[1850]   = hash_str_new("&rsquor;", 8);
    values[1850] = 8217;

    keys[1851]   = hash_str_new("&rthree;", 8);
    values[1851] = 8908;

    keys[1852]   = hash_str_new("&rtimes;", 8);
    values[1852] = 8906;

    keys[1853]   = hash_str_new("&rtri;", 6);
    values[1853] = 9657;

    keys[1854]   = hash_str_new("&rtrie;", 7);
    values[1854] = 8885;

    keys[1855]   = hash_str_new("&rtrif;", 7);
    values[1855] = 9656;

    keys[1856]   = hash_str_new("&rtriltri;", 10);
    values[1856] = 10702;

    keys[1857]   = hash_str_new("&ruluhar;", 9);
    values[1857] = 10600;

    keys[1858]   = hash_str_new("&rx;", 4);
    values[1858] = 8478;

    keys[1859]   = hash_str_new("&sacute;", 8);
    values[1859] = 347;

    keys[1860]   = hash_str_new("&sbquo;", 7);
    values[1860] = 8218;

    keys[1861]   = hash_str_new("&sc;", 4);
    values[1861] = 8827;

    keys[1862]   = hash_str_new("&scE;", 5);
    values[1862] = 10932;

    keys[1863]   = hash_str_new("&scap;", 6);
    values[1863] = 10936;

    keys[1864]   = hash_str_new("&scaron;", 8);
    values[1864] = 353;

    keys[1865]   = hash_str_new("&sccue;", 7);
    values[1865] = 8829;

    keys[1866]   = hash_str_new("&sce;", 5);
    values[1866] = 10928;

    keys[1867]   = hash_str_new("&scedil;", 8);
    values[1867] = 351;

    keys[1868]   = hash_str_new("&scirc;", 7);
    values[1868] = 349;

    keys[1869]   = hash_str_new("&scnE;", 6);
    values[1869] = 10934;

    keys[1870]   = hash_str_new("&scnap;", 7);
    values[1870] = 10938;

    keys[1871]   = hash_str_new("&scnsim;", 8);
    values[1871] = 8937;

    keys[1872]   = hash_str_new("&scpolint;", 10);
    values[1872] = 10771;

    keys[1873]   = hash_str_new("&scsim;", 7);
    values[1873] = 8831;

    keys[1874]   = hash_str_new("&scy;", 5);
    values[1874] = 1089;

    keys[1875]   = hash_str_new("&sdot;", 6);
    values[1875] = 8901;

    keys[1876]   = hash_str_new("&sdotb;", 7);
    values[1876] = 8865;

    keys[1877]   = hash_str_new("&sdote;", 7);
    values[1877] = 10854;

    keys[1878]   = hash_str_new("&seArr;", 7);
    values[1878] = 8664;

    keys[1879]   = hash_str_new("&searhk;", 8);
    values[1879] = 10533;

    keys[1880]   = hash_str_new("&searr;", 7);
    values[1880] = 8600;

    keys[1881]   = hash_str_new("&searrow;", 9);
    values[1881] = 8600;

    keys[1882]   = hash_str_new("&sect", 5);
    values[1882] = 167;

    keys[1883]   = hash_str_new("&sect;", 6);
    values[1883] = 167;

    keys[1884]   = hash_str_new("&semi;", 6);
    values[1884] = 59;

    keys[1885]   = hash_str_new("&seswar;", 8);
    values[1885] = 10537;

    keys[1886]   = hash_str_new("&setminus;", 10);
    values[1886] = 8726;

    keys[1887]   = hash_str_new("&setmn;", 7);
    values[1887] = 8726;

    keys[1888]   = hash_str_new("&sext;", 6);
    values[1888] = 10038;

    keys[1889]   = hash_str_new("&sfr;", 5);
    values[1889] = 120112;

    keys[1890]   = hash_str_new("&sfrown;", 8);
    values[1890] = 8994;

    keys[1891]   = hash_str_new("&sharp;", 7);
    values[1891] = 9839;

    keys[1892]   = hash_str_new("&shchcy;", 8);
    values[1892] = 1097;

    keys[1893]   = hash_str_new("&shcy;", 6);
    values[1893] = 1096;

    keys[1894]   = hash_str_new("&shortmid;", 10);
    values[1894] = 8739;

    keys[1895]   = hash_str_new("&shortparallel;", 15);
    values[1895] = 8741;

    keys[1896]   = hash_str_new("&shy", 4);
    values[1896] = 173;

    keys[1897]   = hash_str_new("&shy;", 5);
    values[1897] = 173;

    keys[1898]   = hash_str_new("&sigma;", 7);
    values[1898] = 963;

    keys[1899]   = hash_str_new("&sigmaf;", 8);
    values[1899] = 962;

    keys[1900]   = hash_str_new("&sigmav;", 8);
    values[1900] = 962;

    keys[1901]   = hash_str_new("&sim;", 5);
    values[1901] = 8764;

    keys[1902]   = hash_str_new("&simdot;", 8);
    values[1902] = 10858;

    keys[1903]   = hash_str_new("&sime;", 6);
    values[1903] = 8771;

    keys[1904]   = hash_str_new("&simeq;", 7);
    values[1904] = 8771;

    keys[1905]   = hash_str_new("&simg;", 6);
    values[1905] = 10910;

    keys[1906]   = hash_str_new("&simgE;", 7);
    values[1906] = 10912;

    keys[1907]   = hash_str_new("&siml;", 6);
    values[1907] = 10909;

    keys[1908]   = hash_str_new("&simlE;", 7);
    values[1908] = 10911;

    keys[1909]   = hash_str_new("&simne;", 7);
    values[1909] = 8774;

    keys[1910]   = hash_str_new("&simplus;", 9);
    values[1910] = 10788;

    keys[1911]   = hash_str_new("&simrarr;", 9);
    values[1911] = 10610;

    keys[1912]   = hash_str_new("&slarr;", 7);
    values[1912] = 8592;

    keys[1913]   = hash_str_new("&smallsetminus;", 15);
    values[1913] = 8726;

    keys[1914]   = hash_str_new("&smashp;", 8);
    values[1914] = 10803;

    keys[1915]   = hash_str_new("&smeparsl;", 10);
    values[1915] = 10724;

    keys[1916]   = hash_str_new("&smid;", 6);
    values[1916] = 8739;

    keys[1917]   = hash_str_new("&smile;", 7);
    values[1917] = 8995;

    keys[1918]   = hash_str_new("&smt;", 5);
    values[1918] = 10922;

    keys[1919]   = hash_str_new("&smte;", 6);
    values[1919] = 10924;

    keys[1920]   = hash_str_new("&smtes;", 7);
    values[1920] = 10924;

    keys[1921]   = hash_str_new("&softcy;", 8);
    values[1921] = 1100;

    keys[1922]   = hash_str_new("&sol;", 5);
    values[1922] = 47;

    keys[1923]   = hash_str_new("&solb;", 6);
    values[1923] = 10692;

    keys[1924]   = hash_str_new("&solbar;", 8);
    values[1924] = 9023;

    keys[1925]   = hash_str_new("&sopf;", 6);
    values[1925] = 120164;

    keys[1926]   = hash_str_new("&spades;", 8);
    values[1926] = 9824;

    keys[1927]   = hash_str_new("&spadesuit;", 11);
    values[1927] = 9824;

    keys[1928]   = hash_str_new("&spar;", 6);
    values[1928] = 8741;

    keys[1929]   = hash_str_new("&sqcap;", 7);
    values[1929] = 8851;

    keys[1930]   = hash_str_new("&sqcaps;", 8);
    values[1930] = 8851;

    keys[1931]   = hash_str_new("&sqcup;", 7);
    values[1931] = 8852;

    keys[1932]   = hash_str_new("&sqcups;", 8);
    values[1932] = 8852;

    keys[1933]   = hash_str_new("&sqsub;", 7);
    values[1933] = 8847;

    keys[1934]   = hash_str_new("&sqsube;", 8);
    values[1934] = 8849;

    keys[1935]   = hash_str_new("&sqsubset;", 10);
    values[1935] = 8847;

    keys[1936]   = hash_str_new("&sqsubseteq;", 12);
    values[1936] = 8849;

    keys[1937]   = hash_str_new("&sqsup;", 7);
    values[1937] = 8848;

    keys[1938]   = hash_str_new("&sqsupe;", 8);
    values[1938] = 8850;

    keys[1939]   = hash_str_new("&sqsupset;", 10);
    values[1939] = 8848;

    keys[1940]   = hash_str_new("&sqsupseteq;", 12);
    values[1940] = 8850;

    keys[1941]   = hash_str_new("&squ;", 5);
    values[1941] = 9633;

    keys[1942]   = hash_str_new("&square;", 8);
    values[1942] = 9633;

    keys[1943]   = hash_str_new("&squarf;", 8);
    values[1943] = 9642;

    keys[1944]   = hash_str_new("&squf;", 6);
    values[1944] = 9642;

    keys[1945]   = hash_str_new("&srarr;", 7);
    values[1945] = 8594;

    keys[1946]   = hash_str_new("&sscr;", 6);
    values[1946] = 120008;

    keys[1947]   = hash_str_new("&ssetmn;", 8);
    values[1947] = 8726;

    keys[1948]   = hash_str_new("&ssmile;", 8);
    values[1948] = 8995;

    keys[1949]   = hash_str_new("&sstarf;", 8);
    values[1949] = 8902;

    keys[1950]   = hash_str_new("&star;", 6);
    values[1950] = 9734;

    keys[1951]   = hash_str_new("&starf;", 7);
    values[1951] = 9733;

    keys[1952]   = hash_str_new("&straightepsilon;", 17);
    values[1952] = 1013;

    keys[1953]   = hash_str_new("&straightphi;", 13);
    values[1953] = 981;

    keys[1954]   = hash_str_new("&strns;", 7);
    values[1954] = 175;

    keys[1955]   = hash_str_new("&sub;", 5);
    values[1955] = 8834;

    keys[1956]   = hash_str_new("&subE;", 6);
    values[1956] = 10949;

    keys[1957]   = hash_str_new("&subdot;", 8);
    values[1957] = 10941;

    keys[1958]   = hash_str_new("&sube;", 6);
    values[1958] = 8838;

    keys[1959]   = hash_str_new("&subedot;", 9);
    values[1959] = 10947;

    keys[1960]   = hash_str_new("&submult;", 9);
    values[1960] = 10945;

    keys[1961]   = hash_str_new("&subnE;", 7);
    values[1961] = 10955;

    keys[1962]   = hash_str_new("&subne;", 7);
    values[1962] = 8842;

    keys[1963]   = hash_str_new("&subplus;", 9);
    values[1963] = 10943;

    keys[1964]   = hash_str_new("&subrarr;", 9);
    values[1964] = 10617;

    keys[1965]   = hash_str_new("&subset;", 8);
    values[1965] = 8834;

    keys[1966]   = hash_str_new("&subseteq;", 10);
    values[1966] = 8838;

    keys[1967]   = hash_str_new("&subseteqq;", 11);
    values[1967] = 10949;

    keys[1968]   = hash_str_new("&subsetneq;", 11);
    values[1968] = 8842;

    keys[1969]   = hash_str_new("&subsetneqq;", 12);
    values[1969] = 10955;

    keys[1970]   = hash_str_new("&subsim;", 8);
    values[1970] = 10951;

    keys[1971]   = hash_str_new("&subsub;", 8);
    values[1971] = 10965;

    keys[1972]   = hash_str_new("&subsup;", 8);
    values[1972] = 10963;

    keys[1973]   = hash_str_new("&succ;", 6);
    values[1973] = 8827;

    keys[1974]   = hash_str_new("&succapprox;", 12);
    values[1974] = 10936;

    keys[1975]   = hash_str_new("&succcurlyeq;", 13);
    values[1975] = 8829;

    keys[1976]   = hash_str_new("&succeq;", 8);
    values[1976] = 10928;

    keys[1977]   = hash_str_new("&succnapprox;", 13);
    values[1977] = 10938;

    keys[1978]   = hash_str_new("&succneqq;", 10);
    values[1978] = 10934;

    keys[1979]   = hash_str_new("&succnsim;", 10);
    values[1979] = 8937;

    keys[1980]   = hash_str_new("&succsim;", 9);
    values[1980] = 8831;

    keys[1981]   = hash_str_new("&sum;", 5);
    values[1981] = 8721;

    keys[1982]   = hash_str_new("&sung;", 6);
    values[1982] = 9834;

    keys[1983]   = hash_str_new("&sup1", 5);
    values[1983] = 185;

    keys[1984]   = hash_str_new("&sup1;", 6);
    values[1984] = 185;

    keys[1985]   = hash_str_new("&sup2", 5);
    values[1985] = 178;

    keys[1986]   = hash_str_new("&sup2;", 6);
    values[1986] = 178;

    keys[1987]   = hash_str_new("&sup3", 5);
    values[1987] = 179;

    keys[1988]   = hash_str_new("&sup3;", 6);
    values[1988] = 179;

    keys[1989]   = hash_str_new("&sup;", 5);
    values[1989] = 8835;

    keys[1990]   = hash_str_new("&supE;", 6);
    values[1990] = 10950;

    keys[1991]   = hash_str_new("&supdot;", 8);
    values[1991] = 10942;

    keys[1992]   = hash_str_new("&supdsub;", 9);
    values[1992] = 10968;

    keys[1993]   = hash_str_new("&supe;", 6);
    values[1993] = 8839;

    keys[1994]   = hash_str_new("&supedot;", 9);
    values[1994] = 10948;

    keys[1995]   = hash_str_new("&suphsol;", 9);
    values[1995] = 10185;

    keys[1996]   = hash_str_new("&suphsub;", 9);
    values[1996] = 10967;

    keys[1997]   = hash_str_new("&suplarr;", 9);
    values[1997] = 10619;

    keys[1998]   = hash_str_new("&supmult;", 9);
    values[1998] = 10946;

    keys[1999]   = hash_str_new("&supnE;", 7);
    values[1999] = 10956;

    keys[2000]   = hash_str_new("&supne;", 7);
    values[2000] = 8843;

    keys[2001]   = hash_str_new("&supplus;", 9);
    values[2001] = 10944;

    keys[2002]   = hash_str_new("&supset;", 8);
    values[2002] = 8835;

    keys[2003]   = hash_str_new("&supseteq;", 10);
    values[2003] = 8839;

    keys[2004]   = hash_str_new("&supseteqq;", 11);
    values[2004] = 10950;

    keys[2005]   = hash_str_new("&supsetneq;", 11);
    values[2005] = 8843;

    keys[2006]   = hash_str_new("&supsetneqq;", 12);
    values[2006] = 10956;

    keys[2007]   = hash_str_new("&supsim;", 8);
    values[2007] = 10952;

    keys[2008]   = hash_str_new("&supsub;", 8);
    values[2008] = 10964;

    keys[2009]   = hash_str_new("&supsup;", 8);
    values[2009] = 10966;

    keys[2010]   = hash_str_new("&swArr;", 7);
    values[2010] = 8665;

    keys[2011]   = hash_str_new("&swarhk;", 8);
    values[2011] = 10534;

    keys[2012]   = hash_str_new("&swarr;", 7);
    values[2012] = 8601;

    keys[2013]   = hash_str_new("&swarrow;", 9);
    values[2013] = 8601;

    keys[2014]   = hash_str_new("&swnwar;", 8);
    values[2014] = 10538;

    keys[2015]   = hash_str_new("&szlig", 6);
    values[2015] = 223;

    keys[2016]   = hash_str_new("&szlig;", 7);
    values[2016] = 223;

    keys[2017]   = hash_str_new("&target;", 8);
    values[2017] = 8982;

    keys[2018]   = hash_str_new("&tau;", 5);
    values[2018] = 964;

    keys[2019]   = hash_str_new("&tbrk;", 6);
    values[2019] = 9140;

    keys[2020]   = hash_str_new("&tcaron;", 8);
    values[2020] = 357;

    keys[2021]   = hash_str_new("&tcedil;", 8);
    values[2021] = 355;

    keys[2022]   = hash_str_new("&tcy;", 5);
    values[2022] = 1090;

    keys[2023]   = hash_str_new("&tdot;", 6);
    values[2023] = 8411;

    keys[2024]   = hash_str_new("&telrec;", 8);
    values[2024] = 8981;

    keys[2025]   = hash_str_new("&tfr;", 5);
    values[2025] = 120113;

    keys[2026]   = hash_str_new("&there4;", 8);
    values[2026] = 8756;

    keys[2027]   = hash_str_new("&therefore;", 11);
    values[2027] = 8756;

    keys[2028]   = hash_str_new("&theta;", 7);
    values[2028] = 952;

    keys[2029]   = hash_str_new("&thetasym;", 10);
    values[2029] = 977;

    keys[2030]   = hash_str_new("&thetav;", 8);
    values[2030] = 977;

    keys[2031]   = hash_str_new("&thickapprox;", 13);
    values[2031] = 8776;

    keys[2032]   = hash_str_new("&thicksim;", 10);
    values[2032] = 8764;

    keys[2033]   = hash_str_new("&thinsp;", 8);
    values[2033] = 8201;

    keys[2034]   = hash_str_new("&thkap;", 7);
    values[2034] = 8776;

    keys[2035]   = hash_str_new("&thksim;", 8);
    values[2035] = 8764;

    keys[2036]   = hash_str_new("&thorn", 6);
    values[2036] = 254;

    keys[2037]   = hash_str_new("&thorn;", 7);
    values[2037] = 254;

    keys[2038]   = hash_str_new("&tilde;", 7);
    values[2038] = 732;

    keys[2039]   = hash_str_new("&times", 6);
    values[2039] = 215;

    keys[2040]   = hash_str_new("&times;", 7);
    values[2040] = 215;

    keys[2041]   = hash_str_new("&timesb;", 8);
    values[2041] = 8864;

    keys[2042]   = hash_str_new("&timesbar;", 10);
    values[2042] = 10801;

    keys[2043]   = hash_str_new("&timesd;", 8);
    values[2043] = 10800;

    keys[2044]   = hash_str_new("&tint;", 6);
    values[2044] = 8749;

    keys[2045]   = hash_str_new("&toea;", 6);
    values[2045] = 10536;

    keys[2046]   = hash_str_new("&top;", 5);
    values[2046] = 8868;

    keys[2047]   = hash_str_new("&topbot;", 8);
    values[2047] = 9014;

    keys[2048]   = hash_str_new("&topcir;", 8);
    values[2048] = 10993;

    keys[2049]   = hash_str_new("&topf;", 6);
    values[2049] = 120165;

    keys[2050]   = hash_str_new("&topfork;", 9);
    values[2050] = 10970;

    keys[2051]   = hash_str_new("&tosa;", 6);
    values[2051] = 10537;

    keys[2052]   = hash_str_new("&tprime;", 8);
    values[2052] = 8244;

    keys[2053]   = hash_str_new("&trade;", 7);
    values[2053] = 8482;

    keys[2054]   = hash_str_new("&triangle;", 10);
    values[2054] = 9653;

    keys[2055]   = hash_str_new("&triangledown;", 14);
    values[2055] = 9663;

    keys[2056]   = hash_str_new("&triangleleft;", 14);
    values[2056] = 9667;

    keys[2057]   = hash_str_new("&trianglelefteq;", 16);
    values[2057] = 8884;

    keys[2058]   = hash_str_new("&triangleq;", 11);
    values[2058] = 8796;

    keys[2059]   = hash_str_new("&triangleright;", 15);
    values[2059] = 9657;

    keys[2060]   = hash_str_new("&trianglerighteq;", 17);
    values[2060] = 8885;

    keys[2061]   = hash_str_new("&tridot;", 8);
    values[2061] = 9708;

    keys[2062]   = hash_str_new("&trie;", 6);
    values[2062] = 8796;

    keys[2063]   = hash_str_new("&triminus;", 10);
    values[2063] = 10810;

    keys[2064]   = hash_str_new("&triplus;", 9);
    values[2064] = 10809;

    keys[2065]   = hash_str_new("&trisb;", 7);
    values[2065] = 10701;

    keys[2066]   = hash_str_new("&tritime;", 9);
    values[2066] = 10811;

    keys[2067]   = hash_str_new("&trpezium;", 10);
    values[2067] = 9186;

    keys[2068]   = hash_str_new("&tscr;", 6);
    values[2068] = 120009;

    keys[2069]   = hash_str_new("&tscy;", 6);
    values[2069] = 1094;

    keys[2070]   = hash_str_new("&tshcy;", 7);
    values[2070] = 1115;

    keys[2071]   = hash_str_new("&tstrok;", 8);
    values[2071] = 359;

    keys[2072]   = hash_str_new("&twixt;", 7);
    values[2072] = 8812;

    keys[2073]   = hash_str_new("&twoheadleftarrow;", 18);
    values[2073] = 8606;

    keys[2074]   = hash_str_new("&twoheadrightarrow;", 19);
    values[2074] = 8608;

    keys[2075]   = hash_str_new("&uArr;", 6);
    values[2075] = 8657;

    keys[2076]   = hash_str_new("&uHar;", 6);
    values[2076] = 10595;

    keys[2077]   = hash_str_new("&uacute", 7);
    values[2077] = 250;

    keys[2078]   = hash_str_new("&uacute;", 8);
    values[2078] = 250;

    keys[2079]   = hash_str_new("&uarr;", 6);
    values[2079] = 8593;

    keys[2080]   = hash_str_new("&ubrcy;", 7);
    values[2080] = 1118;

    keys[2081]   = hash_str_new("&ubreve;", 8);
    values[2081] = 365;

    keys[2082]   = hash_str_new("&ucirc", 6);
    values[2082] = 251;

    keys[2083]   = hash_str_new("&ucirc;", 7);
    values[2083] = 251;

    keys[2084]   = hash_str_new("&ucy;", 5);
    values[2084] = 1091;

    keys[2085]   = hash_str_new("&udarr;", 7);
    values[2085] = 8645;

    keys[2086]   = hash_str_new("&udblac;", 8);
    values[2086] = 369;

    keys[2087]   = hash_str_new("&udhar;", 7);
    values[2087] = 10606;

    keys[2088]   = hash_str_new("&ufisht;", 8);
    values[2088] = 10622;

    keys[2089]   = hash_str_new("&ufr;", 5);
    values[2089] = 120114;

    keys[2090]   = hash_str_new("&ugrave", 7);
    values[2090] = 249;

    keys[2091]   = hash_str_new("&ugrave;", 8);
    values[2091] = 249;

    keys[2092]   = hash_str_new("&uharl;", 7);
    values[2092] = 8639;

    keys[2093]   = hash_str_new("&uharr;", 7);
    values[2093] = 8638;

    keys[2094]   = hash_str_new("&uhblk;", 7);
    values[2094] = 9600;

    keys[2095]   = hash_str_new("&ulcorn;", 8);
    values[2095] = 8988;

    keys[2096]   = hash_str_new("&ulcorner;", 10);
    values[2096] = 8988;

    keys[2097]   = hash_str_new("&ulcrop;", 8);
    values[2097] = 8975;

    keys[2098]   = hash_str_new("&ultri;", 7);
    values[2098] = 9720;

    keys[2099]   = hash_str_new("&umacr;", 7);
    values[2099] = 363;

    keys[2100]   = hash_str_new("&uml", 4);
    values[2100] = 168;

    keys[2101]   = hash_str_new("&uml;", 5);
    values[2101] = 168;

    keys[2102]   = hash_str_new("&uogon;", 7);
    values[2102] = 371;

    keys[2103]   = hash_str_new("&uopf;", 6);
    values[2103] = 120166;

    keys[2104]   = hash_str_new("&uparrow;", 9);
    values[2104] = 8593;

    keys[2105]   = hash_str_new("&updownarrow;", 13);
    values[2105] = 8597;

    keys[2106]   = hash_str_new("&upharpoonleft;", 15);
    values[2106] = 8639;

    keys[2107]   = hash_str_new("&upharpoonright;", 16);
    values[2107] = 8638;

    keys[2108]   = hash_str_new("&uplus;", 7);
    values[2108] = 8846;

    keys[2109]   = hash_str_new("&upsi;", 6);
    values[2109] = 965;

    keys[2110]   = hash_str_new("&upsih;", 7);
    values[2110] = 978;

    keys[2111]   = hash_str_new("&upsilon;", 9);
    values[2111] = 965;

    keys[2112]   = hash_str_new("&upuparrows;", 12);
    values[2112] = 8648;

    keys[2113]   = hash_str_new("&urcorn;", 8);
    values[2113] = 8989;

    keys[2114]   = hash_str_new("&urcorner;", 10);
    values[2114] = 8989;

    keys[2115]   = hash_str_new("&urcrop;", 8);
    values[2115] = 8974;

    keys[2116]   = hash_str_new("&uring;", 7);
    values[2116] = 367;

    keys[2117]   = hash_str_new("&urtri;", 7);
    values[2117] = 9721;

    keys[2118]   = hash_str_new("&uscr;", 6);
    values[2118] = 120010;

    keys[2119]   = hash_str_new("&utdot;", 7);
    values[2119] = 8944;

    keys[2120]   = hash_str_new("&utilde;", 8);
    values[2120] = 361;

    keys[2121]   = hash_str_new("&utri;", 6);
    values[2121] = 9653;

    keys[2122]   = hash_str_new("&utrif;", 7);
    values[2122] = 9652;

    keys[2123]   = hash_str_new("&uuarr;", 7);
    values[2123] = 8648;

    keys[2124]   = hash_str_new("&uuml", 5);
    values[2124] = 252;

    keys[2125]   = hash_str_new("&uuml;", 6);
    values[2125] = 252;

    keys[2126]   = hash_str_new("&uwangle;", 9);
    values[2126] = 10663;

    keys[2127]   = hash_str_new("&vArr;", 6);
    values[2127] = 8661;

    keys[2128]   = hash_str_new("&vBar;", 6);
    values[2128] = 10984;

    keys[2129]   = hash_str_new("&vBarv;", 7);
    values[2129] = 10985;

    keys[2130]   = hash_str_new("&vDash;", 7);
    values[2130] = 8872;

    keys[2131]   = hash_str_new("&vangrt;", 8);
    values[2131] = 10652;

    keys[2132]   = hash_str_new("&varepsilon;", 12);
    values[2132] = 1013;

    keys[2133]   = hash_str_new("&varkappa;", 10);
    values[2133] = 1008;

    keys[2134]   = hash_str_new("&varnothing;", 12);
    values[2134] = 8709;

    keys[2135]   = hash_str_new("&varphi;", 8);
    values[2135] = 981;

    keys[2136]   = hash_str_new("&varpi;", 7);
    values[2136] = 982;

    keys[2137]   = hash_str_new("&varpropto;", 11);
    values[2137] = 8733;

    keys[2138]   = hash_str_new("&varr;", 6);
    values[2138] = 8597;

    keys[2139]   = hash_str_new("&varrho;", 8);
    values[2139] = 1009;

    keys[2140]   = hash_str_new("&varsigma;", 10);
    values[2140] = 962;

    keys[2141]   = hash_str_new("&varsubsetneq;", 14);
    values[2141] = 8842;

    keys[2142]   = hash_str_new("&varsubsetneqq;", 15);
    values[2142] = 10955;

    keys[2143]   = hash_str_new("&varsupsetneq;", 14);
    values[2143] = 8843;

    keys[2144]   = hash_str_new("&varsupsetneqq;", 15);
    values[2144] = 10956;

    keys[2145]   = hash_str_new("&vartheta;", 10);
    values[2145] = 977;

    keys[2146]   = hash_str_new("&vartriangleleft;", 17);
    values[2146] = 8882;

    keys[2147]   = hash_str_new("&vartriangleright;", 18);
    values[2147] = 8883;

    keys[2148]   = hash_str_new("&vcy;", 5);
    values[2148] = 1074;

    keys[2149]   = hash_str_new("&vdash;", 7);
    values[2149] = 8866;

    keys[2150]   = hash_str_new("&vee;", 5);
    values[2150] = 8744;

    keys[2151]   = hash_str_new("&veebar;", 8);
    values[2151] = 8891;

    keys[2152]   = hash_str_new("&veeeq;", 7);
    values[2152] = 8794;

    keys[2153]   = hash_str_new("&vellip;", 8);
    values[2153] = 8942;

    keys[2154]   = hash_str_new("&verbar;", 8);
    values[2154] = 124;

    keys[2155]   = hash_str_new("&vert;", 6);
    values[2155] = 124;

    keys[2156]   = hash_str_new("&vfr;", 5);
    values[2156] = 120115;

    keys[2157]   = hash_str_new("&vltri;", 7);
    values[2157] = 8882;

    keys[2158]   = hash_str_new("&vnsub;", 7);
    values[2158] = 8834;

    keys[2159]   = hash_str_new("&vnsup;", 7);
    values[2159] = 8835;

    keys[2160]   = hash_str_new("&vopf;", 6);
    values[2160] = 120167;

    keys[2161]   = hash_str_new("&vprop;", 7);
    values[2161] = 8733;

    keys[2162]   = hash_str_new("&vrtri;", 7);
    values[2162] = 8883;

    keys[2163]   = hash_str_new("&vscr;", 6);
    values[2163] = 120011;

    keys[2164]   = hash_str_new("&vsubnE;", 8);
    values[2164] = 10955;

    keys[2165]   = hash_str_new("&vsubne;", 8);
    values[2165] = 8842;

    keys[2166]   = hash_str_new("&vsupnE;", 8);
    values[2166] = 10956;

    keys[2167]   = hash_str_new("&vsupne;", 8);
    values[2167] = 8843;

    keys[2168]   = hash_str_new("&vzigzag;", 9);
    values[2168] = 10650;

    keys[2169]   = hash_str_new("&wcirc;", 7);
    values[2169] = 373;

    keys[2170]   = hash_str_new("&wedbar;", 8);
    values[2170] = 10847;

    keys[2171]   = hash_str_new("&wedge;", 7);
    values[2171] = 8743;

    keys[2172]   = hash_str_new("&wedgeq;", 8);
    values[2172] = 8793;

    keys[2173]   = hash_str_new("&weierp;", 8);
    values[2173] = 8472;

    keys[2174]   = hash_str_new("&wfr;", 5);
    values[2174] = 120116;

    keys[2175]   = hash_str_new("&wopf;", 6);
    values[2175] = 120168;

    keys[2176]   = hash_str_new("&wp;", 4);
    values[2176] = 8472;

    keys[2177]   = hash_str_new("&wr;", 4);
    values[2177] = 8768;

    keys[2178]   = hash_str_new("&wreath;", 8);
    values[2178] = 8768;

    keys[2179]   = hash_str_new("&wscr;", 6);
    values[2179] = 120012;

    keys[2180]   = hash_str_new("&xcap;", 6);
    values[2180] = 8898;

    keys[2181]   = hash_str_new("&xcirc;", 7);
    values[2181] = 9711;

    keys[2182]   = hash_str_new("&xcup;", 6);
    values[2182] = 8899;

    keys[2183]   = hash_str_new("&xdtri;", 7);
    values[2183] = 9661;

    keys[2184]   = hash_str_new("&xfr;", 5);
    values[2184] = 120117;

    keys[2185]   = hash_str_new("&xhArr;", 7);
    values[2185] = 10234;

    keys[2186]   = hash_str_new("&xharr;", 7);
    values[2186] = 10231;

    keys[2187]   = hash_str_new("&xi;", 4);
    values[2187] = 958;

    keys[2188]   = hash_str_new("&xlArr;", 7);
    values[2188] = 10232;

    keys[2189]   = hash_str_new("&xlarr;", 7);
    values[2189] = 10229;

    keys[2190]   = hash_str_new("&xmap;", 6);
    values[2190] = 10236;

    keys[2191]   = hash_str_new("&xnis;", 6);
    values[2191] = 8955;

    keys[2192]   = hash_str_new("&xodot;", 7);
    values[2192] = 10752;

    keys[2193]   = hash_str_new("&xopf;", 6);
    values[2193] = 120169;

    keys[2194]   = hash_str_new("&xoplus;", 8);
    values[2194] = 10753;

    keys[2195]   = hash_str_new("&xotime;", 8);
    values[2195] = 10754;

    keys[2196]   = hash_str_new("&xrArr;", 7);
    values[2196] = 10233;

    keys[2197]   = hash_str_new("&xrarr;", 7);
    values[2197] = 10230;

    keys[2198]   = hash_str_new("&xscr;", 6);
    values[2198] = 120013;

    keys[2199]   = hash_str_new("&xsqcup;", 8);
    values[2199] = 10758;

    keys[2200]   = hash_str_new("&xuplus;", 8);
    values[2200] = 10756;

    keys[2201]   = hash_str_new("&xutri;", 7);
    values[2201] = 9651;

    keys[2202]   = hash_str_new("&xvee;", 6);
    values[2202] = 8897;

    keys[2203]   = hash_str_new("&xwedge;", 8);
    values[2203] = 8896;

    keys[2204]   = hash_str_new("&yacute", 7);
    values[2204] = 253;

    keys[2205]   = hash_str_new("&yacute;", 8);
    values[2205] = 253;

    keys[2206]   = hash_str_new("&yacy;", 6);
    values[2206] = 1103;

    keys[2207]   = hash_str_new("&ycirc;", 7);
    values[2207] = 375;

    keys[2208]   = hash_str_new("&ycy;", 5);
    values[2208] = 1099;

    keys[2209]   = hash_str_new("&yen", 4);
    values[2209] = 165;

    keys[2210]   = hash_str_new("&yen;", 5);
    values[2210] = 165;

    keys[2211]   = hash_str_new("&yfr;", 5);
    values[2211] = 120118;

    keys[2212]   = hash_str_new("&yicy;", 6);
    values[2212] = 1111;

    keys[2213]   = hash_str_new("&yopf;", 6);
    values[2213] = 120170;

    keys[2214]   = hash_str_new("&yscr;", 6);
    values[2214] = 120014;

    keys[2215]   = hash_str_new("&yucy;", 6);
    values[2215] = 1102;

    keys[2216]   = hash_str_new("&yuml", 5);
    values[2216] = 255;

    keys[2217]   = hash_str_new("&yuml;", 6);
    values[2217] = 255;

    keys[2218]   = hash_str_new("&zacute;", 8);
    values[2218] = 378;

    keys[2219]   = hash_str_new("&zcaron;", 8);
    values[2219] = 382;

    keys[2220]   = hash_str_new("&zcy;", 5);
    values[2220] = 1079;

    keys[2221]   = hash_str_new("&zdot;", 6);
    values[2221] = 380;

    keys[2222]   = hash_str_new("&zeetrf;", 8);
    values[2222] = 8488;

    keys[2223]   = hash_str_new("&zeta;", 6);
    values[2223] = 950;

    keys[2224]   = hash_str_new("&zfr;", 5);
    values[2224] = 120119;

    keys[2225]   = hash_str_new("&zhcy;", 6);
    values[2225] = 1078;

    keys[2226]   = hash_str_new("&zigrarr;", 9);
    values[2226] = 8669;

    keys[2227]   = hash_str_new("&zopf;", 6);
    values[2227] = 120171;

    keys[2228]   = hash_str_new("&zscr;", 6);
    values[2228] = 120015;

    keys[2229]   = hash_str_new("&zwj;", 5);
    values[2229] = 8205;

    keys[2230]   = hash_str_new("&zwnj;", 6);
    values[2230] = 8204;
}


void html_named_char_ref_map_free()
{

}