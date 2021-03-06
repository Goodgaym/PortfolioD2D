#pragma once

class DirectWrite {
private:
	DirectWrite();
	~DirectWrite();

private:
	static DirectWrite* instance;

public:
	static void Create();
	static void Delete();

	static DirectWrite* Get();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }

	static void CreateBackBuffer();
	static void DeleteBackBuffer();

	static void RenderText(wstring& text, RECT& rect);
private:
	ID2D1Factory1* factory;
	static IDWriteFactory* writeFactory;

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;

	static ID2D1Bitmap1* bitmap; // RTV에 해당하는 공간
	static IDXGISurface* surface; // 백버퍼가 그려질 공간

	static ID2D1SolidColorBrush* brush;
	static IDWriteTextFormat* format;
};