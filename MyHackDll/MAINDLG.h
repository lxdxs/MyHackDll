#pragma once

#include <d3d11.h>

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

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();

public:
	//控制台调试信息输出文件句柄
	FILE *g_Stream;
	//窗口句柄
	HWND hWindow;
	//D3D11参数
	ID3D11Device*  pDevice;
	ID3D11DeviceContext * pContext;
	IDXGISwapChain * pSwapChain;
};
