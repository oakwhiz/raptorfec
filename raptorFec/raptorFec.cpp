
#include "defines.h"
#include "triple_generator.h"
#include <stdio.h>

int test_triple_generator(void)
{
	CTripleGenerator gen;
	int k = 1024;
	for (int x = 0 ; x < k; ++x)
	{
		CTriple triple = gen.Trip(k,x);
		printf("<%d,%d> - <%d,%d,%d>\n",k,x,triple.d,triple.a,triple.b);
	}
	return 0;
}


int main(int argc, char* argv[])
{
	test_triple_generator();
	return 0;
}