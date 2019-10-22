#include "stdafx.h"
#include "DesktopManage.h"


CDesktopManage::CDesktopManage()
{
}


CDesktopManage::~CDesktopManage()
{
}

// 枚举窗口站里的桌面
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

		// 打开窗口站，调用枚举函数进行枚举
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


// 枚举当前会话有哪些窗口站
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


// 切换到指定窗口站的桌面
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

		// 如果指定了窗口站的话，那么需要切换窗口站
		if (!station_name.empty())
		{
			station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS);
			if (station_handle)
			{
				// 获取当前的窗口站，然后设置新的窗口站
				original_station_handle = ::GetProcessWindowStation();
				::SetProcessWindowStation(station_handle);
				CloseWindowStation(station_handle);
			}
		}
		if ((desktop_handle = ::OpenDesktopW(desktop_name.c_str(), NULL, FALSE, GENERIC_ALL)) == NULL)
		{
			break;
		}
		
		// 调用函数切换桌面
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
		// 恢复窗口站
		::SetProcessWindowStation(original_station_handle);
		CloseWindowStation(original_station_handle);
	}

	return success;
}

// 在指定的窗口站创建桌面
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

		// 如果指定了窗口站的话，那么需要切换窗口站
		if (!station_name.empty())
		{
			if (station_handle = ::OpenWindowStationW(station_name.c_str(), FALSE, WINSTA_ALL_ACCESS))
			{
				original_station_handle = ::GetProcessWindowStation();
				::SetProcessWindowStation(station_handle);
				CloseWindowStation(station_handle);
			}
		}

		// 调用函数创建桌面
		if (::CreateDesktop(desktop_name.c_str(), NULL, NULL, NULL, GENERIC_ALL, NULL) == NULL)
		{
			break;
		}

		success = true;
	} while (false);
	if (original_station_handle)
	{
		// 恢复窗口站
		::SetProcessWindowStation(original_station_handle);
		CloseWindowStation(original_station_handle);
	}

	return success;
}


// 获取桌面或窗口站对象的属性
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

		// 先获取需要的内存大小
		DWORD length_needed = 0;
		::GetUserObjectInformationW(obj_handle, index, NULL, 0, &length_needed);
		if (length_needed <= 0)
		{
			success = true;
			break;
		}
		
		// 在调用一次获取内容
		info_length = length_needed;
		*info = new char[length_needed + 1];
		ZeroMemory(*info, length_needed + 1);
		::GetUserObjectInformationW(obj_handle, index, *info, length_needed, &length_needed);

		success = true;
	} while (false);

	return success;
}


// 查询窗口站对象信息
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

		// 查询信息
		success = GetOjbectInfo(station_handle, index, info, info_length);
	} while (false);

	if (station_handle)
	{
		CloseWindowStation(station_handle);
	}

	return success;
}


// 查询窗口站对象信息
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

		// 多次调用 GetObjectInfo 获取信息
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

// 查询桌面对象信息
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

		// 查询信息
		success = GetOjbectInfo(desktop_handle, index, info, info_length);
	} while (false);

	if (desktop_handle)
	{
		CloseDesktop(desktop_handle);
	}

	return success;
}


// 获取桌面的所有信息
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

		// 多次调用 GetObjectInfo 获取信息
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

		// UOI_HEAPSIZE UOI_IO vista及以后才有效
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