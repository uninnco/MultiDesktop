
// multi_desktop.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmulti_desktopApp: 
// �йش����ʵ�֣������ multi_desktop.cpp
//

class Cmulti_desktopApp : public CWinApp
{
public:
	Cmulti_desktopApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	CMap<CString, LPCTSTR, BOOL, BOOL> m_isCreateExplorer;
	CString m_switchDesktopName;
	BOOL m_switchDesktop;

	/*
		���� explorer ����
	*/
	void StartExplorer();

	/*
		ö���̣߳����ڲ����Ƿ���explorer����Ҫ�л������洴��
	*/
	void EnumThread();

	DECLARE_MESSAGE_MAP()
};

extern Cmulti_desktopApp theApp;