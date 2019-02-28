/*
** Version  1.0.0.2
** Date     2019.02.27
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZSocket.h"
class ZStringSocket :
	public ZSocket
{
private:
	int m_nMaxLength;//字符串最大字节长度 默认10K
	int m_nMaxNumCount;//字符串最大长度的位数 默认5
	int m_nBufSize;//数据长度缓冲区大小 默认6
	char * m_p_cLengthBuf;//数据长度缓冲区
	bool InitByMaxLength(void);
	bool SetLengthBufByLength(int nLength);
public:
	ZStringSocket();
	ZStringSocket(ZStringSocket && zstrsock);
	ZStringSocket & operator=(ZStringSocket && zstrsock);
	virtual ~ZStringSocket();
	int StringSend(const CString & in_strData);
	int StringRecv(CString & out_strData);
	bool SetMaxStringLength(int nMaxLength);
};

