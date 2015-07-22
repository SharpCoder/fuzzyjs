#ifndef __FJS_TOKENIZER_H_
#define __FJS_TOKENIZER_H_

#include "fjs_helper.cpp"
#include "fjs_string.h"
#include "fjs_token.h"

namespace fjs {
	Symbol convert(Symbol prev, string* str);
	bool isFinished(string* str, char next, Symbol nxt);
	Token* doUntil(Symbol tokenType, Symbol target, string* code, int* index);
	bool processSymbol(Token* s, List<Token*>* results, string* code, int* index);
	List<Token*> tokenize(char* str);
}

#endif
