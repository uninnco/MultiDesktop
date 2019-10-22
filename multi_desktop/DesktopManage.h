/*
	桌面和窗口站基本管理功能
	1.枚举窗口站和桌面
	2.创建桌面
	3.获取窗口站和桌面的属性
*/
#pragma once
#include <string>

using std::wstring;

// 窗口站的属性
typedef struct _WINSTA_ALL_INFO
{
	bool inherit;	// 是否可被子进程继承
	bool visible;	// 是否可见
	wchar_t* name;	// 名称
	wchar_t* type;	// 类型（总是WindowStation）
	SID user_sid;	// 所属会话的SID

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

// 桌面的属性
typedef struct _DESKTOP_ALL_INFO
{
	bool inherit;					// 是否可被子进程继承
	bool allow_other_account_hook;	// 是否允许其他账户hook
	bool accept_io;					// 是否可以接收用户输入，可以显示，和用户交互
	ULONG heap_size;				// 桌面堆大小
	wchar_t* name;					// 名称
	wchar_t* type;					// 类型（总是Desktop）
	SID user_sid;					// 所属会话的SID

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
		brief: 枚举窗口站里的桌面
		@param_1: 窗口站名称
		@param_2: 枚举回调函数
		@param_3: 回调上下文

		return: true 表示成功, false 表示失败
	*/
	bool EnumDestkop(wstring station_name, DESKTOPENUMPROCW desktop_callback, LPARAM desktop_callback_ctx);

	/*
		brief: 枚举当前会话有哪些窗口站
		@param_1: 窗口站回调函数
		@param_2: 回到上下文

		return:  true 表示成功, false 表示失败
	*/
	bool EnumWindowStation(WINSTAENUMPROCW winsta_callback, LPARAM winsta_callback_ctx);

	/*
		brief: 切换到指定窗口站的桌面
		@param_1: 窗口站名称
		@param_2: 桌面名称

		remarks: 如果进程在不同的窗口站中，想切换到另一个窗口站的桌面中，
			则需要指定窗口站

		return: true 表示成功， false 表示失败
	*/
	bool SwitchDesktop(wstring station_name, wstring desktop_name);

	/*
		brief: 在指定的窗口站创建桌面
		@param_1: 窗口站名称
		@param_2: 桌面名称

		remarks: 如果进程在不同的窗口站中，想要在另一个窗口站中创建桌面，
		则需要指定窗口站

		return: true 表示成功， false 表示失败
	*/
	bool CreateDesktop(wstring station_name, wstring desktop_name);

	/*
		brief: 获取桌面或窗口站对象的属性
		@param_1: 对象句柄
		@param_2: 属性值
		@param_3: 属性内存
		@param_4: 属性长度

		return: true 表示成功， false 表示失败
	*/
	bool GetOjbectInfo(HANDLE obj_handle, int index, void** info, int &info_length);

	/*
		brief: 查询窗口站对象信息
		@param_1: 窗口站名称
		@param_2: 查询信息的标志 UOI_FLAGS UOI_NAME UOI_TYPE UOI_USER_SID
		@param_3: 查询信息的内存 需要 使用 delete[] 释放
		@param_4: 信息的长度

		return true 表示成功， false 表示失败
	*/
	bool GetWinStaInfo(wstring station_name, int index, void** info, int &info_length);

	/*
		brief: 获取窗口站的所有信息
		@param_1: 窗口站的名称
		@param_2: 窗口站属性的引用，用来返回信息

		return true 表示成功， false 表示失败
	*/
	bool GetWinStaAllInfo(wstring station_name, WINSTA_ALL_INFO& winsta_all_info);

	/*
		brief: 查询桌面对象信息
		@param_1: 桌面名称
		@param_2: 查询信息的标志	UOI_FLAGS UOI_HEAPSIZE UOI_IO UOI_NAME UOI_TYPE UOI_USER_SID
			（UOI_HEAPSIZE UOI_IO）vista及以后才有效
		@param_3: 查询信息的内存 需要 使用 delete[] 释放
		@param_4: 信息的长度

		return true 表示成功， false 表示失败
	*/
	bool GetDesktopInfo(wstring desktop_name, int index, void** info, int &info_length);

	/*
		brief: 获取桌面的所有信息
		@param_1: 桌面的名称
		@param_2: 桌面属性的引用，用来返回信息

		return true 表示成功， false 表示失败
	*/
	bool GetDesktopAllInfo(wstring desktop_name, DESKTOP_ALL_INFO& desktop_all_info);

};
