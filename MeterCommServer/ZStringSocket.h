/*
** Version  1.0.0.1
** Date     2018.12.01
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZSocket.h"
class ZStringSocket :
	public ZSocket
{
public:
	ZStringSocket();
	~ZStringSocket();
	int StringSend(const CString & in_strData);
	int StringRecv(CString & out_strData);
};

