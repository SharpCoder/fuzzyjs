#include "fjs_string.h"
#include "fjs_helper.h"

#ifndef __FJS_FLOAT_H_
#define __FJS_FLOAT_H_

namespace fjs {
	typedef		unsigned long		mint;
	
	class jsfloat {
		private:
			string* val;
			
			mint stripDecimal(string* str, int* places) {
				string* res = new string();
				bool found = false;
				*places = 0;
				for ( int i = 0; i < str->size(); i++ ) {
					if ( str->getAt(i) == '.' ) {
						found = true;
						continue;
					}
					if ( found ) (*places)++;
					res->append(str->getAt(i));
				}
				
				int result = 0;
				parseInt(res, &result);
				return (mint)result;
			}
			
			// how many times does left go into right.
			// The method that actually divides stuff.
			void mod(mint left, mint right, mint* result, mint* remainder) {
				
				while ( left < right ) {
					left *= 10;
				}
				
				*result = 0;
				*remainder = 0;
				mint primary = 0;
				do {
					if ( (primary + right) == left ) {
						*remainder = 0;
						*result = (*result) + 1;
						break;
					} else if ( (primary + right) > left ) {
						*remainder = left - ((*result) * right);
						break;
					} 
					
					primary += right;
					*result = *result + 1;			
				} while (true);
			}
			
		public:
			jsfloat(const char* v) {
				val = new string((char*)v);
			}
			
			jsfloat(char* v) {
				val = new string(v);
			}
			
			char* toString() {
				if ( val != (string*)NULL )
					return val->toString();
				return (char*)"0\0";
			}
			
			jsfloat* divide(jsfloat* right) {
				
				int lplaces = 0, rplaces = 0;
				mint left = stripDecimal(this->val, &lplaces);
				mint numerator = stripDecimal(right->val, &rplaces);
				
				// Normalize the placevalues.
				if ( lplaces > rplaces ) {
					int disparity = (lplaces - rplaces) + 1;
					numerator *= pow(10,disparity);
				} else if ( lplaces < rplaces ) {
					int disparity = (rplaces - lplaces) + 1;
					left *= pow(10, disparity);
				}
				
				mint res = 0, remainder = numerator;
				
				//printf("%d remainder %d of %d\n", res, remainder, numerator);
				
				string* result = new string();
				// Do the calculations.
				mod(left,numerator,&res,&remainder);
				if ( left < numerator ) {
					result->append("0.");
					result->append(itoa(res));
				} else {
					result->append(itoa(res));
					result->append(".");
				}
				for ( int i = 0; i < 5; i++ ) {
					if ( remainder == 0 ) {
						result->append("0");
						break;
					}
					left = remainder;
					mod(left, numerator, &res, &remainder);
					result->append(itoa(res));					
				}
				
				
				return new jsfloat(result->toString());
				
			}
	};
}
#endif
