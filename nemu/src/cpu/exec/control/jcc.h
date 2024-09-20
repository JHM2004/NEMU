#ifndef _JCC_H_
#define _JCC_H_

#define m(name) make_helper(name ## _b);\
               make_helper(name ## _v)

m(ja);
m(jae);
m(jb);
m(jbe);
m(jc);
m(je);
m(jz);
m(jg);
m(jge);
m(jl);
m(jle);
m(jna);
m(jnae);
m(jnb);
m(jnbe);
m(jnc);
m(jne);
m(jng);
m(jnge);
m(jnl);
m(jnle);
m(jno);
m(jnp);
m(jns);
m(jnz);
m(jo);
m(jp);
m(jpe);
m(jpo);
m(js);
#undef m
make_helper(jcxz_b);
make_helper(jecxz_b);



#endif