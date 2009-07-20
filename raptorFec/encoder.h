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

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "data.h"
#include "triple_generator.h"
#include <queue>

using namespace std;

class CEncoder
{
private:

	u32 * m_Mseq; // m sequence defined in RFC 5053:5.4.2.3
	CTriple * m_Triples;
	CData   * m_Data;
	u32 m_K;
	u32 m_S;
	u32 m_H;
	u32 m_Hp;
	u32 m_L;
	u32 m_Lp;
	u32 m_Count;

private:

	queue<CData*> LTEnc(u32 k, CData * inter_sym, CTriple * triples);

public:

	CEncoder(u32 K);
	~CEncoder(void);
	queue<CData*> AddData(CData * source);
};

#endif