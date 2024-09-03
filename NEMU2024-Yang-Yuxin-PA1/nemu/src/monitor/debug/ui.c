#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
void insert_wp(char *args);
void print_watchpoints();
void delete_wp(int n);
void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char *rl_gets()
{
	static char *line_read = NULL;

	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read)
	{
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args)
{
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args)
{
	return -1;
}
static int cmd_p(char *args)
{
	bool success = true;
	int i;

	i = expr(args, &success);
	if (success)
	{
		printf("%d\n", i);
	}
	return 0;
}
static int cmd_help(char *args);

static int cmd_si(char *args)
{

	char *arg = strtok(args, " ");
	int num = 1;
	// printf("%s\n",arg);
	if (arg == NULL)
	{
		printf("No things ,system return 1.\n");
	}
	else
		num = atoi(arg);
	// printf("%d",num);
	cpu_exec(num);
	printf("\n");
	return 0;
};

static int cmd_info(char *args)
{
	char *arg = strtok(NULL, " ");
	// printf("%s\n", arg);
	//  cpu info
	if (strcmp(arg, "r") == 0)
	{
		printf("eax  0x%08x  %d\n", cpu.eax, cpu.eax);
		printf("ecx  0x%08x  %d\n", cpu.ecx, cpu.ecx);
		printf("edx  0x%08x  %d\n", cpu.edx, cpu.edx);
		printf("ebx  0x%08x  %d\n", cpu.ebx, cpu.ebx);
		printf("esp  0x%08x  %d\n", cpu.esp, cpu.esp);
		printf("ebp  0x%08x  %d\n", cpu.ebp, cpu.ebp);
		printf("esi  0x%08x  %d\n", cpu.esi, cpu.esi);
		printf("edi  0x%08x  %d\n", cpu.edi, cpu.edi);
		printf("eip  0x%08x  %d\n", cpu.eip, cpu.eip);
		printf("\n");
	}
	else if (strcmp(arg, "w") == 0)
	{
		print_watchpoints();
	}

	return 0;
}
static int cmd_x(char *args)
{
	char *arg = strtok(NULL, " ");
	// printf("%s\n", arg);
	if (arg == NULL)
	{
		printf("you need to putin a num");
		return 1;
	}
	int n = atoi(arg);
	// printf("%d\n",n);
	char *arg2 = strtok(NULL, "\0");
	int addr;
	if (!arg2)
	{
		printf("you need to putin a correct memplace\n");
		return 1;
	}
	else if (!isxdigit(arg2[0]))
	{
		bool suc = true;
		uint32_t result = expr(arg2, &suc);
		if (suc)
		{
			addr = result;
		}
		else
		{
			printf("caculate ERROR\n");
			return 1;
		}
	}
	// printf("%s\n", arg2);
	else
		sscanf(arg2, "%x", &addr);
	int i = 0;
	for (; i < n; i++)
	{
		if (i % 4 == 0)
			printf("%06x:", addr + i * 4);
		printf("0x%08x ", swaddr_read(addr + 4 * i, 4));
		if (i % 4 == 3)
			printf("\n");
	}
	if(i%4!=0)printf("\n");
	printf("\n");
	return 0;
}

static int cmd_w(char *args)
{
	insert_wp(args);
	printf("\n");
	return 0;
}
static int cmd_d(char *args)
{
	int i;
	sscanf(args, "%d", &i);
	delete_wp(i);
	printf("\n");
	return 0;
}
static struct
{
	char *name;
	char *description;
	int (*handler)(char *);
} cmd_table[] = {
	{"help", "Display informations about all supported commands", cmd_help},
	{"c", "Continue the execution of the program", cmd_c},
	{"q", "Exit NEMU", cmd_q},
	{"si", "Let the program execute N instructions in a single step and then suspend execution", cmd_si},
	{"info", "print the Register", cmd_info},
	{"x", "scan the memory", cmd_x},
	{"p", "caculate the express", cmd_p},
	{"w", "set the watchpoints", cmd_w},
	{"d", "delete the watchpoints", cmd_d},
	/* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL)
	{
		/* no argument given */
		for (i = 0; i < NR_CMD; i++)
		{
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else
	{
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(arg, cmd_table[i].name) == 0)
			{
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	printf("\n");
	return 0;
}

void ui_mainloop()
{
	while (1)
	{
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == NULL)
		{
			continue;
		}

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end)
		{
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(cmd, cmd_table[i].name) == 0)
			{
				if (cmd_table[i].handler(args) < 0)
				{
					return;
				}
				break;
			}
		}

		if (i == NR_CMD)
		{
			printf("Unknown command '%s'\n", cmd);
		}
	}
}
