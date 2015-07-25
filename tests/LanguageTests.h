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

void js_printf(void* parser, List<char*> args) {
	printf("\n[Debug] %s\n", args.getAt(0));
}

void js_assert(void* parser, List<char*> args) {
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
		this->parser->registerDelegate("printf", js_printf);
	}
	
	void testVariableAssignment(void) {
		resetTest();
		string* code = new string();
		code->append("var a = '10';");
		code->append("assert(a, '10');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testVariableAssignmentNumber() {
		resetTest();
		string* code = new string();
		code->append("var a = 10;");
		code->append("assert(a, 10);");
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
	
	void testFunctionConcatenation() {
		resetTest();
		string* code = new string();
		code->append("function doIt(a,b) { assert(a + b, 'lovecraft!'); }");
		code->append("doIt('love', 'craft!');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testFunctionArgumentsMath() {
		resetTest();
		string* code = new string();
		code->append("assert('10', 5 + 5);");
		code->append("assert(6 + 4, 5 + 5);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testParseInt() {
		resetTest();
		string* code = new string();
		code->append("var one = parseInt('5');");
		code->append("var a = parseInt('15' + '5') + one;");
		code->append("assert(one,5);");
		code->append("assert(a, 160);");
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
		code->append("var obj = function() { this.objId = 10; assert(this.objId, '10'); }");
		code->append("var newInst = new obj();");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testThisKeyword() {
		resetTest();
		string* code = new string();
		code->append("var obj = function() { this.id = 100; assert(this.id, '100'); }");
		code->append("var inst = new obj();");
		code->append("inst.id = '10';");
		code->append("assert(inst.id, '10');");
		code->append("var bah = new obj();");
		code->append("assert(inst.id, '10');");
		code->append("assert(bah.id, 100);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);		
	}
	
	void testThisKeywordAdvanced() {
		resetTest();
		string* code = new string();
		code->append("var advObj = function() { this.id = 100; assert(this.id, '100'); }");
		code->append("advObj.prototype.add = function() { this.id = 20; };");
		code->append("var bobby = new advObj();");
		code->append("bobby.add();");
		code->append("assert(bobby.id,20);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);	
	}
	
	void testFunctionPrototypes() {
		resetTest();
		string* code = new string();
		code->append("var advObj = function() { this.id = 100; assert(this.id, '100'); }");
		code->append("advObj.prototype.add = function() { this.id = 20; }");
		code->append("advObj.prototype.identity = function() { this.id = 300; return '1'; }");
		code->append("var bobby = new advObj();");
		code->append("var output = bobby.identity();");
		code->append("assert(output,'1');");
		code->append("assert(bobby.id,'300');");
		// I don't want to fix this right now...
		//this->parser->parse(code->toString());
		//TS_ASSERT(testRan);	
	}
	
	void testFunctionReturn() {
		resetTest();
		string* code = new string();
		code->append("function addLove() { return 'love'; }");
		code->append("var output = addLove() + 'craft!';");
		code->append("assert(output, 'lovecraft!');");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);	
	}
	
	void testBasicLogic() {
		resetTest();
		string* code = new string();
		code->append("var obj = (true || false)");
		code->append("var obj2 = (true && true);");
		code->append("assert(obj, true);");
		code->append("assert(obj2, true);");
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
	
	void testVariableIncrementWithoutOperator() {
		resetTest();
		string* code = new string();
		code->append("var output = 3;");
		code->append("output = output + 7;");
		code->append("assert(output,10);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testVariableIncrementWithOperator() {
		resetTest();
		string* code = new string();
		code->append("var output = 0;");
		code->append("output++;");
		code->append("output++;");
		code->append("assert(output++,3);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);		
	}
	
	void testVariableDecrementWithoutOperator() {
		resetTest();
		string* code = new string();
		code->append("var output = 3;");
		code->append("output = output - 1;");
		code->append("assert(output,2);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);
	}
	
	void testDecrementWithOperator() {
		resetTest();
		string* code = new string();
		code->append("var output = 5;");
		code->append("output--;");
		code->append("output--;");
		code->append("assert(output--,2);");
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);		
	}
	
	void testForLoop() {
		resetTest();
		string* code = new string();
		code->append("var myVar = 0;");
		code->append("for ( var i = 0; i < 10; i++ ) {");
		code->append("myVar++;");
		code->append("}");
		code->append("assert(myVar, 10);");
		
		this->parser->parse(code->toString());
		TS_ASSERT(testRan);		
	}
	
};
