#include "eval.h"

void remove_spaces(char* s) {
	int effective_offset = 0;
	char* tmp_ptr = s;
	while (*tmp_ptr != '\n' && *tmp_ptr != '\0') {
		if (*tmp_ptr != ' ' && *tmp_ptr != '\t') {
			*(s + effective_offset) = *tmp_ptr;	
			++effective_offset;
		}
		++tmp_ptr;
	}
	while (*(s + effective_offset) != '\0') {
		*(s + effective_offset) = '\0';	
		++effective_offset;
	}
}

long read_int(char** p_s) {
	// (*p_s) should contain NO space 
	long ret = 0;
	while (isdigit(*(*p_s))) {
		ret *= 10;
		ret += (long)(*(*p_s) - '0');
		++(*p_s);
	}
	return ret;
}

long mod_add(long a, long b, long mod) {
	return (a + b) % mod;
}

long mod_sub(long a, long b, long mod) {
	return (a - b + mod) % mod;
}

long mod_mul(long a, long b, long mod) {
	return (a * b + mod) % mod;
}

long mod_pow(long a, long n, long mod) {
	long ret = 1, tmp = 0;
	while(n){
		if(n & 1)	ret = mod_mul(ret, a, mod); 
		a = mod_mul(a, a, mod);
		n >>= 1;
	}
	return ret;
}

long prime_mod_div(long a, long b, long mod) {
	int n = mod - 2;	
	long tmp =  mod_pow(b, n, mod);
	return mod_mul(a, tmp, mod);
}
