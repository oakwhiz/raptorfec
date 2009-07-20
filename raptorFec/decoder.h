/**
 * decoder.h Copyright (C) 2009 Vicente Sirvent.
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

#ifndef __RAPTOR_DECODER_H__
#define __RAPTOR_DECODER_H__

#include "defines.h"
#include "types.h"
#include "data.h"
#include <queue>

using namespace std;

class CDecoder
{
private:
	u8** m_A;
	u32  m_K;
	u32  m_S;
	u32  m_H;
	u32  m_Hp;
	u32  m_L;
	u32  m_Lp;

	void FillAMatrix(void);
	void GetAMatrix(u8** A,u32 M,u32 L);
	void ReleaseAMatrix(void);

	void SolveMatrixGauss1(u32 M, u32 L, u8** A, CData** C, CData** D);
	queue<CData*> GetIntermediateSymbols(queue<CData*> encoded);

public:

	CDecoder(u32 K);
	~CDecoder(void);

	queue<CData*> Decode(queue<CData*> encoded);	
};

#endif