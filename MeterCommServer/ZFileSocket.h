/*
** Version  1.0.0.1
** Date     2018.12.01
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
public:
	ZFileSocket(void);
	~ZFileSocket(void);
	int FileSend(void);
	int FileRecv(void);
	void SetFilePath(const CString & in_strFilePath);
	void SetFileLength(const ULONGLONG & in_llFileLength);
	int GetBuf(void);
	void SetCurTimes(ULONGLONG llCurTimes);
	ULONGLONG GetCurTimes(void);
	ULONGLONG GetTotalTimes(void);
	ULONGLONG GetFileLength(void);
};

