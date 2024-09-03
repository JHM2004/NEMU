#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	char expr[64];//表达式
	bool suc;
	uint32_t old_values;
	struct watchpoint *next;
	/* TODO: Add more members if necessary */
} WP;

#endif
