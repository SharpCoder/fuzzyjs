#ifndef __FJS_STACK_FRAME_H_
#define __FJS_STACK_FRAME_H_

#include "fjs_context.h"

namespace fjs {

	// A stack frame is an entire collection of variables and methods
	// and scope to run a series of tokens.
	class StackFrame {
		public:
			Token* sym;
			Token* peek;
			Object* container;
			List<Token*> symbols;
			Stack<Token*> stack;
			int index;
			
			StackFrame(List<Token*> tokens) {
				index = 0;
				this->symbols = tokens;
			}
	};

}

#endif
