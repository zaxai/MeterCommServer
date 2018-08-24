// Config.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterCommServer.h"
#include "Config.h"
#include "afxdialogex.h"


// CConfig 对话框

IMPLEMENT_DYNAMIC(CConfig, CDialogEx)

CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfig::IDD, pParent)
	, m_strServerIP(_T("INADDR_ANY"))
	, m_strServerPort(_T("6000"))
	, m_strServerTimeOut(_T("3000"))
	, m_strNetCptIP(_T("10.0.17.99"))
	, m_strNetCptPort(_T("6666"))
	, m_strNetCptTimeOut(_T("3000"))
	, m_strVersion(_T("1.0.0.1"))
	, m_strUpdateFlag(_T("0"))
{

}

CConfig::~CConfig()
{
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfig 消息处理程序


BOOL CConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_EDIT_SERVERIP,m_strServerIP);
	SetDlgItemText(IDC_EDIT_SERVERPORT,m_strServerPort);
	SetDlgItemText(IDC_EDIT_SERVERTIMEOUT,m_strServerTimeOut);
	SetDlgItemText(IDC_EDIT_NETCPTIP,m_strNetCptIP);
	SetDlgItemText(IDC_EDIT_NETCPTPORT,m_strNetCptPort);
	SetDlgItemText(IDC_EDIT_NETCPTTIMEOUT,m_strNetCptTimeOut);
	SetDlgItemText(IDC_EDIT_VERSION,m_strVersion);
	SetDlgItemText(IDC_EDIT_UPDATEFLAG,m_strUpdateFlag);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CConfig::SetParam(const CString & strServerIP,const CString & strServerPort,const CString & strServerTimeOut,const CString & strNetCptIP,const CString & strNetCptPort,const CString & strNetCptTimeOut,const CString & strVersion,const CString & strUpdateFlag)
{
	m_strServerIP=strServerIP;
	m_strServerPort=strServerPort;
	m_strServerTimeOut=strServerTimeOut;
	m_strNetCptIP=strNetCptIP;
	m_strNetCptPort=strNetCptPort;
	m_strNetCptTimeOut=strNetCptTimeOut;
	m_strVersion=strVersion;
	m_strUpdateFlag=strUpdateFlag;
}

void CConfig::GetParam(CString & strServerIP,CString & strServerPort,CString & strServerTimeOut,CString & strNetCptIP,CString & strNetCptPort,CString & strNetCptTimeOut,CString & strVersion,CString & strUpdateFlag)
{
	strServerIP=m_strServerIP;
	strServerPort=m_strServerPort;
	strServerTimeOut=m_strServerTimeOut;
	strNetCptIP=m_strNetCptIP;
	strNetCptPort=m_strNetCptPort;
	strNetCptTimeOut=m_strNetCptTimeOut;
	strVersion=m_strVersion;
	strUpdateFlag=m_strUpdateFlag;
}

void CConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SERVERIP,m_strServerIP);
	GetDlgItemText(IDC_EDIT_SERVERPORT,m_strServerPort);
	GetDlgItemText(IDC_EDIT_SERVERTIMEOUT,m_strServerTimeOut);
	GetDlgItemText(IDC_EDIT_NETCPTIP,m_strNetCptIP);
	GetDlgItemText(IDC_EDIT_NETCPTPORT,m_strNetCptPort);
	GetDlgItemText(IDC_EDIT_NETCPTTIMEOUT,m_strNetCptTimeOut);
	GetDlgItemText(IDC_EDIT_VERSION,m_strVersion);
	GetDlgItemText(IDC_EDIT_UPDATEFLAG,m_strUpdateFlag);
	CDialogEx::OnOK();
}
