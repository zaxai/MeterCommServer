/*
** Version  1.0.0.2
** Date     2019.02.27
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZStringSocket.h"
class ZFileSocket :
	public ZStringSocket
{
private:
	CString m_strFilePath;
	ULONGLONG m_llFileLength;
	ULONGLONG m_llTotalTimes;
	ULONGLONG m_llCurTimes;
	int m_nBuf;
	char * m_p_cSendData;
	char * m_p_cRecvData;
	bool InitByBuf();
public:
	ZFileSocket(void);
	ZFileSocket(ZFileSocket && zfilesock);
	ZFileSocket & operator=(ZFileSocket && zfilesock);
	virtual ~ZFileSocket(void);
	int FileSend(void);
	int FileRecv(void);
	CString GetFilePath(void);
	void SetFilePath(const CString & in_strFilePath);
	ULONGLONG GetFileLength(void);
	void SetFileLength(const ULONGLONG & in_llFileLength);
	ULONGLONG GetTotalTimes(void);
	ULONGLONG GetCurTimes(void);
	void SetCurTimes(ULONGLONG llCurTimes);
	int GetBuf(void);
	bool SetBuf(int nBuf);
};

