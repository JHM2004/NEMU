#include "cpu/exec/template-start.h"
// 指令——RET（C3)
make_helper(concat(ret_nothing_, SUFFIX))
{
   cpu.eip = MEM_R(reg_l(R_ESP)) - 1; // 配重抵消
   reg_l(R_ESP) += DATA_BYTE;
   print_asm("ret");
   return 1;
}
//指令——RET(C2)
make_helper(concat(ret_i_, SUFFIX))
{
   int val = instr_fetch(eip + 1, DATA_BYTE);
   cpu.eip = MEM_R(REG(R_ESP));
   if (DATA_BYTE == 2)
      cpu.eip &= 0xffff;
   REG(R_ESP) += DATA_BYTE;
   cpu.eip--;// 配重抵消
   int i;
   for (i = 0; i < val; i += DATA_BYTE)
      MEM_W(REG(R_ESP) + i, 0);

   REG(R_ESP) += val;
   print_asm("ret\t$0x%x", val);
   return 1;
}

#include "cpu/exec/template-end.h"