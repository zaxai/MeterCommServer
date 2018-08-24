#include "StdAfx.h"
#include "ZFileSocket.h"

const int nBuf=4096;

ZFileSocket::ZFileSocket(void)
	: m_strFilePath(_T(""))
	, m_llFileLength(0)
	, m_llTotalTimes(0)
	, m_llCurTimes(0)
{
}


ZFileSocket::~ZFileSocket(void)
{
}


int ZFileSocket::FileSend(void)
{
	CStdioFile file;
	if(!file.Open(m_strFilePath,CFile::modeRead|CFile::typeBinary))
		return ERROR_OTHER;
	char szSendData[nBuf+10]={0},szRecvData[2]={0};
	int nSendBuf,nLeftBuf,nRtn= ERROR_OK;
	m_llTotalTimes=m_llFileLength/nBuf;
	if(m_llFileLength%nBuf)
	{
		++m_llTotalTimes;
		nLeftBuf=m_llFileLength%nBuf;
	}
	else
		nLeftBuf=nBuf;
	file.Seek(nBuf*m_llCurTimes, CFile::begin);
	for(;m_llCurTimes<m_llTotalTimes;++m_llCurTimes)
	{
		if(m_llCurTimes==m_llTotalTimes-1)
			nSendBuf=nLeftBuf;
		else
			nSendBuf=nBuf;
		file.Read(szSendData,nSendBuf);
		nRtn=Send(szSendData,nSendBuf);
		if(nRtn)
			return nRtn;
		szRecvData[0]='0';
		nRtn=Recv(szRecvData,1);
		if(nRtn)
			return nRtn;
		if(szRecvData[0]=='0')
			return ERROR_OTHER;
	}
	return ERROR_OK;
}


int ZFileSocket::FileRecv(void)
{
	CStdioFile file;
	if(!file.Open(m_strFilePath,CFile::modeCreate| CFile::modeNoTruncate|CFile::modeWrite|CFile::typeBinary))
		return ERROR_OTHER;
	char szRecvData[nBuf+10]={0},szSendData[2]="1";
	int nRecvBuf,nLeftBuf,nRtn= ERROR_OK;
	m_llTotalTimes=m_llFileLength/nBuf;
	if(m_llFileLength%nBuf)
	{
		++m_llTotalTimes;
		nLeftBuf=m_llFileLength%nBuf;
	}
	else
		nLeftBuf=nBuf;
	file.Seek(nBuf*m_llCurTimes, CFile::begin);
	for(;m_llCurTimes<m_llTotalTimes;++m_llCurTimes)
	{	
		if(m_llCurTimes==m_llTotalTimes-1)
			nRecvBuf=nLeftBuf;
		else
			nRecvBuf=nBuf;
		nRtn=Recv(szRecvData,nRecvBuf);
		if(nRtn)
			return nRtn;
		file.Write(szRecvData,nRecvBuf);
		nRtn=Send(szSendData,1);
		if(nRtn)
			return nRtn;
	}
	return ERROR_OK;
}


void ZFileSocket::SetFilePath(const CString & in_strFilePath)
{
	m_strFilePath=in_strFilePath;
}


void ZFileSocket::SetFileLength(const ULONGLONG & in_llFileLength)
{
	m_llFileLength=in_llFileLength;
}


int ZFileSocket::GetBuf(void)
{
	return nBuf;
}


void ZFileSocket::SetCurTimes(ULONGLONG llCurTimes)
{
	m_llCurTimes= llCurTimes;
}


ULONGLONG ZFileSocket::GetCurTimes(void)
{
	return m_llCurTimes;
}


ULONGLONG ZFileSocket::GetTotalTimes(void)
{
	return m_llTotalTimes;
}

ULONGLONG ZFileSocket::GetFileLength(void)
{
	return m_llFileLength;
}