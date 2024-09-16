#include "cpu/exec/template-start.h"
#define instr push
// 指令PUSH 50+n,push_r_v
static void do_execute()
{
    if (DATA_BYTE == 2)
    {
        reg_l(R_ESP) -= 2;
        swaddr_write(reg_l(R_ESP), 2, (DATA_TYPE)op_src->val);
    }
    else
    {
        if (DATA_BYTE == 1)//有tmd一个ret 8imm的sb得给他留出接口
            op_src->val = (int8_t)op_src->val;
        reg_l(R_ESP) -= 4;
        swaddr_write(reg_l(R_ESP), 4, op_src->val);
    }

    print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
