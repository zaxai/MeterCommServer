/*
** Version  1.0.0.2
** Date     2019.02.27
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZUtil.h"
#include <ws2tcpip.h>
class ZSocket
{
public:
	enum TIMEOUT	
	{	
		TIMEOUT_CONT= 0,
		TIMEOUT_ACCEPT,
		TIMEOUT_SEND,	
		TIMEOUT_RECV,	
		MAX_TIMEOUT
	};
	enum ERRORINFO
	{
		ERROR_OK=0,//成功
		ERROR_SOCKDLL,//操作SOCK库出错
		ERROR_INVALIDSOCK,//无效套接字
		ERROR_INITSOCK,//初始化出错
		ERROR_CLOSESOCK,//关闭套接字出错
		ERROR_CONT,//连接出错
		ERROR_CONT_TIMEOUT,//连接超时
		ERROR_CONT_STOP,//连接停止
		ERROR_BIND,//绑定出错
		ERROR_LISTEN,//监听出错
		ERROR_ACCEPT,//接受出错
		ERROR_ACCEPT_TIMEOUT,//接受超时
		ERROR_ACCEPT_STOP,//接受停止
		ERROR_SEND,//发送出错
		ERROR_SEND_TIMEOUT,//发送超时
		ERROR_SEND_STOP,//发送停止
		ERROR_RECV,//接收出错
		ERROR_RECV_TIMEOUT,//接收超时
		ERROR_RECV_STOP,//接收停止
		ERROR_OTHER//其他出错
	};
private:
	SOCKET m_sock;
	HANDLE m_hEvtExitComm;
	bool m_bIsStopCont;
	bool m_bIsStopAccept;
	bool m_bIsStopSend;
	bool m_bIsStopRecv;
	DWORD m_sz_dwTimeout[MAX_TIMEOUT];
	ZSocket(const ZSocket & zsock) = delete;
	ZSocket operator=(const ZSocket & zsock) = delete;
public:
	ZSocket(void);
	ZSocket(ZSocket && zsock);
	ZSocket & operator=(ZSocket && zsock);
	virtual ~ZSocket(void);
	static int CallSocketDll(void);
	static int UncallSocketDll(void);
	int InitSocket(void);
	int CloseSocket(void);
	int Connect(const char * in_p_cServerIp,const unsigned short & in_nServerPort);
	int Connect(const CString & in_strServerIp,const CString & in_strServerPort);
	int Bind(const char * in_p_cLocalIp,const unsigned short & in_nLocalPort);
	int Bind(const CString & in_strLocalIp,const CString & in_strLocalPort);
	int Listen(const int & in_nBackLog);
	int Accept(ZSocket & out_sockClient,CString * out_p_strClientIP=NULL);
	int Send(const char * in_p_cData,const int & in_nSendLen,int & out_nActSendLen);
	int Send(const char * in_p_cData,const int & in_nSendLen);
	int Send(const CString & in_strData);
	int Recv(char * out_p_cData,const int & in_nMaxRecvLen,int & out_nActRecvLen);
	int Recv(char * out_p_cData,const int & in_nRecvLen);
	int Recv(CString & out_strData,const int & in_nMaxRecvLen,const bool & in_bIsBlocking=true);
	int Recv(CString & out_strData, const bool & in_bIsBlocking = true);
	void StopComm(void);
	bool SetTimeOut(const BYTE & in_cTimeoutIndex,const DWORD & in_dwTimeOut);
	CString DomainNameToIp(const CString & strDomainName);
	bool IsStrIP(const CString & str);
};

