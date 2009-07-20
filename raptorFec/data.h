#ifndef __DATA_H__
#define __DATA_H__

#include "types.h"
#include <string.h>

class CData
{
private:

	u8 * m_Data;
	u32  m_Len;

public:
	
	CData(void)
	{
		m_Data = NULL;
		m_Len = 0;
	}

	CData(u8 * data, u32 len)
	{
		m_Data = NULL;
		m_Len = 0;
		SetData(data,len);
	}

	CData(CData &other)
	{
		m_Data = NULL;
		m_Len = 0;
		SetData(other.m_Data,other.m_Len);
	}

	~CData(void)
	{
		FreeData();
	}

	const u8 * GetData(void){ return m_Data; }
	u32        GetLen(void){ return m_Len; }

	void FreeData(void)
	{
		if (m_Data)
		{
			delete[] m_Data;
			m_Data = NULL;
		}	
	}

	void SetData(CData * data)
	{
		return SetData(data->GetData(), data->GetLen());
	}

	void SetData(const u8 * data, u32 len)
	{
		FreeData();
		if (len > 0 && data)
		{
			m_Data = new u8[len];
			memcpy(m_Data,data,len);
			m_Len = len;
		}
	}

	void XorData(CData * data)
	{
		return XorData(data->GetData(),data->GetLen());
	}

	void XorData(const u8 * data, u32 len)
	{
		if (len > m_Len)
		{
			u8 * tmp = new u8[len];
			memset(tmp,0,len);
			if (m_Len > 0)
			{
				memcpy(tmp,m_Data,m_Len);
			}
			FreeData();
			m_Data = tmp;
			m_Len  = len;
		}
		for (u32 i = 0; i < m_Len; ++i)
		{
			m_Data[i] ^= data[i];
		}
	}
};

#endif