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

/* 
 * statement -> expr;
 * expr      -> expr + expr | expr - expr
 * expr      -> expr * expr | expr / expr
 * expr      -> (expr)      | NUM 
 * 
 */

static void statement(void);
static void lpar(void);
static void rpar(void);
static void factor(void);
static void expr_plus_minus(int opt);
static void expr_plus_minus_close(int op);
static void expr_mult_div(int op);
static bool match(int what);
static void error(const char * what, int tok, int pos);

int fcalls;

//#define DBG

#ifdef DBG
#define dbg_call() printf("+++ %s() read %s %s\n", __func__, lex_tok_str(lex_current()), match(NUM) ? lex_get_num() : "")
#define dbg_ret() printf("--- %s()\n", __func__)
#else
#define dbg_call()
#define dbg_ret() 
#endif

#define vm_push() 		vm_load_num(atoi(lex_get_num())), vm_exec(vmPUSH)
#define par_close()		lpar(), rpar()
#define count_call() 	++fcalls
#define report_calls()	printf("\n%d function calls made\n", fcalls), fcalls = 0

//------------------------------------------------------------------------------

void parse_report(void)
{
	puts("Recursive Ascent");
	return;
}
//------------------------------------------------------------------------------

void parse(const char * str)
{
	vm_reset();
	lex_set_str(str);
	lex_advance();
	statement();
	report_calls();
	return;
}
//------------------------------------------------------------------------------

void statement(void)
{
	count_call();
	dbg_call();
	
	int ctok = lex_current();
	int cpos = lex_get_pos();
	lex_advance();	
	switch (ctok)
	{
		case NUM:
			vm_push();
			factor();
			break;
		case LPAR:
			par_close();	
			factor();
			break;
		default:
			error("number or (", ctok, cpos);
			break;
	}	
	
	if (!match(SEMI))
		error(";", lex_current(), lex_get_pos());
		
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void lpar(void)
{
	count_call();
	dbg_call();
		
	int ctok = lex_current();
	int cpos = lex_get_pos();
	lex_advance();
	
	switch (ctok)
	{
		case NUM:
			vm_push();
			factor();
			break;
		case LPAR:
			par_close();
			factor();
			break;
		default:
			error("number or (", ctok, cpos);
			break;
	}	
	
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void rpar(void)
{
	count_call();
	dbg_call();
	
	if (!match(RPAR))
		error(")", lex_current(), lex_get_pos());
		
	lex_advance();
	
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void factor(void)
{
	count_call();
	dbg_call();
		
	int ctok = lex_current();
	int cpos = lex_get_pos();
	
	if (match(RPAR) || match(SEMI))
		return;
	
	lex_advance();
	switch (ctok)
	{
		case DIV:
		case MULT:
			expr_mult_div(ctok);
			break;
		case PLUS:
		case MINUS:
			expr_plus_minus(ctok);
			break;
		default:
			error("+, -, *, or /", ctok, cpos);
			break;
	}	
	
	if (!match(SEMI) && !match(EOL) && !match(RPAR))
		factor();
		
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void expr_mult_div(int op)
{
	count_call();
	dbg_call();
	
	int ctok = lex_current();
	int cpos = lex_get_pos();
	
	lex_advance();
	switch (ctok)
	{
		case LPAR:
			par_close();
			vm_exec((op == MULT) ? vmMULT : vmDIV);
			break;
		case NUM:
			vm_push();
			vm_exec((op == MULT) ? vmMULT : vmDIV);
			break;
		default:
			error("number or (", ctok, cpos);
			break;
	}	
	
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void expr_plus_minus(int op)
{
	count_call();
	dbg_call();
	
	int ctok = lex_current();
	int cpos = lex_get_pos();
	
	lex_advance();
	switch (ctok)
	{
		case LPAR:
			par_close();
			if (match(MULT) || match(DIV))
				expr_plus_minus_close(lex_current());
			vm_exec((op == PLUS) ? vmADD : vmSUB);
			break;
		case NUM:
			vm_push();
			if (match(MULT) || match(DIV))
				expr_plus_minus_close(lex_current());
			vm_exec((op == PLUS) ? vmADD : vmSUB);
			break;
		default:
			error("number or (", ctok, cpos);
			break;
	}	
	
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

void expr_plus_minus_close(int op)
{
	count_call();
	dbg_call();	
	
	int ctok = lex_current();
		
	lex_advance();
	switch (ctok)
	{
		case DIV:
		case MULT:
			expr_mult_div(ctok);
			if (match(MULT) || match(DIV))
				expr_plus_minus_close(lex_current());
			break;
		default:
			break;
	}	
	
	dbg_ret();
	return;
}
//------------------------------------------------------------------------------

bool match(int what)
{
	return what == lex_current();
}
//------------------------------------------------------------------------------

void error(const char * str, int tok, int pos)
{
	vm_stop();
	fprintf(stderr, "Err: line %d: '%s' expected but got '%s' instead\n",
		   lex_get_line(), str, lex_tok_str(tok));
	fprintf(stderr, "%s\n", lex_get_str());
	fprintf(stderr, "%*c\n", pos, '^');
	lex_advance();
	return;
}
//------------------------------------------------------------------------------
