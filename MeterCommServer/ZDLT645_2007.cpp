#include "StdAfx.h"
#include "ZDLT645_2007.h"

HMODULE ZDLT645_2007::m_hDll09SGCC=NULL;
HMODULE ZDLT645_2007::m_hDll13SGCC=NULL;
HMODULE ZDLT645_2007::m_hDll13SPG=NULL;
HMODULE ZDLT645_2007::m_hDll15CSG=NULL;
HMODULE ZDLT645_2007::m_hDll18SGC = NULL;

const TCHAR tcSplitChar = _T('#');
const TCHAR tcSplitError = _T('$');
const TCHAR tcSplitValue = _T('@');
const TCHAR tcSplitNetData = _T('*');

ZDLT645_2007::ZDLT645_2007(void)
	: m_strMeterAddr(_T("AAAAAAAAAAAA"))
	, m_strCtrlCode(_T(""))
	, m_strLength(_T(""))
	, m_strDataItem(_T(""))
	, m_strTgtData(_T(""))
	, m_strGrade(_T("02"))
	, m_strPsWd(_T("123456"))
	, m_strOperaCode(_T("00000000"))
	, m_strDiv(_T(""))
	, m_strRand1(_T(""))
	, m_strEndata1(_T(""))
	, m_strRand2(_T("00000000"))
	, m_strEsamId(_T(""))
	, m_strApdu(_T(""))
	, m_strRand2IR(_T(""))
	, m_nDllType(1)
	, m_nIsRemote(FALSE)
	, m_strServerIP(_T("zaxai.nat123.net"))
	, m_strServerPort(_T("44535"))
	, m_strServerTimeOut(_T("5000"))
	, m_strNetCptIP(_T("10.0.17.99"))
	, m_strNetCptPort(_T("6666")) 
	, m_strNetCptTimeOut(_T("3000"))
{
}


ZDLT645_2007::~ZDLT645_2007(void)
{
}


int ZDLT645_2007::CallExDll(void)
{
	CString strPath=ZUtil::GetExeCatalogPath();
	m_hDll09SGCC=LoadLibraryEx(strPath+_T("\\exdll\\TestZhuzhan.dll"),NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hDll09SGCC==NULL)  
		return 1;
	m_hDll13SGCC= LoadLibraryEx(strPath+_T("\\exdll\\GWV2RemoteD1000.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hDll13SGCC==NULL)  
		return 2;
	m_hDll13SPG= LoadLibraryEx(strPath+_T("\\exdll\\GWV2RemoteD1000-SX.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hDll13SPG==NULL)  
		return 3;
	m_hDll15CSG= LoadLibraryEx(strPath+_T("\\exdll\\MasterStation_HSM.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hDll15CSG==NULL)  
		return 4;
	m_hDll18SGC = LoadLibraryEx(strPath + _T("\\exdll\\18SGC\\Detecting_EMF_HSM.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (m_hDll18SGC == NULL)
		return 5;
	return 0;
}


void ZDLT645_2007::UncallExDll(void)
{
	if(m_hDll09SGCC!=NULL)
		FreeLibrary(m_hDll09SGCC);
	if(m_hDll13SGCC!=NULL)
		FreeLibrary(m_hDll13SGCC);
	if(m_hDll13SPG!=NULL)
		FreeLibrary(m_hDll13SPG);
	if(m_hDll15CSG!=NULL)
		FreeLibrary(m_hDll15CSG);
	if (m_hDll18SGC != NULL)
		FreeLibrary(m_hDll18SGC);
}

bool ZDLT645_2007::CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen)
{
	CString strByte;
	int i=0,j=0,nLen;
	nLen=in_str.GetLength()/2;
	if(nLen>=in_nMaxArrayLen)
		return false;
	for(;i<nLen;++i,j=j+2)
	{
		strByte=in_str.Mid(j,2);
		out_pArray[i]=BYTE(_tcstol(strByte,NULL ,16));
	}
	out_pArray[nLen]='\0';
	return true;
}

CString ZDLT645_2007::CStringBy2Plus33(CString str)
{
	str.MakeUpper();
	CString strTemp;
	int i,nByte,nSize;
	CString strInByte,strOutByte;
	nSize=str.GetLength();
	if(nSize%2)
		--nSize;
	if(nSize<2)
		return strTemp;
	for(i=0;i<nSize;i=i+2)
	{
		strInByte=str.Mid(i,2);
		nByte=_tcstol(strInByte,NULL ,16);
		nByte+=0x33;
		strOutByte.Format(_T("%02X"),nByte);
		strOutByte=strOutByte.Right(2);
		strTemp+=strOutByte;
	}
	return strTemp;
}


CString ZDLT645_2007::Reverse0406XXFF(CString str)
{
	str.MakeUpper();
	CString strRtn;
	int i,nSize;
	nSize=str.GetLength();
	if(nSize!=128&&nSize!=140)
		return CString(_T(""));
	for(i=0;i<(nSize-13*8)/6;++i)
	{
		strRtn+=str.Right(6);
		str=str.Left(nSize-6*(1+i));
	}
	nSize=str.GetLength();
	for(i=0;i<13;++i)
	{
		strRtn+=str.Right(8);
		str=str.Left(nSize-8*(1+i));
	}
	return strRtn;
}

CString ZDLT645_2007::ReverseCStringBy8(CString str)
{
	str.MakeUpper();
	CString strRtn;
	int i,nSize,nTimes;
	nSize=str.GetLength();
	if(nSize<8)
		return CString(_T(""));
	nTimes=nSize/8;
	str=str.Left(nTimes*8);
	for(i=0;i<nTimes;++i)
	{
		strRtn+=str.Right(8);
		str=str.Left(nSize-8*(1+i));
	}
	return strRtn;
}

CString ZDLT645_2007::ReverseCStringBy2(CString str)
{
	str.MakeUpper();
	str.MakeReverse();
	int i;
	int nSize=str.GetLength();
	if(nSize%2)
		--nSize;
	if(nSize<2)
		return CString(_T(""));
	TCHAR tcTemp;
	TCHAR *p_tc=new TCHAR[nSize+1];
	for(i=0;i<nSize;++i)
	{
		p_tc[i]=str[i];
	}
	p_tc[nSize]=_T('\0');
	for(i=0;i<nSize;i=i+2)
	{
		tcTemp=p_tc[i];
		p_tc[i]=p_tc[i+1];
		p_tc[i+1]=tcTemp;
	}
	str=p_tc;
	delete []p_tc;
	return str;
}

CString ZDLT645_2007::ReverseCStringBy2Plus33(CString str)
{
	str=ReverseCStringBy2(str);
	CString strTemp;
	int i,nByte,nSize;
	CString strInByte,strOutByte;
	nSize=str.GetLength();
	if(nSize<2)
		return strTemp;
	for(i=0;i<nSize;i=i+2)
	{
		strInByte=str.Mid(i,2);
		nByte=_tcstol(strInByte,NULL ,16);
		nByte+=0x33;
		strOutByte.Format(_T("%02X"),nByte);
		strOutByte=strOutByte.Right(2);
		strTemp+=strOutByte;
	}
	return strTemp;
}

int ZDLT645_2007::SetIp13SGCC(void)
{
	typedef int (_stdcall *SetIp)(IN char *pIp, IN int iPort, IN int iTimeOut);
	SetIp si;
	int nRtn=-1;
	si=(SetIp)GetProcAddress(m_hDll13SGCC,"SetIp");
	if(si)
	{
		int nPort,nTimeOut;
		char szIp[20]={0};
		nPort=_ttoi(m_strNetCptPort);
		nTimeOut=_ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP,szIp,sizeof(szIp));
		nRtn=si(szIp,nPort,nTimeOut);
	}
	return nRtn;
}

int ZDLT645_2007::SetIp13SPG(void)
{
	typedef int (_stdcall *SetIp)(IN char *pIp, IN int iPort, IN int iTimeOut);
	SetIp si;
	int nRtn=-1;
	si=(SetIp)GetProcAddress(m_hDll13SPG,"SetIp");
	if(si)
	{
		int nPort,nTimeOut;
		char szIp[20]={0};
		nPort=_ttoi(m_strNetCptPort);
		nTimeOut=_ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP,szIp,sizeof(szIp));
		nRtn=si(szIp,nPort,nTimeOut);
	}
	return nRtn;
}

int ZDLT645_2007::OpenDevice15CSG(void)
{
	typedef int (_stdcall *OpenDevice)(const char * szType,const char * cHostIp,unsigned int uiPort, unsigned int timeout);
	OpenDevice od;
	int nRtn=-1;
	od=(OpenDevice)GetProcAddress(m_hDll15CSG,"OpenDevice");
	if(od)
	{
		unsigned int nPort,nTimeOut;
		char szIp[20]={0};
		nPort=_ttoi(m_strNetCptPort);
		nTimeOut=_ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP,szIp,sizeof(szIp));
		nRtn=od("南网加密机",szIp,nPort,nTimeOut/1000);
	}
	return nRtn;
}

CString ZDLT645_2007::CloseDevice15CSG(void)
{
	typedef int (_stdcall *CloseDevice)(void);
	CloseDevice cd;
	int nRtn;
	CString strRtn;
	cd=(CloseDevice)GetProcAddress(m_hDll15CSG,"CloseDevice");
	if(cd)
	{
		nRtn=cd();
		switch(nRtn)
		{
		case 0:
			strRtn=_T("关闭加密机成功!");
			break; 
		default:
			strRtn=_T("关闭加密机失败!");
		}
	}
	return strRtn;
}

CString ZDLT645_2007::ErrorInfoID09SGCC(const int & nError)
{
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch (nError)
	{
	case -1: strErrorInfo += _T("DLL调用出错"); break;
	case 200: strErrorInfo += _T("连接加密机失败"); break;
	case 201: strErrorInfo += _T("取随机数1失败"); break;
	case 202: strErrorInfo += _T("取随机数2失败"); break;
	case 203: strErrorInfo += _T("密钥分散失败"); break;
	case 204: strErrorInfo += _T("数据加密失败"); break;
	case 205: strErrorInfo += _T("取密文失败"); break;
	}
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfoUC09SGCC(const int & nError)
{
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch (nError)
	{
	case -1: strErrorInfo += _T("DLL调用出错"); break;
	case 200: strErrorInfo += _T("连接加密机失败"); break;
	case 201: strErrorInfo += _T("写卡失败"); break;
	case 202: strErrorInfo += _T("读卡失败"); break;
	case 203: strErrorInfo += _T("计算密文失败"); break;
	}
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfo09SGCC(const int & nError)
{
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch (nError)
	{
	case -1: strErrorInfo += _T("DLL调用出错"); break;
	case 200: strErrorInfo += _T("连接加密机失败"); break;
	case 201: strErrorInfo += _T("写卡失败"); break;
	case 202: strErrorInfo += _T("读卡失败"); break;
	case 203: strErrorInfo += _T("计算MAC失败"); break;
	}
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfo13SGCC(const int & nError)
{
	typedef int (_stdcall *GetErrInfo)(IN int iErrCode, OUT char *pErrorInfo);
	char szErrorInfo[200]={0};
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	GetErrInfo gei;
	gei=(GetErrInfo)GetProcAddress(m_hDll13SGCC,"GetErrInfo");
	if(gei)
		gei(nError,szErrorInfo);
	if (nError == -1)
		strErrorInfo+=_T("DLL调用出错");
	else
		strErrorInfo += szErrorInfo;
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfo13SPG(const int & nError)
{
	typedef int (_stdcall *GetErrInfo)(IN int iErrCode, OUT char *pErrorInfo);
	char szErrorInfo[200]={0};
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	GetErrInfo gei;
	gei=(GetErrInfo)GetProcAddress(m_hDll13SPG,"GetErrInfo");
	if (gei)
		gei(nError,szErrorInfo);
	if (nError == -1)
		strErrorInfo += _T("DLL调用出错");
	else
		strErrorInfo += szErrorInfo;
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfo15CSG(const int & nError)
{
	
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch (nError)
	{
	case -1: strErrorInfo += _T("DLL调用出错"); break;
	case 202: strErrorInfo += _T("打开密码机错误"); break;
	case 203: strErrorInfo += _T("关闭密码机错误"); break;
	case 306: strErrorInfo += _T("取随机数错误"); break;
	case 700: strErrorInfo += _T("密钥导出错误"); break;
	case 810: strErrorInfo += _T("MAC效验错误"); break;
	case 900: strErrorInfo += _T("数据加密错误"); break;
	case 902: strErrorInfo += _T("MAC计算错误"); break;
	case 1100: strErrorInfo += _T("认证错误,红外认证时比对密文"); break;
	case 1501: strErrorInfo += _T("参数1错误"); break;
	case 1502: strErrorInfo += _T("参数2错误"); break;
	case 1503: strErrorInfo += _T("参数3错误"); break;
	case 1504: strErrorInfo += _T("参数4错误"); break;
	case 1505: strErrorInfo += _T("参数5错误"); break;
	case 1506: strErrorInfo += _T("参数6错误"); break;
	case 1507: strErrorInfo += _T("参数7错误"); break;
	case 1508: strErrorInfo += _T("参数8错误"); break;
	case 1601: strErrorInfo += _T("不支持的密码机类型"); break;
	case 2000: strErrorInfo += _T("USB Key验证错误"); break;
	}
	return strErrorInfo;
}

CString ZDLT645_2007::ErrorInfo18SGC(const int & nError)
{

	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch (nError)
	{
	case -1: strErrorInfo += _T("DLL调用出错"); break;
	case 1000: strErrorInfo += _T("打开密码机错误"); break;
	case 1600: strErrorInfo += _T("打开读卡器错误"); break;
	case 1601: strErrorInfo += _T("芯片复位错误"); break;
	case 1602: strErrorInfo += _T("卡片类型错误"); break;
	case 1603: strErrorInfo += _T("卡片状态错误"); break;
	case 1604: strErrorInfo += _T("外部认证错误"); break;
	case 2100: strErrorInfo += _T("参数不支持"); break;
	case 2101: strErrorInfo += _T("参数1错误"); break;
	case 2102: strErrorInfo += _T("参数2错误"); break;
	case 2103: strErrorInfo += _T("参数3错误"); break;
	case 2104: strErrorInfo += _T("参数4错误"); break;
	case 2105: strErrorInfo += _T("参数5错误"); break;
	case 2106: strErrorInfo += _T("参数6错误"); break;
	case 2107: strErrorInfo += _T("参数7错误"); break;
	case 2108: strErrorInfo += _T("参数8错误"); break;
	case 2109: strErrorInfo += _T("参数9错误"); break;
	case 2110: strErrorInfo += _T("参数10错误"); break;
	default:
	{
		switch (nError / 100)
		{
		case 11: strErrorInfo += _T("数据加密错误"); break;
		case 12: strErrorInfo += _T("MAC计算错误"); break;
		case 13: strErrorInfo += _T("MAC校验错误"); break;
		case 14: strErrorInfo += _T("取随机数错误"); break;
		case 15: strErrorInfo += _T("密钥导出错误"); break;
		case 17: strErrorInfo += _T("写文件错误"); break;
		case 18: strErrorInfo += _T("读文件错误"); break;
		case 19: strErrorInfo += _T("更新密钥错误"); break;
		case 20: strErrorInfo += _T("选择文件错误"); break;
		}
	}
	break;
	}
	return strErrorInfo;
}

int ZDLT645_2007::InfraredRand13SGCC(void)
{
	typedef int (_stdcall *InfraredRand)(IN char *pOutput);
	char szOutRand[50]={0};
	InfraredRand ir;
	int nRtn=-1;
	ir=(InfraredRand)GetProcAddress(m_hDll13SGCC,"InfraredRand");
	if(ir)
	{	
     	nRtn=ir(szOutRand);//红外认证
		if(!nRtn)
			m_strRand1 = szOutRand;
	}
	return nRtn;
}

int ZDLT645_2007::InfraredRand13SPG(void)
{
	typedef int (_stdcall *InfraredRand)(IN char *pOutput);
	char szOutRand[50]={0};
	InfraredRand ir;
	int nRtn=-1;
	ir=(InfraredRand)GetProcAddress(m_hDll13SPG,"InfraredRand");
	if(ir)
	{	
     	nRtn=ir(szOutRand);//红外认证
		if (!nRtn)
			m_strRand1 = szOutRand;
	}
	return nRtn;
}

int ZDLT645_2007::InfraredRand15CSG(void)
{
	typedef int (_stdcall *InfraredRand)(char *pOutRand1);
	char szOutRand[50]={0};
	InfraredRand ir;
	int nRtn=-1;
	ir=(InfraredRand)GetProcAddress(m_hDll15CSG,"InfraredRand");
	if(ir)
	{	
     	nRtn=ir(szOutRand);//红外认证
		if (!nRtn)
			m_strRand1 = szOutRand;
	}
	return nRtn;
}

int ZDLT645_2007::InfraredAuth13SGCC(CString & strData)
{
	typedef int (_stdcall *InfraredAuth)(IN char *pMeterNo, IN char *pEsamNo, IN char *pRandom1, IN char *pRandomEnc1, IN char *pRandom2, OUT char *pRandomEnc2);
	char szDiv[20]={0},szEsamId[18]={0},szRand1[20]={0},szEndata1[20]={0},szRand2IR[20]={0},szEndata2[20]={0};
	InfraredAuth ia;
	int nRtn=-1;
	ia=(InfraredAuth)GetProcAddress(m_hDll13SGCC,"InfraredAuth");
	if(ia)
	{
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(m_strRand1,szRand1,sizeof(szRand1));
		ZUtil::WtoA(m_strEndata1,szEndata1,sizeof(szEndata1));
		ZUtil::WtoA(m_strRand2IR,szRand2IR,sizeof(szRand2IR));
		nRtn=ia(szDiv,szEsamId,szRand1,szEndata1,szRand2IR,szEndata2);
		if(!nRtn)
			strData=szEndata2;
	}
	return nRtn;
}

int ZDLT645_2007::InfraredAuth13SPG(CString & strData)
{
	typedef int (_stdcall *InfraredAuth)(IN char *pMeterNo, IN char *pEsamNo, IN char *pRandom1, IN char *pRandomEnc1, IN char *pRandom2, OUT char *pRandomEnc2);
	char szDiv[20]={0},szEsamId[18]={0},szRand1[20]={0},szEndata1[20]={0},szRand2IR[20]={0},szEndata2[20]={0};
	InfraredAuth ia;
	int nRtn=-1;
	ia=(InfraredAuth)GetProcAddress(m_hDll13SPG,"InfraredAuth");
	if(ia)
	{
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(m_strRand1,szRand1,sizeof(szRand1));
		ZUtil::WtoA(m_strEndata1,szEndata1,sizeof(szEndata1));
		ZUtil::WtoA(m_strRand2IR,szRand2IR,sizeof(szRand2IR));
		nRtn=ia(szDiv,szEsamId,szRand1,szEndata1,szRand2IR,szEndata2);
		if(!nRtn)
			strData=szEndata2;
	}
	return nRtn;
}

int ZDLT645_2007::InfraredAuth15CSG(CString & strData)
{
	typedef int (_stdcall *InfraredAuth)(int Flag, char * PutDiv,char * PutEsamNo,char * PutRand1,char * PutRand1Endata,char * PutRand2,char * OutRand2Endata);
	char szDiv[20]={0},szEsamId[18]={0},szRand1[20]={0},szEndata1[20]={0},szRand2IR[20]={0},szEndata2[20]={0};
	InfraredAuth ia;
	int nRtn=-1;
	ia=(InfraredAuth)GetProcAddress(m_hDll15CSG,"InfraredAuth");
	if(ia)
	{
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(m_strRand1,szRand1,sizeof(szRand1));
		ZUtil::WtoA(m_strEndata1,szEndata1,sizeof(szEndata1));
		ZUtil::WtoA(m_strRand2IR,szRand2IR,sizeof(szRand2IR));
		nRtn=ia(0,szDiv,szEsamId,szRand1,szEndata1,szRand2IR,szEndata2);
		if(!nRtn)
			strData=szEndata2;
	}
	return nRtn;
}

int ZDLT645_2007::ESAMIdentityAuthentication18SGC(CString & strData)
{
	typedef int(_stdcall *ESAMIdentityAuthentication)(char * IP, int Port, int TimeOut, int Flag, char * inRand, char *OutEndata);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szRand[20] = { 0 }, szEndata[20] = { 0 };
	ESAMIdentityAuthentication eia;
	int nRtn = -1;
	eia = (ESAMIdentityAuthentication)GetProcAddress(m_hDll18SGC, "ESAMIdentityAuthentication");
	if (eia)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strRand2+_T("00000000"), szRand, sizeof(szRand));
		nRtn = eia(szIp, nPort, nTimeOut / 1000, 0, szRand, szEndata);//ESAM外部认证
		if (!nRtn)
		{
			strData = szEndata;
		}
	}
	return nRtn;
}

int ZDLT645_2007::IdentityAuthentication18SGC(void)
{
	typedef int(_stdcall *IdentityAuthentication)(char * IP, int Port, int TimeOut, int Flag, char * PutDiv, char * OutRand, char *pOutEndata);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szOutRand[50] = { 0 }, szOutCipherText[50] = { 0 }, szDiv[20] = { 0 };
	IdentityAuthentication ia;
	int nRtn = -1;
	ia = (IdentityAuthentication)GetProcAddress(m_hDll18SGC, "IdentityAuthentication");
	if (ia)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strDiv, szDiv, sizeof(szDiv));
		nRtn = ia(szIp, nPort, nTimeOut / 1000, 0, szDiv, szOutRand, szOutCipherText);//身份认证
		if (!nRtn)
		{
			m_strRand1 = szOutRand;
			m_strEndata1 = szOutCipherText;
		}
	}
	return nRtn;
}

int ZDLT645_2007::IdentityAuthentication15CSG(void)
{
	typedef int (_stdcall *IdentityAuthentication)(int Flag,char * PutDiv,char * OutRand,char *pOutEndata);
	char szOutRand[50]={0},szOutCipherText[50]={0},szDiv[20]={0};
	IdentityAuthentication ia;
	int nRtn=-1;
	ia=(IdentityAuthentication)GetProcAddress(m_hDll15CSG,"IdentityAuthentication");
	if(ia)
	{	
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
     	nRtn=ia(0,szDiv,szOutRand,szOutCipherText);//身份认证
		if(!nRtn)
		{
			m_strRand1=szOutRand;
			m_strEndata1=szOutCipherText;
		}
	}
	return nRtn;
}

int ZDLT645_2007::IdentityAuthentication13SPG(void)
{
	typedef int (_stdcall *IdentityAuthentication)(IN char *pMeterNo, OUT char *pOutRand, OUT char *pOutCipherText);
	char szOutRand[50]={0},szOutCipherText[50]={0},szDiv[20]={0};
	IdentityAuthentication ia;
	int nRtn=-1;
	ia=(IdentityAuthentication)GetProcAddress(m_hDll13SPG,"IdentityAuthentication");
	if(ia)
	{	
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
     	nRtn=ia(szDiv,szOutRand,szOutCipherText);//身份认证
		if(!nRtn)
		{
			m_strRand1=szOutRand;
			m_strEndata1=szOutCipherText;
		}
	}
	return nRtn;
}

int ZDLT645_2007::IdentityAuthentication13SGCC(void)
{
	typedef int (_stdcall *IdentityAuthentication)(IN char *pMeterNo, OUT char *pOutRand, OUT char *pOutCipherText);
	char szOutRand[50]={0},szOutCipherText[50]={0},szDiv[20]={0};
	IdentityAuthentication ia;
	int nRtn=-1;
	ia=(IdentityAuthentication)GetProcAddress(m_hDll13SGCC,"IdentityAuthentication");
	if(ia)
	{	
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
     	nRtn=ia(szDiv,szOutRand,szOutCipherText);//身份认证
		if(!nRtn)
		{
			m_strRand1=szOutRand;
			m_strEndata1=szOutCipherText;
		}
	}
	return nRtn;
}

int ZDLT645_2007::IdentityAuthentication09SGCC(void)
{
	typedef int (_stdcall *IdentityAuthentication)(char *Div,char *RandAndEndata);
	char szRandAndEndata[50]={0},szDiv[20]={0};
	CString strRandAndEndata;
	IdentityAuthentication ia;
	int nRtn=-1;
	ia=(IdentityAuthentication)GetProcAddress(m_hDll09SGCC,"IdentityAuthentication");
	if(ia)
	{	
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
     	nRtn=ia(szDiv,szRandAndEndata);//身份认证
		if(!nRtn)
		{
			strRandAndEndata=szRandAndEndata;
			m_strRand1=strRandAndEndata.Left(16);
			m_strEndata1=strRandAndEndata.Mid(16,16);
		}
	}
	return nRtn;
}

int ZDLT645_2007::ParameterElseUpdate18SGC(CString & strData)
{
	typedef int(_stdcall *ParameterElseUpdate)(char * IP,int Port,int TimeOut,int Flag, char * PutRand, char * PutDiv, char * PutApdu, char * PutData, char * OutEndata);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szRand2[10] = { 0 }, szDiv[20] = { 0 }, szApdu[12] = { 0 }, szDataIn[500] = { 0 }, szDataOut[500] = { 0 };
	ParameterElseUpdate peu;
	int nRtn = -1;
	peu = (ParameterElseUpdate)GetProcAddress(m_hDll18SGC, "ParameterElseUpdate");
	if (peu)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strRand2, szRand2, sizeof(szRand2));
		ZUtil::WtoA(m_strDiv, szDiv, sizeof(szDiv));
		ZUtil::WtoA(m_strApdu, szApdu, sizeof(szApdu));
		ZUtil::WtoA(strData, szDataIn, sizeof(szDataIn));
		nRtn = peu(szIp, nPort, nTimeOut / 1000, 0, szRand2, szDiv, szApdu, szDataIn, szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterElseUpdate15CSG(CString & strData)
{
	typedef int (_stdcall *ParameterElseUpdate)(int Flag,char * PutRand,char * PutDiv,char * PutApdu,char * PutData,char * OutEndata);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterElseUpdate peu;
	int nRtn=-1;
	peu=(ParameterElseUpdate)GetProcAddress(m_hDll15CSG,"ParameterElseUpdate");
	if(peu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=peu(0,szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterElseUpdate13SPG(CString & strData)
{
	typedef int (_stdcall *ParameterElseUpdate)(IN char *pRandom, IN char *pMeterNo,IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterElseUpdate peu;
	int nRtn=-1;
	peu=(ParameterElseUpdate)GetProcAddress(m_hDll13SPG,"ParameterElseUpdate");
	if(peu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=peu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterElseUpdate13SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterElseUpdate)(IN char *pRandom, IN char *pMeterNo,IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterElseUpdate peu;
	int nRtn=-1;
	peu=(ParameterElseUpdate)GetProcAddress(m_hDll13SGCC,"ParameterElseUpdate");
	if(peu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=peu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterElseUpdate09SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterElseUpdate)(char *RandDivApduData,char *EsamNum,char *dataout);
	CString strRandDivApduData;
	char szRandDivApduData[500]={0},szEsamId[18]={0},szDataOut[500]={0};
	ParameterElseUpdate peu;
	int nRtn=-1;
	peu=(ParameterElseUpdate)GetProcAddress(m_hDll09SGCC,"ParameterElseUpdate");
	if(peu)
	{
		strRandDivApduData=m_strRand2+m_strDiv+m_strApdu+strData;
		ZUtil::WtoA(strRandDivApduData,szRandDivApduData,sizeof(szRandDivApduData));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		nRtn=peu(szRandDivApduData,szEsamId,szDataOut);
		if(!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate18SGC(CString & strData)
{
	typedef int(_stdcall *ParameterUpdate)(char * IP, int Port, int TimeOut, int Flag, char * PutRand, char * PutDiv, char * PutApdu, char * PutData, char * OutData);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szRand2[10] = { 0 }, szDiv[20] = { 0 }, szApdu[12] = { 0 }, szDataIn[500] = { 0 }, szDataOut[500] = { 0 };
	ParameterUpdate pu;
	int nRtn = -1;
	pu = (ParameterUpdate)GetProcAddress(m_hDll18SGC, "ParameterUpdate");
	if (pu)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strRand2, szRand2, sizeof(szRand2));
		ZUtil::WtoA(m_strDiv, szDiv, sizeof(szDiv));
		ZUtil::WtoA(m_strApdu, szApdu, sizeof(szApdu));
		ZUtil::WtoA(strData, szDataIn, sizeof(szDataIn));
		nRtn = pu(szIp, nPort, nTimeOut / 1000, 0, szRand2, szDiv, szApdu, szDataIn, szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate15CSG(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate)(int Flag,char * PutRand,char * PutDiv,char * PutApdu,char * PutData,char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate pu;
	int nRtn=-1;
	pu=(ParameterUpdate)GetProcAddress(m_hDll15CSG,"ParameterUpdate");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(0,szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::Price1Update15CSG(CString & strData)
{
	typedef int (_stdcall *Price1Update)(int Flag,char * PutRand,char * PutDiv,char * PutApdu,char * PutData,char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	Price1Update pu;
	int nRtn=-1;
	pu=(Price1Update)GetProcAddress(m_hDll15CSG,"Price1Update");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(0,szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::Price2Update15CSG(CString & strData)
{
	typedef int (_stdcall *Price2Update)(int Flag,char * PutRand,char * PutDiv,char * PutApdu,char * PutData,char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	Price2Update pu;
	int nRtn=-1;
	pu=(Price2Update)GetProcAddress(m_hDll15CSG,"Price2Update");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(0,szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate13SPG(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate pu;
	int nRtn=-1;
	pu=(ParameterUpdate)GetProcAddress(m_hDll13SPG,"ParameterUpdate");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate113SPG(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate1)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate1 pu;
	int nRtn=-1;
	pu=(ParameterUpdate1)GetProcAddress(m_hDll13SPG,"ParameterUpdate1");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate213SPG(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate2)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate2 pu;
	int nRtn=-1;
	pu=(ParameterUpdate2)GetProcAddress(m_hDll13SPG,"ParameterUpdate2");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate13SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate pu;
	int nRtn=-1;
	pu=(ParameterUpdate)GetProcAddress(m_hDll13SGCC,"ParameterUpdate");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate113SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate1)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate1 pu;
	int nRtn=-1;
	pu=(ParameterUpdate1)GetProcAddress(m_hDll13SGCC,"ParameterUpdate1");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate213SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate2)(IN char *pRandom, IN char *pMeterNo, IN char *pApdu, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szApdu[12]={0},szDataIn[500]={0},szDataOut[500]={0};
	ParameterUpdate2 pu;
	int nRtn=-1;
	pu=(ParameterUpdate2)GetProcAddress(m_hDll13SGCC,"ParameterUpdate2");
	if(pu)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strApdu,szApdu,sizeof(szApdu));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=pu(szRand2,szDiv,szApdu,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate09SGCC(CString & strData)
{
	typedef int (_stdcall *ParameterUpdate)(char *RandDivApduData,char *dataout);
	CString strRandDivApduData;
	char szRandDivApduData[500]={0},szDataOut[500]={0};
	ParameterUpdate pu;
	int nRtn=-1;
	pu=(ParameterUpdate)GetProcAddress(m_hDll09SGCC,"ParameterUpdate");
	if(pu)
	{
		strRandDivApduData=m_strRand2+m_strDiv+m_strApdu+strData;
		ZUtil::WtoA(strRandDivApduData,szRandDivApduData,sizeof(szRandDivApduData));
		nRtn=pu(szRandDivApduData,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate109SGCC(CString & strData)
{
	typedef int (_stdcall *Parameter1)(char *RandDivApduData,char *dataout);
	CString strRandDivApduData;
	char szRandDivApduData[500]={0},szDataOut[500]={0};
	Parameter1 pu;
	int nRtn=-1;
	pu=(Parameter1)GetProcAddress(m_hDll09SGCC,"Parameter1");
	if(pu)
	{
		strRandDivApduData=m_strRand2+m_strDiv+m_strApdu+strData;
		ZUtil::WtoA(strRandDivApduData,szRandDivApduData,sizeof(szRandDivApduData));
		nRtn=pu(szRandDivApduData,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ParameterUpdate209SGCC(CString & strData)
{
	typedef int (_stdcall *Parameter2)(char *RandDivApduData,char *dataout);
	CString strRandDivApduData;
	char szRandDivApduData[500]={0},szDataOut[500]={0};
	Parameter2 pu;
	int nRtn=-1;
	pu=(Parameter2)GetProcAddress(m_hDll09SGCC,"Parameter2");
	if(pu)
	{
		strRandDivApduData=m_strRand2+m_strDiv+m_strApdu+strData;
		ZUtil::WtoA(strRandDivApduData,szRandDivApduData,sizeof(szRandDivApduData));
		nRtn=pu(szRandDivApduData,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

void ZDLT645_2007::GetN1NmData(CString & strData)
{
	CString strTime,strTimeR,strTemp,strRtn;
	int nYear;
	CTime curTime = CTime::GetCurrentTime();    
	nYear = curTime.GetYear();  // 获取当前年份   
	strTime = curTime.Format(_T("%m%d%H%M%S"));
	strTimeR= curTime.Format(_T("%S%M%H%d%m"));
	++nYear;
	strTemp.Format(_T("%02d"),nYear);
	strTemp=strTemp.Right(2);
	strTime=strTemp+strTime;
	strTimeR+=strTemp;
	if(m_strCtrlCode==_T("1A"))
	{
		strData=_T("1A00")+strTime;
	}
	if(m_strCtrlCode==_T("1B"))
	{
		strData=_T("1B00")+strTime;
	}
	if(m_strCtrlCode==_T("1C"))
	{
		strData+=_T("00");
		if(m_strGrade==_T("02")||m_strGrade==_T("04")||m_strGrade==_T("03")||m_strGrade==_T("00")||m_strGrade==_T("11"))
		{
			strData+=strTimeR;
		}
		if(m_strGrade==_T("98")||m_strGrade==_T("96"))
		{
			strData+=strTime;
		}
	}
}

int ZDLT645_2007::DataClear115CSG(CString & strData)
{
	typedef int (_stdcall *DataClear1)(int Flag, char * PutRand,char * PutDiv,char * PutData,char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear1 dc1;
	int nRtn=-1;
	dc1=(DataClear1)GetProcAddress(m_hDll15CSG,"DataClear1");
	if(dc1)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc1(0,szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::DataClear113SPG(CString & strData)
{
	typedef int (_stdcall *DataClear1)(IN char *pRandom, IN char *pMeterNo, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear1 dc1;
	int nRtn=-1;
	dc1=(DataClear1)GetProcAddress(m_hDll13SPG,"DataClear1");
	if(dc1)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc1(szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::DataClear113SGCC(CString & strData)
{
	typedef int (_stdcall *DataClear1)(IN char *pRandom, IN char *pMeterNo, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear1 dc1;
	int nRtn=-1;
	dc1=(DataClear1)GetProcAddress(m_hDll13SGCC,"DataClear1");
	if(dc1)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc1(szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::DataClear215CSG(CString & strData)
{
	typedef int (_stdcall *DataClear2)(int Flag, char * PutRand,char * PutDiv,char * PutData,char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear2 dc2;
	int nRtn=-1;
	dc2=(DataClear2)GetProcAddress(m_hDll15CSG,"DataClear2");
	if(dc2)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc2(0,szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::DataClear213SPG(CString & strData)
{
	typedef int (_stdcall *DataClear2)(IN char *pRandom, IN char *pMeterNo, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear2 dc2;
	int nRtn=-1;
	dc2=(DataClear2)GetProcAddress(m_hDll13SPG,"DataClear2");
	if(dc2)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc2(szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::DataClear213SGCC(CString & strData)
{
	typedef int (_stdcall *DataClear2)(IN char *pRandom, IN char *pMeterNo, IN char *pData, OUT char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	DataClear2 dc2;
	int nRtn=-1;
	dc2=(DataClear2)GetProcAddress(m_hDll13SGCC,"DataClear2");
	if(dc2)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=dc2(szRand2,szDiv,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::UserControl18SGC(CString & strData)
{
	typedef int(_stdcall *UserControl)(char * IP, int Port, int TimeOut, int Flag, char * PutRand, char * PutDiv, char * PutEsamNo, char * PutData, char * OutEndata);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szRand2[10] = { 0 }, szDiv[20] = { 0 }, szEsamId[18] = { 0 }, szDataIn[500] = { 0 }, szDataOut[500] = { 0 };
	UserControl uc;
	int nRtn = -1;
	uc = (UserControl)GetProcAddress(m_hDll18SGC, "UserControl");
	if (uc)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strRand2, szRand2, sizeof(szRand2));
		ZUtil::WtoA(m_strDiv, szDiv, sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId, szEsamId, sizeof(szEsamId));
		ZUtil::WtoA(strData, szDataIn, sizeof(szDataIn));
		nRtn = uc(szIp, nPort, nTimeOut / 1000, 0, szRand2, szDiv, szEsamId, szDataIn, szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::UserControl15CSG(CString & strData)
{
	typedef int (_stdcall *UserControl)(int Flag,char * PutRand, char * PutDiv, char * PutEsamNo, char * PutData, char * OutEndata);
	char szRand2[10]={0},szDiv[20]={0},szEsamId[18]={0},szDataIn[500]={0},szDataOut[500]={0};
	UserControl uc;
	int nRtn=-1;
	uc=(UserControl)GetProcAddress(m_hDll15CSG,"UserControl");
	if(uc)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=uc(0,szRand2,szDiv,szEsamId,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::UserControl13SPG(CString & strData)
{
	typedef int (_stdcall *UserControl)(char *pRandom, char *pMeterNo, char *pEsamNo, char *pData, char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szEsamId[18]={0},szDataIn[500]={0},szDataOut[500]={0};
	UserControl uc;
	int nRtn=-1;
	uc=(UserControl)GetProcAddress(m_hDll13SPG,"UserControl");
	if(uc)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=uc(szRand2,szDiv,szEsamId,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::UserControl13SGCC(CString & strData)
{
	typedef int (_stdcall *UserControl)(char *pRandom, char *pMeterNo, char *pEsamNo, char *pData, char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szEsamId[18]={0},szDataIn[500]={0},szDataOut[500]={0};
	UserControl uc;
	int nRtn=-1;
	uc=(UserControl)GetProcAddress(m_hDll13SGCC,"UserControl");
	if(uc)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(m_strEsamId,szEsamId,sizeof(szEsamId));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=uc(szRand2,szDiv,szEsamId,szDataIn,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::UserControl09SGCC(CString & strData)
{
	typedef int (_stdcall *UserControl)(char *RandDivEsamNumData,char *dataOut);
	CString strRandDivEsamNumData;
	char szRandDivEsamNumData[500]={0},szDataOut[500]={0};
	UserControl uc;
	int nRtn=-1;
	uc=(UserControl)GetProcAddress(m_hDll09SGCC,"UserControl");
	if(uc)
	{
		strRandDivEsamNumData=m_strRand2+m_strDiv+m_strEsamId+strData;
		ZUtil::WtoA(strRandDivEsamNumData,szRandDivEsamNumData,sizeof(szRandDivEsamNumData));
		nRtn=uc(szRandDivEsamNumData,szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::InitPurse18SGC(CString & strData)
{
	typedef int(_stdcall *InitPurse)(char * IP, int Port, int TimeOut, int Flag, char * PutRand, char * PutDiv, char * PutData, char * OutData);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char szRand2[10] = { 0 }, szDiv[20] = { 0 }, szDataIn[500] = { 0 }, szDataOut[500] = { 0 };
	InitPurse ip;
	int nRtn = -1;
	ip = (InitPurse)GetProcAddress(m_hDll18SGC, "InitPurse");
	if (ip)
	{
		nPort = _ttoi(m_strNetCptPort);
		nTimeOut = _ttoi(m_strNetCptTimeOut);
		ZUtil::WtoA(m_strNetCptIP, szIp, sizeof(szIp));
		ZUtil::WtoA(m_strRand2, szRand2, sizeof(szRand2));
		ZUtil::WtoA(m_strDiv, szDiv, sizeof(szDiv));
		ZUtil::WtoA(strData, szDataIn, sizeof(szDataIn));
		nRtn = ip(szIp, nPort, nTimeOut / 1000, 0, szRand2, szDiv, szDataIn, szDataOut);
		if (!nRtn)
			strData = szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::InitPurse15CSG(CString & strData)
{
	typedef int (_stdcall *InitPurse)(int Flag,char * PutRand, char * PutDiv,char * PutData, char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	InitPurse ip;
	int nRtn=-1;
	ip=(InitPurse)GetProcAddress(m_hDll15CSG,"InitPurse");
	if(ip)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=ip(0,szRand2,szDiv,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::InintPurse13SPG(CString & strData)
{
	typedef int (_stdcall *InintPurse)(IN char *pRandom, IN char *pMeterNo, IN char*pData, char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	InintPurse ip;
	int nRtn=-1;
	ip=(InintPurse)GetProcAddress(m_hDll13SPG,"InintPurse");
	if(ip)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=ip(szRand2,szDiv,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::InintPurse13SGCC(CString & strData)
{
	typedef int (_stdcall *InintPurse)(IN char *pRandom, IN char *pMeterNo, IN char*pData, char *pOutput);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	InintPurse ip;
	int nRtn=-1;
	ip=(InintPurse)GetProcAddress(m_hDll13SGCC,"InintPurse");
	if(ip)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=ip(szRand2,szDiv,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::InCreasePurse09SGCC(CString & strData)
{
	typedef int (_stdcall *InCreasePurse)(char *RandDivData,char *dataout);
	CString strRandDivData;
	char szRandDivData[500]={0},szDataOut[500]={0};
	InCreasePurse icp;
	int nRtn=-1;
	icp=(InCreasePurse)GetProcAddress(m_hDll09SGCC,"InCreasePurse");
	if(icp)
	{
		strRandDivData=m_strRand2+m_strDiv+strData;
		ZUtil::WtoA(strRandDivData,szRandDivData,sizeof(szRandDivData));
		nRtn=icp(szRandDivData,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::SwitchChargeMode15CSG(CString & strData)
{
	typedef int (_stdcall *SwitchChargeMode)(int Flag,char * PutRand, char * PutDiv,char * PutData, char * OutData);
	char szRand2[10]={0},szDiv[20]={0},szDataIn[500]={0},szDataOut[500]={0};
	SwitchChargeMode scm;
	int nRtn=-1;
	scm=(SwitchChargeMode)GetProcAddress(m_hDll15CSG,"SwitchChargeMode");
	if(scm)
	{
		ZUtil::WtoA(m_strRand2,szRand2,sizeof(szRand2));
		ZUtil::WtoA(m_strDiv,szDiv,sizeof(szDiv));
		ZUtil::WtoA(strData,szDataIn,sizeof(szDataIn));
		nRtn=scm(0,szRand2,szDiv,szDataIn,szDataOut);
		if(!nRtn)
			strData=szDataOut;
	}
	return nRtn;
}

int ZDLT645_2007::ReadParamPresetCard18SGC(CString & strCardNum, CString & strAppBina, CString & strMoney, CString & strTimeZoneParam)
{
	typedef int(_stdcall *ReadParamPresetCard)(char * cardNum, char * fillAppBina, char * fileMoney, char * fileTimeZoneParam);
	char sz_cCardNum[20] = { 0 }, sz_cAppBina[500] = { 0 }, sz_cMoney[500] = { 0 }, sz_cTimeZoneParam[500] = { 0 };
	ReadParamPresetCard rppc;
	int nRtn = -1;
	rppc = (ReadParamPresetCard)GetProcAddress(m_hDll18SGC, "ReadParamPresetCard");
	if (rppc)
	{
		nRtn = rppc(sz_cCardNum, sz_cAppBina, sz_cMoney, sz_cTimeZoneParam);
		if (!nRtn)
		{
			strCardNum = sz_cCardNum;
			strAppBina = sz_cAppBina;
			strMoney = sz_cMoney;
			strTimeZoneParam = sz_cTimeZoneParam;
		}
	}
	return nRtn;
}

int ZDLT645_2007::WriteParamPresetCard18SGC(const CString & strIp, const CString & strPort, const CString & strTimeOut, const CString & strAppBina, const CString & strMoney, const CString & strTimeZoneParam)
{
	typedef int(_stdcall *WriteParamPresetCard)(char * IP, int Port, int TimeOut, char * fillAppBina, char * fileMoney, char * fileTimeZoneParam);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char sz_cAppBina[500] = { 0 }, sz_cMoney[500] = { 0 }, sz_cTimeZoneParam[500] = { 0 };
	WriteParamPresetCard wppc;
	int nRtn = -1;
	wppc = (WriteParamPresetCard)GetProcAddress(m_hDll18SGC, "WriteParamPresetCard");
	if (wppc)
	{
		nPort = _ttoi(strPort);
		nTimeOut = _ttoi(strTimeOut);
		ZUtil::WtoA(strIp, szIp, sizeof(szIp));
		ZUtil::WtoA(strAppBina, sz_cAppBina, sizeof(sz_cAppBina));
		ZUtil::WtoA(strMoney, sz_cMoney, sizeof(sz_cMoney));
		ZUtil::WtoA(strTimeZoneParam, sz_cTimeZoneParam, sizeof(sz_cTimeZoneParam));
		nRtn = wppc(szIp, nPort, nTimeOut / 1000, sz_cAppBina, sz_cMoney, sz_cTimeZoneParam);
	}
	return nRtn;
}

int ZDLT645_2007::ReadTestingCard18SGC(CString & strCardNum, CString & strESAMAppBina, CString & strESAMWorkInfo, CString & strESAMMoney)
{
	typedef int(_stdcall *ReadTestingCard)(char * cardNum, char * fillESAMAppBina, char * fileESAMWorkInfo, char * fileESAMMoney);
	char sz_cCardNum[20] = { 0 }, sz_cESAMAppBina[500] = { 0 }, sz_cESAMWorkInfo[500] = { 0 }, sz_cESAMMoney[500] = { 0 };
	ReadTestingCard rtc;
	int nRtn = -1;
	rtc = (ReadTestingCard)GetProcAddress(m_hDll18SGC, "ReadTestingCard");
	if (rtc)
	{
		nRtn = rtc(sz_cCardNum, sz_cESAMAppBina, sz_cESAMWorkInfo, sz_cESAMMoney);
		if (!nRtn)
		{
			strCardNum = sz_cCardNum;
			strESAMAppBina = sz_cESAMAppBina;
			strESAMWorkInfo = sz_cESAMWorkInfo;
			strESAMMoney = sz_cESAMMoney;
		}
	}
	return nRtn;
}

int ZDLT645_2007::ReadIncreaseAmountCard18SGC(CString & strCardNum, CString & strAppBina)
{
	typedef int(_stdcall *ReadIncreaseAmountCard)(char * cardNum, char * fillAppBina);
	char sz_cCardNum[20] = { 0 }, sz_cAppBina[500] = { 0 };
	ReadIncreaseAmountCard riac;
	int nRtn = -1;
	riac = (ReadIncreaseAmountCard)GetProcAddress(m_hDll18SGC, "ReadIncreaseAmountCard");
	if (riac)
	{
		nRtn = riac(sz_cCardNum, sz_cAppBina);
		if (!nRtn)
		{
			strCardNum = sz_cCardNum;
			strAppBina = sz_cAppBina;
		}
	}
	return nRtn;
}

int ZDLT645_2007::WriteIncreaseAmountCard18SGC(const CString & strIp, const CString & strPort, const CString & strTimeOut, const CString & strAppBina)
{
	typedef int(_stdcall *WriteIncreaseAmountCard)(char * IP, int Port, int TimeOut, char * fillAppBina);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char sz_cAppBina[500] = { 0 };
	WriteIncreaseAmountCard wiac;
	int nRtn = -1;
	wiac = (WriteIncreaseAmountCard)GetProcAddress(m_hDll18SGC, "WriteIncreaseAmountCard");
	if (wiac)
	{
		nPort = _ttoi(strPort);
		nTimeOut = _ttoi(strTimeOut);
		ZUtil::WtoA(strIp, szIp, sizeof(szIp));
		ZUtil::WtoA(strAppBina, sz_cAppBina, sizeof(sz_cAppBina));
		nRtn = wiac(szIp, nPort, nTimeOut / 1000, sz_cAppBina);
	}
	return nRtn;
}

int ZDLT645_2007::ReadModifyMeterNumCard18SGC(CString & strCardNum, CString & strAppBina, CString & strReverAppBina)
{
	typedef int(_stdcall *ReadModifyMeterNumCard)(char * cardNum, char * fillAppBina, char * fileReverAppBina);
	char sz_cCardNum[20] = { 0 }, sz_cAppBina[500] = { 0 }, sz_cReverAppBina[500] = { 0 };
	ReadModifyMeterNumCard rmmnc;
	int nRtn = -1;
	rmmnc = (ReadModifyMeterNumCard)GetProcAddress(m_hDll18SGC, "ReadModifyMeterNumCard");
	if (rmmnc)
	{
		nRtn = rmmnc(sz_cCardNum, sz_cAppBina, sz_cReverAppBina);
		if (!nRtn)
		{
			strCardNum = sz_cCardNum;
			strAppBina = sz_cAppBina;
			strReverAppBina = sz_cReverAppBina;
		}
	}
	return nRtn;
}

int ZDLT645_2007::WriteModifyMeterNumCard18SGC(const CString & strIp, const CString & strPort, const CString & strTimeOut, const CString & strAppBina, const CString & strReverAppBina)
{
	typedef int(_stdcall *WriteModifyMeterNumCard)(char * IP, int Port, int TimeOut, char * fillAppBina, char * fileReverAppBina);
	int nPort, nTimeOut;
	char szIp[20] = { 0 };
	char sz_cAppBina[500] = { 0 }, sz_cReverAppBina[500] = { 0 };
	WriteModifyMeterNumCard wmmnc;
	int nRtn = -1;
	wmmnc = (WriteModifyMeterNumCard)GetProcAddress(m_hDll18SGC, "WriteModifyMeterNumCard");
	if (wmmnc)
	{
		nPort = _ttoi(strPort);
		nTimeOut = _ttoi(strTimeOut);
		ZUtil::WtoA(strIp, szIp, sizeof(szIp));
		ZUtil::WtoA(strAppBina, sz_cAppBina, sizeof(sz_cAppBina));
		ZUtil::WtoA(strReverAppBina, sz_cReverAppBina, sizeof(sz_cReverAppBina));
		nRtn = wmmnc(szIp, nPort, nTimeOut / 1000, sz_cAppBina, sz_cReverAppBina);
	}
	return nRtn;
}

int ZDLT645_2007::GetEncodeRemoteData(CString & strData,CString & strError)
{
	int nRtn = 0;
	CString strDataIn,strDataOut,strHead;
	strDataIn=GetParamByCstring();
	strHead.Format(_T("[%06d11]"),strDataIn.GetLength()+10);
	strDataIn=strHead+strDataIn;
	if(!SocketCommunication(strDataIn,strDataOut))
	{
		strError = _T("error:1094") + CString(tcSplitError) + _T("远程数据获取失败!");
		return 1094;
	}
	if(strDataOut.Mid(0,1)==_T("[")&&strDataOut.Mid(9,1)==_T("]")&&_ttoi(strDataOut.Mid(1,6))==strDataOut.GetLength()&&strDataOut.Mid(7,2)==_T("11"))
	{
		strDataOut.Delete(0,10);
		std::vector<CString> vec_strData;
		ZUtil::StrSplit(strDataOut, vec_strData, tcSplitNetData, strDataOut.Right(1) == tcSplitNetData);
		if (vec_strData.size() == 2)
		{
			nRtn = _ttoi(vec_strData[0]);
			if (nRtn)
			{
				strError = vec_strData[1];
			}
			else
			{
				SetParamByCstring(vec_strData[1].Left(vec_strData[1].ReverseFind(_T(','))));
				strData = vec_strData[1].Mid(vec_strData[1].ReverseFind(_T(',')) + 1);
			}
			return nRtn;
		}
	}
	strError = _T("error:1095") + CString(tcSplitError) + _T("远程数据格式错误!");
	return 1095;
}

bool ZDLT645_2007::SocketCommunication(const CString & strDataIn,CString & strDataOut)
{
	ZStringSocket zSock;
	int nRtn=zSock.InitSocket();
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	zSock.SetTimeOut(ZSocket::TIMEOUT_CONT,_ttoi(m_strServerTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_SEND,_ttoi(m_strServerTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_RECV,_ttoi(m_strServerTimeOut));
	nRtn=zSock.Connect(m_strServerIP,m_strServerPort);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	nRtn=zSock.StringSend(strDataIn);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	nRtn=zSock.StringRecv(strDataOut);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	zSock.CloseSocket();
	return true;
}

int ZDLT645_2007::GetApdu(void)
{
	CString strDeviation,strFileFlag;
	m_strApdu=_T("04D6");
	int nRtn=GetApduDeviation(strDeviation);
	if(nRtn)
	{
		strFileFlag.Format(_T("%02d"),nRtn);
		m_strApdu+=strFileFlag;
		m_strApdu+=strDeviation;
	}
	return nRtn;
}

int ZDLT645_2007::GetApduDeviation(CString & strDeviation)
{
	const CString sz_strDI[] =
	{
		_T("04000108"),_T("04001001"),_T("04001002"),_T("04000306"),_T("04000307"),_T("04000402"),
		_T("0400040E"),_T("070001FF"),_T("040501FF"),_T("040604FF"),_T("040606FF"),_T("040607FF"),
		_T("040502FF"),_T("040605FF"),_T("04060AFF"),_T("04060BFF"),_T("04000109")
	};
	const CString sz_strDeviSGCC[] =//国网陕西地电ESAM
	{
		_T("0A"),_T("10"),_T("14"),_T("18"),_T("1B"),_T("1E"),
		_T("24"),_T("2B"),_T("04"),_T("84"),_T("34"),_T("7A"),
		_T("04"),_T("84"),_T("34"),_T("7A"),_T("C4")
	};
	const CString sz_strDeviCSG[] =//南网ESAM
	{
		_T("0A"),_T("10"),_T("14"),_T("18"),_T("1B"),_T("1E"),
		_T("24"),_T("2B"),_T("04"),_T("84"),_T("34"),_T("7A"),
		_T("04"),_T("84"),_T("34"),_T("7A"),_T("C0")
	};
	const CString sz_strDeviSGC[] =//蒙西ESAM
	{
		_T("05"),_T("0F"),_T("13"),_T("17"),_T("1A"),_T("A6"),
		_T("25"),_T("AC"),_T("00"),_T("00"),_T("00"),_T("00"),
		_T("00"),_T("00"),_T("00"),_T("00"),_T("00")
	};
	const int sz_nRtnSGCC[]=
	{
		82,82,82,82,82,82,
		82,82,83,83,83,83,
		84,84,84,84,84
	};
	const int sz_nRtnCSG[]=
	{
		81,81,81,81,81,81,
		81,81,83,83,83,83,
		84,84,84,84,84
	};
	const int sz_nRtnSGC[] =
	{
		81,81,81,81,81,81,
		81,81,81,81,81,81,
		81,81,81,81,81
	};
	int i;
	for(i=0;i<17;++i)
	{
		if(m_strDataItem==sz_strDI[i])
		{
			if(m_strGrade==_T("99"))
			{
				if (m_nDllType == 5)
				{
					strDeviation = sz_strDeviSGC[i];
					return sz_nRtnSGC[i];
				}
				else
				{
					strDeviation = sz_strDeviSGCC[i];
					return sz_nRtnSGCC[i];
				}
			}
			else
			{
				strDeviation= sz_strDeviCSG[i];
				return sz_nRtnCSG[i];
			}
		}
	}
	return 0;
}

CString ZDLT645_2007::GetCheckSum(CString str)
{
	CString strCheckSum;
	BYTE szData[1000]={0};
	if(CStringToBYTEArrBy2(str,szData,sizeof(szData)))
	{
		strCheckSum.Format(_T("%02X"),GetCheckSum(szData,str.GetLength()/2));
	}
	return strCheckSum;
}


BYTE ZDLT645_2007::GetCheckSum(BYTE * p_c,int nLen)
{
	int nCheckSum=0;
	for(int i=0;i<nLen;++i)
		nCheckSum+=p_c[i];
	return BYTE(nCheckSum%256);
}


int ZDLT645_2007::DataEncode(CString & strData, CString & strError)
{
	m_strMeterAddr.MakeUpper();
	m_strCtrlCode.MakeUpper();
	m_strDataItem.MakeUpper();
	m_strTgtData.MakeUpper();
	CString str68(_T("68"));
	CString strFMeterAddr, strFDataItem, strFCtrlCode, strFLength, strFGrade, strFPsWd, strFOperatorCode, strFTargetData, strFCheckSum, strFDiv, strFRand1, strFEndata1, strFEndata2;
	strFCtrlCode = m_strCtrlCode;
	strFMeterAddr = ReverseCStringBy2(m_strMeterAddr);
	strFDataItem = ReverseCStringBy2Plus33(m_strDataItem);
	if (m_strCtrlCode == _T("08"))
	{
		strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
		m_strLength.Format(_T("%02X"), strFTargetData.GetLength() / 2);
		strFLength = m_strLength;
		strData = _T("6899999999999968") + strFCtrlCode + strFLength + strFTargetData;
	}
	else if (m_strCtrlCode == _T("11"))
	{
		m_strLength.Format(_T("%02X"), m_strDataItem.GetLength() / 2);
		strFLength = m_strLength;
		strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem;
	}
	else if (m_strCtrlCode == _T("13"))
	{
		m_strLength.Format(_T("%02X"), 0);
		strFLength = m_strLength;
		strData = _T("68AAAAAAAAAAAA68") + strFCtrlCode + strFLength;
	}
	else if (m_strCtrlCode == _T("15"))
	{
		strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
		m_strLength.Format(_T("%02X"), strFTargetData.GetLength() / 2);
		strFLength = m_strLength;
		strData = _T("68AAAAAAAAAAAA68") + strFCtrlCode + strFLength + strFTargetData;
	}
	else if (m_strCtrlCode == _T("16"))
	{
		strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
		m_strLength.Format(_T("%02X"), strFTargetData.GetLength() / 2);
		strFLength = m_strLength;
		strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFTargetData;
	}
	else if (m_strCtrlCode == _T("17"))
	{
		strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
		m_strLength.Format(_T("%02X"), strFTargetData.GetLength() / 2);
		strFLength = m_strLength;
		strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFTargetData;
	}
	else if (m_strCtrlCode == _T("18"))
	{
		CString strDataArea;
		strFTargetData = ReverseCStringBy2Plus33(m_strTgtData.Left(2));
		strFTargetData += ReverseCStringBy2Plus33(m_strTgtData.Mid(2, 6));
		strFTargetData += ReverseCStringBy2Plus33(m_strTgtData.Mid(8, 2));
		strFTargetData += ReverseCStringBy2Plus33(m_strTgtData.Mid(10, 6));
		strDataArea = strFDataItem + strFTargetData;
		m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
		strFLength = m_strLength;
		strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFTargetData;
	}
	else if (m_strCtrlCode == _T("03"))
	{
		if (m_nIsRemote)
			return GetEncodeRemoteData(strData, strError);
		if (m_strDataItem == _T("070000FF"))
		{
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = IdentityAuthentication09SGCC();
				if (nRtn)
				{
					strError = _T("error:0102") + CString(tcSplitError) + ErrorInfoID09SGCC(nRtn);
					return 102;
				}
			}
			break;
			case 2:
			{
				int nRtn = SetIp13SGCC();
				if (nRtn)
				{
					strError = _T("error:0201") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 201;
				}
				nRtn = IdentityAuthentication13SGCC();
				if (nRtn)
				{
					strError = _T("error:0202") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 202;
				}
			}
			break;
			case 3:
			{
				int nRtn = SetIp13SPG();
				if (nRtn)
				{
					strError = _T("error:0301") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 301;
				}
				nRtn = IdentityAuthentication13SPG();
				if (nRtn)
				{
					strError = _T("error:0302") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 302;
				}
			}
			break;
			case 4:
			{
				int nRtn = OpenDevice15CSG();
				if (nRtn)
				{
					strError = _T("error:0401") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 401;
				}
				nRtn = IdentityAuthentication15CSG();
				if (nRtn)
				{
					strError = _T("error:0402") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 402;
				}
			}
			break;
			case 5:
			{
				int nRtn = IdentityAuthentication18SGC();
				if (nRtn)
				{
					strError = _T("error:0502") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 502;
				}
			}
			break;
			}
			CString strDataArea;
			strDataArea = m_strDataItem + m_strOperaCode + m_strEndata1 + m_strRand1 + m_strDiv;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			strFEndata1 = ReverseCStringBy2Plus33(m_strEndata1);
			strFRand1 = ReverseCStringBy2Plus33(m_strRand1);
			strFDiv = ReverseCStringBy2Plus33(m_strDiv);
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strFEndata1 + strFRand1 + strFDiv;
		}
		else if (m_strDataItem == _T("070001FF"))
		{
			CString strDataArea;
			strFGrade = ReverseCStringBy2Plus33(m_strGrade);
			strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			GetApdu();
			int LC = m_strTgtData.GetLength() / 2 + 4;
			CString LCstr;
			LCstr.Format(_T("%02X"), LC);
			m_strApdu += LCstr;
			strFTargetData = m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
			{
				m_strDiv = _T("0000000000000001");
				int nRtn = ParameterUpdate09SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0105") + CString(tcSplitError) + ErrorInfo09SGCC(nRtn);
					return 105;
				}
			}
			break;
			case 2:
			{
				int nRtn = ParameterUpdate13SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0205") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 205;
				}
			}
			break;
			case 3:
			{
				int nRtn = ParameterUpdate13SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0305") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 305;
				}
			}
			break;
			case 4:
			{
				int nRtn = ParameterUpdate15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0405") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 405;
				}
			}
			break;
			case 5:
			{
				int nRtn = ParameterUpdate18SGC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0505") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 505;
				}
			}
			break;
			}
			CString strCiphertext, strMAC;
			strCiphertext = strFTargetData.Left(strFTargetData.GetLength() - 8);
			strMAC = strFTargetData.Right(8);
			strCiphertext = ReverseCStringBy2Plus33(strCiphertext);
			strMAC = ReverseCStringBy2Plus33(strMAC);
			strDataArea = strFDataItem + strFOperatorCode + strCiphertext + strMAC;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strCiphertext + strMAC;
		}
		else if (m_strDataItem == _T("070103FF"))
		{
			CString strDataArea;
			strFGrade = ReverseCStringBy2Plus33(m_strGrade);
			strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			strFTargetData = m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = InCreasePurse09SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0109") + CString(tcSplitError) + ErrorInfo09SGCC(nRtn);
					return 109;
				}
			}
			break;
			case 2:
			{
				int nRtn = InintPurse13SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0209") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 209;
				}
			}
			break;
			case 3:
			{
				int nRtn = InintPurse13SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0309") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 309;
				}
			}
			break;
			case 4:
			{
				int nRtn = InitPurse15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0409") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 409;
				}
			}
			break;
			case 5:
			{
				int nRtn = InitPurse18SGC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0509") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 509;
				}
			}
			break;
			}
			CString strMoney, strMAC1, strTimes, strMAC2;
			strMoney = strFTargetData.Mid(0, 8);
			strMAC1 = strFTargetData.Mid(8, 8);
			strTimes = strFTargetData.Mid(16, 8);
			strMAC2 = strFTargetData.Mid(24, 8);
			strMoney = ReverseCStringBy2Plus33(strMoney);
			strMAC1 = ReverseCStringBy2Plus33(strMAC1);
			strTimes = ReverseCStringBy2Plus33(strTimes);
			strMAC2 = ReverseCStringBy2Plus33(strMAC2);
			strDataArea = strFDataItem + strFOperatorCode + strMoney + strMAC1 + strTimes + strMAC2;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strMoney + strMAC1 + strTimes + strMAC2;
		}
		else if (m_strDataItem == _T("078001FF"))
		{
			CString strDataArea;
			strDataArea = m_strDataItem + m_strOperaCode + m_strTgtData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strFTargetData;
		}
		else if (m_strDataItem == _T("07A006FF"))
		{
			CString strDataArea;
			strFGrade = ReverseCStringBy2Plus33(m_strGrade);
			strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			strFTargetData = m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
			case 2:
			case 3:
				break;
			case 4:
			{
				int nRtn = SwitchChargeMode15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0410") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 410;
				}
			}
			break;
			}
			CString strDiv, strMode, strMAC1, strMoney, strTimes, strMAC2;
			strMode = strFTargetData.Mid(0, 2);
			strMAC1 = strFTargetData.Mid(2, 8);
			strMoney = strFTargetData.Mid(10, 8);
			strTimes = strFTargetData.Mid(18, 8);
			strMAC2 = strFTargetData.Mid(26, 8);
			strDiv = ReverseCStringBy2Plus33(m_strDiv);
			strMode = ReverseCStringBy2Plus33(strMode);
			strMAC1 = ReverseCStringBy2Plus33(strMAC1);
			strMoney = ReverseCStringBy2Plus33(strMoney);
			strTimes = ReverseCStringBy2Plus33(strTimes);
			strMAC2 = ReverseCStringBy2Plus33(strMAC2);
			strDataArea = strFDataItem + strFOperatorCode + strDiv + strMode + strMAC1 + strMoney + strTimes + strMAC2;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strDiv + strMode + strMAC1 + strMoney + strTimes + strMAC2;
		}
		else if (m_strDataItem == _T("078003FF"))
		{
			switch (m_nDllType)
			{
			case 1:
				break;
			case 2:
			{
				int nRtn = SetIp13SGCC();
				if (nRtn)
				{
					strError = _T("error:0201") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 201;
				}
				nRtn = InfraredRand13SGCC();
				if (nRtn)
				{
					strError = _T("error:0203") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 203;
				}
			}
			break;
			case 3:
			{
				int nRtn = SetIp13SPG();
				if (nRtn)
				{
					strError = _T("error:0301") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 301;
				}
				nRtn = InfraredRand13SPG();
				if (nRtn)
				{
					strError = _T("error:0303") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 303;
				}
			}
			break;
			case 4:
			{
				int nRtn = OpenDevice15CSG();
				if (nRtn)
				{
					strError = _T("error:0401") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 401;
				}
				nRtn = InfraredRand15CSG();
				if (nRtn)
				{
					strError = _T("error:0403") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 403;
				}
			}
			break;
			}
			CString strDataArea;
			strDataArea = m_strDataItem + m_strOperaCode + m_strRand1;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
			strFRand1 = ReverseCStringBy2Plus33(m_strRand1);
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strFRand1;
		}
		else if (m_strDataItem == _T("070003FF"))
		{
			switch (m_nDllType)
			{
			case 1:
				break;
			case 2:
			{
				int nRtn = InfraredAuth13SGCC(strFEndata2);
				if (nRtn)
				{
					strError = _T("error:0204") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 204;
				}
			}
			break;
			case 3:
			{
				int nRtn = InfraredAuth13SPG(strFEndata2);
				if (nRtn)
				{
					strError = _T("error:0304") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 304;
				}
			}
			break;
			case 4:
			{
				int nRtn = InfraredAuth15CSG(strFEndata2);
				if (nRtn)
				{
					strError = _T("error:0404") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 404;
				}
			}
			break;
			case 5:
			{
				int nRtn = ESAMIdentityAuthentication18SGC(strFEndata2);
				if (nRtn)
				{
					strError = _T("error:0504") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 504;
				}
			}
			break;
			}
			CString strDataArea;
			if (m_nDllType == 5)//ESAM外部认证
			{		
				strDataArea = m_strDataItem + m_strOperaCode + strFEndata2+ m_strDiv;
				m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
				strFLength = m_strLength;
				strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
				strFEndata2 = ReverseCStringBy2Plus33(strFEndata2);
				strFDiv = ReverseCStringBy2Plus33(m_strDiv);
				strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strFEndata2+ strFDiv;
			}
			else//红外认证
			{
				strDataArea = m_strDataItem + m_strOperaCode + strFEndata2;
				m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
				strFLength = m_strLength;
				strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
				strFEndata2 = ReverseCStringBy2Plus33(strFEndata2);
				strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFOperatorCode + strFEndata2;
			}
		}
		else
		{
			strError = _T("error:0092") + CString(tcSplitError) + _T("数据项错误!");
			return 92;
		}
	}
	else if (m_strCtrlCode == _T("14"))
	{
		if (m_nIsRemote)
			return GetEncodeRemoteData(strData, strError);
		CString strDataArea;
		strFGrade = ReverseCStringBy2Plus33(m_strGrade);
		strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
		strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
		if (m_strGrade == _T("02") || m_strGrade == _T("04") || m_strGrade == _T("03") || m_strGrade == _T("00"))
		{
			strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
			strDataArea = strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else if (m_strGrade == _T("98") || m_strGrade == _T("96"))
		{
			int nDI2 = _tcstol(m_strDataItem.Mid(2, 2), NULL, 16);
			if (m_strGrade == _T("98"))
			{
				switch (nDI2 % 5)
				{
				case 0:
					m_strApdu = _T("04D68900");
					break;
				case 1:
					m_strApdu = _T("04D69000");
					break;
				case 2:
					m_strApdu = _T("04D69100");
					break;
				case 3:
					m_strApdu = _T("04D69200");
					break;
				case 4:
					m_strApdu = _T("04D69300");
					break;
				}
				if (m_nDllType == 5)
					m_strApdu = _T("04D68400");
			}
			else
			{
				switch (nDI2 % 5)
				{
				case 0:
					m_strApdu = _T("04D68800");
					break;
				case 1:
					m_strApdu = _T("04D68900");
					break;
				case 2:
					m_strApdu = _T("04D69000");
					break;
				case 3:
					m_strApdu = _T("04D69100");
					break;
				case 4:
					m_strApdu = _T("04D69200");
					break;
				}
			}
			int nLC = (m_strTgtData.GetLength() / 2 / 16 + 1) * 16 + 4;
			CString strLC;
			strLC.Format(_T("%02X"), nLC);
			m_strApdu += strLC;
			strFTargetData = m_strDataItem + m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = ParameterElseUpdate09SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0106") + CString(tcSplitError) + ErrorInfo09SGCC(nRtn);
					return 106;
				}
			}
			break;
			case 2:
			{
				int nRtn = ParameterElseUpdate13SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0206") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 206;
				}
			}
			break;
			case 3:
			{
				int nRtn = ParameterElseUpdate13SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0306") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 306;
				}
			}
			break;
			case 4:
			{
				int nRtn = ParameterElseUpdate15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0406") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 406;
				}
			}
			break;
			case 5:
			{
				int nRtn = ParameterElseUpdate18SGC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0506") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 506;
				}
			}
			break;
			}
			CString strCiphertext, strMAC;
			strCiphertext = strFTargetData.Left(strFTargetData.GetLength() - 8);
			strMAC = strFTargetData.Right(8);
			strCiphertext = ReverseCStringBy2Plus33(strCiphertext);
			strMAC = ReverseCStringBy2Plus33(strMAC);
			strDataArea = strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strCiphertext + strMAC;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strCiphertext + strMAC;
		}
		else if (m_strGrade == _T("99") || m_strGrade == _T("97"))
		{
			int nRtnApdu = GetApdu();
			int nLC = m_strTgtData.GetLength() / 2 + 4;
			CString strLC;
			strLC.Format(_T("%02X"), nLC);
			m_strApdu += strLC;
			strFTargetData = m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
			{
				m_strDiv = _T("0000000000000001");
				int nRtn;
				if (nRtnApdu == 82)
					nRtn = ParameterUpdate09SGCC(strFTargetData);
				else if (nRtnApdu == 83)
					nRtn = ParameterUpdate109SGCC(strFTargetData);
				else
					nRtn = ParameterUpdate209SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0105") + CString(tcSplitError) + ErrorInfo09SGCC(nRtn);
					return 105;
				}
			}
			break;
			case 2:
			{
				int nRtn;
				if (nRtnApdu == 82)
					nRtn = ParameterUpdate13SGCC(strFTargetData);
				else if (nRtnApdu == 83)
					nRtn = ParameterUpdate113SGCC(strFTargetData);
				else
					nRtn = ParameterUpdate213SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0205") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 205;
				}
			}
			break;
			case 3:
			{
				int nRtn;
				if (nRtnApdu == 82)
					nRtn = ParameterUpdate13SPG(strFTargetData);
				else if (nRtnApdu == 83)
					nRtn = ParameterUpdate113SPG(strFTargetData);
				else
					nRtn = ParameterUpdate213SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0305") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 305;
				}
			}
			break;
			case 4:
			{
				int nRtn;
				if (nRtnApdu == 81)
					nRtn = ParameterUpdate15CSG(strFTargetData);
				else if (nRtnApdu == 83)
					nRtn = Price1Update15CSG(strFTargetData);
				else
					nRtn = Price2Update15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0405") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 405;
				}
			}
			break;
			case 5:
			{
				int nRtn = ParameterUpdate18SGC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0505") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 505;
				}
			}
			break;
			}
			CString strCiphertext, strMAC;
			strCiphertext = strFTargetData.Left(strFTargetData.GetLength() - 8);
			strMAC = strFTargetData.Right(8);
			if (m_strDataItem == _T("040502FF") || m_strDataItem == _T("040501FF"))
				strCiphertext = ReverseCStringBy8(strCiphertext);
			if (m_strDataItem == _T("040604FF") || m_strDataItem == _T("040605FF") || m_strDataItem == _T("040606FF") || m_strDataItem == _T("040607FF") || m_strDataItem == _T("04060AFF") || m_strDataItem == _T("04060BFF"))
				strCiphertext = Reverse0406XXFF(strCiphertext);
			strCiphertext = ReverseCStringBy2Plus33(strCiphertext);
			strMAC = ReverseCStringBy2Plus33(strMAC);
			strDataArea = strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strCiphertext + strMAC;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFDataItem + strFGrade + strFPsWd + strFOperatorCode + strCiphertext + strMAC;
		}
		else
		{
			strError = _T("error:0091") + CString(tcSplitError) + _T("密码权限错误！");
			return 91;
		}
	}
	else if (m_strCtrlCode == _T("19"))
	{
		CString strDataArea;
		strFGrade = ReverseCStringBy2Plus33(m_strGrade);
		strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
		strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
		strDataArea = strFGrade + strFPsWd + strFOperatorCode;
		m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
		strFLength = m_strLength;
		strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode;
	}
	else if (m_strCtrlCode == _T("1A"))
	{
		if (m_nIsRemote)
			return GetEncodeRemoteData(strData, strError);
		CString strDataArea;
		strFGrade = ReverseCStringBy2Plus33(m_strGrade);
		strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
		strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
		if (m_strGrade == _T("02") || m_strGrade == _T("04") || m_strGrade == _T("03") || m_strGrade == _T("00"))
		{
			strDataArea = strFGrade + strFPsWd + strFOperatorCode;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode;
		}
		else if (m_strGrade == _T("98") || m_strGrade == _T("96"))
		{
			GetN1NmData(strFTargetData);
			switch (m_nDllType)
			{
			case 1:
				break;
			case 2:
			{
				int nRtn = DataClear113SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0208") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 208;
				}
			}
			break;
			case 3:
			{
				int nRtn = DataClear113SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0308") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 308;
				}
			}
			break;
			case 4:
			{
				int nRtn = DataClear115CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0408") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 408;
				}
			}
			break;
			}
			strFTargetData = ReverseCStringBy2Plus33(strFTargetData);
			strDataArea = strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else
		{
			strError = _T("error:0091") + CString(tcSplitError) + _T("密码权限错误！");
			return 91;
		}
	}
	else if (m_strCtrlCode == _T("1B"))
	{
		if (m_nIsRemote)
			return GetEncodeRemoteData(strData, strError);
		CString strDataArea;
		strFGrade = ReverseCStringBy2Plus33(m_strGrade);
		strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
		strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
		if (m_strGrade == _T("02") || m_strGrade == _T("04") || m_strGrade == _T("03") || m_strGrade == _T("00"))
		{
			strFTargetData = ReverseCStringBy2Plus33(m_strTgtData);
			strDataArea = strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else if (m_strGrade == _T("98") || m_strGrade == _T("96"))
		{
			GetN1NmData(strFTargetData);
			strFTargetData += m_strTgtData;
			switch (m_nDllType)
			{
			case 1:
				break;
			case 2:
			{
				int nRtn = DataClear213SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0211") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 211;
				}
			}
			break;
			case 3:
			{
				int nRtn = DataClear213SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0311") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 311;
				}
			}
			break;
			case 4:
			{
				int nRtn = DataClear215CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0411") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 411;
				}
			}
			break;
			}
			strFTargetData = ReverseCStringBy2Plus33(strFTargetData);
			strDataArea = strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else
		{
			strError = _T("error:0091") + CString(tcSplitError) + _T("密码权限错误！");
			return 91;
		}
	}
	else if (m_strCtrlCode == _T("1C"))
	{
		if (m_nIsRemote)
			return GetEncodeRemoteData(strData, strError);
		CString strDataArea;
		strFGrade = ReverseCStringBy2Plus33(m_strGrade);
		strFPsWd = ReverseCStringBy2Plus33(m_strPsWd);
		strFOperatorCode = ReverseCStringBy2Plus33(m_strOperaCode);
		strFTargetData = m_strTgtData;
		GetN1NmData(strFTargetData);
		if (m_strGrade == _T("02") || m_strGrade == _T("04") || m_strGrade == _T("03") || m_strGrade == _T("00") || m_strGrade == _T("11"))
		{
			strFTargetData = CStringBy2Plus33(strFTargetData);
			strDataArea = strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else if (m_strGrade == _T("98") || m_strGrade == _T("96"))
		{
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = UserControl09SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0107") + CString(tcSplitError) + ErrorInfoUC09SGCC(nRtn);
					return 107;
				}
			}
			break;
			case 2:
			{
				int nRtn = UserControl13SGCC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0207") + CString(tcSplitError) + ErrorInfo13SGCC(nRtn);
					return 207;
				}
			}
			break;
			case 3:
			{
				int nRtn = UserControl13SPG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0307") + CString(tcSplitError) + ErrorInfo13SPG(nRtn);
					return 307;
				}
			}
			break;
			case 4:
			{
				int nRtn = UserControl15CSG(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0407") + CString(tcSplitError) + ErrorInfo15CSG(nRtn);
					return 407;
				}
			}
			break;
			case 5:
			{
				int nRtn = UserControl18SGC(strFTargetData);
				if (nRtn)
				{
					strError = _T("error:0507") + CString(tcSplitError) + ErrorInfo18SGC(nRtn);
					return 507;
				}
			}
			break;
			}
			strFTargetData = ReverseCStringBy2Plus33(strFTargetData);
			strDataArea = strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
			m_strLength.Format(_T("%02X"), strDataArea.GetLength() / 2);
			strFLength = m_strLength;
			strData = str68 + strFMeterAddr + str68 + strFCtrlCode + strFLength + strFGrade + strFPsWd + strFOperatorCode + strFTargetData;
		}
		else
		{
			strError = _T("error:0091") + CString(tcSplitError) + _T("密码权限错误！");
			return 91;
		}
	}
	else
	{
		strError = _T("error:0093") + CString(tcSplitError) + _T("控制码错误!");
		return 93;
	}
	strFCheckSum = GetCheckSum(strData);
	strData = _T("FEFEFEFE") + strData + strFCheckSum + _T("16");
	return 0;
}

bool ZDLT645_2007::DataDecode(CString strData, CString & strValue)
{
	strData.Remove(_T(' '));
	strData = strData.Mid(strData.Find(_T("68")));
	if (!Check645_2007(strData))
		return false;
	strValue.Empty();
	CString strDataItem;
	CString strCtrlCode = strData.Mid(16, 2);
	CString strExt = _T("-") + strCtrlCode + _T("H");
	if (strCtrlCode == _T("08"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strValue = strValue + CString(tcSplitValue) + strExt;
	}
	else if (strCtrlCode == _T("11"))
	{
		strDataItem = strData.Mid(20, 8);
		strDataItem = ReverseCStringBy2Minus33(strDataItem);
		strValue = CString(tcSplitValue) + strDataItem + strExt;
	}
	else if (strCtrlCode == _T("91"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strDataItem = strValue.Right(8);
		strValue = strValue.Mid(0, strValue.GetLength() - 8);
		strValue = strValue + CString(tcSplitValue) + strDataItem + strExt;
		if (strDataItem == _T("04000401"))
		{
			m_strMeterAddr = strValue.Left(12);
		}
		if (strDataItem == _T("04000402"))
		{
			m_strDiv = _T("0000") + strValue.Left(12);
		}
	}
	else if (strCtrlCode == _T("B1"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strDataItem = strValue.Right(8);
		strValue = strValue.Mid(0, strValue.GetLength() - 8);
		strValue = strValue + CString(tcSplitValue) + strDataItem + strExt;
		if (strDataItem == _T("04000401"))
		{
			m_strMeterAddr = strValue.Left(12);
		}
		if (strDataItem == _T("04000402"))
		{
			m_strDiv = _T("0000") + strValue.Left(12);
		}
	}
	else if (strCtrlCode == _T("13"))
	{
		strValue = CString(tcSplitValue) + _T("读通信地址") + strExt;
	}
	else if (strCtrlCode == _T("93"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strValue += CString(tcSplitValue) + strExt;
		m_strMeterAddr = strValue.Left(12);
	}
	else if (strCtrlCode == _T("14"))
	{
		strDataItem = strData.Mid(20, 8);
		strDataItem = ReverseCStringBy2Minus33(strDataItem);
		strValue = CString(tcSplitValue) + strDataItem + strExt;
	}
	else if (strCtrlCode == _T("94"))
	{
		strValue = CString(tcSplitValue) + _T("设置成功") + strExt;
		if (m_strDataItem == _T("04000401"))
		{
			m_strMeterAddr = m_strTgtData;
		}
		if (m_strDataItem == _T("04000402"))
		{
			m_strDiv = _T("0000") + m_strTgtData;
		}
	}
	else if (strCtrlCode == _T("15"))
	{
		strValue = CString(tcSplitValue) + _T("写通信地址") + strExt;
	}
	else if (strCtrlCode == _T("95"))
	{
		strValue = CString(tcSplitValue) + _T("写通信地址成功") + strExt;
		m_strMeterAddr = m_strTgtData;
	}
	else if (strCtrlCode == _T("16"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strValue = strValue + CString(tcSplitValue) + strExt;
	}
	else if (strCtrlCode == _T("96"))
	{
		strValue = CString(tcSplitValue) + _T("冻结命令设置成功") + strExt;
	}
	else if (strCtrlCode == _T("17"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strValue = strValue + CString(tcSplitValue) + strExt;
	}
	else if (strCtrlCode == _T("97"))
	{
		strValue = CString(tcSplitValue) + _T("更改通信速率成功") + strExt;
	}
	else if (strCtrlCode == _T("18"))
	{
		strDataItem = strData.Mid(20, 8);
		strDataItem = ReverseCStringBy2Minus33(strDataItem);
		strValue = CString(tcSplitValue) + strDataItem + strExt;
	}
	else if (strCtrlCode == _T("98"))
	{
		strValue = CString(tcSplitValue) + _T("修改密码成功") + strExt;
	}
	else if (strCtrlCode == _T("03"))
	{
		strDataItem = strData.Mid(20, 8);
		strDataItem = ReverseCStringBy2Minus33(strDataItem);
		strValue = CString(tcSplitValue) + strDataItem + strExt;
	}
	else if (strCtrlCode == _T("83"))
	{
		strValue = strData.Mid(20, strData.GetLength() - 24);
		strValue = ReverseCStringBy2Minus33(strValue);
		strDataItem = strValue.Right(8);
		strValue = strValue.Mid(0, strValue.GetLength() - 8);
		if (strDataItem == _T("070000FF"))
		{
			m_strRand2 = strValue.Right(8);
			m_strEsamId = strValue.Mid(0, strValue.GetLength() - 8);
			strValue =  m_strRand2 + CString(tcSplitChar) + m_strEsamId + CString(tcSplitValue) + strDataItem + CString(tcSplitChar)+ _T("身份认证成功") + strExt;
		}
		else if (strDataItem == _T("070001FF"))
		{
			strValue =  CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + _T("身份认证时效设置成功") + strExt;
		}
		else if (strDataItem == _T("070103FF"))
		{
			strValue =  CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + _T("钱包初始化成功") + strExt;
		}
		else if (strDataItem == _T("078001FF"))
		{
			CString strMAC, strFlag, strData;
			strMAC = strValue.Left(8);
			strFlag = strValue.Right(16);
			strData = strValue.Mid(8, strValue.GetLength() - 24);
			strValue = strData +  CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + strFlag + CString(tcSplitChar) + strMAC + strExt;
		}
		else if (strDataItem == _T("07A006FF"))
		{
			strValue = strValue + CString(tcSplitValue) + strDataItem + strExt;
		}
		else if (strDataItem == _T("078003FF"))
		{
			m_strDiv = _T("0000") + strValue.Mid(48, 12);
			m_strEsamId = strValue.Mid(32, 16);
			m_strEndata1 = strValue.Mid(16, 16);
			m_strRand2IR = strValue.Mid(0, 16);
			strValue =  m_strDiv + CString(tcSplitChar) + m_strEsamId + CString(tcSplitChar) + m_strEndata1 + CString(tcSplitChar) + m_strRand2IR + CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + _T("红外认证查询成功 ") + strExt;
		}
		else if (strDataItem == _T("070003FF"))
		{
			strValue = CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + _T("红外认证/ESAM外部认证成功") + strExt;
		}
		else
		{
			strValue = CString(tcSplitValue) + strDataItem + CString(tcSplitChar) + _T("成功") + strExt;
		}
	}
	else if (strCtrlCode == _T("19"))
	{
		strValue = CString(tcSplitValue) + _T("最大需量清零") + strExt;
	}
	else if (strCtrlCode == _T("99"))
	{
		strValue = CString(tcSplitValue) + _T("最大需量清零成功") + strExt;
	}
	else if (strCtrlCode == _T("1A"))
	{
		strValue = CString(tcSplitValue) + _T("电表清零") + strExt;
	}
	else if (strCtrlCode == _T("9A"))
	{
		strValue = CString(tcSplitValue) + _T("电表清零成功") + strExt;
	}
	else if (strCtrlCode == _T("1B"))
	{
		strValue = CString(tcSplitValue) + _T("事件清零") + strExt;
	}
	else if (strCtrlCode == _T("9B"))
	{
		strValue = CString(tcSplitValue) + _T("事件清零成功") + strExt;
	}
	else if (strCtrlCode == _T("1C"))
	{
		strValue = CString(tcSplitValue) + _T("远程控制") + strExt;
	}
	else if (strCtrlCode == _T("9C"))
	{
		strValue = CString(tcSplitValue) + _T("远程控制成功") + strExt;
	}
	else
	{
		strValue = _T("ErrorData") + strExt;
	}
	return true;
}

bool ZDLT645_2007::Check645_2007(CString str)
{
	str.Remove(_T(' '));
	str=str.Mid(str.Find(_T("68")));
	BYTE sz_cData[1000]={0};
	if(CStringToBYTEArrBy2(str,sz_cData,sizeof(sz_cData))==false)
		return false;
	int nCheckSumIndex=10+sz_cData[9];
	BYTE cCheckSum=GetCheckSum(sz_cData,nCheckSumIndex);
	if(sz_cData[0]==0x68&&sz_cData[7]==0x68&&sz_cData[nCheckSumIndex+1]==0x16&&nCheckSumIndex+2==str.GetLength()/2&&sz_cData[nCheckSumIndex]==cCheckSum)
		return true;
	else
		return false;
}

CString ZDLT645_2007::ReverseCStringBy2Minus33(CString str)
{
	str=ReverseCStringBy2(str);
	CString strTemp;
	int i,nByte,nSize;
	CString strInByte,strOutByte;
	nSize=str.GetLength();
	if(nSize<2)
		return strTemp;
	for(i=0;i<nSize;i=i+2)
	{
		strInByte=str.Mid(i,2);
		nByte=_tcstol(strInByte,NULL ,16);
		nByte-=0x33;
		strOutByte.Format(_T("%02X"),nByte);
		strOutByte=strOutByte.Right(2);
		strTemp+=strOutByte;
	}
	return strTemp;
}

void ZDLT645_2007::ChooseDll(const int & nDllType)
{
	m_nDllType=nDllType;
}

int ZDLT645_2007::GetDllChoose(void)
{
	return m_nDllType;
}

void ZDLT645_2007::ChooseMode(const BOOL & nIsRemote)
{
	m_nIsRemote=nIsRemote;
}

BOOL ZDLT645_2007::GetModeChoose(void)
{
	return m_nIsRemote;
}


void ZDLT645_2007::SetServerInfo(const CString & strIP, const CString & strPort, const CString & strTimeOut)
{
	m_strServerIP = strIP;
	m_strServerPort = strPort;
	m_strServerTimeOut = strTimeOut;
}


void ZDLT645_2007::GetServerInfo(CString & strIP, CString & strPort, CString & strTimeOut)
{
	strIP = m_strServerIP;
	strPort = m_strServerPort;
	strTimeOut = m_strServerTimeOut;
}


void ZDLT645_2007::SetNetCptInfo(const CString & strIP, const CString & strPort, const CString & strTimeOut)
{
	m_strNetCptIP = strIP;
	m_strNetCptPort = strPort;
	m_strNetCptTimeOut = strTimeOut;
}


void ZDLT645_2007::GetNetCptInfo(CString & strIP, CString & strPort, CString & strTimeOut)
{
	strIP = m_strNetCptIP;
	strPort = m_strNetCptPort;
	strTimeOut = m_strNetCptTimeOut;
}


bool ZDLT645_2007::Set15Param(const CString & strParam)
{
	std::vector<CString> vec_strDatas;
	ZUtil::StrSplit(strParam,vec_strDatas,_T(','),strParam.Right(1)==_T(','));
	if(vec_strDatas.size()!=15)
		return false;
	int i=0;
	if(!vec_strDatas[i].IsEmpty())
		m_strMeterAddr=vec_strDatas[i++];
	else
		i++;
	m_strCtrlCode=vec_strDatas[i++];
	m_strDataItem=vec_strDatas[i++];
	m_strTgtData=vec_strDatas[i++];
	m_strGrade=vec_strDatas[i++];
	m_strPsWd=vec_strDatas[i++];
	m_strOperaCode=vec_strDatas[i++];
	m_strServerIP=vec_strDatas[i++];
	m_strServerPort=vec_strDatas[i++];
	m_strServerTimeOut=vec_strDatas[i++];
	m_strNetCptIP=vec_strDatas[i++];
	m_strNetCptPort=vec_strDatas[i++];
	m_strNetCptTimeOut=vec_strDatas[i++];
	m_nDllType=_ttoi(vec_strDatas[i++]);
	m_nIsRemote=_ttoi(vec_strDatas[i++]);
	return true;
}

CString ZDLT645_2007::GetCtrlCode(void)
{
	return m_strCtrlCode; 
}

void ZDLT645_2007::SetCtrlCode(const CString & strCtrlCode)
{
	m_strCtrlCode=strCtrlCode;
}

CString ZDLT645_2007::GetDataItem(void)
{
	return m_strDataItem; 
}

void ZDLT645_2007::SetDataItem(const CString & strDataItem)
{
	m_strDataItem=strDataItem;
}

CString ZDLT645_2007::GetMeterAddr(void)
{
	return m_strMeterAddr;
}

void ZDLT645_2007::SetMeterAddr(const CString & strMeterAddr)
{
	m_strMeterAddr=strMeterAddr;
}

CString ZDLT645_2007::GetGrade(void)
{
	return m_strGrade;
}

CString ZDLT645_2007::GetPsWd(void)
{
	return m_strPsWd;
}

CString ZDLT645_2007::GetOperaCode(void)
{
	return m_strOperaCode;
}

CString ZDLT645_2007::GetLength(void)
{
	return m_strLength;
}

CString ZDLT645_2007::GetDiv(void)
{
	return m_strDiv;
}

CString ZDLT645_2007::GetParamByCstring(void)
{
	CString strRtn,strDllType;
	CString str(_T(","));
	strDllType.Format(_T("%d"),m_nDllType);
	strRtn=m_strMeterAddr+str+m_strCtrlCode+str+m_strLength+str+m_strDataItem+str+m_strTgtData+str+m_strGrade+str+m_strPsWd+str+m_strOperaCode+str
		+m_strDiv+str+m_strRand1+str+m_strEndata1+str+m_strRand2+str+m_strEsamId+str+m_strApdu+str+m_strRand2IR+str+strDllType+str;
	return strRtn;
}

bool ZDLT645_2007::SetParamByCstring(const CString & strParam)
{
	std::vector<CString> vec_strDatas;
	ZUtil::StrSplit(strParam,vec_strDatas,_T(','),strParam.Right(1)==_T(','));
	if(vec_strDatas.size()!=16)
		return false;
	int i=0;
	m_strMeterAddr=vec_strDatas[i++];
	m_strCtrlCode=vec_strDatas[i++];
	m_strLength=vec_strDatas[i++];
	m_strDataItem=vec_strDatas[i++];
	m_strTgtData=vec_strDatas[i++];
	m_strGrade=vec_strDatas[i++];
	m_strPsWd=vec_strDatas[i++];
	m_strOperaCode=vec_strDatas[i++];
	m_strDiv=vec_strDatas[i++];
	m_strRand1=vec_strDatas[i++];
	m_strEndata1=vec_strDatas[i++];
	m_strRand2=vec_strDatas[i++];
	m_strEsamId=vec_strDatas[i++];
	m_strApdu=vec_strDatas[i++];
	m_strRand2IR=vec_strDatas[i++];
	m_nDllType=_ttoi(vec_strDatas[i++]);
	return true;
}