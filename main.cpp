#include "has_arrow_operator.h"
#include "string_convert.h"
#include "call_private_func.h"
#include "is_a.h"
#include "filler.h"
#include "append.h"
#include "has_property.h"

int main(int argc, const char * argv[]) {
    test_has_arrow_operator();
    test_string_convert();
    test_call_private_func();
    test_is_a();
    test_filler();
    test_append();
    test_has_property();
    
    return 0;
}
