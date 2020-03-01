#include "has_arrow_operator.h"
#include "string_convert.h"
#include "call_private_func.h"
#include "is_a.h"
#include "filler.h"
#include "append.h"
#include "has_property.h"
#include "either_is_int.h"
#include "transform.h"
#include "all_true.h"
#include "index_sequence.h"
#include "tuple_cat.h"
#include "which_base.h"

int main(int argc, const char * argv[]) {
    test_has_arrow_operator();
    test_string_convert();
    test_call_private_func();
    test_is_a();
    test_filler();
    test_append();
    test_has_property();
    test_either_is_int();
    test_transform();
    test_all_true();
    test_index_sequence();
    test_tuple_cat();
    test_which_base();
    
    return 0;
}
