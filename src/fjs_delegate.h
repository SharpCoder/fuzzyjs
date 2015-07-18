#ifndef __FJS_DELEGATE_H_
#define __FJS_DELEGATE_H_

#include "fjs_string.h"

// The method delegate maps a c++ method to a javascript method.
class JSDelegate {
	private:
		void (*ptr)(List<Variable*> args);
		
	public:
		string* name;
		JSDelegate();
		JSDelegate(char* identifier, void (*func)(List<Variable*> args));
		void invoke(void);
		void invoke(List<Variable*> args);		
};

JSDelegate::JSDelegate(char* identifier, void (*func)(List<Variable*> args)) {
	this->name = new string(identifier);
	this->ptr = func;
}

void JSDelegate::invoke(void) {
	List<Variable*> empty;
	this->ptr(empty);
}

void JSDelegate::invoke(List<Variable*> args) {
	this->ptr(args);
}

#endif
