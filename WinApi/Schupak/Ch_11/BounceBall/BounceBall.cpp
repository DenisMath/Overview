//////////////////////////////////////////////////////////////////////
// � ���� ��������� ...
//       ����� �� ����������� ��� � ���������� ��������, ��� 
// ������ ���� ������������ ������ ���������, � �������� ��������������� ������� ����.
//
//////////////////////////////////////////////////////////////////////
// BounceBall.cpp
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "KWnd.h"
#include "resource.h"

#define Pi 3.14159265

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBall(HWND, HDC, HBITMAP, BITMAP, FLOAT, FLOAT, int);
//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd("Bounce ball", hInstance, nCmdShow, WndProc);	

	while (GetMessage(&msg, NULL, 0, 0))  {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//====================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam)
{
	static FLOAT x = 0, y = 0; // ������� ������� ����
	static FLOAT dX, dY; // ���������� ��������� ��� ������ �� ����� �������
	static int alpha = 0; // ����, ������������ �������� ����

	static HDC hDC;          // �������� �������
	static HBRUSH hBkBrush;  // ������� �����
	HBITMAP hBmpBkgr;        // ����� ��� ������� �����
	static HBITMAP hBmpBall; // ����� � ������������ ����
	static BITMAP bm;        // ��������� ������ 
	RECT rect;  // ������������� ���������� �������
	RECT rBall; // �������������, �������������� ����������� ����

	switch(uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		dX = rect.right / 100.;
		dY = rect.bottom / 50.;
		// ������� ������ (0.1 ���)
		SetTimer(hWnd, 1, 100, NULL);

		hBmpBkgr = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDB_STONE));
		hBkBrush = CreatePatternBrush(hBmpBkgr);
		SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)hBkBrush);

		hBmpBall = CreateCompatibleBitmap(
   hDC, 
  rect.right - rect.left, 
  rect.top - rect.bottom
); 
			/*LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDB_BALL));*/
		GetObject(hBmpBall, sizeof(bm), (LPSTR)&bm);

		SetGraphicsMode(hDC, GM_ADVANCED);
		break;
     
	case WM_TIMER:
		GetClientRect(hWnd, &rect);
		// ������� ������� �������� ����
		SetRect(&rBall, (int)x- 2*bm.bmWidth, (int)y- 2*bm.bmHeight, (int)x + 2*bm.bmWidth,
			(int)y + 2*bm.bmHeight);
		//FillRect(hDC, &rBall, hBkBrush);

		// ����� ������� ����
		x += dX;
		y += dY;
		alpha += 10;
		if (alpha > 360)  alpha = 0;

		// ���� ��� ������ ���� ����, ����������� ��� �������� ����������
		if(x + bm.bmWidth > rect.right || x < 0)
			dX = -dX;
		if(y + bm.bmHeight > rect.bottom || y < 0)
			dY = -dY;

		DrawBall(hWnd, hDC, hBmpBall, bm, x, y, alpha);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);	
		break;
     
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
     }
     return 0;
}

//====================================================================
void DrawBall(HWND hwnd, HDC hdc, HBITMAP hBmp, BITMAP bm, FLOAT x, FLOAT y, int alpha) {
	XFORM xform, xform1, xform2, xform3;
	HRGN hRgn;
	RECT rect; 
	GetClientRect(hwnd, &rect);
	static HDC hBallMemDC = CreateCompatibleDC(hdc);
	static HDC hBallMemDC1 = CreateCompatibleDC(hdc);
	static HDC hBallMemDC2 = CreateCompatibleDC(hdc);
	static HDC hBallMemDC3 = CreateCompatibleDC(hdc);

	static HBITMAP hBmp1 = CreateCompatibleBitmap(hBallMemDC1 ,
 300, 
 300
);
	static HBITMAP hBmp2= CreateCompatibleBitmap(hBallMemDC1 ,
 300, 
 300
); 
	static HBITMAP hBmp3= CreateCompatibleBitmap(hBallMemDC1 ,
 300, 
 300
); 
	
	
	// ���������� � ������ ����
	

	hBmp = CreateCompatibleBitmap(hBallMemDC,
 rect.right - rect.left, rect.top - rect.bottom);

	hBmp1 = CreateCompatibleBitmap(hBallMemDC1 ,
 300, 
 300
); 

 hBmp2 = CreateCompatibleBitmap(hBallMemDC2 ,
 300, 
 300
); 

	hBmp3 = CreateCompatibleBitmap(hBallMemDC3 ,
 300, 
 300
); 
	
	SelectObject(hBallMemDC1, hBmp1);
	SelectObject(hBallMemDC2, hBmp2);
	SelectObject(hBallMemDC3, hBmp3);

	for(int i=0; i<200; i++)
	{
		for(int j=0; 200; j++)
		{
	       SetPixel(hBallMemDC1,i,j,0x00FFFFFF);
		}
	}

		/*for(int i=0; i<100; i++)
		{
		for(int j=0; j<100; j++)
		{
	       SetPixel(hdc,i,j,0x00000000);
		}
		}*/
	 
	
	// ������� ������ ���������
	hRgn = CreateEllipticRgn(x, y, x + 2*bm.bmWidth, y + 2*bm.bmHeight);
	//SelectClipRgn(hdc, hRgn);
	
	// ������� �������������� ��� ����������� � �������� ����
	
	SaveDC(hBallMemDC);	
	BitBlt(hBallMemDC, 0, 0, 
		 rect.right - rect.left, rect.top - rect.bottom, hdc, 0, 0, SRCCOPY);
	RestoreDC(hBallMemDC, -1);
	// ����� ����
	SaveDC(hdc);	
	xform.eM11 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform.eM12 = (FLOAT) sin(alpha * 2 * Pi / 360);	 //��������
	xform.eM21 = (FLOAT) -sin(alpha * 2 * Pi / 360); //��������
	xform.eM22 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform.eDx  = x + bm.bmWidth / 2.;		 //�������� �� ��� x
	xform.eDy  = y + bm.bmHeight / 2.;		 //�������� �� ��� y

	xform1.eM11 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform1.eM12 = (FLOAT) sin(alpha * 2 * Pi / 360);	 //��������
	xform1.eM21 = (FLOAT) -sin(alpha * 2 * Pi / 360); //��������
	xform1.eM22 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform1.eDx  = x + bm.bmWidth / 2.;		 //�������� �� ��� x
	xform1.eDy  = y + bm.bmHeight / 2.;		 //�������� �� ��� y

	xform2.eM11 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform2.eM12 = (FLOAT) sin(alpha * 2 * Pi / 360);	 //��������
	xform2.eM21 = (FLOAT) -sin(alpha * 2 * Pi / 360); //��������
	xform2.eM22 = (FLOAT) cos(alpha * 2 * Pi / 360);	 //��������
	xform2.eDx  = x + bm.bmWidth / 2.;		 //�������� �� ��� x
	xform2.eDy  = y + bm.bmHeight / 2.;		 //�������� �� ��� y

	xform3.eM11 = 1;	 //��������
	xform3.eM12 = 0;	 //��������
	xform3.eM21 = 0; //��������
	xform3.eM22 = 1;	 //��������
	xform3.eDx  = 0;		 //�������� �� ��� x
	xform3.eDy  = 0;		 //�������� �� ��� y
	/*BitBlt(hBallMemDC, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hdc, 0, 0, SRCCOPY);*/

	BOOL ret = SetWorldTransform(hdc, &xform);
	SetWorldTransform(hBallMemDC1, &xform);
	SetWorldTransform(hBallMemDC2, &xform1);
	SetWorldTransform(hBallMemDC3, &xform2);
	SetWorldTransform(hBallMemDC, &xform3);


	BitBlt(hdc, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);

	BitBlt(hBallMemDC1, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);

	BitBlt(hBallMemDC2, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);

	BitBlt(hBallMemDC3, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);

	BitBlt(hBallMemDC, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC1, 0, 0, SRCCOPY);
	BitBlt(hBallMemDC, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC2, 0, 0, SRCCOPY);
	BitBlt(hBallMemDC, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC3, 0, 0, SRCCOPY);

	//xform.eM11 = (FLOAT) cos(alpha * 2 * Pi / 360)/2;	 //��������
	//xform.eM12 = (FLOAT) sin(alpha * 2 * Pi / 360)/2;	 //��������
	//xform.eM21 = (FLOAT) -sin(alpha * 2 * Pi / 360)/2; //��������
	//xform.eM22 = (FLOAT) cos(alpha * 2 * Pi / 360)/2;	 //��������
	//xform.eDx  = x + bm.bmWidth / 2.;		 //�������� �� ��� x
	//xform.eDy  = y + bm.bmHeight / 2.;		 //�������� �� ��� y

	//ret = SetWorldTransform(hdc, &xform);
	//BitBlt(hdc, -bm.bmWidth/2+100, -bm.bmHeight/2+100, 
	//		bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, -bm.bmWidth/2, -bm.bmHeight/2, 
			bm.bmWidth, bm.bmHeight, hBallMemDC, 0, 0, SRCCOPY);
	RestoreDC(hdc, -1);

	SelectClipRgn(hdc, NULL);
	DeleteObject(hRgn);
	DeleteDC(hBallMemDC);
}
//////////////////////////////////////////////////////////////////////
