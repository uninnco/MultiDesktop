/*
	����ʹ���վ����������
	1.ö�ٴ���վ������
	2.��������
	3.��ȡ����վ�����������
*/
#pragma once
#include <string>

using std::wstring;

// ����վ������
typedef struct _WINSTA_ALL_INFO
{
	bool inherit;	// �Ƿ�ɱ��ӽ��̼̳�
	bool visible;	// �Ƿ�ɼ�
	wchar_t* name;	// ����
	wchar_t* type;	// ���ͣ�����WindowStation��
	SID user_sid;	// �����Ự��SID

	_WINSTA_ALL_INFO()
	{
		ZeroMemory(this, sizeof(_WINSTA_ALL_INFO));
	}
	~_WINSTA_ALL_INFO()
	{
		if (name)
		{
			delete[] name;
		}

		if (type)
		{
			delete[] type;
		}
	}
}WINSTA_ALL_INFO, *PWINSTA_ALL_INFO;

// ���������
typedef struct _DESKTOP_ALL_INFO
{
	bool inherit;					// �Ƿ�ɱ��ӽ��̼̳�
	bool allow_other_account_hook;	// �Ƿ����������˻�hook
	bool accept_io;					// �Ƿ���Խ����û����룬������ʾ�����û�����
	ULONG heap_size;				// ����Ѵ�С
	wchar_t* name;					// ����
	wchar_t* type;					// ���ͣ�����Desktop��
	SID user_sid;					// �����Ự��SID

	_DESKTOP_ALL_INFO()
	{
		ZeroMemory(this, sizeof(DESKTOP_ALL_INFO));
	}
	~_DESKTOP_ALL_INFO()
	{
		if (name)
		{
			delete[] name;
		}
		if (type)
		{
			delete[] type;
		}
	}

}DESKTOP_ALL_INFO, *PDESKTOP_ALL_INFO;

class CDesktopManage
{
public:
	CDesktopManage();
	~CDesktopManage();

	/*
		brief: ö�ٴ���վ�������
		@param_1: ����վ����
		@param_2: ö�ٻص�����
		@param_3: �ص�������

		return: true ��ʾ�ɹ�, false ��ʾʧ��
	*/
	bool EnumDestkop(wstring station_name, DESKTOPENUMPROCW desktop_callback, LPARAM desktop_callback_ctx);

	/*
		brief: ö�ٵ�ǰ�Ự����Щ����վ
		@param_1: ����վ�ص�����
		@param_2: �ص�������

		return:  true ��ʾ�ɹ�, false ��ʾʧ��
	*/
	bool EnumWindowStation(WINSTAENUMPROCW winsta_callback, LPARAM winsta_callback_ctx);

	/*
		brief: �л���ָ������վ������
		@param_1: ����վ����
		@param_2: ��������

		remarks: ��������ڲ�ͬ�Ĵ���վ�У����л�����һ������վ�������У�
			����Ҫָ������վ

		return: true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool SwitchDesktop(wstring station_name, wstring desktop_name);

	/*
		brief: ��ָ���Ĵ���վ��������
		@param_1: ����վ����
		@param_2: ��������

		remarks: ��������ڲ�ͬ�Ĵ���վ�У���Ҫ����һ������վ�д������棬
		����Ҫָ������վ

		return: true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool CreateDesktop(wstring station_name, wstring desktop_name);

	/*
		brief: ��ȡ����򴰿�վ���������
		@param_1: ������
		@param_2: ����ֵ
		@param_3: �����ڴ�
		@param_4: ���Գ���

		return: true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool GetOjbectInfo(HANDLE obj_handle, int index, void** info, int &info_length);

	/*
		brief: ��ѯ����վ������Ϣ
		@param_1: ����վ����
		@param_2: ��ѯ��Ϣ�ı�־ UOI_FLAGS UOI_NAME UOI_TYPE UOI_USER_SID
		@param_3: ��ѯ��Ϣ���ڴ� ��Ҫ ʹ�� delete[] �ͷ�
		@param_4: ��Ϣ�ĳ���

		return true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool GetWinStaInfo(wstring station_name, int index, void** info, int &info_length);

	/*
		brief: ��ȡ����վ��������Ϣ
		@param_1: ����վ������
		@param_2: ����վ���Ե����ã�����������Ϣ

		return true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool GetWinStaAllInfo(wstring station_name, WINSTA_ALL_INFO& winsta_all_info);

	/*
		brief: ��ѯ���������Ϣ
		@param_1: ��������
		@param_2: ��ѯ��Ϣ�ı�־	UOI_FLAGS UOI_HEAPSIZE UOI_IO UOI_NAME UOI_TYPE UOI_USER_SID
			��UOI_HEAPSIZE UOI_IO��vista���Ժ����Ч
		@param_3: ��ѯ��Ϣ���ڴ� ��Ҫ ʹ�� delete[] �ͷ�
		@param_4: ��Ϣ�ĳ���

		return true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool GetDesktopInfo(wstring desktop_name, int index, void** info, int &info_length);

	/*
		brief: ��ȡ�����������Ϣ
		@param_1: ���������
		@param_2: �������Ե����ã�����������Ϣ

		return true ��ʾ�ɹ��� false ��ʾʧ��
	*/
	bool GetDesktopAllInfo(wstring desktop_name, DESKTOP_ALL_INFO& desktop_all_info);

};
