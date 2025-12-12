#pragma once

#define __BIT(n) ((uint64_t)1 << (n))

#define CSS_RULE_STYLE                  __BIT(1)
#define CSS_RULE_IMPORT                 __BIT(2)
#define CSS_RULE_MEDIA                  __BIT(3)
#define CSS_RULE_FONT_FACE              __BIT(4)
#define CSS_RULE_PAGE                   __BIT(5)
#define CSS_RULE_MARGIN                 __BIT(6)
#define CSS_RULE_NAMESPACE              __BIT(7)
#define CSS_RULE_KEYFRAMES              __BIT(8)
#define CSS_RULE_KEYFRAME               __BIT(9)
#define CSS_RULE_COUNTER_STYLE          __BIT(10)
#define CSS_RULE_SUPPORTS               __BIT(11)
#define CSS_RULE_FONT_FEATURE_VALUES    __BIT(12)
#define CSS_RULE_GROUP                  __BIT(13)

