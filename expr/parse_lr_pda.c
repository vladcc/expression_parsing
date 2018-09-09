#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vm.h"
#include "lex.h"
#include "parse.h"

/*
 * statement  -> expression;
 * expression -> expression + term | expression - term | term
 * term       -> term * factor     | term / factor     | factor
 * factor     -> (expression)      | number
 * 
 */

#define MAX_STATES 128

int sp;
int stack[MAX_STATES];

#define state() stack[sp-1]

enum {START = 100, STATMNT, EXPR, TERM, FACTOR};

static void go(void);
static bool match(int what);
static void error_expect(const char * what);
static void error_msg(const char * msg);

int fcalls;
#define count_call() 	++fcalls
#define report_calls()	printf("\n%d function calls made\n", fcalls), fcalls = 0

//#define DBG

#ifdef DBG
#define dbg_report() report_state()
#else
#define dbg_report()
#endif

#define vm_push() vm_load_num(atoi(lex_get_num())), vm_exec(vmPUSH)

//------------------------------------------------------------------------------

void parse_report(void)
{
	puts("LR PDA");
	return;
}
//------------------------------------------------------------------------------

void parse(const char * str)
{
	vm_reset();
	lex_set_str(str);
	go();
	report_calls();
	return;
}
//------------------------------------------------------------------------------

void report_state(void)
{
	// debug purposes
	static const char * state[] = {
		"START", "STATMNT", "EXPR", "TERM", "FACTOR"};
	
	const char * st = (state() >= 100) 	? state[state()-100] : lex_tok_str(state());
	printf("State: %s; Current token: %s\n", st, lex_tok_str(lex_current()));
}

static void push(int state)
{
	count_call();
	if(sp >= MAX_STATES)
	{
		fprintf(stderr, "Err: parser: stack full\n"); 
		exit(EXIT_FAILURE);
	} 
	stack[sp++] = state;
	return;
}

static void pop(void)
{
	count_call();
	if(sp <= 0)
	{
		fprintf(stderr, "Err: parser: attempt to pop an empty stack\n");
		exit(EXIT_FAILURE);
	} 
	--sp;
	return;
}

void go(void)
{
	push(START);	

	while (true)
	{
		dbg_report();
		switch (state())
		{				
			case START:
			case PLUS:
			case MINUS:			
			case DIV:	
			case MULT:
			case LPAR:
				lex_advance();
				if (match(NUM))
					vm_push();
				else if (match(LPAR))
				{
					push(LPAR);
					break;
				}
				else 			
					error_expect("number or (");
				lex_advance();	
				push(FACTOR);
				break;
				
			case RPAR:
				pop(); // rpar
				pop(); // expr
				if (LPAR == state())
					pop(); // lpar
				else
					error_msg("Unmatched parenthesis");
					
				lex_advance();
				push(FACTOR);
				break;
				
			case SEMI:
				pop(); // semi
				pop(); // expr
				push(STATMNT);
				break;
			
			case STATMNT:
				pop(); // statmnt
				if (state() != START)
					error_msg("Malformed expression");
				return;
				break;
				
			case EXPR:
				pop(); // expr
				if (PLUS == state() || MINUS == state())
				{
					vm_exec((PLUS == state()) ? vmADD : vmSUB);
					pop(); // plus | minus
					pop(); // expr
					push(TERM);
				}
				else
				{
					push(EXPR);
					if (match(PLUS))
						push(PLUS);
					else if (match(MINUS))
						push(MINUS);
					else if (match(RPAR))
						push(RPAR);
					else if (match(SEMI))
						push(SEMI);
					else
					{
						error_expect("+, -, *, /, ), or ;");
						pop();
					}
				}
				break;
			
			case TERM:
				pop(); // term
				if (MULT == state() || DIV == state())
				{
					vm_exec((MULT == state()) ? vmMULT : vmDIV);
					pop(); // mult | div
					pop(); // term
					push(FACTOR);
				}
				else
				{
					push(TERM);
					if (match(MULT)) 
						push(MULT);
					else if (match(DIV))
						push(DIV);
					else
					{
						pop();
						push(EXPR);
					}
				}
				break;
				
			case FACTOR:
				pop(); // factor
				push(TERM);
				break;
					
			default:
				break;
		}
	}
	return;
}
//------------------------------------------------------------------------------

bool match(int what)
{
	return what == lex_current();
}
//------------------------------------------------------------------------------

void error_expect(const char * str)
{
	vm_stop();
	fprintf(stderr, "Err: line %d: '%s' expected but got '%s' instead\n",
		   lex_get_line(), str, lex_tok_str(lex_current()));
	fprintf(stderr, "%s\n", lex_get_str());
	fprintf(stderr, "%*c\n", lex_get_pos(), '^');
	
	if (match(EOL))
		exit(EXIT_FAILURE);
	return;
}
//------------------------------------------------------------------------------

void error_msg(const char * msg)
{
	vm_stop();
	fprintf(stderr, "Err: line %d: %s\n", lex_get_line(), msg);
	fprintf(stderr, "%s\n", lex_get_str());
	fprintf(stderr, "%*c\n", lex_get_pos(), '^');
	
	if (match(EOL))
		exit(EXIT_FAILURE);
	return;
}
//------------------------------------------------------------------------------
