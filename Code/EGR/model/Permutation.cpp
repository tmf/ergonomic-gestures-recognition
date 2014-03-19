#include "stdafx.h"
#include "Permutation.h"

int *Permutation::first_permutation(int a, int b){
	int *result = new int[b];
	for (int i=0; i<b; i++) {
		result[i]=i;
	}
	return result;
}
bool Permutation::is_valid_permutation(int a, int b, int *p){
	bool result = true;
	int max = a<b?b:a;
	int min = a<b?a:b;
	int *test = p;
	int *probe;
	if (min>1) {
		probe = test+1;
		while (result && test < (p + min-1)) {
			result = result && *test!=*probe && *test < max && *probe < max;
			probe++;
			if (probe >= p+min) {
				test++;
				probe = test +1;
			}
		}
	}else {
		result = *test < max;
	}

	return result;
}
bool Permutation::next_permutation(int a, int b, int *p){
	if (b>a) {
		return next_permutation(b, a, p);
	}else {
		bool v = false;
		int max = a<b?b:a;
		int min = a<b?a:b;
		int *i = p+(min-1);
		while (*i<max) {
			(*i)++;
			v=is_valid_permutation(a, b, p);
			if (v) {
				break;
			}
		}
		if (!v && *p!=max) {
			*i=0;
			next_permutation(a, b-1, p);
			if(!is_valid_permutation(a, b, p)) {
				next_permutation(a, b, p);
			}
		}
		return is_valid_permutation(a, b, p);
	}
}
void Permutation::copy_permutation(int *p1, int *p2, int permutation_size){
	for(int i=0; i<permutation_size; i++){
		p2[i] = p1[i];
	}
}
Permutation::Permutation(void)
{
}

Permutation::~Permutation(void)
{
}
