#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	REG,
	HEXN,
	NOTEQUAL,
	AND,
	OR,
	NOT,
	POINT,
	NEG
	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE},			// spaces
	{"\\+", '+'},			// plus
	{"==", EQ},				// equal
	{"0x[0-9,a-f]+", HEXN}, // hexnum
	{"[0-9]+", NUM},		// 10 num
	{"\\$[a-z]{2,3}", REG}, // reg
	{"\\(", '('},
	{"\\)", ')'},
	{"-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"!=", NOTEQUAL},
	{"&&", AND},
	{"\\|\\|", OR},
	{"!", '!'},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX]; // 存放编译后的正则表达式的空间

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;
	// printf("%s\n", e);
	// printf("%c", e[position]);
	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				// char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				int j;
				for (j = 0; j < 32; j++)
				{ 
					tokens[nr_token].str[j] = '\0';//致命清空问题，会导致严重的判断错误而计算出错
				}
				if (substr_len < 32)
					switch (rules[i].token_type)
					{
					case NUM:
						tokens[nr_token].type = NUM;
						strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
						nr_token++;
						break;
					case REG:
						tokens[nr_token].type = REG;
						strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
						nr_token++;
						break;
					case HEXN:
						tokens[nr_token].type = HEXN;
						strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
						nr_token++;
						break;
					case '+':
						tokens[nr_token++].type = '+';
						break;
					case '-':
						tokens[nr_token++].type = '-';
						break;
					case '*':
						tokens[nr_token++].type = '*';
						break;
					case '/':
						tokens[nr_token++].type = '/';
						break;
					case '(':
						tokens[nr_token++].type = '(';
						break;
					case ')':
						tokens[nr_token++].type = ')';
						break;
					case AND:
						tokens[nr_token].type = AND;
						strcpy(tokens[nr_token].str, "&&");
						nr_token++;
						break;
					case OR:
						tokens[nr_token].type = OR;
						strcpy(tokens[nr_token].str, "||");
						nr_token++;
						break;
					case EQ:
						tokens[nr_token].type = EQ;
						strcpy(tokens[nr_token].str, "==");
						nr_token++;
						break;
					case NOTEQUAL:
						tokens[nr_token].type = NOTEQUAL;
						strcpy(tokens[nr_token].str, "!=");
						nr_token++;
						break;
					case '!':
						tokens[nr_token].type = '!';
						nr_token++;
						break;
					case NOTYPE:
						break;
					default:
						assert(0);
					}
				else
				{
					printf("Buffer Overflow,system wrong!");
					return false;
				}
				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}
bool check_parenthese(int p, int q)
{
	int l = 0, r = 0;
	if (tokens[p].type == '(' && tokens[q].type == ')')
	{
		int i = p;
		for (i = p; i <= q; i++)
		{
			if (tokens[i].type == '(')
				l++;
			if (tokens[i].type == ')')
				r++;
			if (i != q && l == r)
				return false;
		}
		if (l == r)
		{
			return true;
		}
		else
			return false;
	}
	return false;
}
uint32_t dominate_operator(int p, int q)
{
	int result = -1;
	int pre = 100;
	int i = p;
	int tokenflag = 0;
	for (i = p; i <= q; i++)
	{
		if (tokens[i].type == '(')
			tokenflag++;
		else if (tokens[i].type == ')')
			tokenflag--;
		else if (tokens[i].type == '*' || tokens[i].type == '/')
		{
			if (pre >= 5 && !tokenflag)
				result = i, pre = 5;
		}
		else if (tokens[i].type == '+' || tokens[i].type == '-')
		{
			if (pre >= 4 && !tokenflag)
				result = i, pre = 4;
		}
		else if (tokens[i].type == EQ || tokens[i].type == NOTEQUAL)
		{
			if (pre >= 3 && !tokenflag)
				result = i, pre = 3;
		}
		else if (tokens[i].type == AND)
		{
			if (pre >= 2 && !tokenflag)
				result = i, pre = 2;
		}
		else if (tokens[i].type == OR)
		{
			if (pre >= 1 && !tokenflag)
				result = i, pre = 1;
		}
		else
			continue;
	}
	if (tokenflag)
		result = -2;
	return result;
}
uint32_t eval(int p, int q)
{
	if (p > q)
	{
		/*Bad expression*/
		printf("Wrong caculate,system failed");
		assert(0);
	}
	else if (p == q)
	{
		/*Single token
		For now this token should be a number
		return the value of the number*/
		// check for the 10 or hex or reg
		//printf("now at position %d\n", p);
		int result = 0;
		if (tokens[p].type == NUM)
		{
			sscanf(tokens[p].str, "%d", &result);
			//printf("%d\n", result);
			return result;
		}
		else if (tokens[p].type == HEXN) // hex num 0x123456
		{
			int i = 2;
			for (i = 2; tokens[p].str[i] != 0; i++)
			{
				result *= 16;
				result += tokens[p].str[i] <= '9' ? tokens[p].str[i] - '0' : tokens[p].str[i] - 'a' + 10;
			}
			// printf("%d\n", result);
			return result;
		}
		else if (tokens[p].type == REG)
		{
			if (!strcmp(tokens[p].str, "$eax"))
			{
				result = cpu.eax;
			}
			else if (!strcmp(tokens[p].str, "$ecx"))
			{
				result = cpu.ecx;
			}
			else if (!strcmp(tokens[p].str, "$edx"))
			{
				result = cpu.edx;
			}
			else if (!strcmp(tokens[p].str, "$ebx"))
			{
				result = cpu.ebx;
			}
			else if (!strcmp(tokens[p].str, "$esp"))
			{
				result = cpu.esp;
			}
			else if (!strcmp(tokens[p].str, "$ebp"))
			{
				result = cpu.ebp;
			}
			else if (!strcmp(tokens[p].str, "$esi"))
			{
				result = cpu.esi;
			}
			else if (!strcmp(tokens[p].str, "$edi"))
			{
				result = cpu.edi;
			}
			else if (!strcmp(tokens[p].str, "$eip"))
			{
				result = cpu.eip;
			}
			else
			{
				return 0;
			}
			//printf("%d\n", result);
			return result;
		}
		else
		{
			printf("hex or num or reg ERROR,system failed");
			assert(0);
		}
	}
	else if (check_parenthese(p, q))
		return eval(p + 1, q - 1);
	else // calc
	{
		int op = dominate_operator(p, q);
		//printf("op at position %d\n", op);
		if (op == -2)
			assert(0);
		else if (op == -1) // 指针解引用问题或负号问题
		{
			if (tokens[p].type == NEG)
			{
				int result = eval(p + 1, q);
				return -result;
			}
			else if (tokens[p].type == POINT)
			{
				int result = 0;
				int val = eval(p + 1, q);
				result = swaddr_read(val, 4);
				return result;
			}
			else if (tokens[p].type == '!')
			{
				int result = eval(p + 1, q);
				return !result;
			}
		}

		int val1 = eval(p, op - 1);
		// printf("%d\n", val1);
		int val2 = eval(op + 1, q);
		// printf("%d\n", val2);
		switch (tokens[op].type)
		{
		case '+':
			// printf("calc now\n val1=%d\n val2=%d\n ans=%d + %d=%d\n", val1, val2, val1, val2, val1 + val2);
			return val1 + val2;
		case '-':
			// printf("calc now\n val1=%d\n val2=%d\n ans=%d - %d=%d\n", val1, val2, val1, val2, val1 - val2);
			return val1 - val2;
		case '*':
			// printf("calc now\n val1=%d\n val2=%d\n ans=%d * %d=%d\n", val1, val2, val1, val2, val1 * val2);
			return val1 * val2;
		case '/':
			// printf("calc now\n val1=%d\n val2=%d\n ans=%d / %d=%d\n", val1, val2, val1, val2, val1 / val2);
			return val1 / val2;
		case OR:
			return val1 || val2;
		case AND:
			return val1 && val2;
		case EQ:
			if (val1 == val2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case NOTEQUAL:
			if (val1 != val2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		default:
			assert(0);
		}
	}
	return 0;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	int i;
	for (i = 0; i < nr_token; i++)
	{
		if (tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != REG && tokens[i - 1].type != NUM && tokens[i - 1].type != HEXN && tokens[i - 1].type != ')')))
		{
			tokens[i].type = POINT;
		}
		if (tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != REG && tokens[i - 1].type != NUM && tokens[i - 1].type != HEXN && tokens[i - 1].type != ')')))
		{
			tokens[i].type = NEG;
		}
	}
	return eval(0, nr_token - 1);

	/* TODO: Insert codes to evaluate the expression. */
}
