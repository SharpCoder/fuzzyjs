#ifndef __FJS_TOKENIZER_H_
#define __FJS_TOKENIZER_H_

#include "fjs_helper.cpp"
#include "fjs_string.h"
#include "fjs_token.h"

Symbol convert(Symbol prev, string* str);
bool isFinished(string* str, char next, Symbol nxt);
List<Token*> tokenize(char* str);

#endif
