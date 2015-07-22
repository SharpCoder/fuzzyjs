#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include <stdlib.h>
#include "../src/fjs.h"

using namespace fjs;

// This may be silly, but it works. Since the TS_ASSERT happens
// in the parser, we need to have a way outside the parser to make
// sure that the test was actually ran. Otherwise the code may
// not even be ASSERTING anything.
static bool testRan = false;
void resetTest() {
	testRan = false;
}

void js_assert(List<char*> args) {
	string* arg = new string(args.getAt(0));
	string* tar = new string(args.getAt(1));
	
	TS_ASSERT(strcmp(arg,tar));
	testRan = true;
}

class LanguageTests : public CxxTest::TestSuite 
{
	JSParser* parser;
	
public:	
	void setUp() {
		this->parser = new JSParser();
		this->parser->registerDelegate("assert", js_assert);
	}
	
	void testVariableAssignment(void) {
		resetTest();
		string* code = new string();
		code->append("var a = '10';");
		code->append("assert(a, '10');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testStringConcatenation() {
		resetTest();
		string* code = new string();
		code->append("var b = 'world!';");
		code->append("var c = 'hello, ' + b;");
		code->append("assert(c,'hello, world!');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testAddition() {
		resetTest();
		string* code = new string();
		code->append("var b = 10 + 5;");
		code->append("assert(b,'15');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}

	void testFunctions() {
		resetTest();
		string* code = new string();
		code->append("function doIt() { assert('true','true'); }");
		code->append("doIt();");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testArguments() {
		resetTest();
		string* code = new string();
		code->append("function doIt(arg1,arg2) { assert(arg1,arg2); }");
		code->append("doIt('10','10');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testObjectInstantiation() {
		resetTest();
		string* code = new string();
		code->append("var obj = function() { this.id = 10; assert(this.id, '10'); }");
		code->append("var inst = new obj();");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testBasicLogic() {
		resetTest();
		string* code = new string();
		code->append("var obj = (true || false)");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testBasicLogicWithExpressions() {
		resetTest();
		string* code = new string();
		code->append("var obj = (true && (true || false));");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testNotLogic() {
		resetTest();
		string* code = new string();
		code->append("var obj = (true && false != true);");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testCallbacks() {
		resetTest();
		string* code = new string();
		code->append("function test(callback) { assert('true','true'); callback.call(false); }");
		code->append("test(function(method){ assert(method, 'false'); });");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
};
