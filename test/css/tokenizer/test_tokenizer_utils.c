#include "test_tokenizer_utils.h"

#include "test_utils.h"
#include "css/tokenizer/types.h"

void ASSERT_CSS_TOKEN(css_token_t* a, css_token_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    ASSERT_EQUAL(a->type, e->type);
    ASSERT_EQUAL(a->data_size, e->data_size);
    if (a->data_size == e->data_size)
    {
        ASSERT_STRING((char)a->data, (char)e->data, a->data_size);
    }
    ASSERT_EQUAL(a->integer, e->integer);
    ASSERT_EQUAL(a->real, e->real);
    ASSERT_EQUAL(a->hash_type, e->hash_type);
}