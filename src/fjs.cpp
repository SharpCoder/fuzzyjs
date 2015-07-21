// NOTE: These are just to support
// printf() (only for debugging, not needed in actual program).
// malloc()
// and the new keyword.
#include <stdio.h>
#include <stdlib.h>

// Include the primary include.
#include "fjs.h"

// Some glue functions, for testing.
void js_printf(List<char*> args) {
	if ( args.getLength() > 0 ) {
		printf("[%s]\n", args.getAt(0));
	} else {
		printf("Hello, world\n");
	}
}

int main(void) {
	
	JSParser* parser = new JSParser();
	parser->registerDelegate((char*)"printf", js_printf);
	
	// Parse the javascript
	string* code = new string();
	code->append("var s = ('false' == (1 == 10));");
	code->append("var a = 'hello, ' + 'world!';");
	code->append("printf(a); printf(s);");
	code->append("function doIt(){ var r = 20; printf(r); }");
	code->append("doIt();");
	code->append("doIt();");
	code->append("function myFunc ( first, second ){ printf(first + second); }");
	code->append("myFunc('starcraft ', 'love');");
	code->append("function addLove() { return 'love'; }");
	code->append("var t = (addLove() + 'craft'); printf(t);");
	code->append("var obj = function() { printf('constructor'); };");
	code->append("obj.prototype.method = function(i){ printf('method' + i); };");
	code->append("var instance = new obj();");
	code->append("var other = new obj();");
	code->append("instance.method('1');");
	code->append("other.method('2');");
	code->append("function invoke(callback) { printf('in function'); callback.call(); }");
	code->append("function cd() { printf('callback!'); }");
	code->append("invoke(function() { printf('wat wat'); });");
	
	List<Token*> tokens = tokenize(code->toString());
	// Print some debug information
	for ( int i = 0; i < tokens.getLength(); i++ ) {
		printf(tokens.getAt(i)->val);
	}
	
	printf("\n\n");
	parser->parse(code->toString());
	
	// Terminate program.
	printf("\n");
	return 0;
}

