#pragma once

#define __BIT(n) ((uint64_t)1 << (n))

// https://dom.spec.whatwg.org/#interface-node
#define DOM_NODE_INVALID                __BIT(1)
#define DOM_NODE_ELEMENT                __BIT(2)
#define DOM_NODE_ATTRIBUTE              __BIT(3)
#define DOM_NODE_TEXT                   __BIT(4)
#define DOM_NODE_CDATA_SECTION          __BIT(5)
#define DOM_NODE_ENTITY_REFERENCE       __BIT(6)
#define DOM_NODE_ENTITY                 __BIT(7)
#define DOM_NODE_PROCESSING_INSTRUCTION __BIT(8)
#define DOM_NODE_COMMENT                __BIT(9)
#define DOM_NODE_DOCTYPE                __BIT(10)
#define DOM_NODE_DOCUMENT               __BIT(11)
#define DOM_NODE_DOCUMENT_TYPE          __BIT(12)
#define DOM_NODE_DOCUMENT_FRAGMENT      __BIT(13)
#define DOM_NODE_NOTATION               __BIT(14)
#define HTML_NODE_ELEMENT               __BIT(15)
#define HTML_NODE_SELECT                __BIT(16)
#define HTML_NODE_OPTION                __BIT(17)
