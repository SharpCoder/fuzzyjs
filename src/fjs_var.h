#ifndef __FJS_VAR_H_
#define __FJS_VAR_H_

class Variable {
	public:
		string* name;
		string* val;
		Variable() {}
		Variable(char* n, char* v) {
			this->name = new string(n);
			this->val = new string(v);
		}
};

#endif
