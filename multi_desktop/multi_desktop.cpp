
// multi_desktop.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "multi_desktop.h"
#include "multi_desktopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmulti_desktopApp

BEGIN_MESSAGE_MAP(Cmulti_desktopApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cmulti_desktopApp ����

Cmulti_desktopApp::Cmulti_desktopApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cmulti_desktopApp ����

Cmulti_desktopApp theApp;

unsigned int __stdcall NewDlg(LPVOID lparam)
{
	HANDLE hThreadExitEvent = *(HANDLE*)(lparam);

	//
	// ���̺߳Ϳɽ����û�������������
	//
	HDESK hDesktop = NULL;
	hDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
	SetThreadDesktop(hDesktop);
	CloseDesktop(hDesktop);

	//
	// ����explorer
	//
	theApp.StartExplorer();

	//
	// �����Ի���
	//
	Cmulti_desktopDlg aboutDlg;
	aboutDlg.DoModal();

	::SetEvent(hThreadExitEvent);
	return 0;
}


// Cmulti_desktopApp ��ʼ��

BOOL Cmulti_desktopApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	do
	{
		m_switchDesktop = false;
		HANDLE hThreadExitEvent = NULL;

		hThreadExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		//
		// ���µ��߳��д����Ի��� �� explorer(explorer���Ǳ���)
		//
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, NewDlg, &hThreadExitEvent, 0, NULL);
		CloseHandle(hThread);

		WaitForSingleObject(hThreadExitEvent, INFINITE);

		CloseHandle(hThreadExitEvent);
	} while (m_switchDesktop);


	//Cmulti_desktopDlg* dlg = new Cmulti_desktopDlg;
	//m_pMainWnd = dlg;
	//INT_PTR nResponse = dlg->DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO:  �ڴ˷��ô����ʱ��
	//	//  ��ȷ�������رնԻ���Ĵ���
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO:  �ڴ˷��ô����ʱ��
	//	//  ��ȡ�������رնԻ���Ĵ���
	//}
	//else if (nResponse == -1)
	//{
	//	TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
	//	TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	//}
	//delete dlg;

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


void Cmulti_desktopApp::StartExplorer()
{
	if (m_switchDesktopName.IsEmpty())
	{
		return;
	}

	//
	// �����ظ����� explorer
	//
	BOOL isStart = m_isCreateExplorer[m_switchDesktopName.GetBuffer()];
	if (isStart)
	{
		return;
	}

	//
	// ö��explorer�����explorer���߳��������ӵ�����������棬��ô�Ͳ�����
	//
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };

	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = m_switchDesktopName.GetBuffer();
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_NORMAL;

	wchar_t wszCmd[MAX_PATH] = { 0 };
	GetWindowsDirectoryW(wszCmd, MAX_PATH);
	wcscat_s(wszCmd, MAX_PATH, L"\\explorer.exe");
	if (CreateProcessW(NULL, wszCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		m_isCreateExplorer[m_switchDesktopName] = TRUE;
	}

	if (pi.hProcess)
	{
		CloseHandle(pi.hProcess);
	}
	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
	}
}

void Cmulti_desktopApp::EnumThread()
{

}