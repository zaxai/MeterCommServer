#include "StdAfx.h"
#include "MainData.h"


CMainData::CMainData(void)
	: m_strClientIP(_T(""))
	, m_pMainDlg(NULL)
{
}

CMainData::CMainData(const ZFileSocket & sock,const CString & strIP,CWnd * pWnd)
	: m_sockClient(sock)
	, m_strClientIP(strIP)
	, m_pMainDlg(pWnd)
{
}

CMainData::~CMainData(void)
{
}
