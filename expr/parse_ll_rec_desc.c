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
 * factor     -> number            | (expression)
 * 
 */

/*
 * statement   -> expression;
 * expression  -> term expression'
 * expression' -> + term expression' | - term expression' | E
 * term        -> factor term'
 * term'       -> * factor term'     | / factor term'     | E
 * factor      -> number             | (expression)
 * 
 */
 
static void statement(void);
static void expression(void);
static void expression_prm(void);
static void term(void);
static void term_prm(void);
static void factor(void);
static bool match(int what);
static void error(const char * what);

int fcalls;
#define count_call() 	++fcalls
#define report_calls()	printf("\n%d function calls made\n", fcalls), fcalls = 0

//------------------------------------------------------------------------------

void parse_report(void)
{
	puts("Recursive Descent");
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
	// statement -> expression;
	expression();
	if (!match(SEMI))
		error(";");
	return;
}
//------------------------------------------------------------------------------

void expression(void)
{
	count_call();
	// expression -> term expression'
	term();
	expression_prm();
	return;
}
//------------------------------------------------------------------------------

void expression_prm(void)
{
	count_call();
	// expression' -> + term expression' | - term expression' | E
	
	if (match(PLUS) || match(MINUS))
	{
		int match = lex_current();
		lex_advance();
		term();
		(match == PLUS) ? vm_exec(vmADD) : vm_exec(vmSUB);
		expression_prm();
	}
		
	return;
}
//------------------------------------------------------------------------------

void term(void)
{
	count_call();
	// term -> factor term'
	
	factor();
	term_prm();
	return;
}
//------------------------------------------------------------------------------

void term_prm(void)
{
	count_call();
	// term' -> * factor term' | / factor term' | E
	
	if (match(MULT) || match(DIV))
	{
		int match = lex_current();
		lex_advance();
		factor();
		(match == MULT) ? vm_exec(vmMULT) : vm_exec(vmDIV);
		term_prm();
	}
		
	return;
}
//------------------------------------------------------------------------------

void factor(void)
{
	count_call();
	// factor -> number | (expression)
	
	if (match(NUM))
	{
		lex_advance();
		vm_load_num(atoi(lex_get_num()));
		vm_exec(vmPUSH);
	}
	else if (match(LPAR))
	{
		lex_advance();
		expression();
		if (match(RPAR))
			lex_advance();
		else
			error(")");
	}
	else
		error("number or (");
	
	return;
}
//------------------------------------------------------------------------------

bool match(int what)
{
	return what == lex_current();
}
//------------------------------------------------------------------------------

void error(const char * str)
{
	vm_stop();
	fprintf(stderr, "Err: line %d: '%s' expected but got '%s' instead\n",
		   lex_get_line(), str, lex_tok_str(lex_current()));
	fprintf(stderr, "%s\n", lex_get_str());
	fprintf(stderr, "%*c\n", lex_get_pos(), '^');
	return;
}
//------------------------------------------------------------------------------
