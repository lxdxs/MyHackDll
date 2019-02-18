#pragma once


//����̨������Ϣ����ļ����
FILE *g_Stream;
//��ϷPresent������ַ
GD3D11Present oPresent;
//��ϷDrawIndexed������ַ
GD3D11DrawIndexed oDrawIndexed;


//��������
HRESULT __stdcall HKD3D11Present(IDXGISwapChain * This, UINT SyncInterval, UINT Flags);

VOID __stdcall HKD3D11DrawIndexed(ID3D11DeviceContext * pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

//D3D11����
IDXGISwapChain * pSwapChain;
ID3D11Device*  pDevice;
ID3D11DeviceContext * pContext;
//index
ID3D11Buffer *inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;
//�Ƿ��һ������


//����eDepthState
enum eDepthState
{
	ENABLED,
	DISABLED,
	READ_NO_WRITE,
	NO_READ_NO_WRITE,
	_DEPTH_COUNT
};


//����myDepthStencilStates
ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

//��ɫ�õ�
ID3D11PixelShader* psRed = NULL;
ID3D11PixelShader* psGreen = NULL;

#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }



//init only once
BOOL firstTime = TRUE;

BOOL bShowWindow = TRUE;

//͸��ģ�����ݱ�ʶ
int  g_iStride = 0;
int  g_inByteWidth = 0;
int  g_veByteWidth = 0;

//͸��(����/�ر�)��ʶ
BOOL g_bClose_ZEnable = false;  
//��ɫ(����/�ر�)��ʶ
bool g_bColouring = false;

int test_value = 0;


//vertex
ID3D11Buffer *veBuffer;
UINT Stride = 0;
UINT veBufferOffset = 0;
D3D11_BUFFER_DESC vedesc;

//�����ж�
#define Player ( (Stride == 40)&&( indesc.ByteWidth == 1146))

