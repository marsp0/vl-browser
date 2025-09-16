#include "html/tag_constants.h"

hash_str_t html;
hash_str_t HTML;
hash_str_t head;
hash_str_t HEAD;
hash_str_t body;
hash_str_t BODY;
hash_str_t br;
hash_str_t BR;
hash_str_t base;
hash_str_t BASE;
hash_str_t basefont;
hash_str_t BASEFONT;
hash_str_t bgsound;
hash_str_t BGSOUND;
hash_str_t link;
hash_str_t LINK;
hash_str_t meta;
hash_str_t META;
hash_str_t title;
hash_str_t TITLE;
hash_str_t script;
hash_str_t SCRIPT;
hash_str_t noscript;
hash_str_t NOSCRIPT;
hash_str_t noframes;
hash_str_t NOFRAMES;
hash_str_t style;
hash_str_t STYLE;
hash_str_t template;
hash_str_t TEMPLATE;
hash_str_t frame;
hash_str_t FRAME;
hash_str_t frameset;
hash_str_t FRAMESET;
hash_str_t dd;
hash_str_t DD;
hash_str_t dt;
hash_str_t DT;
hash_str_t li;
hash_str_t LI;
hash_str_t optgroup;
hash_str_t OPTGROUP;
hash_str_t option;
hash_str_t OPTION;
hash_str_t p;
hash_str_t P;
hash_str_t rb;
hash_str_t RB;
hash_str_t rp;
hash_str_t RP;
hash_str_t rt;
hash_str_t RT;
hash_str_t rtc;
hash_str_t RTC;
hash_str_t svg;
hash_str_t SVG;
hash_str_t tbody;
hash_str_t TBODY;
hash_str_t td;
hash_str_t TD;
hash_str_t tfoot;
hash_str_t TFOOT;
hash_str_t th;
hash_str_t TH;
hash_str_t thead;
hash_str_t THEAD;
hash_str_t table;
hash_str_t TABLE;
hash_str_t tr;
hash_str_t TR;
hash_str_t address;
hash_str_t ADDRESS;
hash_str_t article;
hash_str_t ARTICLE;
hash_str_t aside;
hash_str_t ASIDE;
hash_str_t blockquote;
hash_str_t BLOCKQUOTE;
hash_str_t center;
hash_str_t CENTER;
hash_str_t details;
hash_str_t DETAILS;
hash_str_t dialog;
hash_str_t DIALOG;
hash_str_t dir;
hash_str_t DIR;
hash_str_t div;
hash_str_t DIV;
hash_str_t dl;
hash_str_t DL;
hash_str_t fieldset;
hash_str_t FIELDSET;
hash_str_t figcaption;
hash_str_t FIGCAPTION;
hash_str_t figure;
hash_str_t FIGURE;
hash_str_t footer;
hash_str_t FOOTER;
hash_str_t header;
hash_str_t HEADER;
hash_str_t hgroup;
hash_str_t HGROUP;
hash_str_t main_tag;
hash_str_t MAIN;
hash_str_t menu;
hash_str_t MENU;
hash_str_t nav;
hash_str_t NAV;
hash_str_t ol;
hash_str_t OL;
hash_str_t search;
hash_str_t SEARCH;
hash_str_t section;
hash_str_t SECTION;
hash_str_t summary;
hash_str_t SUMMARY;
hash_str_t ul;
hash_str_t UL;
hash_str_t h1;
hash_str_t H1;
hash_str_t h2;
hash_str_t H2;
hash_str_t h3;
hash_str_t H3;
hash_str_t h4;
hash_str_t H4;
hash_str_t h5;
hash_str_t H5;
hash_str_t h6;
hash_str_t H6;
hash_str_t pre;
hash_str_t PRE;
hash_str_t listing;
hash_str_t LISTING;
hash_str_t form;
hash_str_t FORM;
hash_str_t font;
hash_str_t FONT;
hash_str_t button;
hash_str_t BUTTON;
hash_str_t plaintext;
hash_str_t PLAINTEXT;
hash_str_t a;
hash_str_t A;
hash_str_t b;
hash_str_t B;
hash_str_t big;
hash_str_t BIG;
hash_str_t code;
hash_str_t CODE;
hash_str_t em;
hash_str_t EM;
hash_str_t i;
hash_str_t I;
hash_str_t s;
hash_str_t S;
hash_str_t small;
hash_str_t SMALL;
hash_str_t strike;
hash_str_t STRIKE;
hash_str_t strong;
hash_str_t STRONG;
hash_str_t tt;
hash_str_t TT;
hash_str_t u;
hash_str_t U;
hash_str_t nobr;
hash_str_t NOBR;
hash_str_t applet;
hash_str_t APPLET;
hash_str_t marquee;
hash_str_t MARQUEE;
hash_str_t math;
hash_str_t MATH;
hash_str_t object;
hash_str_t OBJECT;
hash_str_t area;
hash_str_t AREA;
hash_str_t embed;
hash_str_t EMBED;
hash_str_t img;
hash_str_t IMG;
hash_str_t keygen;
hash_str_t KEYGEN;
hash_str_t wbr;
hash_str_t WBR;
hash_str_t input;
hash_str_t INPUT;
hash_str_t param;
hash_str_t PARAM;
hash_str_t source;
hash_str_t SOURCE;
hash_str_t track;
hash_str_t TRACK;
hash_str_t hr;
hash_str_t HR;
hash_str_t image;
hash_str_t IMAGE;
hash_str_t textarea;
hash_str_t TEXTAREA;
hash_str_t xmp;
hash_str_t XMP;
hash_str_t iframe;
hash_str_t IFRAME;
hash_str_t noembed;
hash_str_t NOEMBED;
hash_str_t select;
hash_str_t SELECT;
hash_str_t col;
hash_str_t COL;
hash_str_t colgroup;
hash_str_t COLGROUP;
hash_str_t caption;
hash_str_t CAPTION;
hash_str_t span;
hash_str_t SPAN;
hash_str_t cite;
hash_str_t CITE;

void html_populate_tags()
{
    html           = hash_str_new("html", 4);
    HTML           = hash_str_new("HTML", 4);

    head           = hash_str_new("head", 4);
    HEAD           = hash_str_new("HEAD", 4);

    body           = hash_str_new("body", 4);
    BODY           = hash_str_new("BODY", 4);

    br             = hash_str_new("br", 2);
    BR             = hash_str_new("BR", 2);

    base           = hash_str_new("base", 4);
    BASE           = hash_str_new("BASE", 4);

    basefont       = hash_str_new("basefont", 8);
    BASEFONT       = hash_str_new("BASEFONT", 8);

    bgsound        = hash_str_new("bgsound", 7);
    BGSOUND        = hash_str_new("BGSOUND", 7);

    link           = hash_str_new("link", 4);
    LINK           = hash_str_new("LINK", 4);

    meta           = hash_str_new("meta", 4);
    META           = hash_str_new("META", 4);

    title          = hash_str_new("title", 5);
    TITLE          = hash_str_new("TITLE", 5);

    script         = hash_str_new("script", 6);
    SCRIPT         = hash_str_new("SCRIPT", 6);

    noscript       = hash_str_new("noscript", 8);
    NOSCRIPT       = hash_str_new("NOSCRIPT", 8);

    noframes       = hash_str_new("noframes", 8);
    NOFRAMES       = hash_str_new("NOFRAMES", 8);

    style          = hash_str_new("style", 5);
    STYLE          = hash_str_new("STYLE", 5);

    template       = hash_str_new("template", 8);
    TEMPLATE       = hash_str_new("TEMPLATE", 8);

    frame          = hash_str_new("frame", 5);
    FRAME          = hash_str_new("FRAME", 5);

    frameset       = hash_str_new("frameset", 8);
    FRAMESET       = hash_str_new("FRAMESET", 8);

    dd             = hash_str_new("dd", 2);
    DD             = hash_str_new("DD", 2);

    dt             = hash_str_new("dt", 2);
    DT             = hash_str_new("DT", 2);

    li             = hash_str_new("li", 2);
    LI             = hash_str_new("LI", 2);

    optgroup       = hash_str_new("optgroup", 8);
    OPTGROUP       = hash_str_new("OPTGROUP", 8);

    option         = hash_str_new("option", 6);
    OPTION         = hash_str_new("OPTION", 6);

    p              = hash_str_new("p", 1);
    P              = hash_str_new("P", 1);

    rb             = hash_str_new("rb", 2);
    RB             = hash_str_new("RB", 2);

    rp             = hash_str_new("rp", 2);
    RP             = hash_str_new("RP", 2);

    rt             = hash_str_new("rt", 2);
    RT             = hash_str_new("RT", 2);

    rtc            = hash_str_new("rtc", 3);
    RTC            = hash_str_new("RTC", 3);

    svg            = hash_str_new("svg", 3);
    SVG            = hash_str_new("SVG", 3);

    tbody          = hash_str_new("tbody", 5);
    TBODY          = hash_str_new("TBODY", 5);

    td             = hash_str_new("td", 2);
    TD             = hash_str_new("TD", 2);

    tfoot          = hash_str_new("tfoot", 5);
    TFOOT          = hash_str_new("TFOOT", 5);

    th             = hash_str_new("th", 2);
    TH             = hash_str_new("TH", 2);

    thead          = hash_str_new("thead", 5);
    THEAD          = hash_str_new("THEAD", 5);

    table          = hash_str_new("table", 5);
    TABLE          = hash_str_new("TABLE", 5);

    tr             = hash_str_new("tr", 2);
    TR             = hash_str_new("TR", 2);

    address        = hash_str_new("address", 7);
    ADDRESS        = hash_str_new("ADDRESS", 7);

    article        = hash_str_new("article", 7);
    ARTICLE        = hash_str_new("ARTICLE", 7);

    aside          = hash_str_new("aside", 5);
    ASIDE          = hash_str_new("ASIDE", 5);

    blockquote     = hash_str_new("blockquote", 10);
    BLOCKQUOTE     = hash_str_new("BLOCKQUOTE", 10);

    center         = hash_str_new("center", 6);
    CENTER         = hash_str_new("CENTER", 6);

    details        = hash_str_new("details", 7);
    DETAILS        = hash_str_new("DETAILS", 7);

    dialog         = hash_str_new("dialog", 6);
    DIALOG         = hash_str_new("DIALOG", 6);

    dir            = hash_str_new("dir", 3);
    DIR            = hash_str_new("DIR", 3);

    div            = hash_str_new("div", 3);
    DIV            = hash_str_new("DIV", 3);

    dl             = hash_str_new("dl", 2);
    DL             = hash_str_new("DL", 2);

    fieldset       = hash_str_new("fieldset", 8);
    FIELDSET       = hash_str_new("FIELDSET", 8);

    figcaption     = hash_str_new("figcaption", 10);
    FIGCAPTION     = hash_str_new("FIGCAPTION", 10);

    figure         = hash_str_new("figure", 6);
    FIGURE         = hash_str_new("FIGURE", 6);

    footer         = hash_str_new("footer", 6);
    FOOTER         = hash_str_new("FOOTER", 6);

    header         = hash_str_new("header", 6);
    HEADER         = hash_str_new("HEADER", 6);

    hgroup         = hash_str_new("hgroup", 6);
    HGROUP         = hash_str_new("HGROUP", 6);

    main_tag       = hash_str_new("main", 4);
    MAIN           = hash_str_new("MAIN", 4);

    menu           = hash_str_new("menu", 4);
    MENU           = hash_str_new("MENU", 4);

    nav            = hash_str_new("nav", 3);
    NAV            = hash_str_new("NAV", 3);

    ol             = hash_str_new("ol", 2);
    OL             = hash_str_new("OL", 2);

    search         = hash_str_new("search", 6);
    SEARCH         = hash_str_new("SEARCH", 6);

    section        = hash_str_new("section", 7);
    SECTION        = hash_str_new("SECTION", 7);

    summary        = hash_str_new("summary", 7);
    SUMMARY        = hash_str_new("SUMMARY", 7);

    ul             = hash_str_new("ul", 2);
    UL             = hash_str_new("UL", 2);

    h1             = hash_str_new("h1", 2);
    H1             = hash_str_new("H1", 2);

    h2             = hash_str_new("h2", 2);
    H2             = hash_str_new("H2", 2);

    h3             = hash_str_new("h3", 2);
    H3             = hash_str_new("H3", 2);

    h4             = hash_str_new("h4", 2);
    H4             = hash_str_new("H4", 2);

    h5             = hash_str_new("h5", 2);
    H5             = hash_str_new("H5", 2);

    h6             = hash_str_new("h6", 2);
    H6             = hash_str_new("H6", 2);

    pre            = hash_str_new("pre", 3);
    PRE            = hash_str_new("PRE", 3);

    listing        = hash_str_new("listing", 7);
    LISTING        = hash_str_new("LISTING", 7);

    form           = hash_str_new("form", 4);
    FORM           = hash_str_new("FORM", 4);

    font           = hash_str_new("font", 4);
    FONT           = hash_str_new("FONT", 4);

    button         = hash_str_new("button", 6);
    BUTTON         = hash_str_new("BUTTON", 6);

    plaintext      = hash_str_new("plaintext", 9);
    PLAINTEXT      = hash_str_new("PLAINTEXT", 9);

    a              = hash_str_new("a", 1);
    A              = hash_str_new("A", 1);

    b              = hash_str_new("b", 1);
    B              = hash_str_new("B", 1);

    big            = hash_str_new("big", 3);
    BIG            = hash_str_new("BIG", 3);

    code           = hash_str_new("code", 4);
    CODE           = hash_str_new("CODE", 4);

    em             = hash_str_new("em", 2);
    EM             = hash_str_new("EM", 2);

    i              = hash_str_new("i", 1);
    I              = hash_str_new("I", 1);

    s              = hash_str_new("s", 1);
    S              = hash_str_new("S", 1);

    small          = hash_str_new("small", 5);
    SMALL          = hash_str_new("SMALL", 5);

    strike         = hash_str_new("strike", 6);
    STRIKE         = hash_str_new("STRIKE", 6);

    strong         = hash_str_new("strong", 6);
    STRONG         = hash_str_new("STRONG", 6);

    tt             = hash_str_new("tt", 2);
    TT             = hash_str_new("TT", 2);

    u              = hash_str_new("u", 1);
    U              = hash_str_new("U", 1);

    nobr           = hash_str_new("nobr", 4);
    NOBR           = hash_str_new("NOBR", 4);

    applet         = hash_str_new("applet", 6);
    APPLET         = hash_str_new("APPLET", 6);

    marquee        = hash_str_new("marquee", 7);
    MARQUEE        = hash_str_new("MARQUEE", 7);

    math           = hash_str_new("math", 4);
    MATH           = hash_str_new("MATH", 4);

    object         = hash_str_new("object", 6);
    OBJECT         = hash_str_new("OBJECT", 6);

    area           = hash_str_new("area", 4);
    AREA           = hash_str_new("AREA", 4);

    embed          = hash_str_new("embed", 5);
    EMBED          = hash_str_new("EMBED", 5);

    img            = hash_str_new("img", 3);
    IMG            = hash_str_new("IMG", 3);

    keygen         = hash_str_new("keygen", 6);
    KEYGEN         = hash_str_new("KEYGEN", 6);

    wbr            = hash_str_new("wbr", 3);
    WBR            = hash_str_new("WBR", 3);

    input          = hash_str_new("input", 5);
    INPUT          = hash_str_new("INPUT", 5);

    param          = hash_str_new("param", 5);
    PARAM          = hash_str_new("PARAM", 5);

    source         = hash_str_new("source", 6);
    SOURCE         = hash_str_new("SOURCE", 6);

    track          = hash_str_new("track", 5);
    TRACK          = hash_str_new("TRACK", 5);

    hr             = hash_str_new("hr", 2);
    HR             = hash_str_new("HR", 2);

    image          = hash_str_new("image", 5);
    IMAGE          = hash_str_new("IMAGE", 5);

    textarea       = hash_str_new("textarea", 8);
    TEXTAREA       = hash_str_new("TEXTAREA", 8);

    xmp            = hash_str_new("xmp", 3);
    XMP            = hash_str_new("XMP", 3);

    iframe         = hash_str_new("iframe", 6);
    IFRAME         = hash_str_new("IFRAME", 6);

    noembed        = hash_str_new("noembed", 7);
    NOEMBED        = hash_str_new("NOEMBED", 7);

    select         = hash_str_new("select", 6);
    SELECT         = hash_str_new("SELECT", 6);

    col            = hash_str_new("col", 3);
    COL            = hash_str_new("COL", 3);

    colgroup       = hash_str_new("colgroup", 8);
    COLGROUP       = hash_str_new("COLGROUP", 8);

    caption        = hash_str_new("caption", 7);
    CAPTION        = hash_str_new("CAPTION", 7);

    span           = hash_str_new("span", 4);
    SPAN           = hash_str_new("SPAN", 4);

    cite           = hash_str_new("cite", 4);
    CITE           = hash_str_new("CITE", 4);

    hash_str_pool_stats();
}

hash_str_t html_tag_html()
{
    return html;
}


hash_str_t html_tag_HTML()
{
    return HTML;
}


hash_str_t html_tag_head()
{
    return head;
}


hash_str_t html_tag_HEAD()
{
    return HEAD;
}


hash_str_t html_tag_body()
{
    return body;
}


hash_str_t html_tag_BODY()
{
    return BODY;
}


hash_str_t html_tag_br()
{
    return br;
}


hash_str_t html_tag_BR()
{
    return BR;
}


hash_str_t html_tag_base()
{
    return base;
}


hash_str_t html_tag_BASE()
{
    return BASE;
}


hash_str_t html_tag_basefont()
{
    return basefont;
}


hash_str_t html_tag_BASEFONT()
{
    return BASEFONT;
}


hash_str_t html_tag_bgsound()
{
    return bgsound;
}


hash_str_t html_tag_BGSOUND()
{
    return BGSOUND;
}


hash_str_t html_tag_link()
{
    return link;
}


hash_str_t html_tag_LINK()
{
    return LINK;
}


hash_str_t html_tag_meta()
{
    return meta;
}


hash_str_t html_tag_META()
{
    return META;
}


hash_str_t html_tag_title()
{
    return title;
}


hash_str_t html_tag_TITLE()
{
    return TITLE;
}


hash_str_t html_tag_script()
{
    return script;
}


hash_str_t html_tag_SCRIPT()
{
    return SCRIPT;
}


hash_str_t html_tag_noscript()
{
    return noscript;
}


hash_str_t html_tag_NOSCRIPT()
{
    return NOSCRIPT;
}


hash_str_t html_tag_noframes()
{
    return noframes;
}


hash_str_t html_tag_NOFRAMES()
{
    return NOFRAMES;
}


hash_str_t html_tag_style()
{
    return style;
}


hash_str_t html_tag_STYLE()
{
    return STYLE;
}


hash_str_t html_tag_template()
{
    return template;
}


hash_str_t html_tag_TEMPLATE()
{
    return TEMPLATE;
}


hash_str_t html_tag_frame()
{
    return frame;
}


hash_str_t html_tag_FRAME()
{
    return FRAME;
}


hash_str_t html_tag_frameset()
{
    return frameset;
}


hash_str_t html_tag_FRAMESET()
{
    return FRAMESET;
}


hash_str_t html_tag_dd()
{
    return dd;
}


hash_str_t html_tag_DD()
{
    return DD;
}


hash_str_t html_tag_dt()
{
    return dt;
}


hash_str_t html_tag_DT()
{
    return DT;
}


hash_str_t html_tag_li()
{
    return li;
}


hash_str_t html_tag_LI()
{
    return LI;
}


hash_str_t html_tag_optgroup()
{
    return optgroup;
}


hash_str_t html_tag_OPTGROUP()
{
    return OPTGROUP;
}


hash_str_t html_tag_option()
{
    return option;
}


hash_str_t html_tag_OPTION()
{
    return OPTION;
}


hash_str_t html_tag_p()
{
    return p;
}


hash_str_t html_tag_P()
{
    return P;
}


hash_str_t html_tag_rb()
{
    return rb;
}


hash_str_t html_tag_RB()
{
    return RB;
}


hash_str_t html_tag_rp()
{
    return rp;
}


hash_str_t html_tag_RP()
{
    return RP;
}


hash_str_t html_tag_rt()
{
    return rt;
}


hash_str_t html_tag_RT()
{
    return RT;
}


hash_str_t html_tag_rtc()
{
    return rtc;
}


hash_str_t html_tag_RTC()
{
    return RTC;
}


hash_str_t html_tag_svg()
{
    return svg;
}


hash_str_t html_tag_SVG()
{
    return SVG;
}


hash_str_t html_tag_tbody()
{
    return tbody;
}


hash_str_t html_tag_TBODY()
{
    return TBODY;
}


hash_str_t html_tag_td()
{
    return td;
}


hash_str_t html_tag_TD()
{
    return TD;
}


hash_str_t html_tag_tfoot()
{
    return tfoot;
}


hash_str_t html_tag_TFOOT()
{
    return TFOOT;
}


hash_str_t html_tag_th()
{
    return th;
}


hash_str_t html_tag_TH()
{
    return TH;
}


hash_str_t html_tag_thead()
{
    return thead;
}


hash_str_t html_tag_THEAD()
{
    return THEAD;
}


hash_str_t html_tag_table()
{
    return table;
}


hash_str_t html_tag_TABLE()
{
    return TABLE;
}


hash_str_t html_tag_tr()
{
    return tr;
}


hash_str_t html_tag_TR()
{
    return TR;
}


hash_str_t html_tag_address()
{
    return address;
}


hash_str_t html_tag_ADDRESS()
{
    return ADDRESS;
}


hash_str_t html_tag_article()
{
    return article;
}


hash_str_t html_tag_ARTICLE()
{
    return ARTICLE;
}


hash_str_t html_tag_aside()
{
    return aside;
}


hash_str_t html_tag_ASIDE()
{
    return ASIDE;
}


hash_str_t html_tag_blockquote()
{
    return blockquote;
}


hash_str_t html_tag_BLOCKQUOTE()
{
    return BLOCKQUOTE;
}


hash_str_t html_tag_center()
{
    return center;
}


hash_str_t html_tag_CENTER()
{
    return CENTER;
}


hash_str_t html_tag_details()
{
    return details;
}


hash_str_t html_tag_DETAILS()
{
    return DETAILS;
}


hash_str_t html_tag_dialog()
{
    return dialog;
}


hash_str_t html_tag_DIALOG()
{
    return DIALOG;
}


hash_str_t html_tag_dir()
{
    return dir;
}


hash_str_t html_tag_DIR()
{
    return DIR;
}


hash_str_t html_tag_div()
{
    return div;
}


hash_str_t html_tag_DIV()
{
    return DIV;
}


hash_str_t html_tag_dl()
{
    return dl;
}


hash_str_t html_tag_DL()
{
    return DL;
}


hash_str_t html_tag_fieldset()
{
    return fieldset;
}


hash_str_t html_tag_FIELDSET()
{
    return FIELDSET;
}


hash_str_t html_tag_figcaption()
{
    return figcaption;
}


hash_str_t html_tag_FIGCAPTION()
{
    return FIGCAPTION;
}


hash_str_t html_tag_figure()
{
    return figure;
}


hash_str_t html_tag_FIGURE()
{
    return FIGURE;
}


hash_str_t html_tag_footer()
{
    return footer;
}


hash_str_t html_tag_FOOTER()
{
    return FOOTER;
}


hash_str_t html_tag_header()
{
    return header;
}


hash_str_t html_tag_HEADER()
{
    return HEADER;
}


hash_str_t html_tag_hgroup()
{
    return hgroup;
}


hash_str_t html_tag_HGROUP()
{
    return HGROUP;
}


hash_str_t html_tag_main()
{
    return main_tag;
}


hash_str_t html_tag_MAIN()
{
    return MAIN;
}


hash_str_t html_tag_menu()
{
    return menu;
}


hash_str_t html_tag_MENU()
{
    return MENU;
}


hash_str_t html_tag_nav()
{
    return nav;
}


hash_str_t html_tag_NAV()
{
    return NAV;
}


hash_str_t html_tag_ol()
{
    return ol;
}


hash_str_t html_tag_OL()
{
    return OL;
}


hash_str_t html_tag_search()
{
    return search;
}


hash_str_t html_tag_SEARCH()
{
    return SEARCH;
}


hash_str_t html_tag_section()
{
    return section;
}


hash_str_t html_tag_SECTION()
{
    return SECTION;
}


hash_str_t html_tag_summary()
{
    return summary;
}


hash_str_t html_tag_SUMMARY()
{
    return SUMMARY;
}


hash_str_t html_tag_ul()
{
    return ul;
}


hash_str_t html_tag_UL()
{
    return UL;
}


hash_str_t html_tag_h1()
{
    return h1;
}


hash_str_t html_tag_H1()
{
    return H1;
}


hash_str_t html_tag_h2()
{
    return h2;
}


hash_str_t html_tag_H2()
{
    return H2;
}


hash_str_t html_tag_h3()
{
    return h3;
}


hash_str_t html_tag_H3()
{
    return H3;
}


hash_str_t html_tag_h4()
{
    return h4;
}


hash_str_t html_tag_H4()
{
    return H4;
}


hash_str_t html_tag_h5()
{
    return h5;
}


hash_str_t html_tag_H5()
{
    return H5;
}


hash_str_t html_tag_h6()
{
    return h6;
}


hash_str_t html_tag_H6()
{
    return H6;
}


hash_str_t html_tag_pre()
{
    return pre;
}


hash_str_t html_tag_PRE()
{
    return PRE;
}


hash_str_t html_tag_listing()
{
    return listing;
}


hash_str_t html_tag_LISTING()
{
    return LISTING;
}


hash_str_t html_tag_form()
{
    return form;
}


hash_str_t html_tag_FORM()
{
    return FORM;
}


hash_str_t html_tag_font()
{
    return font;
}


hash_str_t html_tag_FONT()
{
    return FONT;
}


hash_str_t html_tag_button()
{
    return button;
}


hash_str_t html_tag_BUTTON()
{
    return BUTTON;
}


hash_str_t html_tag_plaintext()
{
    return plaintext;
}


hash_str_t html_tag_PLAINTEXT()
{
    return PLAINTEXT;
}


hash_str_t html_tag_a()
{
    return a;
}


hash_str_t html_tag_A()
{
    return A;
}


hash_str_t html_tag_b()
{
    return b;
}


hash_str_t html_tag_B()
{
    return B;
}


hash_str_t html_tag_big()
{
    return big;
}


hash_str_t html_tag_BIG()
{
    return BIG;
}


hash_str_t html_tag_code()
{
    return code;
}


hash_str_t html_tag_CODE()
{
    return CODE;
}


hash_str_t html_tag_em()
{
    return em;
}


hash_str_t html_tag_EM()
{
    return EM;
}


hash_str_t html_tag_i()
{
    return i;
}


hash_str_t html_tag_I()
{
    return I;
}


hash_str_t html_tag_s()
{
    return s;
}


hash_str_t html_tag_S()
{
    return S;
}


hash_str_t html_tag_small()
{
    return small;
}


hash_str_t html_tag_SMALL()
{
    return SMALL;
}


hash_str_t html_tag_strike()
{
    return strike;
}


hash_str_t html_tag_STRIKE()
{
    return STRIKE;
}


hash_str_t html_tag_strong()
{
    return strong;
}


hash_str_t html_tag_STRONG()
{
    return STRONG;
}


hash_str_t html_tag_tt()
{
    return tt;
}


hash_str_t html_tag_TT()
{
    return TT;
}


hash_str_t html_tag_u()
{
    return u;
}


hash_str_t html_tag_U()
{
    return U;
}


hash_str_t html_tag_nobr()
{
    return nobr;
}


hash_str_t html_tag_NOBR()
{
    return NOBR;
}


hash_str_t html_tag_applet()
{
    return applet;
}


hash_str_t html_tag_APPLET()
{
    return APPLET;
}


hash_str_t html_tag_marquee()
{
    return marquee;
}


hash_str_t html_tag_MARQUEE()
{
    return MARQUEE;
}


hash_str_t html_tag_math()
{
    return math;
}


hash_str_t html_tag_MATH()
{
    return MATH;
}


hash_str_t html_tag_object()
{
    return object;
}


hash_str_t html_tag_OBJECT()
{
    return OBJECT;
}


hash_str_t html_tag_area()
{
    return area;
}


hash_str_t html_tag_AREA()
{
    return AREA;
}


hash_str_t html_tag_embed()
{
    return embed;
}


hash_str_t html_tag_EMBED()
{
    return EMBED;
}


hash_str_t html_tag_img()
{
    return img;
}


hash_str_t html_tag_IMG()
{
    return IMG;
}


hash_str_t html_tag_keygen()
{
    return keygen;
}


hash_str_t html_tag_KEYGEN()
{
    return KEYGEN;
}


hash_str_t html_tag_wbr()
{
    return wbr;
}


hash_str_t html_tag_WBR()
{
    return WBR;
}


hash_str_t html_tag_input()
{
    return input;
}


hash_str_t html_tag_INPUT()
{
    return INPUT;
}


hash_str_t html_tag_param()
{
    return param;
}


hash_str_t html_tag_PARAM()
{
    return PARAM;
}


hash_str_t html_tag_source()
{
    return source;
}


hash_str_t html_tag_SOURCE()
{
    return SOURCE;
}


hash_str_t html_tag_track()
{
    return track;
}


hash_str_t html_tag_TRACK()
{
    return TRACK;
}


hash_str_t html_tag_hr()
{
    return hr;
}


hash_str_t html_tag_HR()
{
    return HR;
}


hash_str_t html_tag_image()
{
    return image;
}


hash_str_t html_tag_IMAGE()
{
    return IMAGE;
}


hash_str_t html_tag_textarea()
{
    return textarea;
}


hash_str_t html_tag_TEXTAREA()
{
    return TEXTAREA;
}


hash_str_t html_tag_xmp()
{
    return xmp;
}


hash_str_t html_tag_XMP()
{
    return XMP;
}


hash_str_t html_tag_iframe()
{
    return iframe;
}


hash_str_t html_tag_IFRAME()
{
    return IFRAME;
}


hash_str_t html_tag_noembed()
{
    return noembed;
}


hash_str_t html_tag_NOEMBED()
{
    return NOEMBED;
}


hash_str_t html_tag_select()
{
    return select;
}


hash_str_t html_tag_SELECT()
{
    return SELECT;
}


hash_str_t html_tag_col()
{
    return col;
}


hash_str_t html_tag_COL()
{
    return COL;
}


hash_str_t html_tag_colgroup()
{
    return colgroup;
}


hash_str_t html_tag_COLGROUP()
{
    return COLGROUP;
}


hash_str_t html_tag_caption()
{
    return caption;
}


hash_str_t html_tag_CAPTION()
{
    return CAPTION;
}


hash_str_t html_tag_span()
{
    return span;
}


hash_str_t html_tag_SPAN()
{
    return SPAN;
}


hash_str_t html_tag_cite()
{
    return cite;
}


hash_str_t html_tag_CITE()
{
    return CITE;
}
