#include "StdAfx.h"
#include "ZFileSocket.h"


ZFileSocket::ZFileSocket(void)
	: m_strFilePath(_T(""))
	, m_llFileLength(0)
	, m_llTotalTimes(0)
	, m_llCurTimes(0)
	, m_nBuf(4096)
	, m_p_cSendData(nullptr)
	, m_p_cRecvData(nullptr)
{
	InitByBuf();
}


ZFileSocket::ZFileSocket(ZFileSocket && zfilesock)
	: ZStringSocket(std::move(zfilesock))
	, m_strFilePath(zfilesock.m_strFilePath)
	, m_llFileLength(zfilesock.m_llFileLength)
	, m_llTotalTimes(zfilesock.m_llTotalTimes)
	, m_llCurTimes(zfilesock.m_llCurTimes)
	, m_nBuf(zfilesock.m_nBuf)
	, m_p_cSendData(zfilesock.m_p_cSendData)
	, m_p_cRecvData(zfilesock.m_p_cRecvData)
{
	zfilesock.m_p_cSendData = nullptr;
	zfilesock.m_p_cRecvData = nullptr;
}


ZFileSocket & ZFileSocket::operator=(ZFileSocket && zfilesock)
{
	ZStringSocket::operator=(std::move(zfilesock));
	if (m_p_cSendData)
		delete[] m_p_cSendData;
	if (m_p_cRecvData)
		delete[] m_p_cRecvData;
	m_strFilePath = zfilesock.m_strFilePath;
	m_llFileLength = zfilesock.m_llFileLength;
	m_llTotalTimes = zfilesock.m_llTotalTimes;
	m_llCurTimes = zfilesock.m_llCurTimes;
	m_nBuf = zfilesock.m_nBuf;
	m_p_cSendData = zfilesock.m_p_cSendData;
	m_p_cRecvData = zfilesock.m_p_cRecvData;
	zfilesock.m_p_cSendData = nullptr;
	zfilesock.m_p_cRecvData = nullptr;
	return *this;
}


ZFileSocket::~ZFileSocket(void)
{
	if (m_p_cSendData)
		delete[] m_p_cSendData;
	if (m_p_cRecvData)
		delete[] m_p_cRecvData;
}


int ZFileSocket::FileSend(void)
{
	CStdioFile file;
	if(!file.Open(m_strFilePath,CFile::modeRead|CFile::typeBinary))
		return ERROR_OTHER;
	char szRecvFlag[2]={0};
	int nSendBuf,nLeftBuf,nRtn= ERROR_OK;
	m_llTotalTimes=m_llFileLength/ m_nBuf;
	if(m_llFileLength%m_nBuf)
	{
		++m_llTotalTimes;
		nLeftBuf=m_llFileLength% m_nBuf;
	}
	else
		nLeftBuf= m_nBuf;
	file.Seek(m_nBuf*m_llCurTimes, CFile::begin);
	for(;m_llCurTimes<m_llTotalTimes;++m_llCurTimes)
	{
		if(m_llCurTimes==m_llTotalTimes-1)
			nSendBuf=nLeftBuf;
		else
			nSendBuf= m_nBuf;
		file.Read(m_p_cSendData,nSendBuf);
		nRtn=Send(m_p_cSendData,nSendBuf);
		if(nRtn)
			return nRtn;
		szRecvFlag[0]='0';
		nRtn=Recv(szRecvFlag,1);
		if(nRtn)
			return nRtn;
		if(szRecvFlag[0]=='0')
			return ERROR_OTHER;
	}
	return ERROR_OK;
}


int ZFileSocket::FileRecv(void)
{
	CStdioFile file;
	if(!file.Open(m_strFilePath,CFile::modeCreate| CFile::modeNoTruncate|CFile::modeWrite|CFile::typeBinary))
		return ERROR_OTHER;
	char szSendFlag[2]="1";
	int nRecvBuf,nLeftBuf,nRtn= ERROR_OK;
	m_llTotalTimes=m_llFileLength/ m_nBuf;
	if(m_llFileLength%m_nBuf)
	{
		++m_llTotalTimes;
		nLeftBuf=m_llFileLength% m_nBuf;
	}
	else
		nLeftBuf= m_nBuf;
	file.Seek(m_nBuf*m_llCurTimes, CFile::begin);
	for(;m_llCurTimes<m_llTotalTimes;++m_llCurTimes)
	{	
		if(m_llCurTimes==m_llTotalTimes-1)
			nRecvBuf=nLeftBuf;
		else
			nRecvBuf= m_nBuf;
		nRtn=Recv(m_p_cRecvData,nRecvBuf);
		if(nRtn)
			return nRtn;
		file.Write(m_p_cRecvData,nRecvBuf);
		nRtn=Send(szSendFlag,1);
		if(nRtn)
			return nRtn;
	}
	return ERROR_OK;
}


CString ZFileSocket::GetFilePath(void)
{
	return m_strFilePath;
}


void ZFileSocket::SetFilePath(const CString & in_strFilePath)
{
	m_strFilePath=in_strFilePath;
}


ULONGLONG ZFileSocket::GetFileLength(void)
{
	return m_llFileLength;
}


void ZFileSocket::SetFileLength(const ULONGLONG & in_llFileLength)
{
	m_llFileLength=in_llFileLength;
}


ULONGLONG ZFileSocket::GetTotalTimes(void)
{
	return m_llTotalTimes;
}


ULONGLONG ZFileSocket::GetCurTimes(void)
{
	return m_llCurTimes;
}


void ZFileSocket::SetCurTimes(ULONGLONG llCurTimes)
{
	m_llCurTimes = llCurTimes;
}


int ZFileSocket::GetBuf(void)
{
	return m_nBuf;
}


bool ZFileSocket::SetBuf(int nBuf)
{
	if (m_nBuf != nBuf)
	{
		m_nBuf = nBuf;
		return InitByBuf();
	}
	return true;
}


bool ZFileSocket::InitByBuf()
{
	if (m_p_cSendData)
		delete[] m_p_cSendData;
	if (m_p_cRecvData)
		delete[] m_p_cRecvData;
	m_p_cSendData = new char[m_nBuf + 1];
	m_p_cRecvData = new char[m_nBuf + 1];
	if (m_p_cSendData && m_p_cRecvData)
		return true;
	else
		return false;
}