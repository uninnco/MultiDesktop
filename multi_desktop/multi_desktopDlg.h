
// multi_desktopDlg.h : ͷ�ļ�
//

#pragma once
#include "DesktopManage.h"
#include "afxcmn.h"


// Cmulti_desktopDlg �Ի���
class Cmulti_desktopDlg : public CDialogEx
{
// ����
public:
	Cmulti_desktopDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTI_DESKTOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ö������ص�����
	static BOOL CALLBACK EnumDesktopProc(LPWSTR lpDesktopName, LPARAM lpParam);

	// ö�ٴ���վ�ص�����
	static BOOL CALLBACK EnumWinStationProc(LPWSTR lpWinStaName, LPARAM lpParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	CListCtrl m_listWinSta;			// ����վ�б�
	CListCtrl m_listDesktop;		// �����б�

	afx_msg void OnItemchangedListWinsta(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCreate();
	afx_msg void OnItemchangedListDesktop(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
