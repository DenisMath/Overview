// getDouble.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include<string>
// Global Variables:
HINSTANCE hInst;								// current instance
LRESULT CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{BOOL bRet = FALSE;
	BOOL bCmd = FALSE;
	char sText[256];
	double temp;
	switch (message) 
	{
		case WM_INITDIALOG:
            			bRet = TRUE;
			break;
		case WM_COMMAND:
			bRet = TRUE;
			bCmd = TRUE;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			EndDialog(hWnd, TRUE); 
			break;
	}
	if(bCmd)
	switch(LOWORD(wParam))
	{
		case IDOK:
			if(GetDlgItemText(hWnd,IDC_EDIT1,sText,255))
			{
               temp = atof(sText);
			   sprintf(sText, "%f", temp+1);
			   SetDlgItemText(hWnd,IDC_EDIT2,sText);
			}
			break;
		case IDM_EXIT:
		case IDCANCEL: 
			SendMessage(hWnd,WM_DESTROY,0,0);
			break;
	}
	return bRet;}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	DialogBox((hInst = hInstance), (LPCTSTR)IDD_DIALOG, NULL, (DLGPROC)DlgProc);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	BOOL bCmd = FALSE;
	char sText[256];
	switch (message) 
	{
		case WM_INITDIALOG:
            DialogBox((hInst = GetModuleHandle(NULL)), (LPCTSTR)IDD_DIALOG, NULL, (DLGPROC)DlgProc1);
			bRet = TRUE;
			break;
		case WM_COMMAND:
			bRet = TRUE;
			bCmd = TRUE;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			EndDialog(hWnd, TRUE); 
			break;
	}
	if(bCmd)
	switch(LOWORD(wParam))
	{
		case IDOK:
			if(GetDlgItemText(hWnd,IDC_EDIT1,sText,255))
			   SetDlgItemText(hWnd,IDC_EDIT2,sText);
			break;
		case IDM_EXIT:
		case IDCANCEL: 
			SendMessage(hWnd,WM_DESTROY,0,0);
			break;
	}
	return bRet;
}
