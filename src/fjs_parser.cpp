#include "fjs_parser.h"

Token* sym = (Token*)NULL;
List<Token*> symbols;
Stack<Token*> stack;
List<Variable*> variables;
int index = 0;

void addVar(char* identifier, char* val) {
	Variable* newvar = new Variable(identifier, val);
	variables.add(newvar);
}

char* getVar(char* identifier) {
	string* name = new string(identifier);
	for ( int i = 0; i < variables.getLength(); i++ ) {
		Variable* var = variables.getAt(i);
		if (strcmp(name, var->name))
			return var->val->toString();
	}
	return (char*)"\0";
}

int accept(Symbol s) {
	// NULL Check
	if ( sym == (Token*)NULL ) return 0;
	
	// Check if the symbol we're on equals the one we expect.
	if (s == sym->sym) {
		nextsym();
		return 1;
	}
	return 0;
}

// Return whether a symbol matches. If it does, automatically add
// to the stack.
int expect(Symbol s) {
	if ( s == sym->sym ) {
		stack.push(sym);
		nextsym();
		return 1;
	}
	return 0;
}

void nextsym(void) {
	if ( index < symbols.getLength() ) {
		sym = symbols.getAt(index++);
	}
}

void assignment() {		
	if (accept(eql)) {
		// Identifier and it's value.
		char* identifier = stack.pop()->val;
		char* value = sym->val;
		
		// Do the logic!
		addVar(identifier, value);
	}
}

void invoke() {
	char* method = stack.pop()->val;
	List<char*> arguments;
	// While we're not at the rparam, create a 
	// list of arguments.
	while (expect(ident)) {
		// Copy the variable.
		arguments.add(stack.pop()->val);
	}
	accept(rparen);
}

// Any block of code.
void block() {
	// Variable assignment.
	if (accept(varsym)) {
		// Get the identifier.
		if (expect(ident)) {
			// We have a variable name!
			assignment();
		}
	} else if (expect(ident)) {
		
		// Functions
		if (accept(lparen)){
			// Method call?
			invoke();
		}
	} else if (expect(semicolon)) {
		// Line termination.
		
	} else {
		// Compiler error?
		nextsym();
	}
}

void program() {
	accept(scriptstartsym);
	do {
		block();
	} while (sym->sym != scriptendsym);
}

void parse(List<Token*> syms) {
	symbols = syms;
	nextsym();
	program();
}


