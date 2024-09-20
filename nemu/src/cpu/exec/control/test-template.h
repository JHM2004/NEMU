#include "cpu/exec/template-start.h"

#define instr test
//指令综合：386手册
static void do_execute () {
	DATA_TYPE DEST = op_dest -> val & op_src -> val;
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    update_eflags_pf_zf_sf(DEST);
    print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"