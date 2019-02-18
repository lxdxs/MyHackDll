#pragma once


//控制台调试信息输出文件句柄
FILE *g_Stream;
//游戏Present函数地址
GD3D11Present oPresent;
//游戏DrawIndexed函数地址
GD3D11DrawIndexed oDrawIndexed;


//声明函数
HRESULT __stdcall HKD3D11Present(IDXGISwapChain * This, UINT SyncInterval, UINT Flags);

VOID __stdcall HKD3D11DrawIndexed(ID3D11DeviceContext * pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

//D3D11参数
IDXGISwapChain * pSwapChain;
ID3D11Device*  pDevice;
ID3D11DeviceContext * pContext;
//index
ID3D11Buffer *inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;
//是否第一次运行


//定义eDepthState
enum eDepthState
{
	ENABLED,
	DISABLED,
	READ_NO_WRITE,
	NO_READ_NO_WRITE,
	_DEPTH_COUNT
};


//定义myDepthStencilStates
ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

//上色用的
ID3D11PixelShader* psRed = NULL;
ID3D11PixelShader* psGreen = NULL;

#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }



//init only once
BOOL firstTime = TRUE;

BOOL bShowWindow = TRUE;

//透视模型数据标识
int  g_iStride = 0;
int  g_inByteWidth = 0;
int  g_veByteWidth = 0;

//透视(开启/关闭)标识
BOOL g_bClose_ZEnable = false;  
//上色(开启/关闭)标识
bool g_bColouring = false;

int test_value = 0;


//vertex
ID3D11Buffer *veBuffer;
UINT Stride = 0;
UINT veBufferOffset = 0;
D3D11_BUFFER_DESC vedesc;

//人物判断
#define Player ( (Stride == 40)&&( indesc.ByteWidth == 1146))

