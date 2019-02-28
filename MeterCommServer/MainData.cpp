#include "StdAfx.h"
#include "MainData.h"


CMainData::CMainData(ZFileSocket & sock,const CString & strIP,CWnd * pWnd)
	: m_sockClient(std::move(sock))
	, m_strClientIP(strIP)
	, m_pMainDlg(pWnd)
{
}


CMainData::CMainData(CMainData && md)
{
	m_sockClient=std::move(md.m_sockClient);
	m_strClientIP=md.m_strClientIP;
	m_pMainDlg=md.m_pMainDlg;
	m_dlt645 = md.m_dlt645;
	m_dlt698 = md.m_dlt698;
}


CMainData::~CMainData(void)
{
}
