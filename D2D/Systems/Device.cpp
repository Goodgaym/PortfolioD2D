#include "stdafx.h"
#include "Device.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lPCmdLine,
	int nCmdShow
){
	InitWindow(hInstance, nCmdShow);	// ������ �ʱ�ȭ
	InitDirect3D(hInstance);

	Running();
	Destroy();

	return 0;
}

// extern ���� ����
UINT Width = 320 * 4;
UINT Height = 224 * 4;

HWND Hwnd = NULL;
wstring Title = L"D2D";

IDXGISwapChain* SwapChain = NULL;
ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
ID3D11RenderTargetView* RTV = NULL;

Keyboard* Key = NULL;
CMouse* Mouse = NULL;
AudioSystem* Audio = NULL;

void InitWindow(HINSTANCE hInstance, int nCmdShow) {
	// Register Wnd Class
	{
		WNDCLASSEX wc; // ������ Ŭ���� ����
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;					// ����, ����ũ�� ���ҽ� �ٽ� �׸�
		wc.lpfnWndProc = WndProc;							// ������ �޼��� ó���Լ� ����
		wc.cbClsExtra = NULL;								// ���� ����
		wc.cbWndExtra = NULL;
		wc.hInstance = hInstance;							// ���� ���α׷� �ν��Ͻ�
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// �ּ�ȭ �Ǿ����� ��µ� ������
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// ����� ���콺 Ŀ��
		wc.hbrBackground = static_cast<HBRUSH>(WHITE_BRUSH);// ������ �귯�� ����
		wc.lpszMenuName = NULL;								// ���α׷��� ����� �޴� ����
		wc.lpszClassName = Title.c_str();					//������ Ŭ���� �̸�
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		WORD check = RegisterClassEx(&wc);
		assert(check != NULL);
	}
	// Create Window & Show Window
	{
		Hwnd = CreateWindowEx(	// ������ ����
			NULL,
			Title.c_str(),		// Ÿ��Ʋ Ŭ���� �̸�
			Title.c_str(),		// Ÿ��Ʋ �� �̸�
			WS_OVERLAPPEDWINDOW,// ������ ��Ÿ��
			CW_USEDEFAULT,		// ������ ��ǥ x
			CW_USEDEFAULT,		// ������ ��ǥ y
			Width,				// �ʺ� ũ��
			Height,				// ���� ũ��
			NULL,				// ������ �ڵ� ����
			NULL,				// �޴��� �ڵ� ����
			hInstance,			// �ν��Ͻ� �ڵ� ����
			NULL				// lParam���� ���޵� ����ü�� ������
		);
		assert(Hwnd != NULL);
	}

	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
}

void InitDirect3D(HINSTANCE hInstance) {
	// SwapChain
	DXGI_MODE_DESC bufferDesc;											// �ĸ���� �Ӽ� ����ü
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;								// ȭ�� ��� ��
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// ���÷��� ��ĵ���� ���
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// �ȼ� ����(UNORM : 0~1������ ��)

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;								// �ĸ� ���� ����
	swapDesc.BufferDesc = bufferDesc;						// �ĸ� ���� �Ӽ�
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ������ �뵵 (ǥ��, �ڿ��� ��� ���� Ÿ������ ����)
	swapDesc.SampleDesc.Count = 1;							// ��Ƽ ���ø��� ���� ������ ǥ���� ������ ǰ�� ����
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.OutputWindow = Hwnd;							// ������ ����� ����� ������
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// ������ �׷ȴ� �� ���

	// Featured Level
	vector<D3D_FEATURE_LEVEL> featured_level =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create Device and SwapChain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,	// 2D�� �����Ҷ� ���� �÷���
		featured_level.data(),
		featured_level.size(),
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		NULL,
		&DeviceContext
	);
	assert(SUCCEEDED(hr));

	CreateBackBuffer();
}

void Destroy() {
	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);

	DeleteBackBuffer();
}

WPARAM Running() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Init Singleton
	ImGui::Create(Hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark();

	DirectWrite::Create();

	Key = new Keyboard();
	Mouse = new CMouse(Hwnd);
	Audio = new AudioSystem();

	Time::Create();
	Time::Get()->Start();

	InitScene();

	while (true) {
		// �޼����� ���� �ִ��� �˻� �� ����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);	// �ؼ�
			DispatchMessage(&msg);	// ����
		}
		else {
			Time::Get()->Update();
			Update();
			Mouse->Update();
			Audio->Update();
			ImGui::Update();
			Render();
		}
	}

	DestroyScene();
	
	// Delete Singleton
	SAFE_DELETE(Key);
	SAFE_DELETE(Mouse);
	SAFE_DELETE(Audio);

	Time::Delete();
	ImGui::Delete();
	DirectWrite::Delete();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui::WndProc((UINT*)Hwnd, msg, wParam, lParam))
		return true;

	if (Mouse != NULL)
		Mouse->WndProc(msg, wParam, lParam);

	switch (msg) {
	case WM_SIZE: {
		if (Device != NULL) {

			Width = LOWORD(lParam);
			Height = HIWORD(lParam);

			DirectWrite::DeleteBackBuffer();
			DeleteBackBuffer();

			ImGui::Invalidate();

			HRESULT hr = SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
			assert(SUCCEEDED(hr));

			DirectWrite::CreateBackBuffer();
			CreateBackBuffer();

			ImGui::Validate();
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void CreateBackBuffer() {
	//Get BackBuffer
	ID3D11Texture2D* BackBuffer;
	HRESULT hr = SwapChain->GetBuffer(
		0,							// ������� ��ȣ
		__uuidof(ID3D11Texture2D),	// ����ۿ� �����ϴ� �������̽�
		(void**)&BackBuffer			// ����۸� �޾ƿ� ����
	);	
	assert(SUCCEEDED(hr));

	//Create RTV
	hr = Device->CreateRenderTargetView(
		BackBuffer,	// �信�� �������ϴ� ���ҽ�
		NULL,		// ���� Ÿ�� �� ����
		&RTV		// ���� Ÿ�� �並 �޾ƿ� ����
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);

	//OM Set
	DeviceContext->OMSetRenderTargets(
		1,		// ���� Ÿ���� ��
		&RTV,	// ���� Ÿ�� ���� �迭
		NULL	// ����/���ٽ� ����
	);

	//Create Viewport
	{
		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = (float)Width;
		viewPort.Height = (float)Height;

		DeviceContext->RSSetViewports(1, &viewPort);
	}
}

void DeleteBackBuffer() {
	SAFE_RELEASE(RTV);
}
