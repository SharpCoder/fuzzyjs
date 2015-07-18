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
	List<Token*> tokens = tokenize((char*)"var s = ('true'== (0 == 1)); printf(s);function doIt(){ var r = 20; printf(r); } doIt();");
	
	// Print some debug information
	for ( int i = 0; i < tokens.getLength(); i++ ) {
		printf(tokens.getAt(i)->val);
	}
	
	printf("\n\n");
	parser->parse(tokens);
	
	// Terminate program.
	printf("\n");
	return 0;
}

