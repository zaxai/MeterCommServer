#include "StdAfx.h"
#include "ZSocket.h"

ZSocket::ZSocket(void)
	: m_sock(INVALID_SOCKET)
	, m_hEvtExitComm(NULL)
	, m_bIsStopCont(false)
	, m_bIsStopAccept(false)
	, m_bIsStopSend(false)
	, m_bIsStopRecv(false)
{
	m_sz_dwTimeout[TIMEOUT_CONT]=3000;
	m_sz_dwTimeout[TIMEOUT_ACCEPT]=-1;
	m_sz_dwTimeout[TIMEOUT_SEND]=-1;
	m_sz_dwTimeout[TIMEOUT_RECV]=-1;
	m_hEvtExitComm=CreateEvent(NULL,TRUE,TRUE,NULL);
}


ZSocket::~ZSocket(void)
{
	
}

int ZSocket::CallSocketDll(void)
{
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int nErr;
	wVersionRequested=MAKEWORD(1,1);
	nErr = WSAStartup(wVersionRequested,&wsaData);  
	if(nErr!= 0)
		return ERROR_SOCKDLL;
	if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1)
	{  
		WSACleanup();  
		return ERROR_SOCKDLL;
	}  
	return ERROR_OK;
}

int ZSocket::UncallSocketDll(void)
{
	if(WSACleanup()!=0)
		return ERROR_SOCKDLL;
	return ERROR_OK;
}

int ZSocket::InitSocket(void)
{
	m_sock =socket(AF_INET,SOCK_STREAM,0); 
	if(m_sock==INVALID_SOCKET)
		return ERROR_INVALIDSOCK;
	DWORD dwMode=1;
	if(ioctlsocket(m_sock,FIONBIO,&dwMode)!=0)// 设为非阻塞模式
		return ERROR_INITSOCK;
	return ERROR_OK;
}

int ZSocket::CloseSocket(void)
{
	if(m_hEvtExitComm!=NULL)
	{
		CloseHandle(m_hEvtExitComm);
		m_hEvtExitComm=NULL;
	}
	if(m_sock!=INVALID_SOCKET)
	{
		if(closesocket(m_sock)!=0)
			return ERROR_CLOSESOCK;
		m_sock=INVALID_SOCKET;
	}
	return ERROR_OK;
}

int ZSocket::Connect(const char * in_p_cServerIp,const unsigned short & in_nServerPort)
{
	ResetEvent(m_hEvtExitComm);
	int nRtn= ERROR_OK;
	if(m_sock==INVALID_SOCKET)
	{
		nRtn= ERROR_INVALIDSOCK;
		goto end;
	}
	SOCKADDR_IN addrSrv; 
	/*double dVersion = _ttof(GetWinVersion_Z());
	if (dVersion >= 6.0)
		inet_pton(AF_INET, in_p_cServerIp, &addrSrv.sin_addr);
	else*/
	addrSrv.sin_addr.S_un.S_addr = inet_addr(in_p_cServerIp);
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(in_nServerPort);
	DWORD dwStart,dwSpan;
	dwStart=GetTickCount();
	if(connect(m_sock, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR))==0)
		nRtn= ERROR_OK;
	else
	{
		typedef int socklen_t;
		int nError=-1;
		socklen_t len=sizeof(int); 
		fd_set set;  
		timeval timeVal;
		timeVal.tv_sec=0;
		timeVal.tv_usec=10000;
		m_bIsStopCont=false;
		while(!m_bIsStopCont)
		{
			FD_ZERO(&set);  
			FD_SET(m_sock, &set);  
			if(select(0,NULL,&set,NULL,&timeVal)>0) 
			{
				getsockopt(m_sock,SOL_SOCKET,SO_ERROR,(char *)&nError,&len);
				if(nError==0)
					nRtn= ERROR_OK;
				else
					nRtn= ERROR_CONT;
				goto end;
			}
			dwSpan=GetTickCount()-dwStart;
			if(dwSpan>=m_sz_dwTimeout[TIMEOUT_CONT])
			{
				nRtn= ERROR_CONT_TIMEOUT;
				goto end;
			}
		}
		if(m_bIsStopCont)
			nRtn= ERROR_CONT_STOP;
	}
end:
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZSocket::Connect(const CString & in_strServerIp,const CString & in_strServerPort)
{
	CString strServerIp;
	strServerIp=DomainNameToIp(in_strServerIp);
	char sz_cServerIp[20]={0};
	BOOL nRtn=TtoA_Z(strServerIp,sz_cServerIp,sizeof(sz_cServerIp));
	if(!nRtn)
		return ERROR_OTHER;
	unsigned short nServerPort=_ttoi(in_strServerPort);
	return Connect(sz_cServerIp,nServerPort);
}

int ZSocket::Bind(const char * in_p_cLocalIp,const unsigned short & in_nLocalPort)
{
	if(m_sock==INVALID_SOCKET)
		return ERROR_INVALIDSOCK;
	SOCKADDR_IN addrSrv; 
	/*double dVersion = _ttof(GetWinVersion_Z());
	if (dVersion >= 6.0)
		inet_pton(AF_INET, in_p_cLocalIp, &addrSrv.sin_addr);
	else*/
	addrSrv.sin_addr.S_un.S_addr = inet_addr(in_p_cLocalIp);
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(in_nLocalPort);
	int nRtn=bind(m_sock, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if(nRtn)
		return ERROR_BIND;
	return ERROR_OK;
}

int ZSocket::Bind(const CString & in_strLocalIp,const CString & in_strLocalPort)
{
	CString strLocalIp(in_strLocalIp);
	char sz_cLocalIp[20]={0};
	if(strLocalIp==_T("INADDR_ANY"))
		strLocalIp=_T("0.0.0.0");
	if(!TtoA_Z(strLocalIp, sz_cLocalIp, sizeof(sz_cLocalIp)))
		return ERROR_OTHER;
	unsigned short nLocalPort=_ttoi(in_strLocalPort);
	return Bind(sz_cLocalIp,nLocalPort);
}

int ZSocket::Listen(const int & in_nBackLog)
{
	if(m_sock==INVALID_SOCKET)
		return ERROR_INVALIDSOCK;
	int nRtn=listen(m_sock,in_nBackLog);
	if(nRtn)
		return ERROR_LISTEN;
	return ERROR_OK;
}

int ZSocket::Accept(ZSocket & out_sockClient,CString * out_p_strClientIP)
{
	ResetEvent(m_hEvtExitComm);
	int nRtn= ERROR_OK;
	if(m_sock==INVALID_SOCKET)
	{
		nRtn= ERROR_INVALIDSOCK;
		goto end;
	}
	SOCKADDR_IN addrClient;
	int nAddLen=sizeof(SOCKADDR);
	SOCKET sock=INVALID_SOCKET;
	DWORD dwStart,dwSpan;
	dwStart=GetTickCount();
	fd_set set;  
	timeval timeVal;
	timeVal.tv_sec=0;
	timeVal.tv_usec=10000;
	m_bIsStopAccept=false;
	while(!m_bIsStopAccept)
	{
		FD_ZERO(&set);  
		FD_SET(m_sock, &set);  
		if(select(0,&set,NULL,NULL,&timeVal)>0) 
		{
			sock= accept(m_sock,(SOCKADDR*)&addrClient, &nAddLen);
			if(sock!=INVALID_SOCKET)
			{
				out_sockClient.m_sock=sock;
				if (out_p_strClientIP != NULL)
				{
					/*double dVersion = _ttof(GetWinVersion_Z());
					if (dVersion >= 6.0)
					{
						char sz_cIP[16] = { 0 };
						inet_ntop(AF_INET, &addrClient.sin_addr, sz_cIP, sizeof(sz_cIP) / sizeof(char));
						*out_p_strClientIP = sz_cIP;
					}
					else*/
					*out_p_strClientIP = inet_ntoa(addrClient.sin_addr);
				}
				nRtn= ERROR_OK;
				goto end;
			}
		}
		dwSpan=GetTickCount()-dwStart;
		if(dwSpan>=m_sz_dwTimeout[TIMEOUT_ACCEPT])
		{
			nRtn= ERROR_ACCEPT_TIMEOUT;
			goto end;
		}
	}
	if(m_bIsStopAccept)
		nRtn= ERROR_ACCEPT_STOP;
end:
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZSocket::Send(const char * in_p_cData,const int & in_nSendLen,int & out_nActSendLen)
{
	if(m_sock==INVALID_SOCKET)
		return ERROR_INVALIDSOCK;
	out_nActSendLen=send(m_sock,in_p_cData,in_nSendLen, 0); 
	if(out_nActSendLen==SOCKET_ERROR)
		return ERROR_SEND;
	return ERROR_OK;
}

int ZSocket::Send(const char * in_p_cData,const int & in_nSendLen)
{
	ResetEvent(m_hEvtExitComm);
	int nRtn= ERROR_OK;
	if(m_sock==INVALID_SOCKET)
	{
		nRtn= ERROR_INVALIDSOCK;
		goto end;
	}
	int nActSendLen=0,nActTotalSendLen=0,nSendLen=in_nSendLen;
	DWORD dwStart,dwSpan;
	dwStart=GetTickCount();
	fd_set set;  
	timeval timeVal;
	timeVal.tv_sec=1;
	timeVal.tv_usec=0;
	m_bIsStopSend=false;
	while(!m_bIsStopSend)
	{
		FD_ZERO(&set);  
		FD_SET(m_sock, &set); 
		if(select(0,NULL,&set,NULL,&timeVal)>0)
		{
			nActSendLen=send(m_sock,in_p_cData+nActTotalSendLen,nSendLen, 0); 
			if(nActSendLen==SOCKET_ERROR)
			{
				if(WSAGetLastError()!=WSAEWOULDBLOCK) 
				{
					nRtn= ERROR_SEND;
					goto end;
				}
			}
			else if(nActSendLen==nSendLen)
			{
				nRtn= ERROR_OK;
				goto end;
			}
			else
			{
				nActTotalSendLen+=nActSendLen;
				nSendLen=in_nSendLen-nActTotalSendLen;
			}
		}
		dwSpan=GetTickCount()-dwStart;
		if(dwSpan>=m_sz_dwTimeout[TIMEOUT_SEND])
		{
			nRtn= ERROR_SEND_TIMEOUT;
			goto end;
		}
	}
	if(m_bIsStopSend)
		nRtn= ERROR_SEND_STOP;
end:
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZSocket::Send(const CString & in_strData)
{
	int nStrLen=in_strData.GetLength();
	if(nStrLen==0)
		return ERROR_OK;
	int	nActSendLen,nMaxLen;
	nMaxLen=nStrLen*2+1;
	char *p_cData=new char[nMaxLen];
	if(p_cData==NULL)
		return ERROR_OTHER;
	BOOL nRtnChange=TtoA_Z(in_strData,p_cData,nMaxLen);
	if(!nRtnChange)
	{
		delete []p_cData;
		return ERROR_OTHER;
	}
	int nSendLen=strlen(p_cData);
	int nRtn=Send(p_cData,nSendLen,nActSendLen);
	delete []p_cData;
	if(!nRtn&&nSendLen!=nActSendLen)
		nRtn= ERROR_OTHER;
	return nRtn;
}

int ZSocket::Recv(char * out_p_cData,const int & in_nMaxRecvLen,int & out_nActRecvLen)
{
	if(m_sock==INVALID_SOCKET)
		return ERROR_INVALIDSOCK;
	out_nActRecvLen=recv(m_sock,out_p_cData,in_nMaxRecvLen, 0);
	if(out_nActRecvLen==SOCKET_ERROR)
		return ERROR_RECV;
	if(out_nActRecvLen==0)
		return ERROR_OTHER;
	return ERROR_OK;
}

int ZSocket::Recv(char * out_p_cData,const int & in_nRecvLen)
{
	ResetEvent(m_hEvtExitComm);
	int nRtn= ERROR_OK;
	if(m_sock==INVALID_SOCKET)
	{
		nRtn= ERROR_INVALIDSOCK;
		goto end;
	}
	int nActRecvLen=0,nActTotalRecvLen=0,nRecvLen=in_nRecvLen;
	DWORD dwStart,dwSpan;
	dwStart=GetTickCount();
	fd_set set;  
	timeval timeVal;
	timeVal.tv_sec=1;
	timeVal.tv_usec=0;
	m_bIsStopRecv=false;
	while(!m_bIsStopRecv)
	{
		FD_ZERO(&set);  
		FD_SET(m_sock, &set); 
		if(select(0,&set,NULL,NULL,&timeVal)>0)
		{
			nActRecvLen=recv(m_sock,out_p_cData+nActTotalRecvLen,nRecvLen, 0);
			if(nActRecvLen==SOCKET_ERROR)
			{
				if(WSAGetLastError()!=WSAEWOULDBLOCK) 
				{
					nRtn= ERROR_RECV;
					goto end;
				}
			}
			else if(nActRecvLen==nRecvLen)
			{
				nRtn= ERROR_OK;
				goto end;
			}
			else if(nActRecvLen==0)
			{
				nRtn= ERROR_OTHER;
				goto end;
			}
			else
			{
				nActTotalRecvLen+=nActRecvLen;
				nRecvLen=in_nRecvLen-nActTotalRecvLen;
			}
		}
		dwSpan=GetTickCount()-dwStart;
		if(dwSpan>=m_sz_dwTimeout[TIMEOUT_RECV])
		{
			nRtn= ERROR_RECV_TIMEOUT;
			goto end;
		}
	}
	if(m_bIsStopRecv)
		nRtn= ERROR_RECV_STOP;
end:
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZSocket::Recv(CString & out_strData,const int & in_nMaxRecvLen,const bool & in_bIsBlocking)
{
	ResetEvent(m_hEvtExitComm);
	out_strData.Empty();
	int nRtn= ERROR_OK,nActRecvLen;
	if(in_nMaxRecvLen<=0)
	{
		nRtn= ERROR_OTHER;
		goto end;
	}
	char *p_cData=new char[in_nMaxRecvLen+1];
	if(p_cData==NULL)
	{
		nRtn= ERROR_OTHER;
		goto end;
	}
	if(in_bIsBlocking)
	{
		DWORD dwStart,dwSpan;
		dwStart=GetTickCount();
		fd_set set;  
		timeval timeVal;
		timeVal.tv_sec=1;
		timeVal.tv_usec=0;
		m_bIsStopRecv=false;
		while(!m_bIsStopRecv)
		{
			FD_ZERO(&set);  
			FD_SET(m_sock, &set); 
			if(select(0,&set,NULL,NULL,&timeVal)>0)
			{
				nRtn=Recv(p_cData,in_nMaxRecvLen,nActRecvLen);
				if(nRtn== ERROR_OK)
				{
					p_cData[nActRecvLen]='\0';
					out_strData=p_cData;
					goto end;
				}
				else if(nRtn== ERROR_RECV)
				{
					if(WSAGetLastError()!=WSAEWOULDBLOCK) 
						goto end;
				}
				else
					goto end;
			}
			dwSpan=GetTickCount()-dwStart;
			if(dwSpan>=m_sz_dwTimeout[TIMEOUT_RECV])
			{
				nRtn= ERROR_RECV_TIMEOUT;
				goto end;
			}
		}
		if(m_bIsStopRecv)
			nRtn= ERROR_RECV_STOP;
	}
	else
	{
		nRtn=Recv(p_cData,in_nMaxRecvLen,nActRecvLen);
		if(nRtn== ERROR_OK)
		{
			p_cData[nActRecvLen]='\0';
			out_strData=p_cData;
		}
	}
end:
	if(p_cData)
		delete []p_cData;
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZSocket::Recv(CString & out_strData, const bool & in_bIsBlocking)
{
	ResetEvent(m_hEvtExitComm);
	out_strData.Empty();
	int nRtn = ERROR_OK, nActRecvLen;
	const int nMaxRecvLen = 1024;
	CStringA strRecv;
	if (nMaxRecvLen <= 0)
	{
		nRtn = ERROR_OTHER;
		goto end;
	}
	char *p_cData = new char[nMaxRecvLen + 1];
	if (p_cData == NULL)
	{
		nRtn = ERROR_OTHER;
		goto end;
	}
	if (in_bIsBlocking)
	{
		DWORD dwStart, dwSpan;
		dwStart = GetTickCount();
		fd_set set;
		timeval timeVal;
		timeVal.tv_sec = 1;
		timeVal.tv_usec = 0;
		m_bIsStopRecv = false;
		while (!m_bIsStopRecv)
		{
			FD_ZERO(&set);
			FD_SET(m_sock, &set);
			if (select(0, &set, NULL, NULL, &timeVal)>0)
			{
				nRtn = Recv(p_cData, nMaxRecvLen, nActRecvLen);
				if (nRtn == ERROR_OK)
				{
					p_cData[nActRecvLen] = '\0';
					strRecv += p_cData;
					if (nActRecvLen != nMaxRecvLen)
					{
						out_strData = strRecv;
						goto end;
					}
				}
				else if (nRtn == ERROR_RECV)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
						goto end;
				}
				else
					goto end;
			}
			dwSpan = GetTickCount() - dwStart;
			if (dwSpan >= m_sz_dwTimeout[TIMEOUT_RECV])
			{
				nRtn = ERROR_RECV_TIMEOUT;
				goto end;
			}
		}
		if (m_bIsStopRecv)
			nRtn = ERROR_RECV_STOP;
	}
	else
	{
		nRtn = Recv(p_cData, nMaxRecvLen, nActRecvLen);
		if (nRtn == ERROR_OK)
		{
			p_cData[nActRecvLen] = '\0';
			out_strData = p_cData;
		}
	}
end:
	if (p_cData)
		delete[]p_cData;
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

BOOL ZSocket::ExecReq(const CString & in_strReq,CString & out_strRslt,const int & in_nMaxRecvLen)
{
	int nRtn= ERROR_OK;
	nRtn=Send(in_strReq);
	if(nRtn)
		return FALSE;
	nRtn=Recv(out_strRslt,in_nMaxRecvLen);
	if(nRtn)
		return FALSE;
	return TRUE;
}

void ZSocket::StopComm(void)
{
	m_bIsStopCont=true;
	m_bIsStopAccept=true;
	m_bIsStopSend=true;
	m_bIsStopRecv=true;
	WaitForSingleObject(m_hEvtExitComm,INFINITE);
}

BOOL ZSocket::SetTimeOut(const BYTE & in_cTimeoutIndex,const DWORD & in_dwTimeOut)
{
	if(in_cTimeoutIndex>=MAX_TIMEOUT)
		return FALSE;
	m_sz_dwTimeout[in_cTimeoutIndex]=in_dwTimeOut;
	return TRUE;
}

CString ZSocket::DomainNameToIp(const CString & strDomainName)
{
	CString strIP(strDomainName);
	if(!IsStrIP(strDomainName))
	{ 
		char szDomainName[50] = { 0 };
		if (TtoA_Z(strDomainName, szDomainName, sizeof(szDomainName)))
		{
			addrinfo hints;
			addrinfo *res=NULL;
			sockaddr_in *addr=NULL;
			char sz_cIP[16] = {0};
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_flags = AI_PASSIVE;
			hints.ai_protocol = 0;
			hints.ai_socktype = SOCK_STREAM;
			if (getaddrinfo(szDomainName, NULL, &hints, &res) == 0)
			{
				if (res)
				{
					addr = (struct sockaddr_in *)res->ai_addr;
					/*double dVersion = _ttof(GetWinVersion_Z());
					if (dVersion >= 6.0)
					{
						inet_ntop(AF_INET, &addr->sin_addr, sz_cIP, 16);
						strIP = sz_cIP;
					}
					else*/
					strIP=inet_ntoa(addr->sin_addr);
					freeaddrinfo(res);
				}
			}
		}
	}
	return strIP;
}


bool ZSocket::IsStrIP(const CString & str)
{
	int nLen=str.GetLength();
	int i;
	for(i=0;i<nLen;++i)
	{
		if((str[i]<_T('0')||str[i]>_T('9'))&&str[i]!=_T('.'))
			break;
	}
	if(i<nLen)
		return false;
	else
		return true;
}