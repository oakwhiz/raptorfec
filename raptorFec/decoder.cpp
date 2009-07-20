/**
 * decoder.cpp Copyright (C) 2009 Vicente Sirvent.
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

#include "decoder.h"
#include "general_functions.h"
#include "triple_generator.h"
#include <string.h>
#include <vector>

CDecoder::CDecoder(u32 K)
{
	m_K = K;
	m_S = GetS(m_K);
	m_H = GetH(m_K,m_S);
	m_Hp = GetHp(m_H);
	m_L = GetL(m_K);
	m_Lp = GetLPrim(m_K);
	m_A = NULL;
	FillAMatrix();
}

CDecoder::~CDecoder(void)
{
	ReleaseAMatrix();
}

void
CDecoder::ReleaseAMatrix(void)
{
	if (m_A)
	{
		for (u32 i = 0; i < m_L; ++i)
		{
			delete[] m_A[i];
		}
	}
	delete[] m_A;
	m_A = NULL;
}

void
CDecoder::FillAMatrix(void)
{
	//Free A matrix
	ReleaseAMatrix();
	//Alloc A matrix
	m_A = new u8*[m_L + m_S + m_H]; //Max number of rows that M can achieve
	for (u32 i = 0; i < m_L; ++i)
	{
		m_A[i] = new u8[m_L];
		memset(m_A[i],0,m_L);
	}
	//Fill A Matrix (See RFC5053:5.4.2.4.2)
	//Fill G_LDCP values
	for (u32 i = 0; i < m_K; ++i)
	{
		u32 a = 1 + (i/m_S % (m_S - 1));
		u32 b = i % m_S;
		m_A[b][i] = 1;
		b = (b + a) % m_S;
		m_A[b][i] = 1;
		b = (b + a) % m_S;
		m_A[b][i] = 1;
	}
	//Fill I_S Matrix
	for (u32 i = 0; i < m_S; ++i)
	{
		m_A[i][i + m_K] = 1;
	}

	//Fill I_H Matrix
	for (u32 i = m_S; i < m_S + m_H; ++i)
	{
		m_A[i][i + m_K] = 1;
	}
	
	//Fill m sequence
	u32 mseq_len = m_K + m_S - 1;
	u32* Mseq = new u32[mseq_len];
	for (u32 mcount = 0, i = 0; mcount < mseq_len; ++i)
	{
		u32 g = i^(i/2);
		if (NonZeroBits(g) == m_Hp)
		{
			Mseq[mcount] = g;
			mcount++;
		}
	}	
	
	//Fill G_Half Matrix
	for (u32 h = 0; h < m_H; ++h)
	{
		for (u32 j = 0; j < m_K + m_S; ++j)
		{
			if (IsBitSet(Mseq[j],h))
			{
				m_A[h + m_S][j] = 1;
			}
		}
	}
	delete[] Mseq;

	//Fill GL_T matrix
	//Load triples
	CTripleGenerator triple_gen;	
	for (u32 i = 0; i < m_L; ++i)
	{
		CTriple triple = triple_gen.Trip(m_K,i);
		u32 a = triple.a;
		u32 b = triple.b;
		u32 d = triple.d;
		while (b >= m_L)
		{
			b = (b + a) % m_Lp;
		}
		m_A[i + m_S + m_H][b] = 1;
		u32 jend = MIN(d,m_L);
		for (u32 j = 1; j < jend;++j)
		{
			b = (b + a) % m_Lp;
			while (b >= m_L)
			{
				b = (b + a) % m_Lp;
			}
			m_A[i + m_S + m_H][b] = 1;
		}		
	}
}

void
CDecoder::GetAMatrix(u8** A,u32 M,u32 L)
{
	for (u32 i = 0; i < M; ++i)
	{
		for (u32 j = 0; j < L; ++j)
		{
			A[i][j] = m_A[i][j];
		}
	}
}

queue<CData*> 
CDecoder::GetIntermediateSymbols(queue<CData*> encoded)
{
	u32 N = (u32)encoded.size();
	u32 M = N + m_H + m_S;
	//Get temporal A MxL bit matrix to work with.
	u8** A = new u8*[M];
	for (u32 i = 0; i < m_L; ++i)
	{
		A[i] = new u8[m_L];
	}
	GetAMatrix(A,M,m_L);

	CData ** C = new CData*[m_L]; //Intermediate symbols to be recovered
	CData ** D = new CData*[M]; //Encoded symbols
	for (u32 i = 0; i < M; ++i)
	{
		if (i < m_L)
		{
			C[i] = new CData();
		}
		D[i] = new CData();
	}

	//First S + H encodec symbols inits with zero.
	for (u32 i = m_S + m_H; i < M; ++i)
	{
		*D[i] = *(encoded.front());
		encoded.pop();
	}
	
	//Get C, we have to solve A*C=D with gauss method. 
	SolveMatrixGauss1(M,m_L,A,C,D);

	//free resources

	//free A matrix
	for (u32 i = 0; i < M; ++i)
	{
		delete D[i];
		if (i < m_L)
		{
			delete[] A[i];
			delete C[i];
		}
	}
	delete[] A;
	delete[] C;
	delete[] D;
	
	//free encoded symbols and intermediate symbols
	return encoded;
}

void 
CDecoder::SolveMatrixGauss1(u32 M, u32 L, u8** A, CData** C, CData** D)
{
	//init sequence
	u32* c = new u32[L];
	u32* d = new u32[M];
	for (u32 i = 0; i < M; ++i)
	{
		if (i < L)
		{
			c[i] = i;
		}
		d[i] = i;
	}
	
	/*<----n------>                 <----n---->       
	/*+-----------+-----------------+---------+
	  |			  |					|		  |
	  |		 I	  |	 All Zeros      |		  |
	  |			  |				    |		  |
	  +-----------+-----------------+	U     |
	  |			  |				    |		  |
	  |			  |				    |		  |
	  | All Zeros |		V			|		  |
	  |			  |					|		  |
	  |			  |					|		  |
	  +-----------+-----------------+---------+*/

	for (u32 n = 0; n < L; ++n)
	{
		//Let r be the minimum integer such that at least one row of A has
		//exactly r ones in V.
		vector<u32> ones_pos;		
		u32 nrow = 0;
		u32 nones = L;
		for (u32 i = n; i < M; ++i) //from n row
		{
			u32 row_ones = 0;
			vector<u32> row_ones_pos;
			u8 * row = A[i];
			for (u32 j = n; j < L - n; ++j) //from take out n first from I matrix and n last from U matrix
			{
				if (row[j])
				{
					row_ones++;
					row_ones_pos.push_back(j);
				}
			}
			if (row_ones < nones)
			{
				nones = row_ones;
				ones_pos = row_ones_pos;
				nrow  = i;				
			}
		}
		//TODO: Now we must work with this row
		//TODO: if nones == 2 we must choose between the maximum size component (See RFC 5053:5.5.2.2)
		
		//Move row to first V row
		u8* tmp_row = A[n];
		A[n] = A[nrow];
		A[nrow] = tmp_row;
		u32 tmp_encoded = d[n];
		d[n]    = d[nrow];
		d[nrow] = tmp_encoded;

		//Move one 1 to first column and the rest to the last columns
		u32 last_one = L - n;
		for (vector<u32>::iterator iter = ones_pos.begin(); iter != ones_pos.end(); ++iter)
		{
			//if don't have 1, put a column in it
			if (A[n][n] != 1)
			{
				for (u32 y = 0; y < M; ++i)
				{
					u8 tmp_bit = A[n][n+y];
					A[n][n+y]  = A[n][L-n];
					A[n][L-n]  = tmp_bit;
				}
			}
			//Not working with first column
			if (*iter > 0)
			{

			}
		}		
	}
	//free resources
	delete[] c;
	delete[] d;
}

queue<CData*> 
CDecoder::Decode(queue<CData*> encoded)
{
	return GetIntermediateSymbols(encoded);
}