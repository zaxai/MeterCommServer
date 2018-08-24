
// MeterCommServer.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "MyFuncLib.h"
#include "MyClassLib.h"

// CMeterCommServerApp:
// 有关此类的实现，请参阅 MeterCommServer.cpp
//

class CMeterCommServerApp : public CWinApp
{
public:
	CMeterCommServerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMeterCommServerApp theApp;

#define WM_MSGRECVPRO WM_USER+1
#define WM_TRAYNOTIFY WM_USER+2

enum MSGUSER
{
	MSGUSER_ADDMAINDATA = 0,
	MSGUSER_DELETEMAINDATA,
	MSGUSER_UPDATEEDIT
};