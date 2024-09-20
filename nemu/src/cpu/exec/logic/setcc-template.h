#include "cpu/exec/template-start.h"

#define if_exec() OPERAND_W(op_src, 1);

#define else_exec() OPERAND_W(op_src, 0)

#define finalexec()                     \
	static void do_execute()            \
	{                                   \
		if (SETFLAG)                    \
			if_exec() else else_exec(); \
		print_asm_template1();          \
	}                                   \
	make_instr_helper(rm)

#define instr seta
#define SETFLAG cpu.eflags.CF == 0 && cpu.eflags.ZF == 0
finalexec();
#undef SETFLAG
#undef instr

#define instr setae
#define SETFLAG cpu.eflags.CF == 0
finalexec();
#undef SETFLAG
#undef instr

#define instr setb
#define SETFLAG cpu.eflags.CF == 1
finalexec();
#undef SETFLAG
#undef instr

#define instr sete
#define SETFLAG cpu.eflags.ZF == 1
finalexec();
#undef SETFLAG
#undef instr

#define instr setbe
#define SETFLAG cpu.eflags.ZF == 1 || cpu.eflags.CF == 1
finalexec();
#undef SETFLAG
#undef instr

#define instr setl
#define SETFLAG cpu.eflags.SF != cpu.eflags.OF
finalexec();
#undef SETFLAG
#undef instr

#define instr setle
#define SETFLAG cpu.eflags.ZF == 1 || cpu.eflags.SF != cpu.eflags.OF
finalexec();
#undef SETFLAG
#undef instr

#define instr setg
#define SETFLAG cpu.eflags.ZF == 0 && cpu.eflags.SF == cpu.eflags.OF
finalexec();
#undef SETFLAG
#undef instr

#define instr setge
#define SETFLAG cpu.eflags.SF == cpu.eflags.OF
finalexec();
#undef SETFLAG
#undef instr

#define instr setne
#define SETFLAG cpu.eflags.ZF == 0
finalexec();
#undef SETFLAG
#undef instr

#define instr setns
#define SETFLAG cpu.eflags.SF == 0
finalexec();
#undef SETFLAG
#undef instr

#define instr sets
#define SETFLAG cpu.eflags.SF == 1
finalexec();
#undef SETFLAG
#undef instr

#include "cpu/exec/template-end.h"