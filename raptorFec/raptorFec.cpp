/**
 * raptor_fec.h Copyright (C) 2009 Vicente Sirvent.
 * 
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 2.1 of the License, or (at your option) 
 * any later version.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library; 
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * Contact mail : vicentesirvent@hotmail.com
 */

#include "defines.h"
#include "triple_generator.h"
#include "encoder.h"
#include "decoder.h"

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

int decoder_test(u32 k)
{
	CDecoder decoder(k);	
	return 0;
}

int main(int argc, char* argv[])
{
	u32 k = 1024;

	test_triple_generator();

	//encoder_test(k);
	decoder_test(k);
	return 0;
}