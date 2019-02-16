﻿// MAINDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "MyHackDll.h"
#include "MAINDLG.h"
#include "afxdialogex.h"



//游戏Present函数地址
GD3D11Present oPresent = NULL;
//控制台调试信息输出文件句柄
FILE *g_Stream = NULL;


// CMAINDLG 对话框

IMPLEMENT_DYNAMIC(CMAINDLG, CDialog)

CMAINDLG::CMAINDLG(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINDLG, pParent)
{



}

CMAINDLG::~CMAINDLG()
{
}

void CMAINDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMAINDLG, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMAINDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CMAINDLG::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMAINDLG::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CMAINDLG 消息处理程序


void CMAINDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT1, L"我的辅助");

}


void CMAINDLG::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	

	InitDialog();


	//判断是否选中；
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		
		AllocConsole();
		freopen_s(&g_Stream,"CON", "w", stdout);
		SetConsoleTitle(L"调试信息输出");
		hWindow =::FindWindow(NULL,L"Soul at Stake (64-bit, PCD3D_SM5)");
		fprintf(g_Stream, "HWND:0x%X\n", (UINT)hWindow);
		fprintf(g_Stream, "pSwapChain:%X\n", (UINT)pSwapChain);
		fprintf(g_Stream, "pDevice:%X\n", (UINT)pDevice);
		fprintf(g_Stream, "pContext:%X\n", (UINT)pContext);
		fprintf(g_Stream, "oPresent:%X\n", (UINT)oPresent);

	}
		
	
	//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		fclose(g_Stream);
		FreeConsole();

	}



}


//初始化
VOID CMAINDLG::InitDialog()
{
	
	if (oPresent == NULL)
	{

		//交换链
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));		//填充0

		scd.BufferCount = 1;								//我们只创建一个后缓冲（双缓冲）因此为1
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //设置颜色格式,我们使用RGBA
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  //缩放比
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //扫描线
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //渲染目标输出
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //允许模式切换
		scd.OutputWindow = hWindow;  //在游戏窗体内部绘制另外一个窗口
		scd.SampleDesc.Count = 1;                      //1重采样
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;      //常用参数
		scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;  //是否全屏
		scd.BufferDesc.Width = 1680;
		scd.BufferDesc.Height = 1050;
		scd.BufferDesc.RefreshRate.Numerator = 144;     //刷新率
		scd.BufferDesc.RefreshRate.Denominator = 1;     //分母
		scd.SampleDesc.Quality = 0;                     //采样等级

		D3D_FEATURE_LEVEL featrueLevel = D3D_FEATURE_LEVEL_11_0;

		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL, &featrueLevel,
			1,
			D3D11_SDK_VERSION,
			&scd,
			&pSwapChain,
			&pDevice,
			NULL,
			&pContext
		);

		//保存值
		pSwapChainVT = (DWORD_PTR*)pSwapChain;
		pDeviceVT = (DWORD_PTR*)pDevice;
		pContextVT = (DWORD_PTR*)pContext;
		pSwapChainVT = (DWORD_PTR*)(pSwapChainVT[0]);
		oPresent = (GD3D11Present)pSwapChainVT [8];


	}
	
}

HRESULT __stdcall HKD3D11Present(IDXGISwapChain * This, UINT SyncInterval, UINT Flags)
{

	fprintf(g_Stream, "111111111111111111\n");

	return oPresent(This, SyncInterval, Flags);
}



void CMAINDLG::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码



	InitDialog();
	

		//判断是否选中；
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		
		DetourAttach((PVOID*)&oPresent, (PVOID)HKD3D11Present);
		
		DetourTransactionCommit();
	}

 

		//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())

	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		
		DetourDetach((PVOID*)&oPresent, (PVOID)HKD3D11Present);
		
		DetourTransactionCommit();

	}
}



