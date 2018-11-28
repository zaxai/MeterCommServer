#include "stdafx.h"
#include "ZUtil.h"

void ZUtil::StrSplit(CString strSrc, std::vector<CString> & vec_strData, CString strSplit, bool bLastExist)
{
	vec_strData.clear();
	int nRtn = strSrc.Find(strSplit);
	while (nRtn >= 0)
	{
		vec_strData.push_back(strSrc.Left(nRtn));
		strSrc.Delete(0, nRtn + strSplit.GetLength());
		nRtn = strSrc.Find(strSplit);
	}
	if (!bLastExist)
		vec_strData.push_back(strSrc);
}

void ZUtil::StrSplit(CString strSrc, std::vector<CString> & vec_strData, TCHAR cSplit, bool bLastExist)
{
	StrSplit(strSrc, vec_strData, CString(cSplit), bLastExist);
}

BOOL ZUtil::WtoA(const wchar_t * p_cSrc, char * p_cDest, const int & nDesSize)
{
	int nWcharLen = wcslen(p_cSrc);
	int nCharLen = WideCharToMultiByte(CP_ACP, 0, p_cSrc, nWcharLen, NULL, 0, NULL, NULL);
	if (nCharLen >= nDesSize)
		return FALSE;
	WideCharToMultiByte(CP_ACP, 0, p_cSrc, nWcharLen, p_cDest, nCharLen, NULL, NULL);
	p_cDest[nCharLen] = '\0';
	return TRUE;
}

BOOL ZUtil::WtoUTF8(const wchar_t * p_cSrc, char * p_cDest, const int & nDesSize)
{
	int nWcharLen = wcslen(p_cSrc);
	int nCharLen = WideCharToMultiByte(CP_UTF8, 0, p_cSrc, nWcharLen, NULL, 0, NULL, NULL);
	if (nCharLen >= nDesSize)
		return FALSE;
	WideCharToMultiByte(CP_UTF8, 0, p_cSrc, nWcharLen, p_cDest, nCharLen, NULL, NULL);
	p_cDest[nCharLen] = '\0';
	return TRUE;
}

BOOL ZUtil::AtoW(const char * p_cSrc, wchar_t * p_cDest, const int & nDesSize)
{
	int nCharLen = strlen(p_cSrc);
	int nWcharLen = MultiByteToWideChar(CP_ACP, NULL, p_cSrc, nCharLen, NULL, 0);
	if (nWcharLen >= nDesSize)
		return FALSE;
	MultiByteToWideChar(CP_ACP, NULL, p_cSrc, nCharLen, p_cDest, nWcharLen);
	p_cDest[nWcharLen] = L'\0';
	return TRUE;
}

BOOL ZUtil::UTF8toW(const char * p_cSrc, wchar_t * p_cDest, const int & nDesSize)
{
	int nCharLen = strlen(p_cSrc);
	int nWcharLen = MultiByteToWideChar(CP_UTF8, NULL, p_cSrc, nCharLen, NULL, 0);
	if (nWcharLen >= nDesSize)
		return FALSE;
	MultiByteToWideChar(CP_UTF8, NULL, p_cSrc, nCharLen, p_cDest, nWcharLen);
	p_cDest[nWcharLen] = L'\0';
	return TRUE;
}

BOOL ZUtil::AtoUTF8(const char * p_cSrc, char * p_cDest, const int & nDesSize)
{
	int nWcharLen = MultiByteToWideChar(CP_ACP, NULL, p_cSrc, strlen(p_cSrc), NULL, 0);
	wchar_t * p_c = new wchar_t[nWcharLen + 1];
	AtoW(p_cSrc, p_c, nWcharLen + 1);
	BOOL nRtn = WtoUTF8(p_c, p_cDest, nDesSize);
	delete[]p_c;
	return nRtn;
}

BOOL ZUtil::UTF8toA(const char * p_cSrc, char * p_cDest, const int & nDesSize)
{
	int nWcharLen = MultiByteToWideChar(CP_UTF8, NULL, p_cSrc, strlen(p_cSrc), NULL, 0);
	wchar_t * p_c = new wchar_t[nWcharLen + 1];
	UTF8toW(p_cSrc, p_c, nWcharLen + 1);
	BOOL nRtn = WtoA(p_c, p_cDest, nDesSize);
	delete[]p_c;
	return nRtn;
}

BOOL ZUtil::IsStrUTF8(char* p_str, unsigned long long llLength)
{
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	UCHAR chr;
	BOOL nAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8  
	for (unsigned long long i = 0; i < llLength; ++i)
	{
		chr = p_str[i];
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
			nAllAscii = FALSE;
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数  
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
					return FALSE;
				--nBytes;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx  
		{
			if ((chr & 0xC0) != 0x80)
				return FALSE;
			--nBytes;
		}
	}
	if (nBytes != 0) //违返规则,末位字符非UTF8
		return FALSE;
	if (nAllAscii)//如果全部都是ASCII, 归为ANSI 
		return FALSE;
	return TRUE;
}

int ZUtil::GetFileEncoding(const CString & strFilePath)
{
	int nRtn = ENCODING_FAIL;
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
		return nRtn;
	unsigned char szHead[3] = { 0 };
	file.Read(szHead, sizeof(szHead));
	if (szHead[0] == 0xFF && szHead[1] == 0xFE)
		nRtn = ENCODING_UNICODE;
	else if (szHead[0] == 0xFE && szHead[1] == 0xFF)
		nRtn = ENCODING_UNICODEBIGENDIAN;
	else if (szHead[0] == 0xEF && szHead[1] == 0xBB && szHead[2] == 0xBF)
		nRtn = ENCODING_UTF8;
	else
	{
		unsigned long lLength = (unsigned long)file.GetLength();
		char *p_cData = new char[lLength];
		file.SeekToBegin();
		file.Read(p_cData, lLength);
		if (IsStrUTF8(p_cData, lLength))
			nRtn = ENCODING_UTF8;
		else
			nRtn = ENCODING_ANSI;
		delete[]p_cData;
	}
	file.Close();
	return nRtn;
}

BOOL ZUtil::IsLittle_Endian(void)
{
	union w
	{
		int a;
		char b;
	} c;
	c.a = 1;
	if (c.b == 1)
		return TRUE;
	else
		return FALSE;
}

BOOL ZUtil::ReadFile2CString(const CString & strFilePath, CString & strDest)
{
	strDest.Empty();
	int nEncoding = GetFileEncoding(strFilePath);
	if (nEncoding == ENCODING_FAIL)
		return FALSE;
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	unsigned long lLength = (unsigned long)file.GetLength();
	char *p_cData = new char[lLength + 2];
	if (!p_cData)
	{
		file.Close();
		return FALSE;
	}
	file.SeekToBegin();
	file.Read(p_cData, lLength);
	file.Close();
	p_cData[lLength] = '\0';
	p_cData[lLength + 1] = '\0';
	switch (nEncoding)
	{
	case ENCODING_ANSI:
		strDest = p_cData;
		break;
	case ENCODING_UNICODE:
	{
		if (!IsLittle_Endian())
		{
			for (unsigned long i = 0; i < lLength; i = i + 2)
			{
				std::swap(p_cData[i], p_cData[i + 1]);
			}
		}
		wchar_t * p_c = (wchar_t *)p_cData;
		strDest = p_c;
	}
	break;
	case ENCODING_UNICODEBIGENDIAN:
	{
		if (IsLittle_Endian())
		{
			for (unsigned long i = 0; i < lLength; i = i + 2)
			{
				std::swap(p_cData[i], p_cData[i + 1]);
			}
		}
		wchar_t * p_c = (wchar_t *)p_cData;
		strDest = p_c;
	}
	break;
	case ENCODING_UTF8:
	{
		int nWcharLen = MultiByteToWideChar(CP_UTF8, NULL, p_cData, lLength, NULL, 0);
		wchar_t * p_c = new wchar_t[nWcharLen + 1];
		if (p_c&&UTF8toW(p_cData, p_c, nWcharLen + 1))
		{
			strDest = p_c;
			delete[]p_c;
		}
		else
		{
			if (p_c)
				delete[]p_c;
			delete[]p_cData;
			return FALSE;
		}
	}
	break;
	}
	delete[]p_cData;
	return TRUE;
}


CString ZUtil::GetExeCatalogPath(void)
{
	TCHAR  szPath[MAX_PATH];
	CString strExePath, strExeCatalogPath;
	DWORD dwRtn = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (dwRtn)
	{
		strExePath = szPath;
		strExeCatalogPath = strExePath.Left(strExePath.ReverseFind('\\'));
	}
	return strExeCatalogPath;
}

CString ZUtil::GetExePath(void)
{
	TCHAR  szPath[MAX_PATH];
	CString strExePath;
	DWORD dwRtn = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (dwRtn)
	{
		strExePath = szPath;
	}
	return strExePath;
}


void ZUtil::BrowseCurrentDir(const CString & strFolder, std::vector<CString> & vec_strPaths)
{
	vec_strPaths.clear();
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strFolder + _T("\\*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			strPath = finder.GetFilePath();
			vec_strPaths.push_back(strPath);
		}
	}
	finder.Close();
}


BOOL ZUtil::IsRunasAdmin(void)
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


CString ZUtil::GetWinVersion(void)
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