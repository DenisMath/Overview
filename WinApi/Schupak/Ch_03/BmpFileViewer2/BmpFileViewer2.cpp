//////////////////////////////////////////////////////////////////////
// BmpFileViewer2.cpp
#include <windows.h>
#include "KWnd.h"

#define FILE_NAME "Dromedary.bmp"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("BmpFileViewer2", hInstance, nCmdShow, WndProc);	

	while (GetMessage(&msg, NULL, 0, 0))  {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//====================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	static int bmpWidth, bmpHeight;
	static HBITMAP hBitmap;      // дескриптор битового образа
	HINSTANCE hInst;
	HDC hMemDC;                  // дескриптор совместимого контекста
	BITMAP bm;
	int dX, dY;

	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);

		hInst = (HINSTANCE)GetClassLong(hWnd, GCL_HMODULE);
		hBitmap = (HBITMAP)LoadImage(hInst, FILE_NAME, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap == NULL) {
			MessageBox(hWnd, "Файл " FILE_NAME " не загружен", "Error", MB_OK);
			break;
		}

		GetObject(hBitmap, sizeof(bm), &bm);
		bmpWidth = bm.bmWidth;
		bmpHeight = bm.bmHeight;

		// Подогнать размеры окна программы под размер растра hBitmap
		GetClientRect(hWnd, &rect);
		dX = bmpWidth - rect.right;
		dY = bmpHeight - rect.bottom;
		GetWindowRect(hWnd, &rect);
		InflateRect(&rect, dX/2, dY/2);
		MoveWindow(hWnd, rect.left, rect.top,
			rect.right-rect.left, rect.bottom-rect.top, TRUE);
		ReleaseDC(hWnd, hDC);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		// Создать совместимый контекст
		hMemDC = CreateCompatibleDC(hDC);
		// Выбрать hBitmap в совместимый контекст		
		SelectObject(hMemDC, hBitmap);
		// Скопировать изображение из hMemDC в hDC
		BitBlt(hDC, 0, 0, bmpWidth, bmpHeight, hMemDC, 0, 0, SRCCOPY);
		
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		break;

    case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////
