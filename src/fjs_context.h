#ifndef __FJS_CONTEXT_H_
#define __FJS_CONTEXT_H_

#include "fjs_object.h"
#include "fjs_delegate.h"

class SystemContext {
	public:
		List<JSDelegate*> delegates;
		List<Variable*> variables;
		List<Object*> methods;
		Variable* getVar(char* identifier);
		void setVar(char* identifier, char* val);
		void setMethod(char* identifier, List<Variable*> args, List<Token*> tokens);
		void registerDelegate(char* identifier, void (*func)(List<char*> args));
		
		bool isJSDelegate(char* methodName);
		void* getMethod(char* methodName);
};

void SystemContext::setMethod(char* identifier, List<Variable*> args, List<Token*> tokens) {
	Object* object = new Object(identifier, args, tokens);
	this->methods.add(object);
}

Variable* SystemContext::getVar(char* identifier) {
	string* name = new string(identifier);
	for ( int i = 0; i < variables.getLength(); i++ ) {
		Variable* var = variables.getAt(i);
		if (strcmp(name, var->name))
			return var;
	}
	return (Variable*)NULL;
}

void SystemContext::setVar(char* identifier, char* val) {
	Variable* newvar = new Variable(identifier, val);
	this->variables.add(newvar);
}

void SystemContext::registerDelegate(char* identifier, void (*func)(List<char*> args)) {
	JSDelegate* delegate = new JSDelegate(identifier, func);
	this->delegates.add(delegate);
}

bool SystemContext::isJSDelegate(char* name) {
	string* methodName = new string(name);
	// First, look for a delegate.
	for ( int i = 0; i < this->delegates.getLength(); i++ ) {
		JSDelegate* delegate = this->delegates.getAt(i);
		if (strcmp(delegate->name, methodName))
			return true;
	}
	return false;
}

void* SystemContext::getMethod(char* name) {
	string* methodName = new string(name);
	// First, look for a delegate.
	for ( int i = 0; i < this->delegates.getLength(); i++ ) {
		JSDelegate* delegate = this->delegates.getAt(i);
		if (strcmp(delegate->name, methodName))
			return (void*)delegate;
	}
	
	// Next, look for a javascript method.
	for ( int i = 0; i < this->methods.getLength(); i++ ) {
		Object* object = this->methods.getAt(i);
		if ( strcmp(object->name, methodName))
			return (void*)object;
	}
	
	return (void*)NULL;
}

#endif
