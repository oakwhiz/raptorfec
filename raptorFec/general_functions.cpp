/**
 * general_functions.cpp Copyright (C) 2009 Vicente Sirvent.
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
#include "general_functions.h"
#include "tables.h"
#include <limits.h>
#include <math.h>

double Fact(u32 n)
{
    if (n == 0) return 1.0;
    double res = n;
    for (u32 i = n - 1 ; i > 0 ; --i)
    {
        res *= i;
    }
    return res;    
}

u32 Comb(u32 n, u32 k)
{
    u32 res = (u32)(Fact(n)/(Fact(k)*Fact(n-k)));    
	return res;
}

bool IsPrime(u32 num)
{
	// no number can divide into 0
	if(num == 0)
		return true;

	for(int i = 2; i <= (long)sqrt((float)num); i++)
	{
		if(num % i == 0)
		{
			return false;
		}
	}
	return true;   
}

/* See RFC 5053:5.4.2.3*/
u32 GetL(u32 K)
{
	u32 X,S,H;
	/* See RFC 5053:5.4.2.3 */
	// Get X value
	for (X = 0; X < UINT_MAX; ++X)
	{
		if (X*(X-1) >= 2*K)
		{
			DBG("CTripeGenerator::New X = %d\n",X);
			break;
		}
	}
	//Get S value
	for (S = 0; S < UINT_MAX; ++S)
	{
		if ((S >= ceil(0.01*(double)K) + X) && IsPrime(S))
		{
			DBG("CTripeGenerator::New S = %d\n",S);
			break;
		}
	}
	//Get H value
	for (H = 0; H < UINT_MAX; ++H)
	{
		if (Comb(H,(long)ceil((double)H/2.0)) >= K + S)
		{
			DBG("CTripeGenerator::New H = %d\n",H);
			break;
		}
	}
	u32 L = K+S+H;
	while(!IsPrime(L))
	{
		L++;
	}
	return L;
}

/* See RFC 5053:5.4.4.2*/
u32 Deg(u32 v)
{	
	for (u32 j = 1; j < f_len; ++j)
	{
		if (f[j - 1] <= v && v <= f[j])
		{
			return d[j];
		}
	}
	ERR("CTripleGenerator::Deg: Can't find valid Degree! aborting...\n");
	return 0;
}

/* See RFC 5053:5.4.4.1*/
u32 Rand(u32 X, u32 i, u32 m)
{	
	return (V0[(X + i) % 256] ^ V1[(X/256 + i) % 256]) % m;
}
