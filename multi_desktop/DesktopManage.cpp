#include "stdafx.h"
#include "DesktopManage.h"


CDesktopManage::CDesktopManage()
{
}


CDesktopManage::~CDesktopManage()
{
}

// ö�ٴ���վ�������
bool CDesktopManage::EnumDestkop(wstring station_name, DESKTOPENUMPROCW desktop_callback, LPARAM desktop_callback_ctx)
{
	bool success = false;
	HWINSTA station_handle = NULL;

	do
	{
		if (station_name.empty() || desktop_callback == NULL)
		{
			break;
		}

		// �򿪴���վ������ö�ٺ�������ö��
		if ((station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS)) == NULL)
		{
			break;
		}
		::EnumDesktopsW(station_handle, desktop_callback, desktop_callback_ctx);

		success = true;
	} while (false);

	if (station_handle)
	{
		CloseWindowStation(station_handle);
	}
	return success;
}


// ö�ٵ�ǰ�Ự����Щ����վ
bool CDesktopManage::EnumWindowStation(WINSTAENUMPROCW winsta_callback, LPARAM winsta_callback_ctx)
{
	bool success = false;

	do
	{
		if (winsta_callback == NULL)
		{
			break;
		}
		::EnumWindowStationsW(winsta_callback, winsta_callback_ctx);

		success = true;
	} while (false);

	return success;
}


// �л���ָ������վ������
bool CDesktopManage::SwitchDesktop(wstring station_name, wstring desktop_name)
{
	bool success = false;

	HWINSTA station_handle = NULL, original_station_handle = NULL;
	HDESK desktop_handle = NULL;

	do
	{
		if (desktop_name.empty())
		{
			break;
		}

		// ���ָ���˴���վ�Ļ�����ô��Ҫ�л�����վ
		if (!station_name.empty())
		{
			station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS);
			if (station_handle)
			{
				// ��ȡ��ǰ�Ĵ���վ��Ȼ�������µĴ���վ
				original_station_handle = ::GetProcessWindowStation();
				::SetProcessWindowStation(station_handle);
				CloseWindowStation(station_handle);
			}
		}
		if ((desktop_handle = ::OpenDesktopW(desktop_name.c_str(), NULL, FALSE, GENERIC_ALL)) == NULL)
		{
			break;
		}
		
		// ���ú����л�����
		if (!::SwitchDesktop(desktop_handle))
		{
			break;
		}
		
		success = true;
	} while (false);

	if (desktop_handle)
	{
		CloseDesktop(desktop_handle);
	}

	if (original_station_handle)
	{
		// �ָ�����վ
		::SetProcessWindowStation(original_station_handle);
		CloseWindowStation(original_station_handle);
	}

	return success;
}

// ��ָ���Ĵ���վ��������
bool CDesktopManage::CreateDesktop(wstring station_name, wstring desktop_name)
{
	bool success = false;

	HWINSTA station_handle = NULL, original_station_handle = NULL;

	do
	{
		if (desktop_name.empty())
		{
			break;
		}

		// ���ָ���˴���վ�Ļ�����ô��Ҫ�л�����վ
		if (!station_name.empty())
		{
			if (station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS))
			{
				original_station_handle = ::GetProcessWindowStation();
				::SetProcessWindowStation(station_handle);
				CloseWindowStation(station_handle);
			}
		}

		// ���ú�����������
		if (::CreateDesktop(desktop_name.c_str(), NULL, NULL, NULL, GENERIC_ALL, NULL) == NULL)
		{
			break;
		}

		success = true;
	} while (false);
	if (original_station_handle)
	{
		// �ָ�����վ
		::SetProcessWindowStation(original_station_handle);
		CloseWindowStation(original_station_handle);
	}

	return success;
}


// ��ȡ����򴰿�վ���������
bool CDesktopManage::GetOjbectInfo(HANDLE obj_handle, int index, void** info, int &info_length)
{
	bool success = false;

	do
	{
		info_length = 0;
		if (obj_handle == NULL || info == NULL)
		{
			break;
		}
		*info = NULL;

		// �Ȼ�ȡ��Ҫ���ڴ��С
		DWORD length_needed = 0;
		::GetUserObjectInformationW(obj_handle, index, NULL, 0, &length_needed);
		if (length_needed <= 0)
		{
			success = true;
			break;
		}
		
		// �ڵ���һ�λ�ȡ����
		info_length = length_needed;
		*info = new char[length_needed + 1];
		ZeroMemory(*info, length_needed + 1);
		::GetUserObjectInformationW(obj_handle, index, *info, length_needed, &length_needed);

		success = true;
	} while (false);

	return success;
}


// ��ѯ����վ������Ϣ
bool CDesktopManage::GetWinStaInfo(wstring station_name, int index, void** info, int &info_length)
{
	bool success = false;
	HWINSTA station_handle = NULL;

	do
	{
		if (station_name.empty() || info == NULL)
		{
			break;
		}
		if ((station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS)) == NULL)
		{
			break;
		}

		// ��ѯ��Ϣ
		success = GetOjbectInfo(station_handle, index, info, info_length);
	} while (false);

	if (station_handle)
	{
		CloseWindowStation(station_handle);
	}

	return success;
}


// ��ѯ����վ������Ϣ
bool CDesktopManage::GetWinStaAllInfo(wstring station_name, WINSTA_ALL_INFO& winsta_all_info)
{
	bool success = false;
	HWINSTA station_handle = NULL;

	do
	{
		if (station_name.empty())
		{
			break;
		}
		if ((station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS)) == NULL)
		{
			break;
		}

		// ��ε��� GetObjectInfo ��ȡ��Ϣ
		void* info = NULL;
		int info_length = 0;	

		// UOI_FLAGS
		if (!(success = GetOjbectInfo(station_handle, UOI_FLAGS, &info, info_length)))
		{
			break;
		}
		if (info)
		{
			winsta_all_info.inherit = ((PUSEROBJECTFLAGS)info)->fInherit == TRUE;
			winsta_all_info.visible = ((PUSEROBJECTFLAGS)info)->dwFlags == WSF_VISIBLE;
			delete[] info;
		}

		// UOI_NAME
		if (!(success = GetOjbectInfo(station_handle, UOI_NAME, &info, info_length)))
		{
			break;
		}
		if (info)
		{
			winsta_all_info.name = (wchar_t*)info;
		}

		// UOI_TYPE
		if (!(success = GetOjbectInfo(station_handle, UOI_TYPE, &info, info_length)))
		{
			break;
		}
		if (info)
		{
			winsta_all_info.type = (wchar_t*)info;
		}

		// UOI_USER_SID
		if (!(success =GetOjbectInfo(station_handle, UOI_USER_SID, &info, info_length)))
		{
			break;
		}
		if (info)
		{
			memcpy(&winsta_all_info.user_sid, info, sizeof(SID));
			delete[] info;
		}

		success = true;
	} while (false);

	if (station_handle)
	{
		CloseWindowStation(station_handle);
	}

	return success;
}

// ��ѯ���������Ϣ
bool CDesktopManage::GetDesktopInfo(wstring desktop_name, int index, void** info, int &info_length)
{
	bool success = false;
	HDESK desktop_handle = NULL;

	do
	{
		if (desktop_name.empty() || info == NULL)
		{
			break;
		}

		if ((desktop_handle = ::OpenDesktopW(desktop_name.c_str(), 0, FALSE, GENERIC_ALL)) == NULL)
		{
			break;
		}

		// ��ѯ��Ϣ
		success = GetOjbectInfo(desktop_handle, index, info, info_length);
	} while (false);

	if (desktop_handle)
	{
		CloseDesktop(desktop_handle);
	}

	return success;
}


// ��ȡ�����������Ϣ
bool CDesktopManage::GetDesktopAllInfo(wstring desktop_name, DESKTOP_ALL_INFO& desktop_all_info)
{
	bool success = false;
	HDESK desktop_handle = NULL;

	do
	{
		if (desktop_name.empty())
		{
			break;
		}
		if ((desktop_handle = ::OpenDesktopW(desktop_name.c_str(), 0, FALSE, GENERIC_ALL)) == NULL)
		{
			break;
		}

		// ��ε��� GetObjectInfo ��ȡ��Ϣ
		void* info = NULL;
		int info_length = 0;

		// UOI_FLAGS
		if (!(success = GetOjbectInfo(desktop_handle, UOI_FLAGS, &info, info_length)))
		{
			break;
		}
		desktop_all_info.inherit = ((PUSEROBJECTFLAGS)info)->fInherit == TRUE;
		desktop_all_info.allow_other_account_hook = ((PUSEROBJECTFLAGS)info)->dwFlags & DF_ALLOWOTHERACCOUNTHOOK;
		delete[] info;

		// UOI_HEAPSIZE UOI_IO vista���Ժ����Ч
		OSVERSIONINFOW osVersion = { 0 };
		osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
		GetVersionExW(&osVersion);
		if (osVersion.dwMajorVersion > 5)
		{
			// UOI_HEAPSIZE
			if (!(success = GetOjbectInfo(desktop_handle, UOI_HEAPSIZE, &info, info_length)))
			{
				break;
			}
			desktop_all_info.heap_size = *(ULONG*)info;
			delete[] info;

			// UOI_IO
			if (!(success = GetOjbectInfo(desktop_handle, UOI_IO, &info, info_length)))
			{
				break;
			}
			desktop_all_info.accept_io = *(BOOL*)info;
			delete[] info;
		}

		// UOI_NAME
		if (!(success = GetOjbectInfo(desktop_handle, UOI_NAME, &info, info_length)))
		{
			break;
		}
		desktop_all_info.name = (wchar_t*)info;

		// UOI_TYPE
		if (!(success = GetOjbectInfo(desktop_handle, UOI_TYPE, &info, info_length)))
		{
			break;
		}
		desktop_all_info.type = (wchar_t*)info;

		// UOI_USER_SID
		if (!(success = GetOjbectInfo(desktop_handle, UOI_USER_SID, &info, info_length)))
		{
			break;
		}
		if (info)
		{
			memcpy(&desktop_all_info.user_sid, info, sizeof(SID));
			delete[] info;
		}

		success = true;
	} while (false);

	if (desktop_handle)
	{
		CloseDesktop(desktop_handle);
	}

	return success;
}