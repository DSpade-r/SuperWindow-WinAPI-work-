// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SuperWorkWindow.h"
#include <xstring>
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
	std::allocator<TCHAR> > String;

#define MAX_LOADSTRING 100
#define ID_FIRSTCHILD 100

// Global Variables:
HINSTANCE hInst; // current instance
TCHAR szTitle[MAX_LOADSTRING]; // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name
HINSTANCE hinst;
//вынес отдельно константно размеры экрана для определения логического размера окна
const WORD width = 1920;
const WORD hight = 1080;

const WORD GridStep = 10;
// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SUPERWORKWINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUPERWORKWINDOW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
// FUNCTION: MyRegisterClass()
//
// PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUPERWORKWINDOW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SUPERWORKWINDOW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC MemoryHdc;
	static HBRUSH hBrush;
	static WORD ix, iy; //статические перемнные для хранения координат мыши
	static WORD sx, sy; //статические переменные для хранения ширины и длины окна
	static HWND child;
	RECT pRect;
/*HRGN hRgn;*/

	switch (message)
	{
	case WM_CREATE:
		ix = LOWORD(lParam);
		iy = HIWORD(lParam);
		MemoryHdc = CreateCompatibleDC(0);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		WNDCLASS wPOINT;
		memset(&wPOINT, 0, sizeof(WNDCLASS));
		wPOINT.style = WS_EX_TRANSPARENT;
		wPOINT.lpfnWndProc = ChildProc;
		wPOINT.hInstance = hinst;
		wPOINT.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wPOINT.lpszClassName = TEXT("Child");
		RegisterClass(&wPOINT);
		child = CreateWindowEx(0, TEXT("Child"), (LPCTSTR)NULL, WS_CHILD | WS_BORDER | WS_VISIBLE, ix, iy, 68, 18, hWnd, (HMENU)(int)(ID_FIRSTCHILD), hInst, NULL);
		SetWindowLong(child, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(child, 0, (255 * 100) / 100, LWA_ALPHA);
		ShowWindow(child, SW_NORMAL);
		UpdateWindow(child);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		ix = LOWORD(lParam);
		iy = HIWORD(lParam);
		MoveWindow(child, ix + 10, iy + 10, 67, 18, TRUE);
		WORD nSize;
		TCHAR szBuf[80];
		hdc = GetDC(child);
		nSize = wsprintf(szBuf, TEXT("(%d,%d)"), ix, iy);
		TextOut(hdc, 0, 0, szBuf, nSize);
		ReleaseDC(child, hdc);
		if (wParam == MK_LBUTTON)
			SendMessage(hWnd, WM_LBUTTONDOWN, lParam, wParam);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		MemoryHdc = CreateCompatibleDC(hdc);
		//формирую размер прямоугольника исходя из размера ячейки сетки главного окна
		pRect.left = ix - (ix % GridStep);
		pRect.right = ix - (ix % GridStep) + GridStep;
		pRect.top = iy - (iy % GridStep);
		pRect.bottom = iy - (iy % GridStep) + GridStep;
		FillRect(MemoryHdc, &pRect, hBrush); //рисует прямоуголник(у меня квадрат)
		ReleaseDC(hWnd, hdc);
		SendMessage(hWnd, WM_PAINT, 0, 0);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, MemoryHdc);
		BitBlt(hdc, 0, 0, sx, sy, MemoryHdc, 0, 0, SRCCOPY);
		SelectObject(hdc, MemoryHdc);
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, width, -hight, NULL); //- hight - делаем нижний левый угол началом координат
		SetViewportOrgEx(hdc, 0, 0, NULL);
		for (int x = 0; x < sx; x += 10)
		{
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, sy);
		}
		for (int y = 0; y < sy; y += 10)
		{
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, sx, y);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
LRESULT CALLBACK ChildProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_DESTROY) return 0;
	return DefWindowProc(hwnd, Message, wParam, lParam);
}