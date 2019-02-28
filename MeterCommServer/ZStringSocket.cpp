#include "stdafx.h"
#include "ZStringSocket.h"


ZStringSocket::ZStringSocket()
	: m_nMaxLength(10240)
	, m_nMaxNumCount(5)
	, m_nBufSize(6)
	, m_p_cLengthBuf(nullptr)
{
	InitByMaxLength();
}


ZStringSocket::ZStringSocket( ZStringSocket && zstrsock)
	: ZSocket(std::move(zstrsock))
	, m_nMaxLength(zstrsock.m_nMaxLength)
	, m_nMaxNumCount(zstrsock.m_nMaxNumCount)
	, m_nBufSize(zstrsock.m_nBufSize)
	, m_p_cLengthBuf(zstrsock.m_p_cLengthBuf)
{
	zstrsock.m_p_cLengthBuf = nullptr;
}


ZStringSocket & ZStringSocket::operator=(ZStringSocket && zstrsock)
{
	ZSocket::operator=(std::move(zstrsock));
	if (m_p_cLengthBuf)
		delete[] m_p_cLengthBuf;
	m_nMaxLength = zstrsock.m_nMaxLength;
	m_nMaxNumCount = zstrsock.m_nMaxNumCount;
	m_nBufSize = zstrsock.m_nBufSize;
	m_p_cLengthBuf = zstrsock.m_p_cLengthBuf;
	zstrsock.m_p_cLengthBuf = nullptr;
	return *this;
}


ZStringSocket::~ZStringSocket()
{
	if (m_p_cLengthBuf)
		delete[] m_p_cLengthBuf;
}


int ZStringSocket::StringSend(const CString & in_strData)
{
	int nLength = in_strData.GetLength();
	if (nLength == 0)
		return ERROR_OK;
	int nMaxLength = nLength * 2 + 1;
	char *p_cData = new char[nMaxLength];
	if (p_cData == NULL)
		return ERROR_OTHER;
	if (!ZUtil::WtoA(in_strData, p_cData, nMaxLength))
	{
		delete[]p_cData;
		return ERROR_OTHER;
	}
	if(!SetLengthBufByLength(strlen(p_cData)))
	{
		delete[]p_cData;
		return ERROR_OTHER;
	}
	int nRtn = Send(m_p_cLengthBuf, m_nMaxNumCount);
	if (nRtn)
		goto end;
	nRtn = Recv(m_p_cLengthBuf, m_nMaxNumCount);
	if (nRtn)
		goto end;
	nRtn = Send(p_cData, strlen(p_cData));
end:
	delete[]p_cData;
	return nRtn;
}


int ZStringSocket::StringRecv(CString & out_strData)
{
	memset(m_p_cLengthBuf, 0, m_nBufSize);
	int nRtn = Recv(m_p_cLengthBuf, m_nMaxNumCount);
	if (nRtn)
		return nRtn;
	nRtn = Send(m_p_cLengthBuf, m_nMaxNumCount);
	if (nRtn)
		return nRtn;
	char *p_cData = new char[atoi(m_p_cLengthBuf)+1];
	if (p_cData == NULL)
		return ERROR_OTHER;
	nRtn = Recv(p_cData, atoi(m_p_cLengthBuf));
	p_cData[atoi(m_p_cLengthBuf)] = '\0';
	out_strData = p_cData;
	delete[] p_cData;
	return nRtn;
}


bool ZStringSocket::InitByMaxLength(void)
{
	CString strMaxLength;
	strMaxLength.Format(_T("%d"), m_nMaxLength);
	m_nMaxNumCount = strMaxLength.GetLength();
	m_nBufSize = m_nMaxNumCount + 1;
	if (m_p_cLengthBuf)
		delete[] m_p_cLengthBuf;
	m_p_cLengthBuf = new char[m_nBufSize];
	if (m_p_cLengthBuf)
		return true;
	else
		return false;
}


bool ZStringSocket::SetLengthBufByLength(int nLength)
{
	if (nLength > m_nMaxLength || (!m_p_cLengthBuf))
		return false;
	CString strLengthBuf, strZero;
	strLengthBuf.Format(_T("%d"), nLength);
	strZero = CString(_T('0'), m_nMaxNumCount - strLengthBuf.GetLength());
	strLengthBuf = strZero + strLengthBuf;
	memset(m_p_cLengthBuf, 0, m_nBufSize);
	ZUtil::WtoA(strLengthBuf, m_p_cLengthBuf, m_nBufSize);
	return true;
}


bool ZStringSocket::SetMaxStringLength(int nMaxLength)
{
	if (m_nMaxLength != nMaxLength)
	{
		m_nMaxLength = nMaxLength;
		return InitByMaxLength();
	}
	return true;
}