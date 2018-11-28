#include "stdafx.h"
#include "ZOperatorRegedit.h"


BOOL ZOperatorRegedit::SetAutoRun(const CString & strExePath, LPCTSTR lpValueName, BOOL nIsAutoRun)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	CString strSubKey(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
	if (RegOpenKey(HKEY_LOCAL_MACHINE, strSubKey, &hKey) == ERROR_SUCCESS)
	{
		if (nIsAutoRun)
		{
			CString strPath = _T("\"") + strExePath + _T("\"");
			if (RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (CONST BYTE *)(LPCTSTR)strPath, strPath.GetLength() * sizeof(TCHAR)) == ERROR_SUCCESS)
				nRtn = TRUE;
		}
		else
		{
			if (RegDeleteValue(hKey, lpValueName) == ERROR_SUCCESS)
				nRtn = TRUE;
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZOperatorRegedit::CheckAutoRun(const CString & strExePath, LPCTSTR lpValueName)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	CString strSubKey(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
	if (RegOpenKey(HKEY_LOCAL_MACHINE, strSubKey, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szPath[MAX_PATH] = { 0 };
		DWORD dwSize = sizeof(szPath);
		if (RegQueryValueEx(hKey, lpValueName, NULL, NULL, (LPBYTE)szPath, &dwSize) == ERROR_SUCCESS)
		{
			CString strValue(szPath);
			if (strValue.Find(strExePath) != -1)
				nRtn = TRUE;
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZOperatorRegedit::SetREG_SZData(LPCTSTR lpSubKey, LPCTSTR lpValueName, const CString & strData)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	if (RegCreateKey(HKEY_CURRENT_USER, lpSubKey, &hKey) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (CONST BYTE *)(LPCTSTR)strData, strData.GetLength() * sizeof(TCHAR)) == ERROR_SUCCESS)
			nRtn = TRUE;
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZOperatorRegedit::GetREG_SZData(LPCTSTR lpSubKey, LPCTSTR lpValueName, CString & strData)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, lpSubKey, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szData[MAX_PATH] = { 0 };
		DWORD dwSize = sizeof(szData);
		DWORD dwType;
		if (RegQueryValueEx(hKey, lpValueName, NULL, &dwType, (LPBYTE)szData, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ)
			{
				strData = szData;
				nRtn = TRUE;
			}
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZOperatorRegedit::SetREG_DWORDData(LPCTSTR lpSubKey, LPCTSTR lpValueName, const DWORD & dwData)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	if (RegCreateKey(HKEY_CURRENT_USER, lpSubKey, &hKey) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hKey, lpValueName, 0, REG_DWORD, (CONST BYTE *)&dwData, sizeof(DWORD)) == ERROR_SUCCESS)
			nRtn = TRUE;
		RegCloseKey(hKey);
	}
	return nRtn;
}


BOOL ZOperatorRegedit::GetREG_DWORDData(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD & dwData)
{
	BOOL nRtn = FALSE;
	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, lpSubKey, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType;
		if (RegQueryValueEx(hKey, lpValueName, NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD)
				nRtn = TRUE;
		}
		RegCloseKey(hKey);
	}
	return nRtn;
}