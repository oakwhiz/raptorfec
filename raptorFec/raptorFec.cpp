
#include "defines.h"
#include "triple_generator.h"
#include "encoder.h"
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

int encoder_test(u32 k)
{
	CEncoder encoder(k);

	const u32 DATA_LEN = 1000;
	u8 rndData[DATA_LEN];
	
	for (int i = 0; i < 1000000; ++i)
	{
		for (int i = 0; i < DATA_LEN; ++i)
		{
			rndData[i] = rand();
		}
		CData data(rndData,DATA_LEN);
		encoder.AddData(&data);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	test_triple_generator();
	encoder_test(1024);
	return 0;
}