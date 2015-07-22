#ifndef __FJS_DELEGATE_H_
#define __FJS_DELEGATE_H_

#include "fjs_string.h"

namespace fjs {
	// The method delegate maps a c++ method to a javascript method.
	class JSDelegate {
		private:
			void (*ptr)(List<char*> args);
			
		public:
			string* name;
			JSDelegate();
			JSDelegate(const char* identifier, void (*func)(List<char*> args));
			void invoke(void);
			void invoke(List<char*> args);		
	};

	JSDelegate::JSDelegate(const char* identifier, void (*func)(List<char*> args)) {
		this->name = new string(identifier);
		this->ptr = func;
	}

	void JSDelegate::invoke(List<char*> args) {
		this->ptr(args);
	}
}


#endif
