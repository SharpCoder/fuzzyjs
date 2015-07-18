#ifndef __FJS_OBJECT_H_
#define __FJS_OBJECT_H_

#include "fjs_string.h"

// The object class is the basis for all things in fuzzyjs.
// It is scope. It is a function. It is self-replicable.

class Object {
	public:
		// These are the tokens that make up the function this
		// object represents. I guess.
		List<Token*> tokens;
		List<Object> methods;
		
		// The name of this object.
		string* name;
		
		// Constructors.
		Object() { }
		Object(char* identifier) {
			this->name = new string(identifier);
		}
		Object(char* identifier, List<Token*> methodTokens) {
			this->name = new string(identifier);
			this->tokens = methodTokens;
		}
		
		List<Token*> getTokens() {
			return this->tokens;
		}
};

#endif
