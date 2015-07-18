#include <stdio.h>
#include <stdlib.h>
#include "fjs.h"

const char* revert(Symbol s) {
	if ( s == varsym ) return "var ";
	else if ( s == oddsym || s == ident ) return "Identifier";
	else if ( s == eql ) return "=";
	else if ( s == semicolon ) return "; ";
	else if ( s == lparen ) return "(";
	else if ( s == rparen ) return ")";
	return "";
}

int main(void) {
	
	List<Token*> tokens = tokenize((char*)"var s = 10; printf(s);");
	parse(tokens);
	for ( int i = 0; i < tokens.getLength(); i++ ) {
		Token* token = tokens.getAt(i);
		printf("%s", token->val);
	}
	
	printf("\n");
	return 0;
}

