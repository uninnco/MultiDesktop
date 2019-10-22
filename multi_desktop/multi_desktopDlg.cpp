
// multi_desktopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "multi_desktop.h"
#include "multi_desktopDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmulti_desktopDlg 对话框



Cmulti_desktopDlg::Cmulti_desktopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmulti_desktopDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmulti_desktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINSTA, m_listWinSta);
	DDX_Control(pDX, IDC_LIST_DESKTOP, m_listDesktop);
}

BEGIN_MESSAGE_MAP(Cmulti_desktopDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &Cmulti_desktopDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_SWITCH, &Cmulti_desktopDlg::OnBnClickedBtnSwitch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WINSTA, &Cmulti_desktopDlg::OnItemchangedListWinsta)
	ON_BN_CLICKED(IDC_BTN_CREATE, &Cmulti_desktopDlg::OnBnClickedBtnCreate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DESKTOP, &Cmulti_desktopDlg::OnItemchangedListDesktop)
END_MESSAGE_MAP()


// Cmulti_desktopDlg 消息处理程序

BOOL Cmulti_desktopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	DWORD dwStyle = m_listWinSta.GetStyle();//获取当前窗口style  
	dwStyle &= ~LVS_TYPEMASK; //清除显示方式位  
	dwStyle |= LVS_REPORT; //设置style  
	SetWindowLong(m_listWinSta.m_hWnd, GWL_STYLE, dwStyle);//设置style  
	dwStyle = m_listWinSta.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //网格线
	dwStyle |= LVS_EX_SINGLEROW; //单选
	m_listWinSta.SetExtendedStyle(dwStyle);

	m_listWinSta.InsertColumn(0, L"窗口站名称", 0, 60);
	m_listWinSta.InsertColumn(1, L"可视界面", 0, 60);
	m_listWinSta.InsertColumn(2, L"类型", 0, 60);
	m_listWinSta.InsertColumn(3, L"SID", 0, 60);


	dwStyle = m_listDesktop.GetStyle();//获取当前窗口style  
	dwStyle &= ~LVS_TYPEMASK; //清除显示方式位  
	dwStyle |= LVS_REPORT; //设置style  
	SetWindowLong(m_listDesktop.m_hWnd, GWL_STYLE, dwStyle);//设置style  
	dwStyle = m_listDesktop.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //网格线（只适用与报表风格的listctrl）
	dwStyle |= LVS_EX_SINGLEROW; //单选
	m_listDesktop.SetExtendedStyle(dwStyle);

	m_listDesktop.InsertColumn(0, L"桌面名称", 0, 80);
	m_listDesktop.InsertColumn(1, L"允许其他账户设置钩子过程", 0, 60);

	m_listDesktop.InsertColumn(2, L"窗口堆(KB)", 0, 80);
	m_listDesktop.InsertColumn(3, L"是否接收用户输入", 0, 60);
	m_listDesktop.InsertColumn(4, L"类型", 0, 80);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmulti_desktopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmulti_desktopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmulti_desktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 枚举桌面回调函数
BOOL CALLBACK Cmulti_desktopDlg::EnumDesktopProc(LPWSTR lpDesktopName, LPARAM lpParam)
{
	BOOL bContinue = TRUE;
	Cmulti_desktopDlg *pDlg;

	do
	{
		if ((pDlg = reinterpret_cast<Cmulti_desktopDlg*>(lpParam)) == NULL)
		{
			bContinue = FALSE;
			break;
		}

		if (lpDesktopName)
		{
			TRACE1("WinStaName = %s. \n", lpDesktopName);
			int itemCount = pDlg->m_listDesktop.GetItemCount();
			if (itemCount >= 0)
			{
				// 添加到列表控件
				pDlg->m_listDesktop.InsertItem(itemCount, lpDesktopName);
				DESKTOP_ALL_INFO desktop_all_info;
				if (pDlg->m_desktopManage.GetDesktopAllInfo(lpDesktopName, desktop_all_info))
				{
					CString item_text;
					pDlg->m_listDesktop.SetItemText(itemCount, 1, desktop_all_info.allow_other_account_hook ? L"是" : L"否");
					item_text.Format(L"%dkb", desktop_all_info.heap_size);
					pDlg->m_listDesktop.SetItemText(itemCount, 2, item_text);
					pDlg->m_listDesktop.SetItemText(itemCount, 3, desktop_all_info.accept_io ? L"是" : L"否");
					pDlg->m_listDesktop.SetItemText(itemCount, 4, desktop_all_info.type);
				}
			}
		}

	} while (FALSE);

	return bContinue;
}

// 枚举窗口站回调函数
BOOL CALLBACK Cmulti_desktopDlg::EnumWinStationProc(LPWSTR lpWinStaName, LPARAM lpParam)
{
	BOOL bContinue = TRUE;
	Cmulti_desktopDlg *pDlg;

	do
	{
		if ((pDlg = reinterpret_cast<Cmulti_desktopDlg*>(lpParam)) == NULL)
		{
			bContinue = FALSE;
			break;
		}

		if (lpWinStaName)
		{
			TRACE1("WinStaName = %s. \n", lpWinStaName);
			int itemCount = pDlg->m_listWinSta.GetItemCount();
			if (itemCount >= 0)
			{
				// 添加到列表控件
				pDlg->m_listWinSta.InsertItem(itemCount, lpWinStaName);

				WINSTA_ALL_INFO winsta_all_info;
				if (pDlg->m_desktopManage.GetWinStaAllInfo(lpWinStaName, winsta_all_info))
				{
					pDlg->m_listWinSta.SetItemText(itemCount, 1, winsta_all_info.visible ? L"是" : L"否");
					pDlg->m_listWinSta.SetItemText(itemCount, 2, (wchar_t*)winsta_all_info.type);
				}
			}
		}
	} while (FALSE);

	return bContinue;
}

// 刷新按钮，枚举窗口站
void Cmulti_desktopDlg::OnBnClickedBtnRefresh()
{
	m_listDesktop.DeleteAllItems();
	m_listWinSta.DeleteAllItems();
	m_desktopManage.EnumWindowStation(EnumWinStationProc, (LPARAM)this);
}


// 切换按钮切换桌面后，关闭对话框。然后在App里面新建
void Cmulti_desktopDlg::OnBnClickedBtnSwitch()
{
	if (!m_curSelDesktopName.IsEmpty())
	{
		// 切换桌面
		if (m_desktopManage.SwitchDesktop(m_curSelWinStaName.GetBuffer(), m_curSelDesktopName.GetString()))
		{
			theApp.m_switchDesktop = TRUE;

			// 设置切换的桌面路径，用于启动explorer
			theApp.m_switchDesktopName = m_curSelWinStaName + L"\\" + m_curSelDesktopName;

			PostMessage(WM_CLOSE);
		}
	}
}


// 窗口站列表点击事件，获取窗口站名称
void Cmulti_desktopDlg::OnItemchangedListWinsta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelWinStaName = m_listWinSta.GetItemText(pNMLV->iItem, 0);

	m_curSelDesktopName = L"";
	m_listDesktop.DeleteAllItems();

	// 枚举桌面
	m_desktopManage.EnumDestkop(m_curSelWinStaName.GetBuffer(), EnumDesktopProc, (LPARAM)this);
}


// 创建桌面，从编辑框中取出桌面名称
void Cmulti_desktopDlg::OnBnClickedBtnCreate()
{
	CString strDesktopName;
	GetDlgItemTextW(IDC_EDIT_DESKTOP_NAME, strDesktopName);
	if (strDesktopName.IsEmpty())
	{
		return;
	}

	// 创建桌面
	if (m_desktopManage.CreateDesktop(m_curSelWinStaName.GetBuffer(), strDesktopName.GetBuffer()))
	{
		// 在列表上枚举窗口
		m_listDesktop.DeleteAllItems();
		m_desktopManage.EnumDestkop(m_curSelWinStaName.GetBuffer(), EnumDesktopProc, (LPARAM)this);
	}
}



void Cmulti_desktopDlg::OnItemchangedListDesktop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	m_curSelDesktopName = m_listDesktop.GetItemText(pNMLV->iItem, 0);
}


BOOL Cmulti_desktopDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}
