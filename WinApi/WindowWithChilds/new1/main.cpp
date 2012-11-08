#include <windows.h>
#include <set>
#include "main.h"
#include "resource2.h"

HWND hMainDlg, Temp;
HINSTANCE hInst;
#define DELETE_MSG WM_USER + 1
int counter = 0;
int size = 0;
int nCmdSh = 0 ;
char buff[256];
std::set<HWND> basicPointsWindows;



BOOL CALLBACK ChildDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hWnd, 0);
                    basicPointsWindows.erase(hWnd);
                    SendMessage(hMainDlg,DELETE_MSG,NULL,NULL);
					break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hWnd, 0);
            basicPointsWindows.erase(hWnd);
            SendMessage(hMainDlg,DELETE_MSG,NULL,NULL);
			break;
	}
	
	return false;
}

BOOL CALLBACK MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BUTTON:
                    basicPointsWindows.insert(Temp = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHILDDLG), (HWND)hMainDlg, ChildDlgProc));
                    ShowWindow(Temp,nCmdSh);
                    ++counter;
                    size = basicPointsWindows.size();
                    SetDlgItemText(hWnd,IDC_EDIT1,(LPCWSTR)itoa(size,buff,10));
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_CHILDDLG), hMainDlg, ChildDlgProc);
					break;
			}	// switch(LOWORD(wParam))
			break;
        case DELETE_MSG:
            size = basicPointsWindows.size();
            SetDlgItemText(hWnd,IDC_EDIT1,(LPCWSTR)itoa(size,buff,10));
            break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
	}
	
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
	hInst = hInstance;
	nCmdSh = nCmdShow;
	hMainDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)NULL, MainDlgProc);
	if(!hMainDlg) return false;
	ShowWindow(hMainDlg,nCmdShow);
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	// if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg))
	}	// while (GetMessage(&msg, NULL, NULL, NULL))
	
	return true;
}

