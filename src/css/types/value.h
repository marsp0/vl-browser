#pragma once

#include <stdint.h>

typedef enum css_value_type_unit_e
{
    CSS_VALUE_TYPE_UNIT_NONE,
    CSS_VALUE_TYPE_UNIT_EM,
    CSS_VALUE_TYPE_UNIT_EX,
    CSS_VALUE_TYPE_UNIT_CAP,
    CSS_VALUE_TYPE_UNIT_CH,
    CSS_VALUE_TYPE_UNIT_IC,
    CSS_VALUE_TYPE_UNIT_REM,
    CSS_VALUE_TYPE_UNIT_LH,
    CSS_VALUE_TYPE_UNIT_RLH,
    CSS_VALUE_TYPE_UNIT_VW,
    CSS_VALUE_TYPE_UNIT_VH,
    CSS_VALUE_TYPE_UNIT_VI,
    CSS_VALUE_TYPE_UNIT_VB,
    CSS_VALUE_TYPE_UNIT_VMIN,
    CSS_VALUE_TYPE_UNIT_VMAX,
    CSS_VALUE_TYPE_UNIT_CM,
    CSS_VALUE_TYPE_UNIT_MM,
    CSS_VALUE_TYPE_UNIT_Q,
    CSS_VALUE_TYPE_UNIT_IN,
    CSS_VALUE_TYPE_UNIT_PC,
    CSS_VALUE_TYPE_UNIT_PT,
    CSS_VALUE_TYPE_UNIT_PX,
    CSS_VALUE_TYPE_UNIT_DEG,
    CSS_VALUE_TYPE_UNIT_RAD,
    CSS_VALUE_TYPE_UNIT_GRAD,
    CSS_VALUE_TYPE_UNIT_TURN,
    CSS_VALUE_TYPE_UNIT_S,
    CSS_VALUE_TYPE_UNIT_MS,
    CSS_VALUE_TYPE_UNIT_HZ,
    CSS_VALUE_TYPE_UNIT_KHZ,
    CSS_VALUE_TYPE_UNIT_DPI,
    CSS_VALUE_TYPE_UNIT_DPCM,
    CSS_VALUE_TYPE_UNIT_DPPX,
} css_value_type_unit_e;


typedef enum css_value_type_e
{
    CSS_VALUE_TYPE_INITIAL,
    CSS_VALUE_TYPE_INHERIT,
    CSS_VALUE_TYPE_UNSET,
    CSS_VALUE_TYPE_INTEGER,
    CSS_VALUE_TYPE_NUMBER,
    CSS_VALUE_TYPE_PERCENTAGE,
    CSS_VALUE_TYPE_LENGTH,
    CSS_VALUE_TYPE_ANGLE,
    CSS_VALUE_TYPE_TIME,
    CSS_VALUE_TYPE_FREQ,
    CSS_VALUE_TYPE_RESOLUTION,
    CSS_VALUE_TYPE_COLOR,
    CSS_VALUE_TYPE_IMAGE
} css_value_type_e;

typedef struct css_value_t
{
    css_value_type_e        type;
    css_value_type_unit_e   unit;

    union
    {
        uint32_t color;
        float real;
    };

} css_value_t;

css_value_t*    css_value_new(css_value_type_e type, css_value_type_unit_e unit);
void            css_value_free(css_value_t* val);
