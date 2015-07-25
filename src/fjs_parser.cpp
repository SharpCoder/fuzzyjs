#include "fjs_parser.h"

namespace fjs {
	
	// Glue functions
	// TODO: Make the delegate send in the JSParser so we can manipulate the current
	// stackframe.
	void js_parseInt(void* ctx, List<char*> args) {
		JSParser* parser = (JSParser*)ctx;
		if ( args.getLength() == 1 ) {
			int val = 0;
			if( parseInt(new string(args.getAt(0)), &val)) {
				parser->current->stack.push(new Token(number, args.getAt(0)));
				parser->maths();
			} else {
				parser->current->stack.push(new Token(stringsym, args.getAt(0)));
			}
		} else {
			// Error condition
		}
	}
	
	JSParser::JSParser() {
		this->context = new SystemContext();
		
		// Register system methods here.
		this->registerDelegate("parseInt", js_parseInt);
	}

	StackFrame* JSParser::getFrame() {
		// If there is no current frame or the current frame has
		// run out of work to do, then grab the next one in the stack.	
		
		// Always use the top item in the stack.
		if ( this->frames.getLength() > 0 ) {
			this->current = frames.pop();
			this->context->currentFrame = this->current;
		}
		
		if (this->current->index < this->current->symbols.getLength()) {
			frames.push(this->current);
		}
		
		// If the current item is NULL or the current stackframe is empty
		if (this->current == (StackFrame*)NULL ) {
			// If we have a new frame, get it. Otherwise return null.
			if (this->frames.getLength() > 0) {
				this->current = frames.pop();
				this->context->currentFrame = this->current;
				frames.push(this->current);
			} else {
				this->current = (StackFrame*)NULL;
				this->context->currentFrame = this->current;
			}
		}
		
		// Return the current stack frame.
		return this->current;
	}

	void JSParser::allocate(List<Token*> tokens) {
		StackFrame* frame = new StackFrame(tokens);
		frame->container = context->scope;
		this->frames.push(frame);
		getFrame();
	}

	int JSParser::accept(Symbol s) {
		getFrame();
		// NULL Check
		if ( current == (StackFrame*)NULL) return 0;
		if ( current->sym == (Token*)NULL ) return 0;
		
		// Check if the symbol we're on equals the one we expect.
		if (s == current->sym->sym) {
			this->nextsym();
			return 1;
		}
		return 0;
	}

	int JSParser::expect(Symbol s) {
		if ( current == (StackFrame*)NULL ) return 0;	
		if ( current->sym == (Token*)NULL ) return 0;
		
		if ( s == current->sym->sym ) {
			current->stack.push(current->sym);
			return this->nextsym();
		}
		
		return 0;
	}

	int JSParser::nextsym() {
		if ( current == (StackFrame*)NULL) return 0;
		if ( current->index < current->symbols.getLength() ) {
			current->sym = current->symbols.getAt(current->index++);
			
			// Process the peek.
			if ( current->index < current->symbols.getLength() )
				current->peek = current->symbols.getAt(current->index);
				
			return 1;
		} else {
			current->index++;
			this->getFrame();
			return 1;
		}
	}

	void JSParser::doreturn() {
		char* value;
		Symbol sym = stringsym;
		bool assigned = false;
		getFrame();
		
		// If there's a string here, then let's process it...
		if (expect(stringsym) || expect(number)) {
		
			// Get the string we found and see if there are more strings
			// according to the getString() logic (which searches for contact
			// operations and such).
			string* val = new string(current->stack.pop()->val);
			current->stack.push(new Token(stringsym, val->toString()));
			getString();
			
			// If there were more strings, then concat them.
			if ( current->stack.getLength() > 0 ) {
				Token* target = current->stack.pop();
				if ( target->sym == stringsym )
					val->append(target->val);
			}
			
			// Return the value of all this.
			value = val->toString();
			assigned = true;
		} else if (expect(ident)) {
			char* name = current->stack.pop()->val;
			Object* obj = context->getVar(name);
			sym = obj->sym;
			value = obj->val->toString();
			assigned = true;
		}
		
		// Update the index so it stops processing the script and then
		// pop the frame.
		frames.pop();
		
		// Load the old one.
		StackFrame* f = getFrame();
				
		// Pop the current frame off.
		if ( assigned ) {
			// Set the thing.
			f->stack.push(new Token(sym, value));
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
				return;
			} else if (accept(lparen)) {
				char* parent = current->stack.pop()->val;
				context->setScope(context->getVar(parent));
				frame->stack.push(new Token(ident, name));
				invoke();
				return;
			} else {
				// Accessing a property!
				Object* obj = context->getVar(name);
				if ( obj != (Object*)NULL ) {
					context->setScope(obj);
					frame->stack.push(new Token(obj->sym, obj->val->toString()));
				}
			}
		} else if (accept(prototypesym)) {
			if ( accept(period)) {
				membercall();
				return;
			}
		} else if ( accept(callsym)) {
			char* methodName = frame->stack.pop()->val;
			frame->stack.push(new Token(ident, methodName));
			invoke();
		} else {
			if(accept(period)) {
				membercall();
			}
		}
			
	}

	void JSParser::block() {
		
		// Refresh the current stackframe pointer.
		getFrame();
		
		if ( this->accept(scriptstartsym) ) {
			// Start
			
		} else if (accept(thissym)) {
			context->scope = current->container;
			if (accept(period))  {
				current->stack.push(new Token(member, context->scope->name->toString()));
				membercall();
				
			}
		} else if (accept(ifsym)) {
			ifstatement();
		} else if (this->accept(varsym)) {
		
			// Get the identifier.
			if (this->expect(ident)) {
				// We have a variable name!
				this->assignment();
			}
		} else if (this->expect(ident)) {
			char* memberName = current->stack.pop()->val;
			if ( accept(period)) {
				// Traversing a member.
				context->setScope(memberName);
				current->stack.push(new Token(member, memberName));
				this->membercall();
			} else if (current->sym->sym == eql) {
				context->setScope(memberName);
				current->stack.push(new Token(ident, memberName));
				// Rassignment
				assignment();
			} else if (current->sym->sym == plus || current->sym->sym == minus) {
				context->setScope(memberName);
				current->stack.push(new Token(ident, memberName));
				assignment();
			} else {
				// Method call?
				current->stack.push(new Token(ident, memberName));
				this->invoke();
			}
		} else if (this->expect(semicolon)) {
			// Line termination.
			context->resetScope(current->container);
		} else if (this->accept(functionsym)) {
			if (this->expect(ident)) {
				this->function();
			} 
		} else if (accept(returnsym)) {
			doreturn();
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
				//this->context->setScope(identifier);
				frame->stack.push(new Token(member, identifier));
				function();
				return;
			} else if ( accept(newsym)) {
				// New instance.
				if ( expect(ident)) {
					
					// Object to copy.
					char* objectName = frame->stack.pop()->val;
					
					// New object we are creating.
					Object* instance = new Object(identifier, (char*)"\0");
					
					// Object we are basing this off of.
					Object* obj = context->getVar(objectName);
					
					// Verify the base object is real.
					if ( obj != (Object*)NULL ) {
						// Then do the copy.
						instance->copy(obj);
					}
					
					context->addVar(instance);
					context->setScope(instance);
					
					// Call the constructor.
					if ( obj->tokens.getLength() > 0 ) {
						current->stack.push(new Token(ident, obj->name->toString()));
						invoke();
					}
				}
				return;
			} else if (expect(number)) {
				char* value = frame->stack.pop()->val;
				frame->stack.push(new Token(stringsym, value));
				
				int output = 0;
				if ( parseInt(new string(value), &output)) {
					maths();
				} else {
					getString();
				}
			} else if (expect(stringsym)) {// || expect(ident)) {
				// Get the next identifier added to the stack, if applicable.
				getString();
			} else if (expect(ident)) {
				if ( accept(period)) {
					membercall();
				} else {
					char* name = frame->stack.pop()->val;
					frame->stack.push(new Token(ident, name));
					if ( context->getMethod(name) != (Object*)NULL ) {
						// Check if it's a method.
						invoke();
						maths();
						getString();
						
					} else {
						char* val = context->getVar(name)->val->toString();
						frame->stack.push(new Token(stringsym, val));
						maths();
						getString();
					}
					
				}
			}
			
			if ( frame->stack.getLength() > 0 ) {
				char* val = frame->stack.pop()->val;
				// Do the logic!
				this->context->setVar(identifier, val);
			}
		}
		
		increment();
	}
	
	void JSParser::increment() {
		getFrame();
		expect(ident);
		if ( current->sym->sym == plus && current->peek->sym == plus ) {
			accept(plus);
			accept(plus);
			// Increment operator
			if ( current->stack.getLength() > 0 ) {
				char* name = current->stack.pop()->val;
				Object* var = context->getVar(name);
				if ( var != (Object*)NULL){ 
					int val = 0;
					if ( parseInt(var->val, &val)) {
						val++;
						var->val = new string(itoa(val));
					}
				}
			}
		} else if ( current->sym->sym == minus && current->peek->sym == minus ) {
			accept(minus);
			accept(minus);
			if ( current->stack.getLength() > 0 ) {
				char* name = current->stack.pop()->val;
				Object* var = context->getVar(name);
				if ( var != (Object*)NULL){ 
					int val = 0;
					if ( parseInt(var->val, &val)) {
						val--;
						var->val = new string(itoa(val));
					}
				}
			}
		}
	}
	
	bool JSParser::isTrue(string* token) {
		if (token->equals("1")) return true;
		else if ( token->equals("true")) return true;
		return false;
	}

	void JSParser::logic() {
		Token* left = current->stack.pop();	
		Symbol op = oddsym;
		
		if (accept(andsym)) {		
			 if (accept(andsym)) {
				op = andsym;
			}
		} else if (accept(pipesym)) {
			if (accept(pipesym)) {
				op = pipesym;
			}
		} else if (accept(notsym)) {
			if (accept(eql)) {
				op = notsym;
			}
		}
		
		if ( op != oddsym ) {
			if (accept(lparen)) {
				expression();
			} else if (expect(ident)) {
				if ( current->sym->sym == lparen ) {
					invoke();
				}
			} else if (expect(stringsym) || expect(number)) {
				// Do nothing, we're just going to pop them.
			}
			
			Token* right = current->stack.pop();
			
			// && comparison.
			bool l = isTrue(new string(left->val));
			bool r = isTrue(new string(right->val));
			
			if (op == andsym) {
				if ( l && r ) current->stack.push(new Token(stringsym, (char*)"true"));
				else current->stack.push(new Token(stringsym, (char*)"false"));
			} else if (op == pipesym) {
				if ( l || r ) current->stack.push(new Token(stringsym, (char*)"true"));
				else current->stack.push(new Token(stringsym, (char*)"false"));
			} else if ( op == notsym ) {
				if ( l != r ) current->stack.push(new Token(stringsym, (char*)"true"));
				else current->stack.push(new Token(stringsym, (char*)"false"));
			}
		}
		
		if ( current->sym->sym == andsym || current->sym->sym == pipesym || current->sym->sym == notsym )
			logic();
	}

	void JSParser::ifstatement() {
		accept(lparen);
		Stack<bool> parens;
		parens.push(true);
		while(parens.getLength() > 0) {
			if ( accept(lparen)) {
				parens.push(true);
				expression();
			} else if ( accept(rparen)) {
				parens.pop();
			} else if (expect(ident) || expect(stringsym) || expect(number)) {
				logic();
			} else {
				// ERROR CONDITION.
				// This just prevents an infinite loop.
				nextsym();
			}
		}
		
		char* result = current->stack.pop()->val;
		
		List<Token*> tokens;
		parens.push(true);
		if (accept(lbracket)) {
			// We need to execute all the code between the brackets.
			while(parens.getLength() > 0 ) {
				if (accept(lbracket)) {
					parens.push(true);
				} else if (accept(rbracket)) {
					parens.pop();
				} else {
					tokens.add(current->sym);
					nextsym();
				}
			}
			
			// We need to execute this code.
			if ( isTrue(new string(result)))
				allocate(tokens);
		}
	}

	void JSParser::maths() {
		int value = 0;
		char* left = current->stack.pop()->val;
		current->stack.push(new Token(number, left));
		
		if ( parseInt(new string(left), &value) ) {
			Token* token = (Token*)NULL;

			
			if (expect(plus) || expect(minus) || expect(times)) {
				token = current->stack.pop();
				int rightVal = 0;
				bool process = false;
				
				// If we're adding or subtracting a number...
				if (expect(lparen)) {
					// If we get here, it's an order of operations thing.
					expression();
					// We need to handle this properly...
					process = true;
				} else if (expect(ident)) {
					char* varName = current->stack.pop()->val;
					Object* var = context->getVar(varName);
					if ( var != (Object*)NULL ) {
						current->stack.push(new Token(number, var->val->toString()));
						process = true;
					}
				} else if (expect(number)) {
					process = true;
				}
				
				if (process) {
					char* right = current->stack.pop()->val;
					if (parseInt(new string(right), &rightVal)) {		
						switch(token->sym) {
							case plus:
								value += rightVal;
							break;
							case minus:
								value -= rightVal;
							break;
							case times:
								value *= rightVal;
							break;
						}
					}					
				}
			}
			current->stack.push(new Token(number, itoa(value)));
		} 
		
		if (current->sym->sym == plus || current->sym->sym == minus || current->sym->sym == times )
			maths();
		if (accept(lparen))
			expression();
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
				current->stack.push(new Token(ident, name));
				increment();
				
				// Check if it's a method.
				if (this->context->getMethod(name) != (void*)NULL ) {
					Token* token = current->stack.pop();
					current->stack.push(new Token(ident, name));
					invoke();
				} else {
					current->stack.push(new Token(stringsym, name));
				}
				
				// If this is a double equals.
				if (accept(eql)) {
					if (accept(eql)) {
						comparison();					
					}
				} else if (current->sym->sym == andsym || current->sym->sym == pipesym || current->sym->sym == notsym) {
					logic();
				}
			} else if (expect(number)) {
				maths();
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
		
		Token* token = frame->stack.pop();
		if ( token->sym == stringsym || token->sym == number ) {

			string* value = new string(token->val);
			
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
	}

	void JSParser::invoke() {
		StackFrame* frame = this->getFrame();
		if ( frame == (StackFrame*)NULL) return;
		char* method = frame->stack.pop()->val;
		List<Object*> methods;
		accept(lparen);
		char fn_index[] = { '1','2','3','4','5','6','7','8','9' };
		int index = 0;
		
		if ( !accept(rparen)) {
			// While we're not at the rparam, create a 
			// list of arguments.
			while (true) {
			
				if (expect(ident)) {
					
					// Copy the variable.
					char* identName = frame->stack.pop()->val;
					current->stack.push(new Token(ident, identName));
					increment();
					Object* var = context->getVar(identName);
					if ( var == (Object*)NULL ) {
						// Check methods.
						var = (Object*)context->getMethod(identName);
						if ( var == (Object*)NULL ) {
							// Error condition
							break;
						} else {
							methods.add(var);
							continue;
						}
					}
					
					// Check if it's a member.
					if (current->sym->sym == period ) {
						context->setScope(identName);
						membercall();
						methods.add(new Object(var->name->toString(), frame->stack.pop()->val));
						continue;
					}
					
					// Otherwise, it's a variable.	
					char* identVal = var->val->toString();
					frame->stack.push(new Token(var->sym, identVal));
					getString();					
					identVal = current->stack.pop()->val;
					methods.add(new Object(var->name->toString(), identVal));
				} else if (this->expect(stringsym)){ 
					getString();
					methods.add(new Object((char*)"\0", frame->stack.pop()->val));
				} else if (this->expect(number)) {
					maths();
					methods.add(new Object((char*)"\0", frame->stack.pop()->val));
				} else if (accept(functionsym)) {
					// Create a temp variable to store the function
					string* temp = new string("__temp");
					temp->append(fn_index[index++]);
					
					frame->stack.push(new Token(ident, temp->toString()));
					function();
					// Add it
					methods.add((Object*)context->getMethod(temp->toString()));
					
				} else if (accept(thissym)) {
					if ( current->container != (Object*)NULL )
						context->scope = current->container;
					
					char* scopeName = context->scope->name->toString();	
					frame->stack.push(new Token(member, scopeName));
					membercall();
					
					char* val = frame->stack.pop()->val;
					methods.add(new Object((char*)"\0", val));
				} else if (this->accept(comma)) {
					continue;
				} else {
					break;
				}	
			}			
		}
		
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

			void* ref = (void*)this;
			JSDelegate* delegate = (JSDelegate*)target;
			delegate->invoke(ref, (List<char*>)arguments);
			block();
		} else {
			// Invoke the js object.
			Object* object = (Object*)target;
			if ( object != (Object*)NULL ) {
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
					// Update any old references if applicable..
					context->addVar(object);
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
			
			if ( this->current->index > this->current->symbols.getLength() ) {
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

	void JSParser::registerDelegate(const char* identifier, void (*func)(void* p,List<char*> a)) {
		this->context->registerDelegate(identifier, func);
	}
}
