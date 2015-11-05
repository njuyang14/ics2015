#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */


make_helper_v(jcc_jbe)
make_helper_v(jcc_jne)
make_helper_v(jcc_jle)
make_helper_v(jcc_jg)
make_helper_v(jcc_jge)
make_helper_v(jcc_jl)
make_helper_v(jcc_ja)
make_helper_v(jcc_js)
make_helper_v(jcc_jae)
make_helper_v(jcc_jns)
//make_helper_v(jcc_jo)
//make_helper_v(jcc_jno)
//make_helper_v(jcc_j)
//make_helper_v(jcc_jae)
//make_helper_v(jcc_jae)
//make_helper_v(jcc_jae)




