/**
 * encoder.cpp Copyright (C) 2009 Vicente Sirvent.
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

#include "encoder.h"
#include "general_functions.h"
#include <math.h>

CEncoder::CEncoder(u32 K)
{
	m_K = K;
	m_S = GetS(K);
	m_H = GetH(K,m_S);
	m_Hp = GetHp(m_H);
	m_L = GetL(K);
	m_Lp = GetLPrim(K);
	m_Count = 0;
	//Load triples
	m_Triples = new CTriple[m_L];
	CTripleGenerator triple_gen;
	for (u32 x = 0; x < m_L; ++x)
	{
		m_Triples[x] = triple_gen.Trip(m_K,x);
	}
	//alloc structs for data
	m_Data = new CData[m_L];	

	//Fill m sequence
	u32 mseq_len = m_K + m_S - 1;
	m_Mseq = new u32[mseq_len];
	for (u32 mcount = 0, i = 0; mcount < mseq_len; ++i)
	{
		u32 g = i^(i/2);
		if (NonZeroBits(g) == m_Hp)
		{
			m_Mseq[mcount] = g;
			mcount++;
		}
	}
}

CEncoder::~CEncoder(void)
{
	delete[] m_Triples;
	delete[] m_Data;
}

queue<CData*> 
CEncoder::LTEnc(u32 k, CData * inter_sym, CTriple * triples)
{
	queue<CData*> encoded_sym;
	for (u32 i = 0; i < m_L; ++i)
	{
		u32 a = triples[i].a;
		u32 b = triples[i].b;
		u32 d = triples[i].d;
		CData* result = new CData();
		while (b >= m_L)
		{
			b = (b + a) % m_Lp;
		}
		result->SetData(inter_sym + b);
		u32 jend = MIN(d,m_L);
		for (u32 j = 1; j < jend;++j)
		{
			b = (b + a) % m_Lp;
			while (b >= m_L)
			{
				b = (b + a) % m_Lp;
			}
			result->XorData(inter_sym + b);
		}
		encoded_sym.push(result);
	}
	return encoded_sym;
}

queue<CData*> 
CEncoder::AddData(CData * source)
{
	m_Data[m_Count] = *source;	

	/* See RFC 5053:5.4.2.3 */

	//Fill S symbols
	u32 a = 1 + (m_Count/m_S % (m_S-1));
	u32 b = m_Count % m_S;
	m_Data[m_K + b].XorData(source);
	b = (b + a) % m_S;
	m_Data[m_K + b].XorData(source);
	b = (b + a) % m_S;
	m_Data[m_K + b].XorData(source);

	//Fill H symbols
	for (u32 h = 0; h < m_H; ++h)
	{
		if (IsBitSet(m_Mseq[m_Count],h))
		{
			m_Data[m_K + m_S + h].XorData(source);
		}
	}

	m_Count++;	

	queue<CData*> data;	
	if (m_Count == m_K) //we have all source symbols!!
	{
		//Fill the rest H symbols
		for (u32 h = 0; h < m_H; ++h)
		{
			for (u32 j = m_K; j < m_K + m_S; ++j)
			{
				if (IsBitSet(m_Mseq[j],h))
				{
					m_Data[m_K + m_S + h].XorData(m_Data + j);
				}
			}
		}		

		//Get repair symbols
		data = LTEnc(m_K,m_Data,m_Triples);
	}
	return data;
}

