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
		
	}
	
	if (this->current->index < this->current->symbols.getLength()) {
		frames.push(this->current);
	}
	
	// If the current item is NULL or the current stackframe is empty
	if (this->current == (StackFrame*)NULL ) {
		// If we have a new frame, get it. Otherwise return null.
		if (this->frames.getLength() > 0) {
			this->current = frames.pop();
			frames.push(this->current);
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
	this->current = frame;
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
	if ( frame == (StackFrame*)NULL ) return 0;	
	if ( frame->sym == (Token*)NULL ) return 0;
	
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
	
	if ( current->index < current->symbols.getLength() ) {
		frame->sym = current->symbols.getAt(current->index++);
	} else {
		frame->index++;
		this->getFrame();
	}
}

void JSParser::doreturn() {
	char* value;
	bool assigned = false;
	getFrame();
	
	// If there's a string here, then let's process it...
	if (expect(stringsym)) {
	
		// Get the string we found and see if there are more strings
		// according to the getString() logic (which searches for contact
		// operations and such).
		string* val = new string(current->stack.pop()->val);
		current->stack.push(new Token(stringsym, val->toString()));
		getString();
		
		// If there were more strings, then concat them.
		if ( current->stack.getLength() > 0 )
			val->append(current->stack.pop()->val);
		
		// Return the value of all this.
		value = val->toString();
		assigned = true;
	} else if (expect(ident)) {
		char* name = current->stack.pop()->val;
		value = context->getVar(name)->val->toString();
		assigned = true;
	}
		
	// Pop the current frame off.
	if ( assigned ) {
		// Pop this frame.
		frames.pop();
		
		// Load the old one.
		getFrame();
		
		// Set the thing.
		current->stack.push(new Token(stringsym, value));
		
	}
	
}

void JSParser::membercall() {
	StackFrame* frame = getFrame();
	
	if (expect(member)) {
		char* name = frame->stack.pop()->val;
		
		if (accept(period)) {
			context->setScope(name);
			membercall();
		} else if (frame->sym->sym == eql) {
			// Get the parent thing.
			char* parent = current->stack.pop()->val;
			context->setScope(parent);
			current->stack.push(new Token(member, name));
			assignment();
		} else if (accept(lparen)) {
			char* parent = current->stack.pop()->val;
			context->setScope(context->getVar(parent));
			Object* obj = context->getVar(parent);
			frame->stack.push(new Token(ident, name));
			invoke();
		}
	} else if (accept(prototypesym)) {
		if ( accept(period)) {
			membercall();
		}
	} else if ( accept(callsym)) {
		char* methodName = frame->stack.pop()->val;
		frame->stack.push(new Token(ident, methodName));
		invoke();
	}
		
}

void JSParser::block() {
	
	// Refresh the current stackframe pointer.
	getFrame();
	
	if ( this->accept(scriptstartsym) ) {
		// Start
		
	} else if (this->accept(varsym)) {
	
		// Get the identifier.
		if (this->expect(ident)) {
			// We have a variable name!
			this->assignment();
		}
	} else if (this->expect(ident)) {
		char* memberName = current->stack.pop()->val;
		current->stack.push(new Token(ident, memberName));
		
		if ( accept(period)) {
			// Traversing a member.
			context->setScope(memberName);
			this->membercall();
		} else {
			// Method call?
			this->invoke();
		}
		
	} else if (this->expect(semicolon)) {
		// Line termination.
		context->resetScope();
	} else if (this->accept(functionsym)) {
		if (this->expect(ident)) {
			this->function();
		} 
	} else if (accept(returnsym)) {
		doreturn();
		return;
	} else if (this->accept(lparen)) {
		this->expression();
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
		
		// Check if this has an expression in it.
		if (accept(lparen)) {
			expression();
		} else if ( accept(functionsym)) {
			//this->context->setVar(identifier, (char*)"\0");
			frame->stack.push(new Token(member, identifier));
			function();
			return;
		} else if ( accept(newsym)) {
			// New instance.
			if ( expect(ident)) {
				char* objectName = frame->stack.pop()->val;
				Object* instance = new Object(identifier, (char*)"\0");
				Object* obj = context->getVar(objectName);
				if ( obj != (Object*)NULL ) {
					instance->copy(obj);
				}
				context->variables.add(instance);
				context->scope = instance;
			}
			return;
		} else {
			// Get the next identifier added to the stack, if applicable.
			if (expect(ident) || expect(stringsym)) {
				getString();
			}
		}
	
		char* val = frame->stack.pop()->val;
		
		// Do the logic!
		this->context->setVar(identifier, val);
	}
}

void JSParser::comparison() {
	string* left = new string(current->stack.pop()->val);
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

void JSParser::expression() {
	// Parse the expression.
	Stack<bool> parens;
	List<Token*> tokens;
	
	// (1 == 1)
	while(!accept(rparen)) {
		if (accept(lparen)) {
			expression();
		} else if (expect(ident) || expect(stringsym)) {
			
			char* name = current->stack.pop()->val;
			
			// Check if it's a method.
			if (this->context->getMethod(name) != (void*)NULL ) {
				Token* token = current->stack.pop();
				current->stack.push(new Token(ident, name));
				invoke();
				getString();
			} else {
				current->stack.push(new Token(stringsym, name));
			}
			
			// If this is a double equals.
			if (accept(eql)) {
				if (accept(eql)) {
					comparison();					
				}
			}
		} else {
			nextsym();
		}
	}
}

void JSParser::getString() {
	// Pop the value from the stack.
	StackFrame* frame = getFrame();
	if (frame == (StackFrame*)NULL) return;
	if (frame->stack.getLength() <= 0 ) return;
	
	string* value = new string(frame->stack.pop()->val);
	
	// Check for any string concatination
	if (accept(plus)) {
		getFrame();
		if (expect(stringsym)) {
			if ( frame->stack.getLength() > 0 ) 
				value->append(frame->stack.pop()->val);
			
		} else if (expect(ident)) {
			if ( frame->stack.getLength() > 0 ) {
				char* name = frame->stack.pop()->val;
				value->append(context->getVar(name)->val->toString());
			}
		}
	}
	
	frame->stack.push(new Token(stringsym, value->toString()));
}

void JSParser::invoke() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	char* method = frame->stack.pop()->val;
	
	List<Object*> methods;
	accept(lparen);
	
	// While we're not at the rparam, create a 
	// list of arguments.
	while (true) {
		if (this->expect(ident)) {
			// Copy the variable.
			char* identName = frame->stack.pop()->val;
			Object* var = context->getVar(identName);
			if ( var == (Object*)NULL ) {
				// Check methods.
				var = (Object*)context->getMethod(identName);
				if ( var == (Object*)NULL ) {
					// Error condition
				} else {
					methods.add(var);
					continue;
				}
			}
			
			// Otherwise, it's a variable.
			char* identVal = var->val->toString();
			frame->stack.push(new Token(stringsym, identVal));
			getString();
			methods.add(new Object(var->name->toString(), frame->stack.pop()->val));
		} else if (this->expect(stringsym)) {
			getString();
			methods.add(new Object((char*)"\0", frame->stack.pop()->val));
		} else if (accept(functionsym)) {
			// Create a temp variable to store the function
			frame->stack.push(new Token(ident, (char*)"__temp"));
			function();
			// Add it
			methods.add((Object*)context->getMethod((char*)"__temp"));
			
		} else if (this->accept(comma)) {
			continue;
		} else {
			break;
		}
		
	}
	
	this->accept(rparen);
	
	// Do the logic!
	// Allocate a new stack.
	void* target = this->context->getMethod(method);	
	if (this->context->isJSDelegate(method)) {
		// Invoke the delegate.
		List<char*> arguments;
		
		// Create the arguments.
		for ( int i = 0; i < methods.getLength(); i++ ) {
			arguments.add(methods.getAt(i)->val->toString());
		}
		
		JSDelegate* delegate = (JSDelegate*)target;
		delegate->invoke(arguments);
	} else {
		// Invoke the js object.
		Object* object = (Object*)target;
		this->allocate(object->tokens);	
		
		// Update all the arguments.
		for ( int i = 0; i < methods.getLength(); i++ ) {
			if ( i < object->arguments.getLength() ) {
				char* varName = object->arguments.getAt(i);
				Object* variable = methods.getAt(i);
				if ( variable->tokens.getLength() > 0 ) {
					context->setMethod(varName, variable->arguments, variable->tokens);		
				} else {
					char* varVal = variable->val->toString();
					context->setVar(varName, varVal);
				}
			}
		}
				
		nextsym();
		block();
	}
	
}

void JSParser::function() {
	StackFrame* frame = this->getFrame();
	if ( frame == (StackFrame*)NULL) return;
	
	Token* token = frame->stack.pop();
	string* name = new string(token->val);
	List<Token*> methodTokens;
	List<char*> args;
	Stack<bool> parens;
	
	this->accept(lparen);
	
	// Collect all the parameter names for the argument list.
	while(!this->accept(rparen)){
		if (expect(ident)) {
			args.add(frame->stack.pop()->val);
		} else if (accept(comma)) {
			continue;
		} else {
			break;
		}
	}
	
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
		if ( token->sym == member ) {// && context->scope != (Object*)NULL ) {
			// Assign this method to the scope object.
			Object* object = new Object(name->toString(), args, methodTokens);
			if ( context->scope != (Object*)NULL ) {
				context->setMethod(name->toString(), args, methodTokens);
			} else {
				context->variables.add(object);
			}
		} else {
			// Otherwise, assign it to the global scope.
			this->context->setMethod(name->toString(), args, methodTokens);
		}
	}
	
	current->stack.push(new Token(methodsym, name->toString()));
}

void JSParser::program() {
	do {
		// Interpret some stuff.
		block();
		
		// Verify the frame still has work to do.
		if ( this->current == (StackFrame*)NULL )
			break;
		
		if ( this->current->index >= this->current->symbols.getLength() ) {
			this->current = getFrame();
			if ( current == (StackFrame*)NULL || current->index > current->symbols.getLength() ) {
				break;
			}
		} 
	} while(true);
}

void JSParser::parse(char* code) {
	List<Token*> tokens = tokenize(code);
	this->allocate(tokens);
	this->program();
}

void JSParser::registerDelegate(char* identifier, void (*func)(List<char*>args)) {
	this->context->registerDelegate(identifier, func);
}

