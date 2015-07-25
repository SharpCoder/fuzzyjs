#ifndef __FJS_HELPER_H_
#define __FJS_HELPER_H_

namespace fjs {

	// These are all the symbols our parser will support.
	typedef enum { 
		ident, number, methodsym, lparen, rparen, times, slash, plus, minus, eql, neq, 
		lss, leq, gtr, geq, quotesym, semicolon, endsym, ifsym, stringsym,
		whilesym, rbracket, functionsym, comma, varsym, lbracket, period,
		member, oddsym, singlecommentsym, multicommentsym,
		breaksym, casesym, catchsym, continuesym, defaultsym, deletesym, dosym,
		elsesym, finallysym, forsym, insym, instanceofsym, newsym, returnsym,
		switchsym, thissym, throwsym, trysym, typeofsym, voidsym, withsym,
		notsym, andsym, pipesym, prototypesym, callsym, scriptstartsym, scriptendsym
	} Symbol;

	// Some datatypes.
	typedef		unsigned long		ulong;
	typedef		unsigned int		uint32;
	typedef		volatile unsigned long int	uint32_t;
	typedef		unsigned short		uint16;
	typedef		unsigned char		byte;

	// A list datastructure.
	template<class T>
	class iterator {
		public:
			T val;
			iterator* prev;
			iterator(iterator* previous, T value ) {
				this->val = value;
				this->prev = previous;
			}
			
			iterator* next() {
				if ( this->prev == NULL ) return NULL;
				return this->prev;
			}
			
			T getVal() {
				return this->val;
			}
	};

	template<class T>
	class Stack {
		private:
			iterator<T>* top;
			int length;
			
		public:	
			Stack() {
				this->length = 0;
			}
			
			void push(T val) {
				iterator<T>* next = new iterator<T>( this->top, val );
				this->top = next;
				this->length++;
			}
			
			T pop( void ) {
				// NULL Checking
				if ( this->top == NULL ) return NULL;
				
				// Do the pop
				T result = this->top->getVal();
				this->top = this->top->next();
				this->length--;
				
				// Return the result
				return result;
			}
			
						
			T getAt(int index) {
				if ( this->top == NULL ) return (T)NULL;
				
				if ( index == 0 ) {
					return this->top->val;
				} else {
					iterator<T>* last = this->top;
					do {
						last = last->prev;
						index--;
					} while ( last->prev != NULL && index > 0 );
					return last->val;
				}
			}
			
			int getLength() {
				return this->length;
			}
			
			iterator<T>* getIterator() {
				return this->top;
			}
	};

	template<class T>
	class List {
		public:
			iterator<T>* first;
			uint32 length;
			
			List() {
				this->first = NULL;
				this->length = 0;
			}
		
			void add(T val) {
				// Allocate it.
				
				iterator<T>* newItem = new iterator<T>(NULL, val);
				iterator<T>* last = this->first;
				if ( last == NULL ) {
					this->first = newItem;
				} else if ( last->prev == NULL ) {
					this->first->prev = newItem;
				} else {			
					do {
						last = last->prev;
					} while ( last->prev != NULL );
					last->prev = newItem;
				}
				
				this->length++;
			}
			
			T getAt(int index) {
				if ( this->first == NULL ) return (T)NULL;
				
				if ( index == 0 ) {
					return this->first->val;
				} else {
					iterator<T>* last = this->first;
					do {
						last = last->prev;
						index--;
					} while ( last->prev != NULL && index > 0 );
					return last->val;
				}
			}
			
			T pop() {
				volatile iterator<T>* last = this->first;
				do {
					last = last->prev;
				} while ( last->prev != NULL );
				volatile iterator<T>* result = last->prev;
				last->prev = NULL;
				return result->val;
			}
			
			int getLength() {
				return this->length;
			}
			
			iterator<T>* getIterator() {
				return this->first;
			}
	};

	// Linked list structure/class
	class LinkedList {
	public:
		LinkedList* next;
		LinkedList* previous;
		uint32 value;
		
		// Public
		LinkedList() {
			
		}

		LinkedList* GetNext() {
			return this->next;
		}
		
		LinkedList* GetPrev() {
			return this->previous;
		}
		
		void Add( LinkedList* item ) {
			LinkedList* end;
			// Traverse the linked list.
			while( end->next ) { end = end->next; }
			// Add to the end.
			end->next = item;
		}
		
		void Remove() {
			
			if ( this->next && this->previous ) {
				// Link em.
				this->previous->next = this->next;
				this->next->previous = this->previous;
				// If there is a previous link...
			} else if ( this->previous ) {
				// We don't have any forward link.
				this->previous->next = 0;
			} else if ( this->next ) {
				// well...
				this->value = this->next->value;
				this->next = this->next->next;
				this->previous = 0;
			}
			
		}
	};

	int pow(int base, int power) {
		if ( power <= 1 ) return 1;
		int result = 1;
		for ( int i = 0; i < power - 1; i++ ) {
			result *= base;
		}
		return result;
	}

	char* itoa(uint32 number) {
		char digits = 0;
		do { } while ( digits < 10 && (number / pow(10, (int)digits++)) != 0 );

		char map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		char* res = (char*)malloc((digits+1) * sizeof(char));
		res[digits] = '\0';
		int base = 0;

		// Iterate over the digits;
		for ( int i = digits - 1; i > 1; i-- ) {

			int index;
			if ( i == 0 )
				index = number - base;
			else
				index = (number - base) / pow(10,i-1);
			
			if ( index < 10 && index >= 0)
				res[digits - i - 1] = map[index];
				
			if ( i == 0 ) break;
			base += index * pow(10,i-1);
		}

		// Return the result.
		return res;
	}

	bool parseInt(string* str, int* result) {
		*result = 0;
		char map[] = { '0','1','2','3','4','5','6','7','8','9' };
		int place = 0, index = 0, mapIndex = 0;
		
		for ( ; index < str->size() - 1; index++ ) {
			place = pow(10, (str->size() - index - 1));
			char c = str->getAt(index);
			mapIndex = -1;
			for ( int r = 0; r < 10; r++ ) {
				if ( c == map[r] ) {
					mapIndex = r;
					break;
				 }
			}
			if ( mapIndex == -1 ) {
				return false;
			}
			
			*result += (mapIndex * place);
		}
		
		return true;
	}
}

#endif
