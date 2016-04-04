#include <cstring>
#include "eval.h"

#define MX 1024
#define MOD 1000000007

/**
 * The following rules are of left-associative evaluation.
 * 
 * exp := |term
 * 	|term [+ OR -] term 
 * 	|term [+ OR -] term [+ OR -] term ... 
 *
 * term := |factor
 * 	|factor [* OR /] factor  
 * 	|factor [* OR /] factor [* OR /] factor ...  
 *
 * factor := |number
 * 	|[+ OR -] number
 * 	|(exp)
 *	|[+ OR -] (exp)
 *
 * It's intended that regular-expression notation is NOT used to describe the rules.
 * */

long left_associative_eval(char** p_s, int priority) {
	// (*p_s) should contain NO space 
	
	// The only subtlety to notice is that when evaluating the current component at ANY PRIORITY with UNKNOWN LENGTH, this function will finally set (*p_s) to the EXCLUSIVE END of the component.
	//
	// More quantitatively, if the current component possesses [(*p_s) + 0, (*p_s) + component_length - 1] inclusively, this function will finally set (*p_s) = (*p_s) + component_length.    
	//
	// For example,
	// if starts with "1024*768 ..." at FACTOR priority, it ends with "1024*768 ..."
	// 		   ^				                       ^
	// if starts up with "(7+8-2*3)* ..." at FACTOR priority, it ends up with "(7+8-2*3)* ..." 
	// 		      ^						                    ^				
	// if starts up with "1024*768/16+256 ..." at TERM priority, it ends up with "1024*768/16+256 ..." 
	// 		      ^						                         ^				
	// if starts up with "(7+8-2*3)*32/(5-2*3)- ..." at TERM priority, it ends up with "(7+8-2*3)*32/(5-2*3)- ..." 
	// 		      ^						                                        ^				
	long ret;
	long tmp;
	char binary_op = '\0';

	switch (priority) {
		case EXPRESSION: {
			// evaluate the first term
			ret = left_associative_eval(p_s, TERM);

			binary_op = *(*p_s);
				
			while (binary_op != '\0') {	
				switch (binary_op) {
					case '+': {
						++(*p_s);
						tmp = left_associative_eval(p_s, TERM);	
						ret = mod_add(ret, tmp, MOD);
						break;
					}
					case '-': {
						++(*p_s);
						tmp = left_associative_eval(p_s, TERM);	
						ret = mod_sub(ret, tmp, MOD);
						break;
					}
					default: {
						return ret;
					}
				}
				binary_op = *(*p_s);
			}

			break;
		}
		case TERM: {
			// evaluate the first factor 
			ret = left_associative_eval(p_s, FACTOR);

			binary_op = *(*p_s);

			while (binary_op != '\0') {	
				switch (binary_op) {
					case '*': {
						++(*p_s);
						tmp = left_associative_eval(p_s, FACTOR);
						ret = mod_mul(ret, tmp, MOD);
						break;
					}
					case '/': {
						++(*p_s);
						tmp = left_associative_eval(p_s, FACTOR);
						ret = prime_mod_div(ret, tmp, MOD);
						break;
					}
					default: {
						return ret;
					}
				}	
				binary_op = *(*p_s); 
			}

			break;	
		}
		case FACTOR: {
			int unary_factor = 1;
			char possible_unary_op = *(*p_s);
			if (possible_unary_op == '+' || possible_unary_op == '-') ++(*p_s);  
			if (possible_unary_op == '-') unary_factor = (-1); 
			if (isdigit(*(*p_s))) ret = read_int(p_s); 	
			else {
				++(*p_s);
				ret = left_associative_eval(p_s, EXPRESSION);
				++(*p_s);
			}
			ret *= unary_factor;
			break;
		}
	}
	return ret;
}

int main() {
	char in_s[MX];
	while (fgets(in_s, MX, stdin)) {
		char* s = in_s;
		remove_spaces(s);
		long result = left_associative_eval(&s, EXPRESSION);
		printf("%ld\n", result);
		bzero(in_s, sizeof in_s);	
	}
	return 0;
}


/**
 *  sample in:
 *
 *  	1 + 	1	+        1
 *	1	     +   1     *		2	      +    2	* (	3   +5 *	 4       		)
 *	1	 +  2			*   			3
 *	55 +		68   *				(			5 *	72		+ 32 * 3 *1*1 *2* 1*1*				(4 		+    5		*	(					7+3+2+3		*4*(		5+2*3*2    			)*5*5 )	)			  )
 *
 *	sample out:
 *
 *	3
 *	49
 *	7
 *  	333788119
 *
 * */
