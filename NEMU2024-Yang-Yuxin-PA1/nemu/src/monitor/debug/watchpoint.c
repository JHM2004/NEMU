#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "cpu/reg.h"
#define NR_WP 64

static WP wp_pool[NR_WP];
static WP *head, *free_;
static uint32_t size_s = 0;
bool check_wp_isempty()
{
	return !size_s;
}
uint32_t size()
{
	return size_s;
}
void init_wp_pool()
{
	int i;
	for (i = 0; i < NR_WP; i++)
	{
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

static WP *new_wp()
{
	if (!free_)
		assert(0);
	WP *p = free_;
	free_ = free_->next;
	p->suc = true;
	p->next=NULL;
	return p;
}
static void free_wp(WP *wp)
{
	WP *p = head;
	while (p && p->next && p != wp && p->next != wp)
		p = p->next;
	if (!p || !p->next)
	{
		panic("NULL Point ERROR at free_wp function");
	}
	p->next = wp->next;
	/*clear things of wp*/;
	wp->next = free_;
	int i=0;
	for(;i<64;i++)wp->expr[i]=0;
	free_ = wp;
}
void insert_wp(char *args)
{
	WP *p = new_wp();
	size_s++;
	p->NO = size_s;
	strcpy(p->expr,args);
	p->old_values = expr(p->expr, &p->suc);
	if (!p->suc)
		assert(0);
	if(!head)head=p;
	else
	{
		WP *tail=head;
		while(tail->next)
		{
			tail=tail->next;
		}
		tail->next=p;
	}
	printf("Watchpoint%d : %s was setted correctly\n", p->NO, p->expr);
	return;
}
void print_watchpoints()
{
	WP *p = head;
	if(!p)printf("No watchpoints\n");
	while (p)
	{
		printf("Watchpoint%d : %s \n", p->NO,p->expr);
		p = p->next;
	}
	return;
}
void delete_wp(int n)
{
	WP *p = head;
	while (p)
	{
		if (p->NO == n)
		{
			free_wp(p);
			size_s--;
			break;
		}
		p = p->next;
	}
	p = head;
	head->NO = 1;
	while (p && p->next)
	{
		p->next->NO = p->NO + 1;
		p = p->next;
	}
	return;
}
bool check(WP *p)
{
	uint32_t val = expr(p->expr, &p->suc);
	if (!p->suc)
		assert(0);
	if (val != p->old_values)
	{
		printf ("Hint watchpoint %d at address 0x%08x\n", p->NO, cpu.eip);
		printf ("Watchpoint %d: %s\n",p->NO,p->expr);
		printf ("Old value = %d\n",p->old_values);
		printf ("New value = %d\n",val);
		p->old_values = val;
		return true;
	}
	return false;
}
bool finalcheck_()
{
	bool tag=false;
	WP *p = head;
	while (p)
	{
		if (check(p))tag=true;
		p = p->next;
	}
	return tag;
}