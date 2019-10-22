
// multi_desktop.cpp : 定义应用程序的类行为。
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


// Cmulti_desktopApp 构造

Cmulti_desktopApp::Cmulti_desktopApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cmulti_desktopApp 对象

Cmulti_desktopApp theApp;

unsigned int __stdcall NewDlg(LPVOID lparam)
{
	HANDLE hThreadExitEvent = *(HANDLE*)(lparam);

	//
	// 将线程和可接受用户输入的桌面关联
	//
	HDESK hDesktop = NULL;
	hDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
	SetThreadDesktop(hDesktop);
	CloseDesktop(hDesktop);

	//
	// 开启explorer
	//
	theApp.StartExplorer();

	//
	// 开启对话框
	//
	Cmulti_desktopDlg aboutDlg;
	aboutDlg.DoModal();

	::SetEvent(hThreadExitEvent);
	return 0;
}


// Cmulti_desktopApp 初始化

BOOL Cmulti_desktopApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	do
	{
		m_switchDesktop = false;
		HANDLE hThreadExitEvent = NULL;

		hThreadExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		//
		// 在新的线程中创建对话框 和 explorer(explorer不是必须)
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
	//	// TODO:  在此放置处理何时用
	//	//  “确定”来关闭对话框的代码
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO:  在此放置处理何时用
	//	//  “取消”来关闭对话框的代码
	//}
	//else if (nResponse == -1)
	//{
	//	TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
	//	TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	//}
	//delete dlg;

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


void Cmulti_desktopApp::StartExplorer()
{
	if (m_switchDesktopName.IsEmpty())
	{
		return;
	}

	//
	// 避免重复创建 explorer
	//
	BOOL isStart = m_isCreateExplorer[m_switchDesktopName.GetBuffer()];
	if (isStart)
	{
		return;
	}

	//
	// 枚举explorer，如果explorer的线程桌面连接到了输入的桌面，那么就不创建
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