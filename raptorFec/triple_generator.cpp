/**
 * triple_generator.cpp Copyright (C) 2009 Vicente Sirvent.
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
#include "tables.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"

/* See RFC 5053:5.4.4.4 */
CTriple 
CTripleGenerator::Trip(u32 K, u32 X)
{
	CTriple res;
	if (m_K != K)
	{
		m_K = K;
		m_L = GetLPrim(K);
		DBG("CTripeGenerator::K = %d, L = %d\n",m_K,m_L);		
	}
	u32 A = (53591 + J[K]*977) % m_Q;
	u32 B = (10267*J[K] + 1) % m_Q;
	u32 Y = (B + X*A) % m_Q;
	u32 v = Rand(Y, 0, (u32)pow(2.0f,20.0f));
	res.d = Deg(v);
	res.a = 1 + Rand(Y, 1, m_L - 1);
	res.b = Rand(Y, 2, m_L);
	return res;
}
