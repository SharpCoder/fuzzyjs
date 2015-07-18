#include "fjs_parser.h"

JSParser::JSParser() {
	this->context = new SystemContext();
}

StackFrame* JSParser::getFrame() {
	// If there is no current frame or the current frame has
	// run out of work to do, then grab the next one in the stack.	
	
	// Always use the top item in the stack.
	if ( this->frames.getLength() > 0 ) {
		this->current = frames.pop();
		frames.push(this->current);
	}
	
	// If the current item is NULL or the current stackframe is empty
	if (this->current == (StackFrame*)NULL || this->current->index > this->current->symbols.getLength()) {
		// If we have a new frame, get it. Otherwise return null.
		if (this->frames.getLength() > 0) {
			this->current = frames.pop();
		} else {
			this->current = (StackFrame*)NULL;
		}
	}
	
	// Return the current stack frame.
	return this->current;
}

void JSParser::allocate(List<Token*> tokens) {
	StackFrame* frame = new StackFrame(tokens);
	this->frames.push(frame);
}

int JSParser::accept(Symbol s) {
	// NULL Check
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return 0;
	if ( frame->sym == (Token*)NULL ) return 0;
	
	// Check if the symbol we're on equals the one we expect.
	if (s == frame->sym->sym) {
		this->nextsym();
		return 1;
	}
	return 0;
}

int JSParser::expect(Symbol s) {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return 0;	
	if (frame->sym == (Token*)NULL) return 0;
	
	if ( s == frame->sym->sym ) {
		frame->stack.push(frame->sym);
		this->nextsym();
		return 1;
	}
	return 0;
}

void JSParser::nextsym() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	
	if ( frame->index < frame->symbols.getLength() ) {
		frame->sym = frame->symbols.getAt(frame->index++);
	} else {
		this->getFrame();
	}
}

void JSParser::block() {
	if ( this->accept(scriptstartsym) ) {
		// Start
	} else if (this->accept(varsym)) {
		// Get the identifier.
		if (this->expect(ident)) {
			// We have a variable name!
			this->assignment();
		}
	} else if (this->expect(ident)) {
		
		// Functions
		if (this->accept(lparen)){
			// Method call?
			this->invoke();
		}
	} else if (this->expect(semicolon)) {
		// Line termination.
		
	} else if (this->accept(functionsym)) {
		if (this->expect(ident)) {
			this->function();
		}
	} else {
		// Compiler error?
		this->nextsym();
	}
}

void JSParser::assignment() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	
	if (this->accept(eql)) {
		// Identifier and it's value.
		char* identifier = frame->stack.pop()->val;
		
		if (accept(lparen)) expression();
		else frame->stack.push(new Token(stringsym, frame->sym->val));
		
		char* value = frame->stack.pop()->val;
		
		// Do the logic!
		this->context->setVar(identifier, value);
	} else if (this->accept(lparen)) {
		// Expression time.
		char* identifier = frame->stack.pop()->val;
		this->expression();
		char* value = frame->stack.pop()->val;
	}
}

void JSParser::expression() {
	// Parse the expression.
	Stack<bool> parens;
	List<Token*> tokens;
	
	// (1 == 1)
	while(!accept(rparen)) {
		if (accept(lparen)) {
			expression();
		} else if (expect(ident) || expect(stringsym)) {
			// comparison.
			string* left = new string(current->stack.pop()->val);
			
			// If this is a double equals.
			if (accept(eql)) {
				if (accept(eql)) {
				
					// Comparison
					// Check for more branching.
					bool branched = false;
					if (accept(lparen)) {
						expression();
						branched = true;
					}
					
					// Verify we have something here to work with.
					if (branched || expect(ident) || expect(stringsym)) {
					
						// If we do, get the right side and compare the two
						// values.
						string* right = new string(current->stack.pop()->val);
						if (strcmp(right,left))
							current->stack.push(new Token(stringsym, TRUE));
						else
							current->stack.push(new Token(stringsym, FALSE));
					}
				}
			}
		} else {
			nextsym();
		}
	}
}

void JSParser::invoke() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	
	char* method = frame->stack.pop()->val;
	List<char*> arguments;
	// While we're not at the rparam, create a 
	// list of arguments.
	while (this->expect(ident)) {
		// Copy the variable.
		char* identName = frame->stack.pop()->val;
		char* identVal = context->getVar(identName)->val->toString();
		arguments.add(identVal);
	}
	this->accept(rparen);
	
	// Do the logic!
	// Allocate a new stack.
	void* target = this->context->getMethod(method);
	if (this->context->isJSDelegate(method)) {
		// Invoke the delegate.
		JSDelegate* delegate = (JSDelegate*)target;
		delegate->invoke(arguments);
	} else {
		// Invoke the js object.
		Object* object = (Object*)target;
		this->allocate(object->getTokens());
	}
}

void JSParser::function() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	
	string* name = new string(frame->stack.pop()->val);
	List<Token*> methodTokens;
	Stack<bool> parens;
	
	// TODO: Implement arguments...
	this->accept(lparen);
	this->accept(rparen);
	
	// If there's a left bracket where we expect...
	if (this->accept(lbracket)) {
		
		// Keep track of the brackets.
		parens.push(true);
		
		// Iterate until we've parsed out the whole function.
		while (parens.getLength() > 0) {
			if (this->accept(lbracket)) {
				methodTokens.add(frame->sym);
				parens.push(true);
			} else if (accept(rbracket)) {
				parens.pop();
				if ( parens.getLength() > 0 )
					methodTokens.add(frame->sym);
			} else {
				methodTokens.add(frame->sym);
				this->nextsym();
			}
		}
		
		// Create the object in scope.
		this->context->setMethod(name->toString(), methodTokens);
	}
}

void JSParser::program() {
	do {
		// Interpret some stuff.
		block();
		
		// Verify the frame still has work to do.
		if ( this->current == (StackFrame*)NULL )
			break;
			
		if ( this->current->index >= this->current->symbols.getLength() )
			break;
		
	} while(true);
}

void JSParser::parse(List<Token*> tokens) {
	this->allocate(tokens);
	this->program();
}

void JSParser::registerDelegate(char* identifier, void (*func)(List<char*>args)) {
	this->context->registerDelegate(identifier, func);
}

