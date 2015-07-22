#ifndef __FJS_CONTEXT_H_
#define __FJS_CONTEXT_H_

#include "fjs_object.h"
#include "fjs_delegate.h"

namespace fjs {

	class SystemContext {
		public:
			List<JSDelegate*> delegates;
			List<Object*> variables;
			List<Object*> methods;
			Object* scope;
			Object* getVar(char* identifier);
			void addVar(Object* var);
			void setVar(char* identifier, char* val);
			void setMethod(char* identifier, List<char*> args, List<Token*> tokens);
			void registerDelegate(const char* identifier, void (*func)(List<char*> args));
			void setScope(Object* scope);		
			void setScope(char* scope);
			void resetScope(void);
			void resetScope(Object* scope);
			
			bool isJSDelegate(char* methodName);
			void* getMethod(char* methodName);
	};

	void SystemContext::addVar(Object* var) {
		if ( var == (Object*)NULL ) return;
		Object* old = this->getVar(var->name->toString());
		if ( old != (Object*)NULL )
			*old = *var;
		else
			this->variables.add(var);
	}

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
			if ( target == (Object*)NULL ) {
				target = getVar(member);
				if ( target == (Object*)NULL ) {
					target = new Object(member, (char*)"\0");	
				}
			}
			
			scope = target;		
		}
	}

	void SystemContext::setScope(Object* newScope) {
		this->scope = newScope;
	}

	void SystemContext::resetScope(Object* scope) {
		this->setScope(scope);
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
			// Check for existing method.
			for ( int i = 0; i < this->methods.getLength(); i++ ) {
				if ( strcmp(this->methods.getAt(i)->name, object->name)) {
					Object* method = this->methods.getAt(i);
					*method = *object;
					return;
				}
			}
		
			this->methods.add(object);
		}
	}

	Object* SystemContext::getVar(char* identifier) {
		string* name = new string(identifier);

		// Check in the current scope.
		if ( scope != (Object*)NULL ) {
			Object* target = scope->getMember(identifier);
			if ( target != (Object*)NULL ) {
				return target;
			}
			
			if (strcmp(scope->name, name)) {
				return scope;
			}
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
		Object* oldvar = getVar(identifier);
		Object* newvar = new Object(identifier, val);
		
		// Update old references, if applicable.
		if (oldvar != (Object*)NULL) {
			oldvar->val->clear();
			oldvar->val->append(val);
			free(newvar);
		} else {
			this->variables.add(newvar);
		}
	}

	void SystemContext::registerDelegate(const char* identifier, void (*func)(List<char*> args)) {
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
		
		
		// Next, look at variables.
		for (int i =0; i < this->variables.getLength(); i++ ) {
			Object* object = this->variables.getAt(i);
			if ( strcmp(object->name, methodName)) {
				if ( object->tokens.getLength() > 0 )
					return (void*)object;
			}
		}
		
		return (void*)NULL;
	}
}

#endif
