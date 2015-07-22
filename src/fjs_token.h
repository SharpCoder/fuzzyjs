#ifndef __FJS_TOKEN_H_
#define __FJS_TOKEN_H_

namespace fjs {
	struct Token {
		Symbol sym;
		char* val;
		Token() { }
		Token(Symbol s, char* v) {
			this->sym = s;
			this->val = v;
		}
	};

}

#endif
