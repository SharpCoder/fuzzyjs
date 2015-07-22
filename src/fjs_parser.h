#ifndef __FJS_PARSER_H_
#define __FJS_PARSER_H_

#define TRUE (char*)"true"
#define FALSE (char*)"false"

#include "fjs_tokenizer.cpp"
#include "fjs_helper.cpp"
#include "fjs_string.h"
#include "fjs_token.h"
#include "fjs_context.h"
#include "fjs_object.h"
#include "fjs_delegate.h"
#include "fjs_stackframe.h"

namespace fjs {
	class JSParser {
		private:
			SystemContext* context;
			Stack<StackFrame*> frames;
			StackFrame* current;

			 // Method for creating a new stack frame.
			void allocate(List<Token*> tokens);
			StackFrame* getFrame(void);
			bool isTrue(string* token);
			
			// Methods for parsing.
			int accept(Symbol s);
			int expect(Symbol s);
			void nextsym(void);
			void block(void); 
			void expression(void);
			void assignment(void);
			void invoke(void);
			void function(void);
			void comparison(void);
			void membercall(void);
			void getString(void);
			void doreturn(void);
			void maths(void);
			void logic(void);
			void ifstatement(void);
			void program(void);
			
		public:
			JSParser();
			void parse(char* code);
			void registerDelegate(const char* identifier, void (*func)(List<char*>));
	};
}


#endif
