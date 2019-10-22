
// multi_desktop.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Cmulti_desktopApp: 
// 有关此类的实现，请参阅 multi_desktop.cpp
//

class Cmulti_desktopApp : public CWinApp
{
public:
	Cmulti_desktopApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	CMap<CString, LPCTSTR, BOOL, BOOL> m_isCreateExplorer;
	CString m_switchDesktopName;
	BOOL m_switchDesktop;

	/*
		开启 explorer 进程
	*/
	void StartExplorer();

	/*
		枚举线程，用于查找是否有explorer在需要切换的桌面创建
	*/
	void EnumThread();

	DECLARE_MESSAGE_MAP()
};

extern Cmulti_desktopApp theApp;