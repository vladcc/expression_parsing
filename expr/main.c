#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vm.h"
#include "lex.h"
#include "parse.h"

#define LINELN 1024
char line[LINELN];

FILE * input;
//------------------------------------------------------------------------------

static void use_exit(const char * pname)
{
	printf("Use: %s [filename] [-c]\n", pname);
	puts("-c flag makes it compile, interprets by default");
	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
	input = stdin;
	int vm_mode = vmINTERPRET;
	
	if (argc > 3)
		use_exit(argv[0]);
	
	while (*++argv)
	{
		if ('-' == argv[0][0])
		{
			if ('c' == argv[0][1])
				vm_mode = vmCOMPILE;
			else
				use_exit(argv[0]);
		}
		else
		{
			FILE * inf = fopen(*argv, "r");
			if (!inf)
			{
				fprintf(stderr, "Err: couldn't open file < %s >\n", *argv);
				exit(EXIT_FAILURE);
			}
			input = inf;
		}
	}
	
	vm_set_mode(vm_mode);
	parse_report();
	while (fgets(line, LINELN, input))
	{
		int len = strlen(line);
		if ('#' == *line || len <= 1)
			continue;
		
		if ('\n' == line[len-1])
			line[len-1] = '\0';
		
		if (vmINTERPRET == vm_mode)
			puts(line);
		
		parse(line);
		putchar('\n');
	}
	
	return 0;
}
//------------------------------------------------------------------------------
