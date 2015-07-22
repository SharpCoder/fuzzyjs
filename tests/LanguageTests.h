#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include <stdlib.h>
#include "../src/fjs.h"

using namespace fjs;

void js_printf(List<char*> args) {
	if ( args.getLength() > 0 ) {
		printf("[%s]\n", args.getAt(0));
	}
}

void js_assert(List<char*> args) {
	string* arg = new string(args.getAt(0));
	string* tar = new string(args.getAt(1));
	
	TS_ASSERT(strcmp(arg,tar));
}

class LanguageTests : public CxxTest::TestSuite 
{
	JSParser* parser;
	
public:	
	void setUp() {
		this->parser = new JSParser();
		this->parser->registerDelegate("printf", js_printf);
		this->parser->registerDelegate("assert", js_assert);
	}
	
	void testVariableAssignment(void) {
		string* code = new string();
		code->append("var a = '10';");
		code->append("assert(a, '10');");
		this->parser->parse(code->toString());
	}
	
	void testStringConcatenation() {
		string* code = new string();
		code->append("var b = 'world!';");
		code->append("var c = 'hello, ' + b;");
		code->append("assert(c,'hello, world!');");
		this->parser->parse(code->toString());
	}
	
	void testAddition() {
		string* code = new string();
		code->append("var b = 10 + 5;");
		code->append("assert(b,'15');");
		this->parser->parse(code->toString());
	}

	void testFunctions() {
		string* code = new string();
		code->append("function doIt() { assert('true','true'); }");
		code->append("doIt();");
		this->parser->parse(code->toString());
	}
	
	void testArguments() {
		string* code = new string();
		code->append("function doIt(arg1,arg2) { assert(arg1,arg2); }");
		code->append("doIt('10','10');");
		this->parser->parse(code->toString());
	}
	
	void testObjectInstantiation() {
		string* code = new string();
		code->append("var obj = function() { this.id = 10; assert(this.id, '10'); }");
		code->append("var inst = new obj();");
		this->parser->parse(code->toString());
	}
	
	void testBasicLogic() {
		string* code = new string();
		code->append("var obj = (true || false)");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
	}
	
	void testBasicLogicWithExpressions() {
		string* code = new string();
		code->append("var obj = (true && (true || false));");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
	}
	
	void testNotLogic() {
		string* code = new string();
		code->append("var obj = (true && false != true);");
		code->append("assert(obj, true);");
		this->parser->parse(code->toString());
	}
	
	void testCallbacks() {
		string* code = new string();
		code->append("function test(callback) { assert('true','true'); callback.call(false); }");
		code->append("test(function(method){ assert(method, 'false'); }");
		this->parser->parse(code->toString());	
	}
	
};
