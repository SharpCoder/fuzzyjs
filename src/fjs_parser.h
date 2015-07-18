#ifndef __FJS_PARSER_H_
#define __FJS_PARSER_H_

#include "fjs_helper.cpp"
#include "fjs_string.h"
#include "fjs_token.h"
#include "fjs_var.h"

// Some helper methods.
void addVar(char* identifier, char* val);
char* getVar(char* identifier);

int accept(Token* s);
void nextsym(void);
void block(void); 
void assignment(void);
void invoke(void);
void program(void);
void parse(List<Token*> symbols);


#endif
