
// MeterCommServerDlg.h : 头文件
//

#pragma once
#include <list>
#include "afxwin.h"

// CMeterCommServerDlg 对话框
class CMeterCommServerDlg : public CDialogEx
{
// 构造
public:
	CMeterCommServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CMeterCommServerDlg();

// 对话框数据
	enum { IDD = IDD_METERCOMMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	ZSocket m_sockServer;
	CString m_strServerIP;
	CString m_strServerPort;
	CString m_strServerTimeOut;
	CString m_strNetCptIP;
	CString m_strNetCptPort;
	CString m_strNetCptTimeOut;
	CString m_strVersion;
	bool m_bIsListen;
	std::list<CMainData> m_list_md;
	HANDLE m_hMutex;//连接本地加密机需要独占
	HANDLE m_hEvtExit;
	CEdit m_editShow;
	int m_nEditLength;
	int m_nTotalReq;
	CButton m_checkClearTime;
	CButton m_checkAutoRun;
	NOTIFYICONDATA m_nid;
	bool m_bIsExit;
	CString m_strUpdateFlag;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTraynotify(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonStartserver();
	afx_msg void OnBnClickedButtonStopserver();
	afx_msg void OnBnClickedButtonClearmsg();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedCheckCleartime();
	afx_msg void OnBnClickedCheckAutorun();
	static UINT ListenThreadFunc(LPVOID lpParam);
	static UINT ExecReqThreadFunc(LPVOID lpParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InitTray(void);
	afx_msg void OnExit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
