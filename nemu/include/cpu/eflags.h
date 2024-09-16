#ifndef __EFLAGS_H__
#define __EFLAGS_H__

#include "common.h"

void update_eflags_pf_zf_sf(uint32_t);

#define mc(ccc) static inline bool concat(check_cc_, ccc)()

mc(a)
{
	return !(cpu.eflags.CF | cpu.eflags.ZF);
}

mc(ae)
{
	return !cpu.eflags.CF;
}

mc(b)
{
	return cpu.eflags.CF;
}

mc(be)
{
	return cpu.eflags.CF | cpu.eflags.ZF;
}

mc(c)
{
	return cpu.eflags.CF;
}

mc(cxz)
{
	return !cpu.gpr[2]._16;
}

mc(ecxz)
{
	return !cpu.ecx;
}

mc(e)
{
	return cpu.eflags.ZF;
}

mc(z) { return cpu.eflags.ZF; }

mc(g)
{
	return !(cpu.eflags.ZF | (cpu.eflags.SF ^ cpu.eflags.OF));
}

mc(ge)
{
	return cpu.eflags.ZF | !(cpu.eflags.SF ^ cpu.eflags.OF);
}

mc(l)
{
	return cpu.eflags.SF ^ cpu.eflags.OF;
}

mc(le)
{
	return (cpu.eflags.SF ^ cpu.eflags.OF) | cpu.eflags.ZF;
}

mc(na)
{
	return cpu.eflags.CF | cpu.eflags.ZF;
}

mc(nae)
{
	return cpu.eflags.CF;
}

mc(nb)
{
	return !cpu.eflags.CF;
}

mc(nbe)
{
	return !cpu.eflags.CF & !cpu.eflags.ZF;
}

mc(nc)
{
	return !cpu.eflags.CF;
}

mc(ne)
{
	return !cpu.eflags.ZF;
}

mc(ng)
{
	return cpu.eflags.ZF | (cpu.eflags.SF ^ cpu.eflags.OF);
}

mc(nge)
{
	return !(cpu.eflags.ZF) && (cpu.eflags.SF ^ cpu.eflags.OF);
}

mc(nl)
{
	return cpu.eflags.ZF | !(cpu.eflags.SF ^ cpu.eflags.OF);
}

mc(nle)
{
	return !cpu.eflags.ZF & !(cpu.eflags.SF ^ cpu.eflags.OF);
}

mc(no)
{
	return !cpu.eflags.OF;
}

mc(np)
{
	return !cpu.eflags.PF;
}

mc(ns)
{
	return !cpu.eflags.SF;
}

mc(nz)
{
	return !cpu.eflags.ZF;
}

mc(o)
{
	return cpu.eflags.OF;
}

mc(p)
{
	return cpu.eflags.PF;
}

mc(pe)
{
	return cpu.eflags.PF;
}

mc(po)
{
	return !cpu.eflags.PF;
}

mc(s)
{
	return cpu.eflags.SF;
}

#endif
