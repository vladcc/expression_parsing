#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lex.h"

static const char * what[] = {
	"+", "-", "*", "/", "number", "(", ")", ";", "EOL"
};

static const char * line;

#define NUMLEN 9
static char num[NUMLEN];
int current;
int pos;
int linen;
int line_len;

//------------------------------------------------------------------------------

void lex_set_str(const char * str)
{
	++linen;
	pos = 0;
	line = str;
	line_len = strlen(line);
	return;
}
//------------------------------------------------------------------------------

static int next_char(void)
{
	return (pos > line_len) ? '\0' : line[pos++];
}

static void put_back(void)
{
	--pos;
	return;
}

static void err_exit(void)
{
	fprintf(stderr, "%s\n", line);
	fprintf(stderr, "%*c\n", pos, '^');
	fprintf(stderr, "Now aborting\n");
	exit(EXIT_FAILURE);	
}

void lex_advance(void)
{
	int ch;
	while (isspace(ch = next_char()))
		continue;
		
	switch (ch)
	{
		case '+':
			current = PLUS;
			break;
		case '-':
			current = MINUS;
			break;
		case '*':
			current = MULT;
			break;
		case '/':
			current = DIV;
			break;
		case '(':
			current = LPAR;
			break;
		case ')':
			current = RPAR;
			break;
		case ';':
			current = SEMI;
			break;
		case '\0':
			current = EOL;
			break;
		default:
			if (!isdigit(ch))
			{				
				fprintf(stderr, "Err: line %d: unknown character '%c'\n", linen, ch);
				err_exit();
			}
			
			current = NUM;
			int i;
			for (i = 0; isdigit(ch); ++i, ch = next_char())
			{
				if (i >= NUMLEN)
				{
					fprintf(stderr, "Err: line %d: number too long\n", linen);
					err_exit();
				}
				num[i] = ch;
			}
			num[i] = '\0';
			put_back();
			break;
	}
	return;
}
//------------------------------------------------------------------------------

int lex_current(void)
{
	return current;
}
//------------------------------------------------------------------------------

const char * lex_tok_str(int tok)
{
	int wsize = sizeof(what)/sizeof(*what);
	
	if (tok >= 0 && tok < wsize)
		return what[tok];
	else
		return "unknown token";
}
//------------------------------------------------------------------------------

const char * lex_get_num(void)
{
	return num;
}
//------------------------------------------------------------------------------

int lex_get_line(void)
{
	return linen;
}
//------------------------------------------------------------------------------

int lex_get_pos(void)
{
	return pos;
}
//------------------------------------------------------------------------------

const char * lex_get_str(void)
{
	return line;
}
//------------------------------------------------------------------------------
