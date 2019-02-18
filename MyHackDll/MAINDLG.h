#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "detours.h"

//游戏D3D11Present函数定义
typedef HRESULT(__stdcall *GD3D11Present) (IDXGISwapChain* This, UINT SyncInterval, UINT Flags);
typedef	VOID(__stdcall *GD3D11DrawIndexed) (ID3D11DeviceContext * pContext ,UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation );


// CMAINDLG 对话框

class CMAINDLG : public CDialog
{
	DECLARE_DYNAMIC(CMAINDLG)

public:
	CMAINDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMAINDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
public:

	//窗口句柄
	HWND hWindow;

	//创建D3D交换链
	DXGI_SWAP_CHAIN_DESC scd;
	
	//获取d3d交换链值
	DWORD_PTR* pDeviceVT;
	DWORD_PTR* pContextVT;
	DWORD_PTR* pSwapChainVT;

	//判断是否执行成功
	HRESULT sz_Info;

	
	afx_msg void OnBnClickedCheck4();
};
