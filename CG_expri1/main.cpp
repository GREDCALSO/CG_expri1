#include<d2d1.h>
#include<Windows.h>
#include<cmath>
#pragma comment(lib,"d2d1")

ID2D1HwndRenderTarget* pRenderTarget = nullptr;
ID2D1Factory* pFactory = nullptr;
ID2D1SolidColorBrush* pBrush = nullptr;

HRESULT CreateGraphicsResources(HWND hwnd)
{
	HRESULT hr = S_OK;

	if (!pRenderTarget)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			hr = pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black), &pBrush
			);
		}
	}

	return hr;
}

void DiscardGraphicsResources() {
	if (pBrush)
	{
		pBrush->Release(); pBrush = nullptr;
	}
	if (pRenderTarget)
	{
		pRenderTarget->Release(); pRenderTarget = nullptr;
	}
}

void OnPaint(HWND hwnd)
{
	HRESULT hr = CreateGraphicsResources(hwnd);

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	float OFFSET_X = 500.0f;
	float OFFSET_Y = 300.0f;

	D2D1_ROUNDED_RECT inside_rounded_rect = D2D1::RoundedRect(
		D2D1::RectF(-215.0f + OFFSET_X, -150.0f + OFFSET_Y, 215.0f + OFFSET_X, 150.0f + OFFSET_Y),
		30.0f,
		30.0f
	);
	pRenderTarget->DrawRoundedRectangle(inside_rounded_rect, pBrush, 4.0f);

	D2D1_ROUNDED_RECT outside_rounded_rect = D2D1::RoundedRect(
		D2D1::RectF(-330.0f + OFFSET_X, -230.0f + OFFSET_Y, 330.0f + OFFSET_X, 230.0f + OFFSET_Y),
		70.0f,
		70.0f
	);
	pRenderTarget->DrawRoundedRectangle(outside_rounded_rect, pBrush, 4.0f);

	float RADIUS = 35.0f;//СԲ�׵İ뾶
	D2D1_ELLIPSE circle1 = D2D1::Ellipse(D2D1::Point2F(-260.0f + OFFSET_X, -160.0f + OFFSET_Y), RADIUS, RADIUS);
	D2D1_ELLIPSE circle2 = D2D1::Ellipse(D2D1::Point2F(260.0f + OFFSET_X, -160.0f + OFFSET_Y), RADIUS, RADIUS);
	D2D1_ELLIPSE circle3 = D2D1::Ellipse(D2D1::Point2F(-260.0f + OFFSET_X, 160.0f + OFFSET_Y), RADIUS, RADIUS);
	D2D1_ELLIPSE circle4 = D2D1::Ellipse(D2D1::Point2F(260.0f + OFFSET_X, 160.0f + OFFSET_Y), RADIUS, RADIUS);

	pRenderTarget->DrawEllipse(circle1, pBrush, 4.0f);
	pRenderTarget->DrawEllipse(circle2, pBrush, 4.0f);
	pRenderTarget->DrawEllipse(circle3, pBrush, 4.0f);
	pRenderTarget->DrawEllipse(circle4, pBrush, 4.0f);

	pRenderTarget->EndDraw();

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK D2DWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		return 0;
	case WM_SIZE:
		if (pRenderTarget)
		{
			RECT rc;
			GetClientRect(hwnd, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
			pRenderTarget->Resize(size);
		}
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

	WNDCLASS wnd_class = {};
	wnd_class.lpfnWndProc = D2DWinProc;
	wnd_class.hInstance = hInstance;
	wnd_class.lpszClassName = L"D2D_expri1";
	RegisterClass(&wnd_class);

	HWND hwnd = CreateWindowEx(
		0,
		L"D2D_expri1",
		L"D2D_Experiment1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (pFactory)
		pFactory->Release();
	return 0;
}