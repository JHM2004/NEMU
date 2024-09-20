#ifndef __SETCC_H__
#define __SETCC_H__

#define m(name) make_helper(name ## _rm_b)

m(seta);
m(setae);
m(setb);
m(setbe);
m(setc);
m(sete);
m(setg);
m(setge);
m(setl);
m(setle);
m(setna);
m(setnae);
m(setnb);
m(setnbe);
m(setnc);
m(setne);
m(setng);
m(setnge);
m(setnl);
m(setnle);
m(setno);
m(setnp);
m(setnz);
m(setns);
m(seto);
m(setp);
m(setpe);
m(setpo);
m(sets);
m(setz);
#undef m
#endif