#pragma once
#include "ZSocket.h"
#include "ZDLT645_2007.h"
#include "ZDLT698_45.h"
class CMainData
{
public:
	CMainData(void);
	CMainData(const ZFileSocket & sock,const CString & strIP,CWnd * pWnd);
	~CMainData(void);
	ZFileSocket m_sockClient;
	CString m_strClientIP;
	CWnd *m_pMainDlg;
	ZDLT645_2007 m_dlt645;
	ZDLT698_45 m_dlt698;
};

