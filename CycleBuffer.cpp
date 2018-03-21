#include "StdAfx.h"
#include "CycleBuffer.h"

#include <assert.h> 
#include <memory.h> 
// ����
CCycleBuffer::CCycleBuffer(int size) 
{ 
	m_nBufSize = size; 
	m_nReadPos = 0; 
	m_nWritePos = 0; 
	m_pBuf = new char[m_nBufSize]; 
	m_bEmpty = true; 
	m_bFull = false; 
	test = 0; 
} 

CCycleBuffer::~CCycleBuffer()
{
	delete[] m_pBuf; 
}
/************************************************************************/
/* �򻺳���д�����ݣ�����ʵ��д����ֽ��� */
/************************************************************************/
int CCycleBuffer::write(char* buf, int count) 
{ 
	if(count <= 0) 
		return 0;
	m_bEmpty = false; // ���������������ܼ���д�� 
	if(m_bFull) 
	{
		return 0; 
	} else if(m_nReadPos == m_nWritePos) // ������Ϊ��ʱ
	{ 
		int leftcount = m_nBufSize - m_nWritePos;
		if(leftcount > count)    //ʣ��ռ����д�����ݵĿռ�
		{
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count;
			m_bFull = (m_nWritePos == m_nReadPos);
			return count; 
		} else                  //ʣ��ռ�С�ڵ������ݵĿռ�
		{ 
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);
			m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos; 
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);
			m_bFull = (m_nWritePos == m_nReadPos);
			return leftcount + m_nWritePos; 
		}
	} else if(m_nReadPos < m_nWritePos) // ��ʣ��ռ��д�� 
	{ 
		// ʣ�໺������С(��д��λ�õ�������β)
		int leftcount = m_nBufSize - m_nWritePos;
		int test = m_nWritePos;
		if(leftcount > count) // ���㹻��ʣ��ռ��� 
		{ 
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count; 
			m_bFull = (m_nReadPos == m_nWritePos);
			assert(m_nReadPos <= m_nBufSize);
			assert(m_nWritePos <= m_nBufSize);
			return count; 
		} else // ʣ��ռ䲻�� 
		{ 
			// �������ʣ��ռ䣬�ٻ�ͷ�ҿռ��� 
			memcpy(m_pBuf + test, buf, leftcount); 
			m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;
			memcpy(m_pBuf, buf + leftcount, m_nWritePos); 
			m_bFull = (m_nReadPos == m_nWritePos);
			assert(m_nReadPos <= m_nBufSize);
			assert(m_nWritePos <= m_nBufSize); 
			return leftcount + m_nWritePos; 
		}
	} else
	{ 
		int leftcount = m_nReadPos - m_nWritePos;
		if(leftcount > count) 
		{
			// ���㹻��ʣ��ռ��� 
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count; 
			m_bFull = (m_nReadPos == m_nWritePos);
			assert(m_nReadPos <= m_nBufSize);
			assert(m_nWritePos <= m_nBufSize); 
			return count;
		} else 
		{ 
			// ʣ��ռ䲻��ʱҪ������������� 
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);
			m_nWritePos += leftcount;
			m_bFull = (m_nReadPos == m_nWritePos); 
			assert(m_bFull); 
			assert(m_nReadPos <= m_nBufSize); 
			assert(m_nWritePos <= m_nBufSize); 
			return leftcount; 
		} 
	} 
} 
/************************************************************************/
/* �ӻ����������ݣ�����ʵ�ʶ�ȡ���ֽ��� */ 
/************************************************************************/
int CCycleBuffer::read(char* buf, int count) 
{ 
	if(count <= 0)
		return 0;
	m_bFull = false; 
	if(m_bEmpty) // �������գ����ܼ�����ȡ���� 
	{ 
		return 0; 
	} else if(m_nReadPos == m_nWritePos) // ��������ʱ 
	{ 
		int leftcount = m_nBufSize - m_nReadPos; 
		if(leftcount > count)
		{ 
			memcpy(buf, m_pBuf + m_nReadPos, count);
			m_nReadPos += count;
			m_bEmpty = (m_nReadPos == m_nWritePos); 
			return count; 
		} else 
		{ 
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);
			m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos; 
			memcpy(buf + leftcount, m_pBuf, m_nReadPos); 
			m_bEmpty = (m_nReadPos == m_nWritePos); return leftcount + m_nReadPos;
		} 
    } else if(m_nReadPos < m_nWritePos) // дָ����ǰ(δ�����������ӵ�) 
	{ 
		int leftcount = m_nWritePos - m_nReadPos;
		int c = (leftcount > count) ? count : leftcount; 
		memcpy(buf, m_pBuf + m_nReadPos, c); 
		m_nReadPos += c; m_bEmpty = (m_nReadPos == m_nWritePos); 
		assert(m_nReadPos <= m_nBufSize);
		assert(m_nWritePos <= m_nBufSize);
		return c; 
	} else // ��ָ����ǰ(δ�����ݿ����ǲ����ӵ�)
	{ 
		int leftcount = m_nBufSize - m_nReadPos; 
		if(leftcount > count) // δ������������ֱ�Ӷ�ȡ���� 
		{ 
			memcpy(buf, m_pBuf + m_nReadPos, count);
			m_nReadPos += count; 
			m_bEmpty = (m_nReadPos == m_nWritePos); 
			assert(m_nReadPos <= m_nBufSize); 
			assert(m_nWritePos <= m_nBufSize); 
			return count; 
		} else // δ�����������㣬��ص�������ͷ��ʼ��
		{ 
			memcpy(buf, m_pBuf + m_nReadPos, leftcount); 
			m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(buf + leftcount, m_pBuf, m_nReadPos); m_bEmpty = (m_nReadPos == m_nWritePos); 
			assert(m_nReadPos <= m_nBufSize); 
			assert(m_nWritePos <= m_nBufSize);
			return leftcount + m_nReadPos; 
		} 
	} 
}

/************************************************************************/ 
/* ��ȡ��������Ч���ݳ��� */ 
/************************************************************************/ 
int CCycleBuffer::getLength() 
{
	if(m_bEmpty) 
	{ 
		return 0; 
	} else if(m_bFull) 
	{ 
		return m_nBufSize; 
	} else if(m_nReadPos < m_nWritePos) 
	{ 
		return m_nWritePos - m_nReadPos;
	} else 
	{ 
		return m_nBufSize - m_nReadPos + m_nWritePos; 
	} 
} 

void CCycleBuffer::empty() 
{ 
	m_nReadPos = 0; 
	m_nWritePos = 0; 
	m_bEmpty = true; 
	m_bFull = false; 
} 

bool CCycleBuffer::isEmpty() 
{ 
	return m_bEmpty; 
} 

bool CCycleBuffer::isFull() 
{ 
	return m_bFull; 
}