#include "svg_attr_map.h"

#include <string.h>

#define SIZE 58

static hash_str_t keys[SIZE];
static hash_str_t values[SIZE];

void svg_attr_map_init()
{
    memset(keys, 0, sizeof(hash_str_t) * SIZE);
    memset(values, 0, sizeof(hash_str_t) * SIZE);

    keys[0]     = hash_str_new("attributename", 13);
    values[0]   = hash_str_new("attributeName", 13);
    keys[1]     = hash_str_new("attributetype", 13);
    values[1]   = hash_str_new("attributeType", 13);
    keys[2]     = hash_str_new("basefrequency", 13);
    values[2]   = hash_str_new("baseFrequency", 13);
    keys[3]     = hash_str_new("baseprofile", 11);
    values[3]   = hash_str_new("baseProfile", 11);
    keys[4]     = hash_str_new("calcmode", 8);
    values[4]   = hash_str_new("calcMode", 8);
    keys[5]     = hash_str_new("clippathunits", 13);
    values[5]   = hash_str_new("clipPathUnits", 13);
    keys[6]     = hash_str_new("diffuseconstant", 15);
    values[6]   = hash_str_new("diffuseConstant", 15);
    keys[7]     = hash_str_new("edgemode", 8);
    values[7]   = hash_str_new("edgeMode", 8);
    keys[8]     = hash_str_new("filterunits", 11);
    values[8]   = hash_str_new("filterUnits", 11);
    keys[9]     = hash_str_new("glyphref", 8);
    values[9]   = hash_str_new("glyphRef", 8);
    keys[10]    = hash_str_new("gradienttransform", 17);
    values[10]  = hash_str_new("gradientTransform", 17);
    keys[11]    = hash_str_new("gradientunits", 13);
    values[11]  = hash_str_new("gradientUnits", 13);
    keys[12]    = hash_str_new("kernelmatrix", 12);
    values[12]  = hash_str_new("kernelMatrix", 12);
    keys[13]    = hash_str_new("kernelunitlength", 16);
    values[13]  = hash_str_new("kernelUnitLength", 16);
    keys[14]    = hash_str_new("keypoints", 9);
    values[14]  = hash_str_new("keyPoints", 9);
    keys[15]    = hash_str_new("keysplines", 10);
    values[15]  = hash_str_new("keySplines", 10);
    keys[16]    = hash_str_new("keytimes", 8);
    values[16]  = hash_str_new("keyTimes", 8);
    keys[17]    = hash_str_new("lengthadjust", 12);
    values[17]  = hash_str_new("lengthAdjust", 12);
    keys[18]    = hash_str_new("limitingconeangle", 17);
    values[18]  = hash_str_new("limitingConeAngle", 17);
    keys[19]    = hash_str_new("markerheight", 12);
    values[19]  = hash_str_new("markerHeight", 12);
    keys[20]    = hash_str_new("markerunits", 11);
    values[20]  = hash_str_new("markerUnits", 11);
    keys[21]    = hash_str_new("markerwidth", 11);
    values[21]  = hash_str_new("markerWidth", 11);
    keys[22]    = hash_str_new("maskcontentunits", 16);
    values[22]  = hash_str_new("maskContentUnits", 16);
    keys[23]    = hash_str_new("maskunits", 9);
    values[23]  = hash_str_new("maskUnits", 9);
    keys[24]    = hash_str_new("numoctaves", 10);
    values[24]  = hash_str_new("numOctaves", 10);
    keys[25]    = hash_str_new("pathlength", 10);
    values[25]  = hash_str_new("pathLength", 10);
    keys[26]    = hash_str_new("patterncontentunits", 19);
    values[26]  = hash_str_new("patternContentUnits", 19);
    keys[27]    = hash_str_new("patterntransform", 16);
    values[27]  = hash_str_new("patternTransform", 16);
    keys[28]    = hash_str_new("patternunits", 12);
    values[28]  = hash_str_new("patternUnits", 12);
    keys[29]    = hash_str_new("pointsatx", 9);
    values[29]  = hash_str_new("pointsAtX", 9);
    keys[30]    = hash_str_new("pointsaty", 9);
    values[30]  = hash_str_new("pointsAtY", 9);
    keys[31]    = hash_str_new("pointsatz", 9);
    values[31]  = hash_str_new("pointsAtZ", 9);
    keys[32]    = hash_str_new("preservealpha", 13);
    values[32]  = hash_str_new("preserveAlpha", 13);
    keys[33]    = hash_str_new("preserveaspectratio", 19);
    values[33]  = hash_str_new("preserveAspectRatio", 19);
    keys[34]    = hash_str_new("primitiveunits", 14);
    values[34]  = hash_str_new("primitiveUnits", 14);
    keys[35]    = hash_str_new("refx", 4);
    values[35]  = hash_str_new("refX", 4);
    keys[36]    = hash_str_new("refy", 4);
    values[36]  = hash_str_new("refY", 4);
    keys[37]    = hash_str_new("repeatcount", 11);
    values[37]  = hash_str_new("repeatCount", 11);
    keys[38]    = hash_str_new("repeatdur", 9);
    values[38]  = hash_str_new("repeatDur", 9);
    keys[39]    = hash_str_new("requiredextensions", 18);
    values[39]  = hash_str_new("requiredExtensions", 18);
    keys[40]    = hash_str_new("requiredfeatures", 16);
    values[40]  = hash_str_new("requiredFeatures", 16);
    keys[41]    = hash_str_new("specularconstant", 16);
    values[41]  = hash_str_new("specularConstant", 16);
    keys[42]    = hash_str_new("specularexponent", 16);
    values[42]  = hash_str_new("specularExponent", 16);
    keys[43]    = hash_str_new("spreadmethod", 12);
    values[43]  = hash_str_new("spreadMethod", 12);
    keys[44]    = hash_str_new("startoffset", 11);
    values[44]  = hash_str_new("startOffset", 11);
    keys[45]    = hash_str_new("stddeviation", 12);
    values[45]  = hash_str_new("stdDeviation", 12);
    keys[46]    = hash_str_new("stitchtiles", 11);
    values[46]  = hash_str_new("stitchTiles", 11);
    keys[47]    = hash_str_new("surfacescale", 12);
    values[47]  = hash_str_new("surfaceScale", 12);
    keys[48]    = hash_str_new("systemlanguage", 14);
    values[48]  = hash_str_new("systemLanguage", 14);
    keys[49]    = hash_str_new("tablevalues", 11);
    values[49]  = hash_str_new("tableValues", 11);
    keys[50]    = hash_str_new("targetx", 7);
    values[50]  = hash_str_new("targetX", 7);
    keys[51]    = hash_str_new("targety", 7);
    values[51]  = hash_str_new("targetY", 7);
    keys[52]    = hash_str_new("textlength", 10);
    values[52]  = hash_str_new("textLength", 10);
    keys[53]    = hash_str_new("viewbox", 7);
    values[53]  = hash_str_new("viewBox", 7);
    keys[54]    = hash_str_new("viewtarget", 10);
    values[54]  = hash_str_new("viewTarget", 10);
    keys[55]    = hash_str_new("xchannelselector", 16);
    values[55]  = hash_str_new("xChannelSelector", 16);
    keys[56]    = hash_str_new("ychannelselector", 16);
    values[56]  = hash_str_new("yChannelSelector", 16);
    keys[57]    = hash_str_new("zoomandpan", 10);
    values[57]  = hash_str_new("zoomAndPan", 10);
}


hash_str_t svg_attr_map_get(hash_str_t key)
{
    for (uint32_t i = 0; i < SIZE; i++)
    {
        if (keys[i] == key) { return values[i]; }
    }

    return key;
}
