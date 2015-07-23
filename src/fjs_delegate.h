#ifndef __FJS_DELEGATE_H_
#define __FJS_DELEGATE_H_

#include "fjs_string.h"

namespace fjs {
	// The method delegate maps a c++ method to a javascript method.
	class JSDelegate {
		private:
			void (*ptr)(void* caller, List<char*> args);
			
		public:
			string* name;
			JSDelegate();
			JSDelegate(const char* identifier, void (*func)(void* parser, List<char*> args));
			void invoke(void* p, List<char*> a);		
	};

	JSDelegate::JSDelegate(const char* identifier, void (*func)(void* parser, List<char*> args)) {
		this->name = new string(identifier);
		this->ptr = func;
	}

	void JSDelegate::invoke(void* parser, List<char*> args) {
		this->ptr(parser, args);
	}
}


#endif
