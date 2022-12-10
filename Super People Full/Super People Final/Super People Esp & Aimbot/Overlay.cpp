#include "pch.h"

Overlay* g_pOverlay = new Overlay();
Overlay::Overlay() {}
Overlay::~Overlay() {}

void Overlay::createWindow()
{
	HWND hwndMain;
	hwndMain = FindWindowEx(NULL, NULL, L"TXGuiFoundation", L"Gameloop");
	if (IsWindow(hwndMain))
	{
		HWND hwndSub = FindWindowEx(hwndMain, NULL, L"AEngineRenderWindowClass", NULL);
		RECT rect;
		GetWindowRect(hwndSub, &rect);
		left = rect.left;
		top = rect.top;
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	else
	{
		left = 0;
		top = 0;
		width = GetSystemMetrics(SM_CXSCREEN) - 2;
		height = GetSystemMetrics(SM_CYSCREEN) - 2;
	}

	settings.antialiasingLevel = 4;

	window.create(
		sf::VideoMode(width, height),
		"PUBG Corona Drawings",
		sf::Style::None,
		settings
	);

	HWND hwnd = window.getSystemHandle();
	SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(hwnd, &margin);
	SetWindowPos(hwnd, HWND_TOPMOST, left, top, width, height, SWP_SHOWWINDOW);
	UpdateWindow(hwnd);
}