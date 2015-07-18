#ifndef __FJS_STACK_FRAME_H_
#define __FJS_STACK_FRAME_H_

#include "fjs_context.h"

// A stack frame is an entire collection of variables and methods
// and scope to run a series of tokens.
class StackFrame {
	public: 
	// Properties
		Token* sym;
		List<Token*> symbols;
		Stack<Token*> stack;
		int index;
		
	// Methods
		StackFrame(List<Token*> tokens) {
			this->symbols = tokens;
		}
};

#endif
