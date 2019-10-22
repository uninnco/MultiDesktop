
// multi_desktopDlg.h : 头文件
//

#pragma once
#include "DesktopManage.h"
#include "afxcmn.h"


// Cmulti_desktopDlg 对话框
class Cmulti_desktopDlg : public CDialogEx
{
// 构造
public:
	Cmulti_desktopDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MULTI_DESKTOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 枚举桌面回调函数
	static BOOL CALLBACK EnumDesktopProc(LPWSTR lpDesktopName, LPARAM lpParam);

	// 枚举窗口站回调函数
	static BOOL CALLBACK EnumWinStationProc(LPWSTR lpWinStaName, LPARAM lpParam);

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
	CDesktopManage m_desktopManage;
	CString m_curSelWinStaName;
	CString m_curSelDesktopName;

public:
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedBtnSwitch();

	CListCtrl m_listWinSta;			// 窗口站列表
	CListCtrl m_listDesktop;		// 桌面列表

	afx_msg void OnItemchangedListWinsta(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCreate();
	afx_msg void OnItemchangedListDesktop(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
