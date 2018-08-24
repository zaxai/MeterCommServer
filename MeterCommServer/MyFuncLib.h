#ifndef _MYFUNCLIB_H
#define _MYFUNCLIB_H
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#include <vector>

void StrSplit_Z(CString strSrc, std::vector<CString> & vecstrDatas,TCHAR tcSplit,bool bLastExist);
BOOL TtoA_Z(const wchar_t * p_tcSrc, char * p_cDest,const int & nDesSize);
BOOL TtoUTF8_Z(const wchar_t * p_tcSrc, char * p_cDest,const int & nDesSize);
BOOL UTF8toT_Z(const char * p_cSrc, wchar_t * p_tcDest,const int & nDesSize);
BOOL IsStrUTF8_Z(char* p_str,long long llLength,BOOL nIsCheckAll);
int GetFileEncoding_Z(const CString & strFilePath);
BOOL IsLittle_endian_Z(void);
BOOL ReadFile2CString_Z(const CString & strFilePath,CString & strDest);
CString GetExeCatalogPath_Z(void);
CString GetExePath_Z(void);
BOOL CheckFileRelation_Z(const CString & strExt, const CString & strExePath);
BOOL SetFileRelation_Z(const CString & strExt,const CString & strExePath,const CString & strDefaultIcon);
BOOL BackupFileRelation_Z(const CString & strExt);
BOOL RecoverFileRelation_Z(const CString & strExt);
BOOL SetAutoRun_Z(BOOL nIsAutoRun,const CString & strExePath);
BOOL CheckAutoRun_Z(const CString & strExePath);
BOOL SetREG_SZData_Z(const CString & strRegPath,const CString & strData,const LPCTSTR lpValueName);
BOOL GetREG_SZData_Z(const CString & strRegPath,CString & strData,const LPCTSTR lpValueName);
BOOL SetREG_DWORDData_Z(const CString & strRegPath,const DWORD & dwData,const LPCTSTR lpValueName);
BOOL GetREG_DWORDData_Z(const CString & strRegPath,DWORD & dwData,const LPCTSTR lpValueName);
BOOL IsLittle_Endian_Z(void);
void BrowseCurrentDir_Z(const CString & strFolder,std::vector<CString> & vec_strPaths);
BOOL IsRunasAdmin_Z(void);
CString GetWinVersion_Z(void);
#endif