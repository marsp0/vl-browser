/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "color_name_map.h"

#include "util/not_implemented.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

static hash_str_t aliceblue            = 0;
static hash_str_t antiquewhite         = 0;
static hash_str_t aqua                 = 0;
static hash_str_t aquamarine           = 0;
static hash_str_t azure                = 0;
static hash_str_t beige                = 0;
static hash_str_t bisque               = 0;
static hash_str_t black                = 0;
static hash_str_t blanchedalmond       = 0;
static hash_str_t blue                 = 0;
static hash_str_t blueviolet           = 0;
static hash_str_t brown                = 0;
static hash_str_t burlywood            = 0;
static hash_str_t cadetblue            = 0;
static hash_str_t chartreuse           = 0;
static hash_str_t chocolate            = 0;
static hash_str_t coral                = 0;
static hash_str_t cornflowerblue       = 0;
static hash_str_t cornsilk             = 0;
static hash_str_t crimson              = 0;
static hash_str_t cyan                 = 0;
static hash_str_t darkblue             = 0;
static hash_str_t darkcyan             = 0;
static hash_str_t darkgoldenrod        = 0;
static hash_str_t darkgray             = 0;
static hash_str_t darkgreen            = 0;
static hash_str_t darkgrey             = 0;
static hash_str_t darkkhaki            = 0;
static hash_str_t darkmagenta          = 0;
static hash_str_t darkolivegreen       = 0;
static hash_str_t darkorange           = 0;
static hash_str_t darkorchid           = 0;
static hash_str_t darkred              = 0;
static hash_str_t darksalmon           = 0;
static hash_str_t darkseagreen         = 0;
static hash_str_t darkslateblue        = 0;
static hash_str_t darkslategray        = 0;
static hash_str_t darkslategrey        = 0;
static hash_str_t darkturquoise        = 0;
static hash_str_t darkviolet           = 0;
static hash_str_t deeppink             = 0;
static hash_str_t deepskyblue          = 0;
static hash_str_t dimgray              = 0;
static hash_str_t dimgrey              = 0;
static hash_str_t dodgerblue           = 0;
static hash_str_t firebrick            = 0;
static hash_str_t floralwhite          = 0;
static hash_str_t forestgreen          = 0;
static hash_str_t fuchsia              = 0;
static hash_str_t gainsboro            = 0;
static hash_str_t ghostwhite           = 0;
static hash_str_t gold                 = 0;
static hash_str_t goldenrod            = 0;
static hash_str_t gray                 = 0;
static hash_str_t green                = 0;
static hash_str_t greenyellow          = 0;
static hash_str_t grey                 = 0;
static hash_str_t honeydew             = 0;
static hash_str_t hotpink              = 0;
static hash_str_t indianred            = 0;
static hash_str_t indigo               = 0;
static hash_str_t ivory                = 0;
static hash_str_t khaki                = 0;
static hash_str_t lavender             = 0;
static hash_str_t lavenderblush        = 0;
static hash_str_t lawngreen            = 0;
static hash_str_t lemonchiffon         = 0;
static hash_str_t lightblue            = 0;
static hash_str_t lightcoral           = 0;
static hash_str_t lightcyan            = 0;
static hash_str_t lightgoldenrodyellow = 0;
static hash_str_t lightgray            = 0;
static hash_str_t lightgreen           = 0;
static hash_str_t lightgrey            = 0;
static hash_str_t lightpink            = 0;
static hash_str_t lightsalmon          = 0;
static hash_str_t lightseagreen        = 0;
static hash_str_t lightskyblue         = 0;
static hash_str_t lightslategray       = 0;
static hash_str_t lightslategrey       = 0;
static hash_str_t lightsteelblue       = 0;
static hash_str_t lightyellow          = 0;
static hash_str_t lime                 = 0;
static hash_str_t limegreen            = 0;
static hash_str_t linen                = 0;
static hash_str_t magenta              = 0;
static hash_str_t maroon               = 0;
static hash_str_t mediumaquamarine     = 0;
static hash_str_t mediumblue           = 0;
static hash_str_t mediumorchid         = 0;
static hash_str_t mediumpurple         = 0;
static hash_str_t mediumseagreen       = 0;
static hash_str_t mediumslateblue      = 0;
static hash_str_t mediumspringgreen    = 0;
static hash_str_t mediumturquoise      = 0;
static hash_str_t mediumvioletred      = 0;
static hash_str_t midnightblue         = 0;
static hash_str_t mintcream            = 0;
static hash_str_t mistyrose            = 0;
static hash_str_t moccasin             = 0;
static hash_str_t navajowhite          = 0;
static hash_str_t navy                 = 0;
static hash_str_t oldlace              = 0;
static hash_str_t olive                = 0;
static hash_str_t olivedrab            = 0;
static hash_str_t orange               = 0;
static hash_str_t orangered            = 0;
static hash_str_t orchid               = 0;
static hash_str_t palegoldenrod        = 0;
static hash_str_t palegreen            = 0;
static hash_str_t paleturquoise        = 0;
static hash_str_t palevioletred        = 0;
static hash_str_t papayawhip           = 0;
static hash_str_t peachpuff            = 0;
static hash_str_t peru                 = 0;
static hash_str_t pink                 = 0;
static hash_str_t plum                 = 0;
static hash_str_t powderblue           = 0;
static hash_str_t purple               = 0;
static hash_str_t rebeccapurple        = 0;
static hash_str_t red                  = 0;
static hash_str_t rosybrown            = 0;
static hash_str_t royalblue            = 0;
static hash_str_t saddlebrown          = 0;
static hash_str_t salmon               = 0;
static hash_str_t sandybrown           = 0;
static hash_str_t seagreen             = 0;
static hash_str_t seashell             = 0;
static hash_str_t sienna               = 0;
static hash_str_t silver               = 0;
static hash_str_t skyblue              = 0;
static hash_str_t slateblue            = 0;
static hash_str_t slategray            = 0;
static hash_str_t slategrey            = 0;
static hash_str_t snow                 = 0;
static hash_str_t springgreen          = 0;
static hash_str_t steelblue            = 0;
static hash_str_t tan_col              = 0;
static hash_str_t teal                 = 0;
static hash_str_t thistle              = 0;
static hash_str_t tomato               = 0;
static hash_str_t turquoise            = 0;
static hash_str_t violet               = 0;
static hash_str_t wheat                = 0;
static hash_str_t white                = 0;
static hash_str_t whitesmoke           = 0;
static hash_str_t yellow               = 0;
static hash_str_t yellowgreen          = 0;


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/


void css_color_name_map_init()
{
    aliceblue            = hash_str_new("aliceblue", 9);
    antiquewhite         = hash_str_new("antiquewhite", 12);
    aqua                 = hash_str_new("aqua", 4);
    aquamarine           = hash_str_new("aquamarine", 10);
    azure                = hash_str_new("azure", 5);
    beige                = hash_str_new("beige", 5);
    bisque               = hash_str_new("bisque", 6);
    black                = hash_str_new("black", 5);
    blanchedalmond       = hash_str_new("blanchedalmond", 14);
    blue                 = hash_str_new("blue", 4);
    blueviolet           = hash_str_new("blueviolet", 10);
    brown                = hash_str_new("brown", 5);
    burlywood            = hash_str_new("burlywood", 9);
    cadetblue            = hash_str_new("cadetblue", 9);
    chartreuse           = hash_str_new("chartreuse", 10);
    chocolate            = hash_str_new("chocolate", 9);
    coral                = hash_str_new("coral", 5);
    cornflowerblue       = hash_str_new("cornflowerblue", 14);
    cornsilk             = hash_str_new("cornsilk", 8);
    crimson              = hash_str_new("crimson", 7);
    cyan                 = hash_str_new("cyan", 4);
    darkblue             = hash_str_new("darkblue", 8);
    darkcyan             = hash_str_new("darkcyan", 8);
    darkgoldenrod        = hash_str_new("darkgoldenrod", 13);
    darkgray             = hash_str_new("darkgray", 8);
    darkgreen            = hash_str_new("darkgreen", 9);
    darkgrey             = hash_str_new("darkgrey", 8);
    darkkhaki            = hash_str_new("darkkhaki", 9);
    darkmagenta          = hash_str_new("darkmagenta", 11);
    darkolivegreen       = hash_str_new("darkolivegreen", 14);
    darkorange           = hash_str_new("darkorange", 10);
    darkorchid           = hash_str_new("darkorchid", 10);
    darkred              = hash_str_new("darkred", 7);
    darksalmon           = hash_str_new("darksalmon", 10);
    darkseagreen         = hash_str_new("darkseagreen", 12);
    darkslateblue        = hash_str_new("darkslateblue", 13);
    darkslategray        = hash_str_new("darkslategray", 13);
    darkslategrey        = hash_str_new("darkslategrey", 13);
    darkturquoise        = hash_str_new("darkturquoise", 13);
    darkviolet           = hash_str_new("darkviolet", 10);
    deeppink             = hash_str_new("deeppink", 8);
    deepskyblue          = hash_str_new("deepskyblue", 11);
    dimgray              = hash_str_new("dimgray", 7);
    dimgrey              = hash_str_new("dimgrey", 7);
    dodgerblue           = hash_str_new("dodgerblue", 10);
    firebrick            = hash_str_new("firebrick", 9);
    floralwhite          = hash_str_new("floralwhite", 11);
    forestgreen          = hash_str_new("forestgreen", 11);
    fuchsia              = hash_str_new("fuchsia", 7);
    gainsboro            = hash_str_new("gainsboro", 9);
    ghostwhite           = hash_str_new("ghostwhite", 10);
    gold                 = hash_str_new("gold", 4);
    goldenrod            = hash_str_new("goldenrod", 9);
    gray                 = hash_str_new("gray", 4);
    green                = hash_str_new("green", 5);
    greenyellow          = hash_str_new("greenyellow", 11);
    grey                 = hash_str_new("grey", 4);
    honeydew             = hash_str_new("honeydew", 8);
    hotpink              = hash_str_new("hotpink", 7);
    indianred            = hash_str_new("indianred", 9);
    indigo               = hash_str_new("indigo", 6);
    ivory                = hash_str_new("ivory", 5);
    khaki                = hash_str_new("khaki", 5);
    lavender             = hash_str_new("lavender", 8);
    lavenderblush        = hash_str_new("lavenderblush", 13);
    lawngreen            = hash_str_new("lawngreen", 9);
    lemonchiffon         = hash_str_new("lemonchiffon", 12);
    lightblue            = hash_str_new("lightblue", 9);
    lightcoral           = hash_str_new("lightcoral", 10);
    lightcyan            = hash_str_new("lightcyan", 9);
    lightgoldenrodyellow = hash_str_new("lightgoldenrodyellow", 20);
    lightgray            = hash_str_new("lightgray", 9);
    lightgreen           = hash_str_new("lightgreen", 10);
    lightgrey            = hash_str_new("lightgrey", 9);
    lightpink            = hash_str_new("lightpink", 9);
    lightsalmon          = hash_str_new("lightsalmon", 11);
    lightseagreen        = hash_str_new("lightseagreen", 13);
    lightskyblue         = hash_str_new("lightskyblue", 12);
    lightslategray       = hash_str_new("lightslategray", 14);
    lightslategrey       = hash_str_new("lightslategrey", 14);
    lightsteelblue       = hash_str_new("lightsteelblue", 14);
    lightyellow          = hash_str_new("lightyellow", 11);
    lime                 = hash_str_new("lime", 4);
    limegreen            = hash_str_new("limegreen", 9);
    linen                = hash_str_new("linen", 5);
    magenta              = hash_str_new("magenta", 7);
    maroon               = hash_str_new("maroon", 6);
    mediumaquamarine     = hash_str_new("mediumaquamarine", 16);
    mediumblue           = hash_str_new("mediumblue", 10);
    mediumorchid         = hash_str_new("mediumorchid", 12);
    mediumpurple         = hash_str_new("mediumpurple", 12);
    mediumseagreen       = hash_str_new("mediumseagreen", 14);
    mediumslateblue      = hash_str_new("mediumslateblue", 15);
    mediumspringgreen    = hash_str_new("mediumspringgreen", 17);
    mediumturquoise      = hash_str_new("mediumturquoise", 15);
    mediumvioletred      = hash_str_new("mediumvioletred", 15);
    midnightblue         = hash_str_new("midnightblue", 12);
    mintcream            = hash_str_new("mintcream", 9);
    mistyrose            = hash_str_new("mistyrose", 9);
    moccasin             = hash_str_new("moccasin", 8);
    navajowhite          = hash_str_new("navajowhite", 11);
    navy                 = hash_str_new("navy", 4);
    oldlace              = hash_str_new("oldlace", 7);
    olive                = hash_str_new("olive", 5);
    olivedrab            = hash_str_new("olivedrab", 9);
    orange               = hash_str_new("orange", 6);
    orangered            = hash_str_new("orangered", 9);
    orchid               = hash_str_new("orchid", 6);
    palegoldenrod        = hash_str_new("palegoldenrod", 13);
    palegreen            = hash_str_new("palegreen", 9);
    paleturquoise        = hash_str_new("paleturquoise", 13);
    palevioletred        = hash_str_new("palevioletred", 13);
    papayawhip           = hash_str_new("papayawhip", 10);
    peachpuff            = hash_str_new("peachpuff", 9);
    peru                 = hash_str_new("peru", 4);
    pink                 = hash_str_new("pink", 4);
    plum                 = hash_str_new("plum", 4);
    powderblue           = hash_str_new("powderblue", 10);
    purple               = hash_str_new("purple", 6);
    rebeccapurple        = hash_str_new("rebeccapurple", 13);
    red                  = hash_str_new("red", 3);
    rosybrown            = hash_str_new("rosybrown", 9);
    royalblue            = hash_str_new("royalblue", 9);
    saddlebrown          = hash_str_new("saddlebrown", 11);
    salmon               = hash_str_new("salmon", 6);
    sandybrown           = hash_str_new("sandybrown", 10);
    seagreen             = hash_str_new("seagreen", 8);
    seashell             = hash_str_new("seashell", 8);
    sienna               = hash_str_new("sienna", 6);
    silver               = hash_str_new("silver", 6);
    skyblue              = hash_str_new("skyblue", 7);
    slateblue            = hash_str_new("slateblue", 9);
    slategray            = hash_str_new("slategray", 9);
    slategrey            = hash_str_new("slategrey", 9);
    snow                 = hash_str_new("snow", 4);
    springgreen          = hash_str_new("springgreen", 11);
    steelblue            = hash_str_new("steelblue", 9);
    tan_col              = hash_str_new("tan", 3);
    teal                 = hash_str_new("teal", 4);
    thistle              = hash_str_new("thistle", 7);
    tomato               = hash_str_new("tomato", 6);
    turquoise            = hash_str_new("turquoise", 9);
    violet               = hash_str_new("violet", 6);
    wheat                = hash_str_new("wheat", 5);
    white                = hash_str_new("white", 5);
    whitesmoke           = hash_str_new("whitesmoke", 10);
    yellow               = hash_str_new("yellow", 6);
    yellowgreen          = hash_str_new("yellowgreen", 11);
}

uint32_t css_color_name_map_get(hash_str_t col)
{
    if (col == aliceblue)
    {
        return 0xf0f8ff;
    }
    else if (col == antiquewhite)
    {
        return 0xfaebd7;
    }
    else if (col == aqua)
    {
        return 0x00ffff;
    }
    else if (col == aquamarine)
    {
        return 0x7fffd4;
    }
    else if (col == azure)
    {
        return 0xf0ffff;
    }
    else if (col == beige)
    {
        return 0xf5f5dc;
    }
    else if (col == bisque)
    {
        return 0xffe4c4;
    }
    else if (col == black)
    {
        return 0x000000;
    }
    else if (col == blanchedalmond)
    {
        return 0xffebcd;
    }
    else if (col == blue)
    {
        return 0x0000ff;
    }
    else if (col == blueviolet)
    {
        return 0x8a2be2;
    }
    else if (col == brown)
    {
        return 0xa52a2a;
    }
    else if (col == burlywood)
    {
        return 0xdeb887;
    }
    else if (col == cadetblue)
    {
        return 0x5f9ea0;
    }
    else if (col == chartreuse)
    {
        return 0x7fff00;
    }
    else if (col == chocolate)
    {
        return 0xd2691e;
    }
    else if (col == coral)
    {
        return 0xff7f50;
    }
    else if (col == cornflowerblue)
    {
        return 0x6495ed;
    }
    else if (col == cornsilk)
    {
        return 0xfff8dc;
    }
    else if (col == crimson)
    {
        return 0xdc143c;
    }
    else if (col == cyan)
    {
        return 0x00ffff;
    }
    else if (col == darkblue)
    {
        return 0x00008b;
    }
    else if (col == darkcyan)
    {
        return 0x008b8b;
    }
    else if (col == darkgoldenrod)
    {
        return 0xb8860b;
    }
    else if (col == darkgray)
    {
        return 0xa9a9a9;
    }
    else if (col == darkgreen)
    {
        return 0x006400;
    }
    else if (col == darkgrey)
    {
        return 0xa9a9a9;
    }
    else if (col == darkkhaki)
    {
        return 0xbdb76b;
    }
    else if (col == darkmagenta)
    {
        return 0x8b008b;
    }
    else if (col == darkolivegreen)
    {
        return 0x556b2f;
    }
    else if (col == darkorange)
    {
        return 0xff8c00;
    }
    else if (col == darkorchid)
    {
        return 0x9932cc;
    }
    else if (col == darkred)
    {
        return 0x8b0000;
    }
    else if (col == darksalmon)
    {
        return 0xe9967a;
    }
    else if (col == darkseagreen)
    {
        return 0x8fbc8f;
    }
    else if (col == darkslateblue)
    {
        return 0x483d8b;
    }
    else if (col == darkslategray)
    {
        return 0x2f4f4f;
    }
    else if (col == darkslategrey)
    {
        return 0x2f4f4f;
    }
    else if (col == darkturquoise)
    {
        return 0x00ced1;
    }
    else if (col == darkviolet)
    {
        return 0x9400d3;
    }
    else if (col == deeppink)
    {
        return 0xff1493;
    }
    else if (col == deepskyblue)
    {
        return 0x00bfff;
    }
    else if (col == dimgray)
    {
        return 0x696969;
    }
    else if (col == dimgrey)
    {
        return 0x696969;
    }
    else if (col == dodgerblue)
    {
        return 0x1e90ff;
    }
    else if (col == firebrick)
    {
        return 0xb22222;
    }
    else if (col == floralwhite)
    {
        return 0xfffaf0;
    }
    else if (col == forestgreen)
    {
        return 0x228b22;
    }
    else if (col == fuchsia)
    {
        return 0xff00ff;
    }
    else if (col == gainsboro)
    {
        return 0xdcdcdc;
    }
    else if (col == ghostwhite)
    {
        return 0xf8f8ff;
    }
    else if (col == gold)
    {
        return 0xffd700;
    }
    else if (col == goldenrod)
    {
        return 0xdaa520;
    }
    else if (col == gray)
    {
        return 0x808080;
    }
    else if (col == green)
    {
        return 0x008000;
    }
    else if (col == greenyellow)
    {
        return 0xadff2f;
    }
    else if (col == grey)
    {
        return 0x808080;
    }
    else if (col == honeydew)
    {
        return 0xf0fff0;
    }
    else if (col == hotpink)
    {
        return 0xff69b4;
    }
    else if (col == indianred)
    {
        return 0xcd5c5c;
    }
    else if (col == indigo)
    {
        return 0x4b0082;
    }
    else if (col == ivory)
    {
        return 0xfffff0;
    }
    else if (col == khaki)
    {
        return 0xf0e68c;
    }
    else if (col == lavender)
    {
        return 0xe6e6fa;
    }
    else if (col == lavenderblush)
    {
        return 0xfff0f5;
    }
    else if (col == lawngreen)
    {
        return 0x7cfc00;
    }
    else if (col == lemonchiffon)
    {
        return 0xfffacd;
    }
    else if (col == lightblue)
    {
        return 0xadd8e6;
    }
    else if (col == lightcoral)
    {
        return 0xf08080;
    }
    else if (col == lightcyan)
    {
        return 0xe0ffff;
    }
    else if (col == lightgoldenrodyellow)
    {
        return 0xfafad2;
    }
    else if (col == lightgray)
    {
        return 0xd3d3d3;
    }
    else if (col == lightgreen)
    {
        return 0x90ee90;
    }
    else if (col == lightgrey)
    {
        return 0xd3d3d3;
    }
    else if (col == lightpink)
    {
        return 0xffb6c1;
    }
    else if (col == lightsalmon)
    {
        return 0xffa07a;
    }
    else if (col == lightseagreen)
    {
        return 0x20b2aa;
    }
    else if (col == lightskyblue)
    {
        return 0x87cefa;
    }
    else if (col == lightslategray)
    {
        return 0x778899;
    }
    else if (col == lightslategrey)
    {
        return 0x778899;
    }
    else if (col == lightsteelblue)
    {
        return 0xb0c4de;
    }
    else if (col == lightyellow)
    {
        return 0xffffe0;
    }
    else if (col == lime)
    {
        return 0x00ff00;
    }
    else if (col == limegreen)
    {
        return 0x32cd32;
    }
    else if (col == linen)
    {
        return 0xfaf0e6;
    }
    else if (col == magenta)
    {
        return 0xff00ff;
    }
    else if (col == maroon)
    {
        return 0x800000;
    }
    else if (col == mediumaquamarine)
    {
        return 0x66cdaa;
    }
    else if (col == mediumblue)
    {
        return 0x0000cd;
    }
    else if (col == mediumorchid)
    {
        return 0xba55d3;
    }
    else if (col == mediumpurple)
    {
        return 0x9370db;
    }
    else if (col == mediumseagreen)
    {
        return 0x3cb371;
    }
    else if (col == mediumslateblue)
    {
        return 0x7b68ee;
    }
    else if (col == mediumspringgreen)
    {
        return 0x00fa9a;
    }
    else if (col == mediumturquoise)
    {
        return 0x48d1cc;
    }
    else if (col == mediumvioletred)
    {
        return 0xc71585;
    }
    else if (col == midnightblue)
    {
        return 0x191970;
    }
    else if (col == mintcream)
    {
        return 0xf5fffa;
    }
    else if (col == mistyrose)
    {
        return 0xffe4e1;
    }
    else if (col == moccasin)
    {
        return 0xffe4b5;
    }
    else if (col == navajowhite)
    {
        return 0xffdead;
    }
    else if (col == navy)
    {
        return 0x000080;
    }
    else if (col == oldlace)
    {
        return 0xfdf5e6;
    }
    else if (col == olive)
    {
        return 0x808000;
    }
    else if (col == olivedrab)
    {
        return 0x6b8e23;
    }
    else if (col == orange)
    {
        return 0xffa500;
    }
    else if (col == orangered)
    {
        return 0xff4500;
    }
    else if (col == orchid)
    {
        return 0xda70d6;
    }
    else if (col == palegoldenrod)
    {
        return 0xeee8aa;
    }
    else if (col == palegreen)
    {
        return 0x98fb98;
    }
    else if (col == paleturquoise)
    {
        return 0xafeeee;
    }
    else if (col == palevioletred)
    {
        return 0xdb7093;
    }
    else if (col == papayawhip)
    {
        return 0xffefd5;
    }
    else if (col == peachpuff)
    {
        return 0xffdab9;
    }
    else if (col == peru)
    {
        return 0xcd853f;
    }
    else if (col == pink)
    {
        return 0xffc0cb;
    }
    else if (col == plum)
    {
        return 0xdda0dd;
    }
    else if (col == powderblue)
    {
        return 0xb0e0e6;
    }
    else if (col == purple)
    {
        return 0x800080;
    }
    else if (col == rebeccapurple)
    {
        return 0x663399;
    }
    else if (col == red)
    {
        return 0xff0000;
    }
    else if (col == rosybrown)
    {
        return 0xbc8f8f;
    }
    else if (col == royalblue)
    {
        return 0x4169e1;
    }
    else if (col == saddlebrown)
    {
        return 0x8b4513;
    }
    else if (col == salmon)
    {
        return 0xfa8072;
    }
    else if (col == sandybrown)
    {
        return 0xf4a460;
    }
    else if (col == seagreen)
    {
        return 0x2e8b57;
    }
    else if (col == seashell)
    {
        return 0xfff5ee;
    }
    else if (col == sienna)
    {
        return 0xa0522d;
    }
    else if (col == silver)
    {
        return 0xc0c0c0;
    }
    else if (col == skyblue)
    {
        return 0x87ceeb;
    }
    else if (col == slateblue)
    {
        return 0x6a5acd;
    }
    else if (col == slategray)
    {
        return 0x708090;
    }
    else if (col == slategrey)
    {
        return 0x708090;
    }
    else if (col == snow)
    {
        return 0xfffafa;
    }
    else if (col == springgreen)
    {
        return 0x00ff7f;
    }
    else if (col == steelblue)
    {
        return 0x4682b4;
    }
    else if (col == tan_col)
    {
        return 0xd2b48c;
    }
    else if (col == teal)
    {
        return 0x008080;
    }
    else if (col == thistle)
    {
        return 0xd8bfd8;
    }
    else if (col == tomato)
    {
        return 0xff6347;
    }
    else if (col == turquoise)
    {
        return 0x40e0d0;
    }
    else if (col == violet)
    {
        return 0xee82ee;
    }
    else if (col == wheat)
    {
        return 0xf5deb3;
    }
    else if (col == white)
    {
        return 0xffffff;
    }
    else if (col == whitesmoke)
    {
        return 0xf5f5f5;
    }
    else if (col == yellow)
    {
        return 0xffff00;
    }
    else if (col == yellowgreen)
    {
        return 0x9acd32;
    }
    else
    {
        NOT_IMPLEMENTED
    }

    return 0;
}