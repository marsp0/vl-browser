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
        return 0xf0f8ffff;
    }
    else if (col == antiquewhite)
    {
        return 0xfaebd7ff;
    }
    else if (col == aqua)
    {
        return 0x00ffffff;
    }
    else if (col == aquamarine)
    {
        return 0x7fffd4ff;
    }
    else if (col == azure)
    {
        return 0xf0ffffff;
    }
    else if (col == beige)
    {
        return 0xf5f5dcff;
    }
    else if (col == bisque)
    {
        return 0xffe4c4ff;
    }
    else if (col == black)
    {
        return 0x000000ff;
    }
    else if (col == blanchedalmond)
    {
        return 0xffebcdff;
    }
    else if (col == blue)
    {
        return 0x0000ffff;
    }
    else if (col == blueviolet)
    {
        return 0x8a2be2ff;
    }
    else if (col == brown)
    {
        return 0xa52a2aff;
    }
    else if (col == burlywood)
    {
        return 0xdeb887ff;
    }
    else if (col == cadetblue)
    {
        return 0x5f9ea0ff;
    }
    else if (col == chartreuse)
    {
        return 0x7fff00ff;
    }
    else if (col == chocolate)
    {
        return 0xd2691eff;
    }
    else if (col == coral)
    {
        return 0xff7f50ff;
    }
    else if (col == cornflowerblue)
    {
        return 0x6495edff;
    }
    else if (col == cornsilk)
    {
        return 0xfff8dcff;
    }
    else if (col == crimson)
    {
        return 0xdc143cff;
    }
    else if (col == cyan)
    {
        return 0x00ffffff;
    }
    else if (col == darkblue)
    {
        return 0x00008bff;
    }
    else if (col == darkcyan)
    {
        return 0x008b8bff;
    }
    else if (col == darkgoldenrod)
    {
        return 0xb8860bff;
    }
    else if (col == darkgray)
    {
        return 0xa9a9a9ff;
    }
    else if (col == darkgreen)
    {
        return 0x006400ff;
    }
    else if (col == darkgrey)
    {
        return 0xa9a9a9ff;
    }
    else if (col == darkkhaki)
    {
        return 0xbdb76bff;
    }
    else if (col == darkmagenta)
    {
        return 0x8b008bff;
    }
    else if (col == darkolivegreen)
    {
        return 0x556b2fff;
    }
    else if (col == darkorange)
    {
        return 0xff8c00ff;
    }
    else if (col == darkorchid)
    {
        return 0x9932ccff;
    }
    else if (col == darkred)
    {
        return 0x8b0000ff;
    }
    else if (col == darksalmon)
    {
        return 0xe9967aff;
    }
    else if (col == darkseagreen)
    {
        return 0x8fbc8fff;
    }
    else if (col == darkslateblue)
    {
        return 0x483d8bff;
    }
    else if (col == darkslategray)
    {
        return 0x2f4f4fff;
    }
    else if (col == darkslategrey)
    {
        return 0x2f4f4fff;
    }
    else if (col == darkturquoise)
    {
        return 0x00ced1ff;
    }
    else if (col == darkviolet)
    {
        return 0x9400d3ff;
    }
    else if (col == deeppink)
    {
        return 0xff1493ff;
    }
    else if (col == deepskyblue)
    {
        return 0x00bfffff;
    }
    else if (col == dimgray)
    {
        return 0x696969ff;
    }
    else if (col == dimgrey)
    {
        return 0x696969ff;
    }
    else if (col == dodgerblue)
    {
        return 0x1e90ffff;
    }
    else if (col == firebrick)
    {
        return 0xb22222ff;
    }
    else if (col == floralwhite)
    {
        return 0xfffaf0ff;
    }
    else if (col == forestgreen)
    {
        return 0x228b22ff;
    }
    else if (col == fuchsia)
    {
        return 0xff00ffff;
    }
    else if (col == gainsboro)
    {
        return 0xdcdcdcff;
    }
    else if (col == ghostwhite)
    {
        return 0xf8f8ffff;
    }
    else if (col == gold)
    {
        return 0xffd700ff;
    }
    else if (col == goldenrod)
    {
        return 0xdaa520ff;
    }
    else if (col == gray)
    {
        return 0x808080ff;
    }
    else if (col == green)
    {
        return 0x008000ff;
    }
    else if (col == greenyellow)
    {
        return 0xadff2fff;
    }
    else if (col == grey)
    {
        return 0x808080ff;
    }
    else if (col == honeydew)
    {
        return 0xf0fff0ff;
    }
    else if (col == hotpink)
    {
        return 0xff69b4ff;
    }
    else if (col == indianred)
    {
        return 0xcd5c5cff;
    }
    else if (col == indigo)
    {
        return 0x4b0082ff;
    }
    else if (col == ivory)
    {
        return 0xfffff0ff;
    }
    else if (col == khaki)
    {
        return 0xf0e68cff;
    }
    else if (col == lavender)
    {
        return 0xe6e6faff;
    }
    else if (col == lavenderblush)
    {
        return 0xfff0f5ff;
    }
    else if (col == lawngreen)
    {
        return 0x7cfc00ff;
    }
    else if (col == lemonchiffon)
    {
        return 0xfffacdff;
    }
    else if (col == lightblue)
    {
        return 0xadd8e6ff;
    }
    else if (col == lightcoral)
    {
        return 0xf08080ff;
    }
    else if (col == lightcyan)
    {
        return 0xe0ffffff;
    }
    else if (col == lightgoldenrodyellow)
    {
        return 0xfafad2ff;
    }
    else if (col == lightgray)
    {
        return 0xd3d3d3ff;
    }
    else if (col == lightgreen)
    {
        return 0x90ee90ff;
    }
    else if (col == lightgrey)
    {
        return 0xd3d3d3ff;
    }
    else if (col == lightpink)
    {
        return 0xffb6c1ff;
    }
    else if (col == lightsalmon)
    {
        return 0xffa07aff;
    }
    else if (col == lightseagreen)
    {
        return 0x20b2aaff;
    }
    else if (col == lightskyblue)
    {
        return 0x87cefaff;
    }
    else if (col == lightslategray)
    {
        return 0x778899ff;
    }
    else if (col == lightslategrey)
    {
        return 0x778899ff;
    }
    else if (col == lightsteelblue)
    {
        return 0xb0c4deff;
    }
    else if (col == lightyellow)
    {
        return 0xffffe0ff;
    }
    else if (col == lime)
    {
        return 0x00ff00ff;
    }
    else if (col == limegreen)
    {
        return 0x32cd32ff;
    }
    else if (col == linen)
    {
        return 0xfaf0e6ff;
    }
    else if (col == magenta)
    {
        return 0xff00ffff;
    }
    else if (col == maroon)
    {
        return 0x800000ff;
    }
    else if (col == mediumaquamarine)
    {
        return 0x66cdaaff;
    }
    else if (col == mediumblue)
    {
        return 0x0000cdff;
    }
    else if (col == mediumorchid)
    {
        return 0xba55d3ff;
    }
    else if (col == mediumpurple)
    {
        return 0x9370dbff;
    }
    else if (col == mediumseagreen)
    {
        return 0x3cb371ff;
    }
    else if (col == mediumslateblue)
    {
        return 0x7b68eeff;
    }
    else if (col == mediumspringgreen)
    {
        return 0x00fa9aff;
    }
    else if (col == mediumturquoise)
    {
        return 0x48d1ccff;
    }
    else if (col == mediumvioletred)
    {
        return 0xc71585ff;
    }
    else if (col == midnightblue)
    {
        return 0x191970ff;
    }
    else if (col == mintcream)
    {
        return 0xf5fffaff;
    }
    else if (col == mistyrose)
    {
        return 0xffe4e1ff;
    }
    else if (col == moccasin)
    {
        return 0xffe4b5ff;
    }
    else if (col == navajowhite)
    {
        return 0xffdeadff;
    }
    else if (col == navy)
    {
        return 0x000080ff;
    }
    else if (col == oldlace)
    {
        return 0xfdf5e6ff;
    }
    else if (col == olive)
    {
        return 0x808000ff;
    }
    else if (col == olivedrab)
    {
        return 0x6b8e23ff;
    }
    else if (col == orange)
    {
        return 0xffa500ff;
    }
    else if (col == orangered)
    {
        return 0xff4500ff;
    }
    else if (col == orchid)
    {
        return 0xda70d6ff;
    }
    else if (col == palegoldenrod)
    {
        return 0xeee8aaff;
    }
    else if (col == palegreen)
    {
        return 0x98fb98ff;
    }
    else if (col == paleturquoise)
    {
        return 0xafeeeeff;
    }
    else if (col == palevioletred)
    {
        return 0xdb7093ff;
    }
    else if (col == papayawhip)
    {
        return 0xffefd5ff;
    }
    else if (col == peachpuff)
    {
        return 0xffdab9ff;
    }
    else if (col == peru)
    {
        return 0xcd853fff;
    }
    else if (col == pink)
    {
        return 0xffc0cbff;
    }
    else if (col == plum)
    {
        return 0xdda0ddff;
    }
    else if (col == powderblue)
    {
        return 0xb0e0e6ff;
    }
    else if (col == purple)
    {
        return 0x800080ff;
    }
    else if (col == rebeccapurple)
    {
        return 0x663399ff;
    }
    else if (col == red)
    {
        return 0xff0000ff;
    }
    else if (col == rosybrown)
    {
        return 0xbc8f8fff;
    }
    else if (col == royalblue)
    {
        return 0x4169e1ff;
    }
    else if (col == saddlebrown)
    {
        return 0x8b4513ff;
    }
    else if (col == salmon)
    {
        return 0xfa8072ff;
    }
    else if (col == sandybrown)
    {
        return 0xf4a460ff;
    }
    else if (col == seagreen)
    {
        return 0x2e8b57ff;
    }
    else if (col == seashell)
    {
        return 0xfff5eeff;
    }
    else if (col == sienna)
    {
        return 0xa0522dff;
    }
    else if (col == silver)
    {
        return 0xc0c0c0ff;
    }
    else if (col == skyblue)
    {
        return 0x87ceebff;
    }
    else if (col == slateblue)
    {
        return 0x6a5acdff;
    }
    else if (col == slategray)
    {
        return 0x708090ff;
    }
    else if (col == slategrey)
    {
        return 0x708090ff;
    }
    else if (col == snow)
    {
        return 0xfffafaff;
    }
    else if (col == springgreen)
    {
        return 0x00ff7fff;
    }
    else if (col == steelblue)
    {
        return 0x4682b4ff;
    }
    else if (col == tan_col)
    {
        return 0xd2b48cff;
    }
    else if (col == teal)
    {
        return 0x008080ff;
    }
    else if (col == thistle)
    {
        return 0xd8bfd8ff;
    }
    else if (col == tomato)
    {
        return 0xff6347ff;
    }
    else if (col == turquoise)
    {
        return 0x40e0d0ff;
    }
    else if (col == violet)
    {
        return 0xee82eeff;
    }
    else if (col == wheat)
    {
        return 0xf5deb3ff;
    }
    else if (col == white)
    {
        return 0xffffffff;
    }
    else if (col == whitesmoke)
    {
        return 0xf5f5f5ff;
    }
    else if (col == yellow)
    {
        return 0xffff00ff;
    }
    else if (col == yellowgreen)
    {
        return 0x9acd32ff;
    }

    return 0;
}


hash_str_t css_color_name_aliceblue()
{
    return aliceblue;
}


hash_str_t css_color_name_antiquewhite()
{
    return antiquewhite;
}


hash_str_t css_color_name_aqua()
{
    return aqua;
}


hash_str_t css_color_name_aquamarine()
{
    return aquamarine;
}


hash_str_t css_color_name_azure()
{
    return azure;
}


hash_str_t css_color_name_beige()
{
    return beige;
}


hash_str_t css_color_name_bisque()
{
    return bisque;
}


hash_str_t css_color_name_black()
{
    return black;
}


hash_str_t css_color_name_blanchedalmond()
{
    return blanchedalmond;
}


hash_str_t css_color_name_blue()
{
    return blue;
}


hash_str_t css_color_name_blueviolet()
{
    return blueviolet;
}


hash_str_t css_color_name_brown()
{
    return brown;
}


hash_str_t css_color_name_burlywood()
{
    return burlywood;
}


hash_str_t css_color_name_cadetblue()
{
    return cadetblue;
}


hash_str_t css_color_name_chartreuse()
{
    return chartreuse;
}


hash_str_t css_color_name_chocolate()
{
    return chocolate;
}


hash_str_t css_color_name_coral()
{
    return coral;
}


hash_str_t css_color_name_cornflowerblue()
{
    return cornflowerblue;
}


hash_str_t css_color_name_cornsilk()
{
    return cornsilk;
}


hash_str_t css_color_name_crimson()
{
    return crimson;
}


hash_str_t css_color_name_cyan()
{
    return cyan;
}


hash_str_t css_color_name_darkblue()
{
    return darkblue;
}


hash_str_t css_color_name_darkcyan()
{
    return darkcyan;
}


hash_str_t css_color_name_darkgoldenrod()
{
    return darkgoldenrod;
}


hash_str_t css_color_name_darkgray()
{
    return darkgray;
}


hash_str_t css_color_name_darkgreen()
{
    return darkgreen;
}


hash_str_t css_color_name_darkgrey()
{
    return darkgrey;
}


hash_str_t css_color_name_darkkhaki()
{
    return darkkhaki;
}


hash_str_t css_color_name_darkmagenta()
{
    return darkmagenta;
}


hash_str_t css_color_name_darkolivegreen()
{
    return darkolivegreen;
}


hash_str_t css_color_name_darkorange()
{
    return darkorange;
}


hash_str_t css_color_name_darkorchid()
{
    return darkorchid;
}


hash_str_t css_color_name_darkred()
{
    return darkred;
}


hash_str_t css_color_name_darksalmon()
{
    return darksalmon;
}


hash_str_t css_color_name_darkseagreen()
{
    return darkseagreen;
}


hash_str_t css_color_name_darkslateblue()
{
    return darkslateblue;
}


hash_str_t css_color_name_darkslategray()
{
    return darkslategray;
}


hash_str_t css_color_name_darkslategrey()
{
    return darkslategrey;
}


hash_str_t css_color_name_darkturquoise()
{
    return darkturquoise;
}


hash_str_t css_color_name_darkviolet()
{
    return darkviolet;
}


hash_str_t css_color_name_deeppink()
{
    return deeppink;
}


hash_str_t css_color_name_deepskyblue()
{
    return deepskyblue;
}


hash_str_t css_color_name_dimgray()
{
    return dimgray;
}


hash_str_t css_color_name_dimgrey()
{
    return dimgrey;
}


hash_str_t css_color_name_dodgerblue()
{
    return dodgerblue;
}


hash_str_t css_color_name_firebrick()
{
    return firebrick;
}


hash_str_t css_color_name_floralwhite()
{
    return floralwhite;
}


hash_str_t css_color_name_forestgreen()
{
    return forestgreen;
}


hash_str_t css_color_name_fuchsia()
{
    return fuchsia;
}


hash_str_t css_color_name_gainsboro()
{
    return gainsboro;
}


hash_str_t css_color_name_ghostwhite()
{
    return ghostwhite;
}


hash_str_t css_color_name_gold()
{
    return gold;
}


hash_str_t css_color_name_goldenrod()
{
    return goldenrod;
}


hash_str_t css_color_name_gray()
{
    return gray;
}


hash_str_t css_color_name_green()
{
    return green;
}


hash_str_t css_color_name_greenyellow()
{
    return greenyellow;
}


hash_str_t css_color_name_grey()
{
    return grey;
}


hash_str_t css_color_name_honeydew()
{
    return honeydew;
}


hash_str_t css_color_name_hotpink()
{
    return hotpink;
}


hash_str_t css_color_name_indianred()
{
    return indianred;
}


hash_str_t css_color_name_indigo()
{
    return indigo;
}


hash_str_t css_color_name_ivory()
{
    return ivory;
}


hash_str_t css_color_name_khaki()
{
    return khaki;
}


hash_str_t css_color_name_lavender()
{
    return lavender;
}


hash_str_t css_color_name_lavenderblush()
{
    return lavenderblush;
}


hash_str_t css_color_name_lawngreen()
{
    return lawngreen;
}


hash_str_t css_color_name_lemonchiffon()
{
    return lemonchiffon;
}


hash_str_t css_color_name_lightblue()
{
    return lightblue;
}


hash_str_t css_color_name_lightcoral()
{
    return lightcoral;
}


hash_str_t css_color_name_lightcyan()
{
    return lightcyan;
}


hash_str_t css_color_name_lightgoldenrodyellow()
{
    return lightgoldenrodyellow;
}


hash_str_t css_color_name_lightgray()
{
    return lightgray;
}


hash_str_t css_color_name_lightgreen()
{
    return lightgreen;
}


hash_str_t css_color_name_lightgrey()
{
    return lightgrey;
}


hash_str_t css_color_name_lightpink()
{
    return lightpink;
}


hash_str_t css_color_name_lightsalmon()
{
    return lightsalmon;
}


hash_str_t css_color_name_lightseagreen()
{
    return lightseagreen;
}


hash_str_t css_color_name_lightskyblue()
{
    return lightskyblue;
}


hash_str_t css_color_name_lightslategray()
{
    return lightslategray;
}


hash_str_t css_color_name_lightslategrey()
{
    return lightslategrey;
}


hash_str_t css_color_name_lightsteelblue()
{
    return lightsteelblue;
}


hash_str_t css_color_name_lightyellow()
{
    return lightyellow;
}


hash_str_t css_color_name_lime()
{
    return lime;
}


hash_str_t css_color_name_limegreen()
{
    return limegreen;
}


hash_str_t css_color_name_linen()
{
    return linen;
}


hash_str_t css_color_name_magenta()
{
    return magenta;
}


hash_str_t css_color_name_maroon()
{
    return maroon;
}


hash_str_t css_color_name_mediumaquamarine()
{
    return mediumaquamarine;
}


hash_str_t css_color_name_mediumblue()
{
    return mediumblue;
}


hash_str_t css_color_name_mediumorchid()
{
    return mediumorchid;
}


hash_str_t css_color_name_mediumpurple()
{
    return mediumpurple;
}


hash_str_t css_color_name_mediumseagreen()
{
    return mediumseagreen;
}


hash_str_t css_color_name_mediumslateblue()
{
    return mediumslateblue;
}


hash_str_t css_color_name_mediumspringgreen()
{
    return mediumspringgreen;
}


hash_str_t css_color_name_mediumturquoise()
{
    return mediumturquoise;
}


hash_str_t css_color_name_mediumvioletred()
{
    return mediumvioletred;
}


hash_str_t css_color_name_midnightblue()
{
    return midnightblue;
}


hash_str_t css_color_name_mintcream()
{
    return mintcream;
}


hash_str_t css_color_name_mistyrose()
{
    return mistyrose;
}


hash_str_t css_color_name_moccasin()
{
    return moccasin;
}


hash_str_t css_color_name_navajowhite()
{
    return navajowhite;
}


hash_str_t css_color_name_navy()
{
    return navy;
}


hash_str_t css_color_name_oldlace()
{
    return oldlace;
}


hash_str_t css_color_name_olive()
{
    return olive;
}


hash_str_t css_color_name_olivedrab()
{
    return olivedrab;
}


hash_str_t css_color_name_orange()
{
    return orange;
}


hash_str_t css_color_name_orangered()
{
    return orangered;
}


hash_str_t css_color_name_orchid()
{
    return orchid;
}


hash_str_t css_color_name_palegoldenrod()
{
    return palegoldenrod;
}


hash_str_t css_color_name_palegreen()
{
    return palegreen;
}


hash_str_t css_color_name_paleturquoise()
{
    return paleturquoise;
}


hash_str_t css_color_name_palevioletred()
{
    return palevioletred;
}


hash_str_t css_color_name_papayawhip()
{
    return papayawhip;
}


hash_str_t css_color_name_peachpuff()
{
    return peachpuff;
}


hash_str_t css_color_name_peru()
{
    return peru;
}


hash_str_t css_color_name_pink()
{
    return pink;
}


hash_str_t css_color_name_plum()
{
    return plum;
}


hash_str_t css_color_name_powderblue()
{
    return powderblue;
}


hash_str_t css_color_name_purple()
{
    return purple;
}


hash_str_t css_color_name_rebeccapurple()
{
    return rebeccapurple;
}


hash_str_t css_color_name_red()
{
    return red;
}


hash_str_t css_color_name_rosybrown()
{
    return rosybrown;
}


hash_str_t css_color_name_royalblue()
{
    return royalblue;
}


hash_str_t css_color_name_saddlebrown()
{
    return saddlebrown;
}


hash_str_t css_color_name_salmon()
{
    return salmon;
}


hash_str_t css_color_name_sandybrown()
{
    return sandybrown;
}


hash_str_t css_color_name_seagreen()
{
    return seagreen;
}


hash_str_t css_color_name_seashell()
{
    return seashell;
}


hash_str_t css_color_name_sienna()
{
    return sienna;
}


hash_str_t css_color_name_silver()
{
    return silver;
}


hash_str_t css_color_name_skyblue()
{
    return skyblue;
}


hash_str_t css_color_name_slateblue()
{
    return slateblue;
}


hash_str_t css_color_name_slategray()
{
    return slategray;
}


hash_str_t css_color_name_slategrey()
{
    return slategrey;
}


hash_str_t css_color_name_snow()
{
    return snow;
}


hash_str_t css_color_name_springgreen()
{
    return springgreen;
}


hash_str_t css_color_name_steelblue()
{
    return steelblue;
}


hash_str_t css_color_name_tan_col()
{
    return tan_col;
}


hash_str_t css_color_name_teal()
{
    return teal;
}


hash_str_t css_color_name_thistle()
{
    return thistle;
}


hash_str_t css_color_name_tomato()
{
    return tomato;
}


hash_str_t css_color_name_turquoise()
{
    return turquoise;
}


hash_str_t css_color_name_violet()
{
    return violet;
}


hash_str_t css_color_name_wheat()
{
    return wheat;
}


hash_str_t css_color_name_white()
{
    return white;
}


hash_str_t css_color_name_whitesmoke()
{
    return whitesmoke;
}


hash_str_t css_color_name_yellow()
{
    return yellow;
}


hash_str_t css_color_name_yellowgreen()
{
    return yellowgreen;
}