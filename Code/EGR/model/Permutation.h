#pragma once

class Permutation
{
public:
	static int *first_permutation(int a, int b);
	static bool next_permutation(int a, int b, int *p);
	static bool is_valid_permutation(int a, int b, int *p);
	static void copy_permutation(int *p1, int *p2, int permutation_size);
	Permutation(void);
	~Permutation(void);
};
