#include "stdafx.h"
#include "ZStringSocket.h"

const int nLengthBuf = 6;

ZStringSocket::ZStringSocket()
{
}


ZStringSocket::~ZStringSocket()
{
}


int ZStringSocket::StringSend(const CString & in_strData)
{
	int nStrLen = in_strData.GetLength();
	if (nStrLen == 0)
		return ERROR_OK;
	int nMaxLen = nStrLen * 2 + 1;
	char *p_cData = new char[nMaxLen];
	if (p_cData == NULL)
		return ERROR_OTHER;
	if (!TtoA_Z(in_strData, p_cData, nMaxLen))
	{
		delete[]p_cData;
		return ERROR_OTHER;
	}
	if(strlen(p_cData)>81920)//ÏÞ¶¨String80K×Ö½Ú
	{
		delete[]p_cData;
		return ERROR_OTHER;
	}
	char sz_cLength[nLengthBuf] = {0};
	sprintf_s(sz_cLength, nLengthBuf,"%5d", strlen(p_cData));
	int nRtn = Send(sz_cLength, nLengthBuf-1);
	if (nRtn)
		goto end;
	nRtn = Recv(sz_cLength, nLengthBuf-1);
	if (nRtn)
		goto end;
	nRtn = Send(p_cData, strlen(p_cData));
end:
	delete[]p_cData;
	return nRtn;
}


int ZStringSocket::StringRecv(CString & out_strData)
{
	char sz_cLength[nLengthBuf] = { 0 };
	int nRtn = Recv(sz_cLength, nLengthBuf -1);
	if (nRtn)
		return nRtn;
	nRtn = Send(sz_cLength, nLengthBuf-1);
	if (nRtn)
		return nRtn;
	char *p_cData = new char[atoi(sz_cLength)+1];
	if (p_cData == NULL)
		return ERROR_OTHER;
	nRtn = Recv(p_cData, atoi(sz_cLength));
	if (nRtn)
		return nRtn;
	p_cData[atoi(sz_cLength)] = '\0';
	out_strData = p_cData;
	delete[]p_cData;
	return ERROR_OK;
}