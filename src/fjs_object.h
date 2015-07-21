#ifndef __FJS_OBJECT_H_
#define __FJS_OBJECT_H_

#include "fjs_string.h"

// The object class is the basis for all things in fuzzyjs.
// It is scope. It is a function. It is self-replicable.

class Object {
	public:
		// These are the tokens that make up the function this
		// object represents. I guess.
		List<Token*> tokens;
		List<char*> arguments;
		List<Object*> members;
		
		// The name of this object.
		string* name;
		
		// The value, in case this is a variable?
		string* val;
		
		// Constructors.
		Object() { }
		
		Object(char* identifier, char* value) {
			this->name = new string(identifier);
			this->val = new string(value);
		}
		
		Object(char* identifier, List<Token*> methodTokens) {
			this->name = new string(identifier);
			this->tokens = methodTokens;
		}
		
		Object(char* identifier, List<char*> args, List<Token*> methodTokens) {
			this->name = new string(identifier);
			this->arguments = args;
			this->tokens = methodTokens;
		}
		
		void copy(Object* object) {
			for( int i=0; i<object->arguments.getLength(); i++) this->arguments.add(object->arguments.getAt(i));
			for( int i=0; i<object->members.getLength(); i++) this->members.add(object->members.getAt(i));
			for( int i=0; i<object->tokens.getLength(); i++) this->tokens.add(object->tokens.getAt(i));
		}
		
		void addMember(Object* member) {
			this->members.add(member);
		}
		
		Object* getMember(char* identifier) {
			string* memberName = new string(identifier);
			
			for ( int i = 0; i < this->members.getLength(); i++ ) {
				Object* member = this->members.getAt(i);
				if ( strcmp(member->name, memberName)) {
					return member;
				}
			}
			
			return (Object*)NULL;
		}
};

#endif
