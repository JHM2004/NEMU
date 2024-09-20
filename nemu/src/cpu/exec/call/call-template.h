#include "cpu/exec/template-start.h"

#define instr call

// 指令：CALL——r32/r16
make_helper(concat(call_i_, SUFFIX))
{
    int len = concat(decode_i_, SUFFIX)(cpu.eip + 1); // 指令解码，计算指令数字字节长度并返回
    reg_l(R_ESP) -= DATA_BYTE;                        // esp申请栈帧空间
    swaddr_write(reg_l(R_ESP), 4, cpu.eip + len + 1); // 记返回地址入栈，目标是call后面下一条指令
    DATA_TYPE_S imm = op_src->val;                    // 立即数读取
    print_asm("call\t%x", cpu.eip + 1 + len + imm);   // 输出汇编语言
    cpu.eip += imm;                                   // 控制指针eip跳转
    return len + 1;                                   // 返回整条call指令的长度
}

make_helper(concat(call_rm_, SUFFIX))
{
    int len = concat(decode_rm_, SUFFIX)(cpu.eip + 1);
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP), 4, cpu.eip + len + 1);
    DATA_TYPE_S imm = op_src->val;
    print_asm("call\t%x", imm);
    cpu.eip = imm - len - 1;
    return len + 1;
}
//暂留悬念：实现完整的call指令集（结课后玩一玩）
#include "cpu/exec/template-end.h"