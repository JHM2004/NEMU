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

#define mv(name) make_helper_v(name)

mv(ja);
mv(jae);
mv(jb);
mv(jbe);
mv(jc);
mv(je);
mv(jz);
mv(jg);
mv(jge);
mv(jl);
mv(jle);
mv(jna);
mv(jnae);
mv(jnb);
mv(jnbe);
mv(jnc);
mv(jne);
mv(jng);
mv(jnge);
mv(jnl);
mv(jnle);
mv(jno);
mv(jnp);
mv(jns);
mv(jnz);
mv(jo);
mv(jp);
mv(jpe);
mv(jpo);
mv(js);
