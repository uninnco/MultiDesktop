
// multi_desktopDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "multi_desktop.h"
#include "multi_desktopDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cmulti_desktopDlg �Ի���



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


// Cmulti_desktopDlg ��Ϣ�������

BOOL Cmulti_desktopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	DWORD dwStyle = m_listWinSta.GetStyle();//��ȡ��ǰ����style  
	dwStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ  
	dwStyle |= LVS_REPORT; //����style  
	SetWindowLong(m_listWinSta.m_hWnd, GWL_STYLE, dwStyle);//����style  
	dwStyle = m_listWinSta.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //������
	dwStyle |= LVS_EX_SINGLEROW; //��ѡ
	m_listWinSta.SetExtendedStyle(dwStyle);

	m_listWinSta.InsertColumn(0, L"����վ����", 0, 60);
	m_listWinSta.InsertColumn(1, L"���ӽ���", 0, 60);
	m_listWinSta.InsertColumn(2, L"����", 0, 60);
	m_listWinSta.InsertColumn(3, L"SID", 0, 60);


	dwStyle = m_listDesktop.GetStyle();//��ȡ��ǰ����style  
	dwStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ  
	dwStyle |= LVS_REPORT; //����style  
	SetWindowLong(m_listDesktop.m_hWnd, GWL_STYLE, dwStyle);//����style  
	dwStyle = m_listDesktop.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�ֻ�����뱨�����listctrl��
	dwStyle |= LVS_EX_SINGLEROW; //��ѡ
	m_listDesktop.SetExtendedStyle(dwStyle);

	m_listDesktop.InsertColumn(0, L"��������", 0, 80);
	m_listDesktop.InsertColumn(1, L"���������˻����ù��ӹ���", 0, 60);

	m_listDesktop.InsertColumn(2, L"���ڶ�(KB)", 0, 80);
	m_listDesktop.InsertColumn(3, L"�Ƿ�����û�����", 0, 60);
	m_listDesktop.InsertColumn(4, L"����", 0, 80);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmulti_desktopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cmulti_desktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ö������ص�����
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
				// ��ӵ��б�ؼ�
				pDlg->m_listDesktop.InsertItem(itemCount, lpDesktopName);
				DESKTOP_ALL_INFO desktop_all_info;
				if (pDlg->m_desktopManage.GetDesktopAllInfo(lpDesktopName, desktop_all_info))
				{
					CString item_text;
					pDlg->m_listDesktop.SetItemText(itemCount, 1, desktop_all_info.allow_other_account_hook ? L"��" : L"��");
					item_text.Format(L"%dkb", desktop_all_info.heap_size);
					pDlg->m_listDesktop.SetItemText(itemCount, 2, item_text);
					pDlg->m_listDesktop.SetItemText(itemCount, 3, desktop_all_info.accept_io ? L"��" : L"��");
					pDlg->m_listDesktop.SetItemText(itemCount, 4, desktop_all_info.type);
				}
			}
		}

	} while (FALSE);

	return bContinue;
}

// ö�ٴ���վ�ص�����
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
				// ��ӵ��б�ؼ�
				pDlg->m_listWinSta.InsertItem(itemCount, lpWinStaName);

				WINSTA_ALL_INFO winsta_all_info;
				if (pDlg->m_desktopManage.GetWinStaAllInfo(lpWinStaName, winsta_all_info))
				{
					pDlg->m_listWinSta.SetItemText(itemCount, 1, winsta_all_info.visible ? L"��" : L"��");
					pDlg->m_listWinSta.SetItemText(itemCount, 2, (wchar_t*)winsta_all_info.type);
				}
			}
		}
	} while (FALSE);

	return bContinue;
}

// ˢ�°�ť��ö�ٴ���վ
void Cmulti_desktopDlg::OnBnClickedBtnRefresh()
{
	m_listDesktop.DeleteAllItems();
	m_listWinSta.DeleteAllItems();
	m_desktopManage.EnumWindowStation(EnumWinStationProc, (LPARAM)this);
}


// �л���ť�л�����󣬹رնԻ���Ȼ����App�����½�
void Cmulti_desktopDlg::OnBnClickedBtnSwitch()
{
	if (!m_curSelDesktopName.IsEmpty())
	{
		// �л�����
		if (m_desktopManage.SwitchDesktop(m_curSelWinStaName.GetBuffer(), m_curSelDesktopName.GetString()))
		{
			theApp.m_switchDesktop = TRUE;

			// �����л�������·������������explorer
			theApp.m_switchDesktopName = m_curSelWinStaName + L"\\" + m_curSelDesktopName;

			PostMessage(WM_CLOSE);
		}
	}
}


// ����վ�б����¼�����ȡ����վ����
void Cmulti_desktopDlg::OnItemchangedListWinsta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_curSelWinStaName = m_listWinSta.GetItemText(pNMLV->iItem, 0);

	m_curSelDesktopName = L"";
	m_listDesktop.DeleteAllItems();

	// ö������
	m_desktopManage.EnumDestkop(m_curSelWinStaName.GetBuffer(), EnumDesktopProc, (LPARAM)this);
}


// �������棬�ӱ༭����ȡ����������
void Cmulti_desktopDlg::OnBnClickedBtnCreate()
{
	CString strDesktopName;
	GetDlgItemTextW(IDC_EDIT_DESKTOP_NAME, strDesktopName);
	if (strDesktopName.IsEmpty())
	{
		return;
	}

	// ��������
	if (m_desktopManage.CreateDesktop(m_curSelWinStaName.GetBuffer(), strDesktopName.GetBuffer()))
	{
		// ���б���ö�ٴ���
		m_listDesktop.DeleteAllItems();
		m_desktopManage.EnumDestkop(m_curSelWinStaName.GetBuffer(), EnumDesktopProc, (LPARAM)this);
	}
}



void Cmulti_desktopDlg::OnItemchangedListDesktop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_curSelDesktopName = m_listDesktop.GetItemText(pNMLV->iItem, 0);
}


BOOL Cmulti_desktopDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���

	return CDialogEx::PreTranslateMessage(pMsg);
}
