#ifndef __FJS_OBJECT_H_
#define __FJS_OBJECT_H_

#include "fjs_string.h"

// The object class is the basis for all things in fuzzyjs.
// It is scope. It is a function. It is self-replicable.
namespace fjs {
	class Object {
		public:
			// These are the tokens that make up the function this
			// object represents. I guess.
			List<Token*> tokens;
			List<char*> arguments;
			List<Object*> members;
			Object* parent;
			
			// The name of this object.
			string* name;
			
			// The value, in case this is a variable?
			string* val;
			
			// Use to store whether string or number.
			Symbol sym;
			
			// Constructors.
			Object() { }
			
			Object(char* identifier, char* value) {
				this->name = new string(identifier);
				this->val = new string(value);
				this->sym = stringsym;
			}
			
			Object(char* identifier, List<Token*> methodTokens) {
				this->name = new string(identifier);
				this->tokens = methodTokens;
				this->sym = stringsym;
			}
			
			Object(char* identifier, List<char*> args, List<Token*> methodTokens) {
				this->name = new string(identifier);
				this->arguments = args;
				this->tokens = methodTokens;
				this->sym = stringsym;
			}
			
			void copy(Object* object) {
				for( int i=0; i<object->arguments.getLength(); i++) this->arguments.add(object->arguments.getAt(i));
				for( int i=0; i<object->tokens.getLength(); i++) this->tokens.add(object->tokens.getAt(i));
				
				// Copy all of the members.
				for( int i=0; i<object->members.getLength(); i++) {
					// Create a new member.
					Object* oldMember = object->members.getAt(i);
					this->members.add(oldMember);
				 }
			}
			
			void addMember(Object* member) {
				for ( int i = 0; i < this->members.getLength(); i++ ) {
					Object* m = this->members.getAt(i);
					if (strcmp(m->name, member->name)) {
						int val = 0;
						if ( parseInt(member->val, &val))
							member->sym = number;
						*m = *member;
						return;
					}
				}
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
}

#endif
