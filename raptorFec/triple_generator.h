/**
 * triple_generator.h Copyright (C) 2009 Vicente Sirvent.
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

#ifndef __TRIPLE_GENERATOR_H__
#define __TRIPLE_GENERATOR_H__

#include "types.h"

struct CTriple
{
	u32 d;
	u32 a;
	u32 b;		
};

class CTripleGenerator
{
public:


private:
	
	static const u32 m_Q = 65521;
	u32 m_K;
	u32 m_L;
	

private:

	inline double Fact(u32 n);
	inline u32    Comb(u32 n, u32 k);
	inline bool   IsPrime(u32 num);

	/* See RFC 5053:5.4.4.1*/
	inline u32    Rand(u32 X, u32 i, u32 m);
	/* See RFC 5053:5.4.4.2*/
	inline u32    Deg(u32 v);
	/* See RFC 5053:5.4.2.3*/
	inline u32    GetL(u32 K);

public:

	/* See RFC 5053:5.4.4.4 */
	CTriple Trip(u32 K, u32 X);
};

#endif
