/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_LanguageTests_init = false;
#include "/home/joshcole/0xrpi/fuzzyjs/tests/LanguageTests.h"

static LanguageTests suite_LanguageTests;

static CxxTest::List Tests_LanguageTests = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_LanguageTests( "./tests/LanguageTests.h", 30, "LanguageTests", suite_LanguageTests, Tests_LanguageTests );

static class TestDescription_suite_LanguageTests_testVariableAssignment : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testVariableAssignment() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 41, "testVariableAssignment" ) {}
 void runTest() { suite_LanguageTests.testVariableAssignment(); }
} testDescription_suite_LanguageTests_testVariableAssignment;

static class TestDescription_suite_LanguageTests_testVariableAssignmentNumber : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testVariableAssignmentNumber() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 50, "testVariableAssignmentNumber" ) {}
 void runTest() { suite_LanguageTests.testVariableAssignmentNumber(); }
} testDescription_suite_LanguageTests_testVariableAssignmentNumber;

static class TestDescription_suite_LanguageTests_testStringConcatenation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testStringConcatenation() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 59, "testStringConcatenation" ) {}
 void runTest() { suite_LanguageTests.testStringConcatenation(); }
} testDescription_suite_LanguageTests_testStringConcatenation;

static class TestDescription_suite_LanguageTests_testAddition : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testAddition() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 69, "testAddition" ) {}
 void runTest() { suite_LanguageTests.testAddition(); }
} testDescription_suite_LanguageTests_testAddition;

static class TestDescription_suite_LanguageTests_testFunctions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testFunctions() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 78, "testFunctions" ) {}
 void runTest() { suite_LanguageTests.testFunctions(); }
} testDescription_suite_LanguageTests_testFunctions;

static class TestDescription_suite_LanguageTests_testFunctionConcatenation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testFunctionConcatenation() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 87, "testFunctionConcatenation" ) {}
 void runTest() { suite_LanguageTests.testFunctionConcatenation(); }
} testDescription_suite_LanguageTests_testFunctionConcatenation;

static class TestDescription_suite_LanguageTests_testFunctionArgumentsMath : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testFunctionArgumentsMath() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 96, "testFunctionArgumentsMath" ) {}
 void runTest() { suite_LanguageTests.testFunctionArgumentsMath(); }
} testDescription_suite_LanguageTests_testFunctionArgumentsMath;

static class TestDescription_suite_LanguageTests_testParseInt : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testParseInt() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 105, "testParseInt" ) {}
 void runTest() { suite_LanguageTests.testParseInt(); }
} testDescription_suite_LanguageTests_testParseInt;

static class TestDescription_suite_LanguageTests_testArguments : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testArguments() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 114, "testArguments" ) {}
 void runTest() { suite_LanguageTests.testArguments(); }
} testDescription_suite_LanguageTests_testArguments;

static class TestDescription_suite_LanguageTests_testObjectInstantiation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testObjectInstantiation() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 123, "testObjectInstantiation" ) {}
 void runTest() { suite_LanguageTests.testObjectInstantiation(); }
} testDescription_suite_LanguageTests_testObjectInstantiation;

static class TestDescription_suite_LanguageTests_testThisKeyword : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testThisKeyword() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 132, "testThisKeyword" ) {}
 void runTest() { suite_LanguageTests.testThisKeyword(); }
} testDescription_suite_LanguageTests_testThisKeyword;

static class TestDescription_suite_LanguageTests_testThisKeywordAdvanced : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testThisKeywordAdvanced() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 146, "testThisKeywordAdvanced" ) {}
 void runTest() { suite_LanguageTests.testThisKeywordAdvanced(); }
} testDescription_suite_LanguageTests_testThisKeywordAdvanced;

static class TestDescription_suite_LanguageTests_testFunctionPrototypes : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testFunctionPrototypes() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 158, "testFunctionPrototypes" ) {}
 void runTest() { suite_LanguageTests.testFunctionPrototypes(); }
} testDescription_suite_LanguageTests_testFunctionPrototypes;

static class TestDescription_suite_LanguageTests_testBasicLogic : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testBasicLogic() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 172, "testBasicLogic" ) {}
 void runTest() { suite_LanguageTests.testBasicLogic(); }
} testDescription_suite_LanguageTests_testBasicLogic;

static class TestDescription_suite_LanguageTests_testBasicLogicWithExpressions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testBasicLogicWithExpressions() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 183, "testBasicLogicWithExpressions" ) {}
 void runTest() { suite_LanguageTests.testBasicLogicWithExpressions(); }
} testDescription_suite_LanguageTests_testBasicLogicWithExpressions;

static class TestDescription_suite_LanguageTests_testNotLogic : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testNotLogic() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 192, "testNotLogic" ) {}
 void runTest() { suite_LanguageTests.testNotLogic(); }
} testDescription_suite_LanguageTests_testNotLogic;

static class TestDescription_suite_LanguageTests_testCallbacks : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_LanguageTests_testCallbacks() : CxxTest::RealTestDescription( Tests_LanguageTests, suiteDescription_LanguageTests, 201, "testCallbacks" ) {}
 void runTest() { suite_LanguageTests.testCallbacks(); }
} testDescription_suite_LanguageTests_testCallbacks;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
