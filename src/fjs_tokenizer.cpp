#include "fjs_tokenizer.h"
namespace fjs {

	List<Token*> tokenize(char* str) {
		
		string* temp = new string();
		string* code = new string(str);
		List<Token*> result;
		Symbol prev = oddsym;
		
		// Add the script start token.
		result.add(new Token(scriptstartsym, (char*)"\0"));
		
		// Parse the code.
		for ( int i = 0; i < code->size() ; i++ ) {
			// Get the character.
			char c = code->getAt(i);
			Symbol s = convert(prev, new string(c));
			
			// Check for some kind of known symbol.
			if (isFinished(temp, c, s) || i == code->size() - 1) {		
			
				// Skip empty strings.
				if ( temp->size() > 0 ) {
					// Otherwise, convert the string into a symbol and add it.
					prev = convert(prev, temp);
					Token* token = new Token(prev, temp->toString());
					if (!processSymbol(token, &result, code, &i)) {
						prev = token->sym;
						result.add(token);
					}
				}
				
				// Clear stuff out.
				temp->clear();
				
				// If the character is not whitespace and it's not known
				// append it. Otherwise, if it's known, add to result.
				if ( c != ' ' && (s == ident || s == member || s == number) ) {
					temp->append(c);			
				} else if ( s != ident && s != member && s != number) {
					prev = s;
					Token* token = new Token();
					token->sym = prev; token->val = (new string(c))->toString();
					if (!processSymbol(token, &result, code, &i)) {
						prev = token->sym;
						result.add(token);
					}
				}
			} else {
				temp->append(c);
			}
		}
		
		// Add the script terminate token.
		result.add(new Token(scriptendsym, (char*)"\0"));
		return result;
	}

	bool processSymbol(Token* token, List<Token*>* results, string* code, int* index) {
		if ( token->sym == multicommentsym ) {
			(*results).add(doUntil(oddsym, multicommentsym, code, index));
			return true;
		} else if ( token->sym == quotesym ) {
			(*results).add(doUntil(stringsym, quotesym, code, index));
			return true;
		}
		
		// Check if it's a number.
		int value = 0;
		if (parseInt(new string(token->val), &value)) {
			token->sym = number;
			return false;
		}
		
		return false;
	}

	Token* doUntil(Symbol tokenType, Symbol target, string* code, int* index) {
		string* content = new string();
		for ( int i = *index + 1; i < code->size(); i++ ) {
			char c = code->getAt(i);
			char peek = '\0';
			
			if ( i + 1 < code->size() )
				peek = code->getAt(i+1);
			
			// Check for a token.
			string* current = new string(c);
			if ( convert(oddsym, current) == target ) {
				*index = i;
				break;
			}
			
			current->append(peek);
			if ( convert(oddsym, current) == target ) {
				*index = i + 1;
				break;
			}
			
			// Append.
			content->append(c);
		}
		
		return new Token(tokenType, content->toString());
	}

	bool isFinished(string* str, char next, Symbol nxt) {
		if ( next == ' ' ) return true;
		else if ( next == '\n' ) return true;
		else if ( nxt != ident && nxt != member && nxt != number ) return true;
		
		return false;
	}

	Symbol convert(Symbol prev, string* str) {
		
		if ( str->equals("//") ) return singlecommentsym;
		else if ( str->equals("/*") ) return multicommentsym;
		else if ( str->equals("*/") ) return multicommentsym;
		else if ( str->equals("(") ) return lparen;
		else if ( str->equals(")") ) return rparen;
		else if ( str->equals("{") ) return lbracket;
		else if ( str->equals("}") ) return rbracket;
		else if ( str->equals(",") ) return comma;
		else if ( str->equals("=") ) return eql;
		else if ( str->equals("|") ) return pipesym;
		else if ( str->equals("&") ) return andsym;
		else if ( str->equals(">") ) return gtr;
		else if ( str->equals("<") ) return lss;
		else if ( str->equals("+") ) return plus;
		else if ( str->equals("!") ) return notsym;
		else if ( str->equals("-") ) return minus;
		else if ( str->equals("*") ) return times;
		else if ( str->equals(".") ) return period;
		else if ( str->equals(";") ) return semicolon;
		else if ( str->equals("'") ) return quotesym;
		else if ( str->equals("\"") ) return quotesym;
		else if ( str->equals("if") ) return ifsym;
		else if ( str->equals("var") ) return varsym;
		else if ( str->equals("while") ) return whilesym;
		else if ( str->equals("else") ) return elsesym;
		else if ( str->equals("function") ) return functionsym;
		else if ( str->equals("break") ) return breaksym;
		else if ( str->equals("case") ) return casesym;
		else if ( str->equals("catch") ) return catchsym;
		else if ( str->equals("continue") ) return continuesym;
		else if ( str->equals("default") ) return defaultsym;
		else if ( str->equals("delete") ) return deletesym;
		else if ( str->equals("do") ) return dosym;
		else if ( str->equals("finally") ) return finallysym;
		else if ( str->equals("for") ) return forsym;
		else if ( str->equals("in") ) return insym;
		else if ( str->equals("instanceof") ) return instanceofsym;
		else if ( str->equals("new") ) return newsym;
		else if ( str->equals("return") ) return returnsym;
		else if ( str->equals("switch") ) return switchsym;
		else if ( str->equals("this") ) return thissym;
		else if ( str->equals("throw") ) return throwsym;
		else if ( str->equals("try") ) return trysym;
		else if ( str->equals("typeof") ) return typeofsym;
		else if ( str->equals("void") ) return voidsym;
		else if ( str->equals("with") ) return withsym;
		else if ( str->equals("prototype") ) return prototypesym;
		else if ( str->equals("call") && prev == period ) return callsym;
		else if ( str->equals("true") ) return stringsym;
		else if ( str->equals("false") ) return stringsym;
		
		int value = 0;
		if ( prev == period ) return member;
		if (parseInt(str, &value)) return number;
		
		return ident;
	}
}
