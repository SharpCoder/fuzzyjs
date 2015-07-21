#ifndef __FJS_CONTEXT_H_
#define __FJS_CONTEXT_H_

#include "fjs_object.h"
#include "fjs_delegate.h"

class SystemContext {
	public:
		List<JSDelegate*> delegates;
		List<Object*> variables;
		List<Object*> methods;
		Object* scope;
		Object* getVar(char* identifier);
		void setVar(char* identifier, char* val);
		void setMethod(char* identifier, List<char*> args, List<Token*> tokens);
		void registerDelegate(char* identifier, void (*func)(List<char*> args));
		void setScope(Object* scope);		
		void setScope(char* scope);
		void resetScope(void);
		
		bool isJSDelegate(char* methodName);
		void* getMethod(char* methodName);
};

void SystemContext::setScope(char* member) {
	// Update scope.
	if ( this->scope == (Object*)NULL ) {
		Object* target = this->getVar(member);
		if ( target == (Object*)NULL)
			target = new Object(member, (char*)"\0");
		scope = target;
		return;
	} else {
		// Look inside the current scope.
		Object* target = scope->getMember(member);
		if ( target == (Object*)NULL )
			target = new Object(member, (char*)"\0");
		
		scope = target;		
	}
}

void SystemContext::setScope(Object* scope) {
	this->scope = scope;
}

void SystemContext::resetScope(void) {
	this->setScope((Object*)NULL);
}

void SystemContext::setMethod(char* identifier, List<char*> args, List<Token*> tokens) {
	Object* object = new Object(identifier, args, tokens);
	
	// If we're in a scope, this becomes part of the member.
	if ( scope != (Object*)NULL ) {
		scope->addMember(object);
		for (int i =0; i < this->variables.getLength(); i++ ) {
			if ( strcmp(this->variables.getAt(i)->name, scope->name)) {
				// Update the reference.
				Object* ptr = this->variables.getAt(i);
				*ptr = *scope;
			}
		}
	} else {
		this->methods.add(object);
	}
}

Object* SystemContext::getVar(char* identifier) {
	string* name = new string(identifier);

	// Check in the current scope.
	if ( scope != (Object*)NULL ) {
		if ( strcmp(scope->name, name)) return scope;
	}
	
	// Check in the variable register.
	for ( int i = 0; i < variables.getLength(); i++ ) {
		Object* var = variables.getAt(i);
		if (strcmp(name, var->name))
			return var;
	}

	return (Object*)NULL;
}

void SystemContext::setVar(char* identifier, char* val) {
	Object* newvar = new Object(identifier, val);
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
	
	// Next look in the current scope.
	if ( this->scope != (Object*)NULL ) {
		Object* object = scope->getMember(methodName->toString());
		if ( object != (Object*)NULL )
			return (void*)object;
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
