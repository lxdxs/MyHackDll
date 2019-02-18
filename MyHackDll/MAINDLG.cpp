// MAINDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "MyHackDll.h"
#include "MAINDLG.h"
#include "afxdialogex.h"
#include "Info.h"


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
	ON_BN_CLICKED(IDCANCEL, &CMAINDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK3, &CMAINDLG::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMAINDLG::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CMAINDLG 消息处理程序


void CMAINDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT1, L"我的辅助");

}

//Console调用
void CMAINDLG::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码

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
		fprintf(g_Stream, "oDrawIndexed:%X\n", (UINT)oDrawIndexed);
		
	}
		
	
	//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		fclose(g_Stream);
		FreeConsole();

	}
}

//HOOK选项
void CMAINDLG::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	

		//判断是否选中；
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//HOOK D3D11Present
		DetourAttach((PVOID*)&oPresent, (PVOID)HKD3D11Present);
		//HOOK DrawIndexed
		DetourAttach((PVOID*)&oDrawIndexed, (PVOID)HKD3D11DrawIndexed);

		DetourTransactionCommit();
	}

 

		//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())

	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//UNHOOK D3D11Present
		DetourDetach((PVOID*)&oPresent, (PVOID)HKD3D11Present);
		//UNHOOK DrawIndexed
		DetourDetach((PVOID*)&oDrawIndexed, (PVOID)HKD3D11DrawIndexed);

		DetourTransactionCommit();

	}
}


//程序关闭
void CMAINDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

//程序初始化
BOOL CMAINDLG::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// TODO:  在此添加额外的初始化

	SetDlgItemText(IDC_EDIT1, L"程序开始初始化");


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

	sz_Info = D3D11CreateDeviceAndSwapChain(
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

	//获取oPresent
	pSwapChainVT = (DWORD_PTR*)(pSwapChainVT[0]);
	oPresent = (GD3D11Present)pSwapChainVT[8];
	//获取DrawIndexed
	pContextVT = (DWORD_PTR*)(pContextVT[0]);
	oDrawIndexed = (GD3D11DrawIndexed)(pContextVT[12]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
{
	char szCast[] = "struct VS_OUT"
		"{"
		" float4 Position : SV_Position;"
		" float4 Color : COLOR0;"
		"};"

		"float4 main( VS_OUT input ) : SV_Target"
		"{"
		" float4 fake;"
		" fake.a = 1.0f;"
		" fake.r = %f;"
		" fake.g = %f;"
		" fake.b = %f;"
		" return fake;"
		"}";
	ID3D10Blob* pBlob;
	char szPixelShader[1000];

	sprintf_s(szPixelShader,sizeof(szPixelShader), szCast, r, g, b);

	ID3DBlob* d3dErrorMsgBlob;

	HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &d3dErrorMsgBlob);

	if (FAILED(hr))
		return hr;

	hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

void SetDepthStencilState(eDepthState aState)
{
	pContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
}

HRESULT __stdcall HKD3D11Present(IDXGISwapChain * This, UINT SyncInterval, UINT Flags)
{
	if (firstTime)
	{

		firstTime = FALSE;

		//--------------------------------------------
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//ENABLED
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);

		//DISABLED
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);

		//NO_READ_NO_WRITE
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

		//READ_NO_WRITE
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);

		//红色
		if (!psRed)
			GenerateShader(pDevice, &psRed, 1.0f, 0.0f, 0.0f);

		//绿色
		if (!psGreen)
			GenerateShader(pDevice, &psGreen, 0.0f, 1.0f, 0.0f);

	}

	return oPresent(This, SyncInterval, Flags);
}

VOID __stdcall HKD3D11DrawIndexed(ID3D11DeviceContext * pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	

	


	if (!psRed || !psGreen || !pContext)
	{
		return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}
	else
	{

		//  get stride   get vdesc.ByteWidth
		pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
		if (veBuffer)
		{
			veBuffer->GetDesc(&vedesc);
		}
		if (veBuffer != NULL) 
		{ 
			veBuffer->Release(); 
			veBuffer = NULL;
		}
		else
		{
			return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		}
			
		//get indesc.ByteWidth
		pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
		if (inBuffer)
		{
			inBuffer->GetDesc(&indesc);
		}

		if (inBuffer != NULL) 
		{ 
			inBuffer->Release(); 
			inBuffer = NULL; 
		}
		else
		{
			return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		}

		//--------测试用-------------
		if (GetAsyncKeyState('O') & 1) //-
		{
			g_iStride = g_iStride - 1;
		}
		else if (GetAsyncKeyState('P') & 1) //+
		{
			g_iStride = g_iStride + 1;
		}
		else if (GetAsyncKeyState('U') & 1) //-
		{
			g_inByteWidth = g_inByteWidth - 10;
		}
		else if (GetAsyncKeyState('I') & 1) //+
		{
			g_inByteWidth = g_inByteWidth + 10;
		}
		else if (GetAsyncKeyState('T') & 1) //-
		{
			g_veByteWidth = g_veByteWidth - 10;
		}
		else if (GetAsyncKeyState('Y') & 1) //+
		{
			g_veByteWidth = g_veByteWidth + 10;
		}
		else if (GetAsyncKeyState('N') & 1) // == 0
		{
			int  g_iStride = 0;
			int  g_inByteWidth = 0;
			int  g_veByteWidth = 0;
			fprintf(g_Stream, "清理数据");
		}
		else if (GetAsyncKeyState('M') & 1) // == 0
		{
			fprintf(g_Stream,"Stride:%d	vedesc.ByteWidth:%d	indesc.ByteWidth:%d\n", g_iStride, g_veByteWidth, g_inByteWidth);
		}

		//wallhack
		if (Player|| (Stride== g_iStride))
		{
			if (g_bClose_ZEnable)
				SetDepthStencilState(DISABLED);

			if (g_bColouring)
				pContext->PSSetShader(psRed, NULL, NULL);

			if (g_bClose_ZEnable)
				oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

			if (g_bClose_ZEnable)
				SetDepthStencilState(ENABLED);

			if (g_bColouring)
				pContext->PSSetShader(psGreen, NULL, NULL);
		}

	}
	return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}


void CMAINDLG::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码

	//判断是否选中；
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		g_bColouring = TRUE;
	}



	//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		g_bColouring = FALSE;
	}

}


void CMAINDLG::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码


		//判断是否选中；
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		g_bClose_ZEnable = TRUE;
	}



	//判断是否未选中。
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		g_bClose_ZEnable = FALSE;
	}
}
