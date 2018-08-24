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

