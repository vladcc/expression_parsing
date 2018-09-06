#ifndef LEX_H
#define LEX_H

enum {PLUS, MINUS, MULT, DIV, NUM, LPAR, RPAR, SEMI, EOL};

void lex_set_str(const char * str);
const char * lex_get_str(void);
void lex_advance(void);
int lex_current(void);
const char * lex_tok_str(int tok);
const char * lex_get_num(void);
int lex_get_line(void);
int lex_get_pos(void);
#endif
