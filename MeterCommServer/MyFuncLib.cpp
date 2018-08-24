#include "stdafx.h"
#include "MyFuncLib.h"

void StrSplit_Z(CString strSrc, std::vector<CString> & vecstrDatas,TCHAR tcSplit,bool bLastExist)
{
	vecstrDatas.clear();
	CString strTemp;
	int Rtn = strSrc.Find(tcSplit);
	while (Rtn >= 0)
	{	
		strTemp = strSrc.Left(Rtn);
		vecstrDatas.push_back(strTemp);
		strSrc.Delete(0, Rtn + 1);
		Rtn = strSrc.Find(tcSplit);
	}
	if(!bLastExist)
		vecstrDatas.push_back(strSrc);
}

BOOL TtoA_Z(const wchar_t * p_tcSrc, char * p_cDest,const int & nDesSize)
{
	int nWcharLen =wcslen(p_tcSrc);
	int nCharLen = WideCharToMultiByte(CP_ACP, 0, p_tcSrc, nWcharLen, NULL, 0, NULL, NULL); 
	if(nCharLen>=nDesSize)
		return FALSE;
	WideCharToMultiByte(CP_ACP, 0, p_tcSrc, nWcharLen, p_cDest, nCharLen, NULL, NULL); 
	p_cDest[nCharLen] = '\0'; 
	return TRUE;
}

BOOL TtoUTF8_Z(const wchar_t * p_tcSrc, char * p_cDest,const int & nDesSize)
{
	int nWcharLen =wcslen(p_tcSrc);
	int nCharLen = WideCharToMultiByte(CP_UTF8, 0, p_tcSrc, nWcharLen, NULL, 0, NULL, NULL); 
	if(nCharLen>=nDesSize)
		return FALSE;
	WideCharToMultiByte(CP_UTF8, 0, p_tcSrc, nWcharLen, p_cDest, nCharLen, NULL, NULL); 
	p_cDest[nCharLen] = '\0'; 
	return TRUE;
}

BOOL UTF8toT_Z(const char * p_cSrc, wchar_t * p_tcDest,const int & nDesSize)
{
	int nWcharLen=MultiByteToWideChar(CP_UTF8, NULL, p_cSrc,strlen(p_cSrc),NULL,0); 
	if(nWcharLen>=nDesSize)
		return FALSE;
    MultiByteToWideChar(CP_UTF8, NULL, p_cSrc, strlen(p_cSrc), p_tcDest,nWcharLen); 
    p_tcDest[nWcharLen] =L'\0'; 
	return TRUE;
}

BOOL IsStrUTF8_Z(char* p_str,long long llLength,BOOL nIsCheckAll)  
{  
	unsigned int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	UCHAR chr;  
	BOOL nAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8  
	if(!nIsCheckAll&&llLength>1000)
		llLength=1000;
	for(int i=0;i<llLength;++i)  
	{  
		chr=p_str[i];  
		if((chr&0x80)!= 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
			nAllAscii=FALSE;  
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数  
		{  
			if(chr>=0x80)  
			{  
				if(chr>=0xFC&&chr<=0xFD)  
					nBytes=6;  
				else if(chr>=0xF8)  
					nBytes=5;  
				else if(chr>=0xF0)  
					nBytes=4;  
				else if(chr>=0xE0)  
					nBytes=3;  
				else if(chr>=0xC0)  
					nBytes=2;  
				else  
					return FALSE;  
				--nBytes;  
			}  
		}  
		else //多字节符的非首字节,应为 10xxxxxx  
		{  
			if((chr&0xC0)!=0x80)  
				return FALSE;  
			--nBytes;  
		}  
	}
	if(nIsCheckAll)
	{
		if(nBytes>0) //违返规则,末位字符非UTF8
			return FALSE;   
	}
	if(nAllAscii)//如果全部都是ASCII, 说明不是UTF-8  
		return FALSE; 
	return TRUE;  
}

int GetFileEncoding_Z(const CString & strFilePath)
{
	const int nFail=0;
	const int nAnsi=1;
	const int nUnicode=2;
	const int nUnicodeBigEndian=3;
	const int nUTF8=4;
	CStdioFile file;
	if(!file.Open(strFilePath,CFile::modeRead|CFile::typeBinary))
		return nFail;
	unsigned char szHead[3]={0};
	file.Read(szHead,sizeof(szHead));
	int nRtn=nAnsi;
	if(szHead[0]==0xFF&&szHead[1]==0xFE)
		nRtn=nUnicode;
	else if(szHead[0]==0xFE&&szHead[1]==0xFF)
		nRtn=nUnicodeBigEndian;
	else if(szHead[0]==0xEF&&szHead[1]==0xBB&&szHead[2]==0xBF)
		nRtn=nUTF8;
	else
	{
		long lLength=(long)file.GetLength();
		char *p_cData=new char[lLength];
		file.SeekToBegin();
		file.Read(p_cData,lLength);
		if(IsStrUTF8_Z(p_cData,lLength,FALSE))
			nRtn=nUTF8;
		else
			nRtn=nAnsi;
		delete []p_cData;
	}
	file.Close();
	return nRtn;
}

BOOL IsLittle_endian_Z(void)
{
	union w
	{  
		int a;
		char b;
	} c;
	c.a = 1;
	if(c.b==1)
		return TRUE;
	else
		return FALSE;
}

BOOL ReadFile2CString_Z(const CString & strFilePath,CString & strDest)
{
	strDest.Empty();
	const int nFail=0;
	const int nAnsi=1;
	const int nUnicode=2;
	const int nUnicodeBigEndian=3;
	const int nUTF8=4;
	int nEncoding=GetFileEncoding_Z(strFilePath);
	if(nEncoding==nFail)
		return FALSE;
	CStdioFile file;
	if(!file.Open(strFilePath,CFile::modeRead|CFile::typeBinary))
		return FALSE;
	long lLength=(long)file.GetLength();
	char *p_cData=new char[lLength+2];
	if(!p_cData)
	{
		file.Close();
		return FALSE;
	}
	file.SeekToBegin();
	file.Read(p_cData,lLength);
	file.Close();
	p_cData[lLength]=0;
	p_cData[lLength+1]=0;
	switch(nEncoding)
	{
	case nAnsi:
		strDest=p_cData;
		break;
	case nUnicode:
		{
			if(!IsLittle_endian_Z())
			{
				for(long i=0;i<lLength;i=i+2)
				{
					std::swap(p_cData[i],p_cData[i+1]);
				}
			}
			TCHAR *p_tcData=(TCHAR *)p_cData;
			strDest=p_tcData;
		}
		break;
	case nUnicodeBigEndian:
		{
			if(IsLittle_endian_Z())
			{
				for(long i=0;i<lLength;i=i+2)
				{
					std::swap(p_cData[i],p_cData[i+1]);
				}
			}
			TCHAR *p_tcData=(TCHAR *)p_cData;
			strDest=p_tcData;
		}
		break;
	case nUTF8:
		{
			wchar_t * p_tcData=new wchar_t[lLength+1];
			if(p_tcData&&UTF8toT_Z(p_cData,p_tcData,lLength+1))
			{
				strDest=p_tcData;
				delete []p_tcData;
			}
			else
			{
				if(p_tcData)
					delete []p_tcData;
				delete []p_cData;
				return FALSE;
			}
		}
		break;
	}
	delete []p_cData;
	return TRUE;
}

CString GetExeCatalogPath_Z(void)
{
	TCHAR  szPath[MAX_PATH];
	CString strExePath,strExeCatalogPath;
	DWORD dwRtn=GetModuleFileName(NULL,szPath,MAX_PATH); 
	if(dwRtn)
	{
		strExePath=szPath;
		strExeCatalogPath=strExePath.Left(strExePath.ReverseFind('\\'));
	}
	return strExeCatalogPath;
}

CString GetExePath_Z(void)
{
	TCHAR  szPath[MAX_PATH];
	CString strExePath;
	DWORD dwRtn=GetModuleFileName(NULL,szPath,MAX_PATH); 
	if(dwRtn)
	{
		strExePath=szPath;
	}
	return strExePath;
}

BOOL CheckFileRelation_Z(const CString & strExt, const CString & strExePath)
{
	BOOL nRtn=FALSE;
	HKEY hKeyExt,hKeyExtFile;
	TCHAR szPath[_MAX_PATH]={0}; 
	DWORD dwSize=sizeof(szPath); 
	CString strExtFile,strValue;
	if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hKeyExt)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKeyExt,NULL,NULL,NULL,(LPBYTE)szPath,&dwSize)==ERROR_SUCCESS)
		{
			strExtFile=szPath;
			strExtFile+=_T("\\shell\\open\\command");
			dwSize=sizeof(szPath);
			if(RegOpenKey(HKEY_CLASSES_ROOT,strExtFile,&hKeyExtFile)==ERROR_SUCCESS)
			{
				if(RegQueryValueEx(hKeyExtFile,NULL,NULL,NULL,(LPBYTE)szPath,&dwSize)==ERROR_SUCCESS)
				{
					strValue=szPath;
					if(strValue.Find(strExePath)!=-1)
						nRtn=TRUE;
				}
				RegCloseKey(hKeyExtFile);
			}
		}
		RegCloseKey(hKeyExt);
	}
	return nRtn;
}


BOOL SetFileRelation_Z(const CString & strExt,const CString & strExePath,const CString & strDefaultIcon)
{
	if(strExt.IsEmpty()||strExePath.IsEmpty()||strDefaultIcon.IsEmpty())
        return FALSE;
	CString strTemp,strExeFileKey,strExeFullPath,strDescribe;
	HKEY hKey;
	long lRtn=ERROR_SUCCESS;
	lRtn=RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);
	strExeFileKey=_T("zms")+strExt;//根据项目修改
	lRtn+=RegSetValue(hKey,NULL,REG_SZ,strExeFileKey,strExeFileKey.GetLength());
	lRtn+=RegCloseKey(hKey);

	lRtn+=RegCreateKey(HKEY_CLASSES_ROOT,strExeFileKey,&hKey);
	strDescribe.Format(_T("媒体文件(%s)"),strExt);//根据项目修改
	lRtn+=RegSetValue(hKey,NULL,REG_SZ,strDescribe,strDescribe.GetLength());
	lRtn+=RegCloseKey(hKey);

	strTemp.Format(_T("%s\\DefaultIcon"),strExeFileKey);
	lRtn+=RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
	lRtn+=RegSetValue(hKey,NULL,REG_SZ,strDefaultIcon,strDefaultIcon.GetLength());
	lRtn+=RegCloseKey(hKey);

	strTemp.Format(_T("%s\\shell\\open"),strExeFileKey);
	lRtn+=RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
	strTemp=_T("使用 ZMShow 打开");//鼠标右键双击显示项，根据项目修改
	lRtn+=RegSetValue(hKey,NULL,REG_SZ,strTemp,strTemp.GetLength());
	lRtn+=RegCloseKey(hKey);

	strTemp.Format(_T("%s\\shell\\open\\command"),strExeFileKey);
	lRtn+=RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
	strExeFullPath.Format(_T("\"%s\" \"%%1\""),strExePath);
	lRtn+=RegSetValue(hKey,NULL,REG_SZ,strExeFullPath,strExeFullPath.GetLength());
	lRtn+=RegCloseKey(hKey);
	if(lRtn==ERROR_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

BOOL BackupFileRelation_Z(const CString & strExt)
{
	BOOL nRtn=FALSE;
	if(strExt.IsEmpty())
        return nRtn;
	HKEY hKey;
	TCHAR szNullValue[_MAX_PATH]={0}; 
	TCHAR szBakValue[_MAX_PATH]={0}; 
	DWORD dwSize=sizeof(szNullValue); 
	if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,NULL,NULL,NULL,(LPBYTE)szNullValue,&dwSize)==ERROR_SUCCESS)
		{
			dwSize=sizeof(szBakValue); 
			if(RegQueryValueEx(hKey,_T("zmsbak"),NULL,NULL,(LPBYTE)szBakValue,&dwSize)==ERROR_SUCCESS)
			{
				CString strNullValue,strBakValue;
				strNullValue=szNullValue;
				strBakValue=szBakValue;
				if(strNullValue!=strBakValue&&strNullValue!=CString(_T("zms")+strExt))
				{
					if(RegSetValueEx(hKey,_T("zmsbak"),0,REG_SZ,(CONST BYTE *)szNullValue,_tcslen(szNullValue)*sizeof(TCHAR))==ERROR_SUCCESS)
						nRtn=TRUE;
				}
				else
				{
					nRtn=TRUE;
				}
			}
			else
			{
				if(RegSetValueEx(hKey,_T("zmsbak"),0,REG_SZ,(CONST BYTE *)szNullValue,_tcslen(szNullValue)*sizeof(TCHAR))==ERROR_SUCCESS)
					nRtn=TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}

BOOL RecoverFileRelation_Z(const CString & strExt)
{
	BOOL nRtn=FALSE;
	if(strExt.IsEmpty())
        return nRtn;
	HKEY hKey;
	TCHAR szPath[_MAX_PATH]={0}; 
	DWORD dwSize=sizeof(szPath); 
	CString strValue;
	if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,_T("zmsbak"),NULL,NULL,(LPBYTE)szPath,&dwSize)==ERROR_SUCCESS)
		{
			strValue=szPath;
			if(RegSetValue(hKey,NULL,REG_SZ,strValue,strValue.GetLength())==ERROR_SUCCESS)
			{
				RegDeleteValue(hKey,_T("zmsbak"));
				nRtn=TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}

BOOL SetAutoRun_Z(BOOL nIsAutoRun,const CString & strExePath)  
{  
	BOOL nRtn=FALSE;
	HKEY hKey;  
	CString strRunPath(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")); 
	if(RegOpenKey(HKEY_LOCAL_MACHINE,strRunPath,&hKey) == ERROR_SUCCESS)      
	{ 
		if(nIsAutoRun) 
		{    
			CString strPath=_T("\"")+strExePath+_T("\"");
			TCHAR szPath[MAX_PATH]={0}; 
			int nSize=strPath.GetLength();
			for(int i=0;i<nSize;++i)
				szPath[i]=strPath[i];
			if(RegSetValueEx(hKey,_T("ZMS"),0,REG_SZ,(CONST BYTE *)szPath, _tcslen(szPath)*sizeof(TCHAR))== ERROR_SUCCESS) //根据项目修改
				nRtn=TRUE;  
		}  
		else  
		{  

			if(RegDeleteValue(hKey,_T("ZMS"))==ERROR_SUCCESS)//根据项目修改  
				nRtn=TRUE;
		} 
		RegCloseKey(hKey);  
	}
	return nRtn;
} 

BOOL CheckAutoRun_Z(const CString & strExePath)
{
	BOOL nRtn=FALSE;
	HKEY hKey;  
	CString strRunPath(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
	if(RegOpenKey(HKEY_LOCAL_MACHINE,strRunPath,&hKey) == ERROR_SUCCESS)
	{
		TCHAR szPath[_MAX_PATH]={0}; 
		DWORD dwSize=sizeof(szPath); 
		if(RegQueryValueEx(hKey,_T("ZMS"),NULL,NULL,(LPBYTE)szPath,&dwSize)==ERROR_SUCCESS)
		{
			CString strValue=szPath;
			if(strValue.Find(strExePath)!=-1)
				nRtn=TRUE;
		}
		RegCloseKey(hKey);  
	}
	return nRtn;
}

BOOL SetREG_SZData_Z(const CString & strRegPath,const CString & strData,const LPCTSTR lpValueName)
{  
	BOOL nRtn=FALSE;
	HKEY hKey;  
	if(RegCreateKey(HKEY_CURRENT_USER,strRegPath,&hKey) == ERROR_SUCCESS)      
	{ 
		TCHAR szData[MAX_PATH]={0}; 
		int nSize=strData.GetLength();
		for(int i=0;i<nSize;++i)
			szData[i]=strData[i];
		if(RegSetValueEx(hKey,lpValueName,0,REG_SZ,(CONST BYTE *)szData, _tcslen(szData)*sizeof(TCHAR))== ERROR_SUCCESS) //根据项目修改
			nRtn=TRUE;  
		RegCloseKey(hKey);  
	}
	return nRtn;
} 

BOOL SetREG_DWORDData_Z(const CString & strRegPath,const DWORD & dwData,const LPCTSTR lpValueName)
{  
	BOOL nRtn=FALSE;
	HKEY hKey;  
	if(RegCreateKey(HKEY_CURRENT_USER,strRegPath,&hKey) == ERROR_SUCCESS)      
	{ 
		if(RegSetValueEx(hKey,lpValueName,0,REG_DWORD,(CONST BYTE *)&dwData,sizeof(DWORD))== ERROR_SUCCESS) //根据项目修改
			nRtn=TRUE;  
		RegCloseKey(hKey);  
	}
	return nRtn;
} 

BOOL GetREG_SZData_Z(const CString & strRegPath,CString & strData,const LPCTSTR lpValueName)
{  
	BOOL nRtn=FALSE;
	HKEY hKey;  
	if(RegOpenKey(HKEY_CURRENT_USER,strRegPath,&hKey) == ERROR_SUCCESS)      
	{ 
		TCHAR szData[_MAX_PATH]={0}; 
		DWORD dwSize=sizeof(szData); 
		DWORD dwType;
		if(RegQueryValueEx(hKey,lpValueName,NULL,&dwType,(LPBYTE)szData,&dwSize)==ERROR_SUCCESS)
		{
			if(dwType==REG_SZ)
			{
				strData=szData;
				nRtn=TRUE;
			}
		} 
		RegCloseKey(hKey);  
	}
	return nRtn;
} 

BOOL GetREG_DWORDData_Z(const CString & strRegPath,DWORD & dwData,const LPCTSTR lpValueName)
{  
	BOOL nRtn=FALSE;
	HKEY hKey;  
	if(RegOpenKey(HKEY_CURRENT_USER,strRegPath,&hKey) == ERROR_SUCCESS)      
	{ 
		DWORD dwSize=sizeof(DWORD); 
		DWORD dwType;
		if(RegQueryValueEx(hKey,lpValueName,NULL,&dwType,(LPBYTE)&dwData,&dwSize)==ERROR_SUCCESS)
		{
			if(dwType==REG_DWORD)
			{
				nRtn=TRUE;
			}
		} 
		RegCloseKey(hKey);  
	}
	return nRtn;
} 

BOOL IsLittle_Endian_Z(void)
{
	union w
	{  
		int a;
		char b;
	} c;
	c.a = 1;
	if(c.b==1)
		return TRUE;
	else
		return FALSE;
}

void BrowseCurrentDir_Z(const CString & strFolder,std::vector<CString> & vec_strPaths)
{
	vec_strPaths.clear();
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strFolder+_T("\\*.*"));
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDirectory())
		{
			strPath=finder.GetFilePath();
			vec_strPaths.push_back(strPath);
		}
	}
	finder.Close();
}


BOOL IsRunasAdmin_Z(void)
{
	BOOL bIsAdmin = FALSE;
	HANDLE hToken = NULL;
	// Get current process token  
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return FALSE;
	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;
	// Retrieve token elevation information  
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			bIsAdmin = tokenEle.TokenIsElevated;
		}
	}
	CloseHandle(hToken);
	return bIsAdmin;
}


CString GetWinVersion_Z(void)
{
	CString strVersion;
	WKSTA_INFO_100 *wkstaInfo = NULL;
	NET_API_STATUS netStatus = NetWkstaGetInfo(NULL, 100, (BYTE **)&wkstaInfo);
	if (netStatus == NERR_Success)
	{
		DWORD dwMajVer = wkstaInfo->wki100_ver_major;
		DWORD dwMinVer = wkstaInfo->wki100_ver_minor;
		NetApiBufferFree(wkstaInfo);
		strVersion.Format(_T("%d.%d"), dwMajVer, dwMinVer);
	}
	return strVersion;
}