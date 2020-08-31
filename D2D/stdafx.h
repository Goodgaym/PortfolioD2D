#pragma once
//Window
#include <Windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <bitset>
#include <list>
#include <map>
#include <functional>
#include <mutex>

using namespace std;

//DirectX Library
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Fmod
#include <FMOD/fmod.hpp>
#pragma comment(lib, "FMOD/fmodex_vc.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//Macro
#define SAFE_DELETE(p) { if(p) {delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) { if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p) { if(p) {(p)->Release(); (p) = NULL;}}

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX Matrix;
typedef D3DXCOLOR Color;
typedef D3DXQUATERNION Quaternion;

//FrameWork
#include "Systems/Time.h"
#include "Systems/Keyboard.h"
#include "Systems/DirectWrite.h"
#include "Systems/AudioSystem.h"

#include "Renders/Shader.h"
#include "Renders/Sprite.h"
#include "Renders/Clip.h"
#include "Renders/Animation.h"

#include "Collider/Collider.h"

#include "Utilities/Math.h"
#include "Utilities/String.h"

#define BALL_SIZE_X	64
#define BALL_SIZE_Y 52

#define LIMIT_ANGLE 80.0f
#define LAUNCHER_ANGLE 60.0f

//Extern Global
extern UINT Width;
extern UINT Height;

extern HWND Hwnd;
extern wstring Title;

//DX Interface
// DXGI(DirectX Graphic Infrastructure) 그래픽스에 관련한 기초적인 기능 제공
// SwapChain 버퍼 포인터가 서로 교체되면서 화면을 구성
extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device; // PC 자원 생성, CPU와 RAM의 영역 기능, 지원 점검과 자원 할당
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;	// 실제 도화지 RTV -> HDC 의 역할

extern Keyboard* Key;
extern AudioSystem* Audio;