// MyHackDll.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MyHackDll.h"
#include "MAINDLG.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMyHackDllApp

BEGIN_MESSAGE_MAP(CMyHackDllApp, CWinApp)
END_MESSAGE_MAP()


// CMyHackDllApp 构造

CMyHackDllApp::CMyHackDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMyHackDllApp 对象

CMyHackDllApp theApp;

HANDLE g_hMainThread;
HANDLE g_hFreeThread;
// CMyHackDllApp 初始化

VOID UIThread(VOID* Param)
{
	//对话框资源切换（dll工作在游戏内部，游戏内部没有这个对话框资源）

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//保存旧的句柄
	HINSTANCE hResOld;

	hResOld = AfxGetResourceHandle();

	//创建界面

	CMAINDLG* MainDlg = new CMAINDLG;

	MainDlg->DoModal();
	
	//关闭窗口

	g_hFreeThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, theApp.m_hInstance, 0, 0);

	//释放界面线程和句柄
	CloseHandle(g_hFreeThread);
	CloseHandle(g_hMainThread);

	//释放类
	delete MainDlg;
	MainDlg = NULL;

	//将资源切换回去
	AfxSetResourceHandle(hResOld);

}

BOOL CMyHackDllApp::InitInstance()
{
	CWinApp::InitInstance();
	//创建一个线程(防止DLL运行时,游戏卡死)
	g_hMainThread = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)UIThread, NULL, 0, 0);

	
	return TRUE;
}
