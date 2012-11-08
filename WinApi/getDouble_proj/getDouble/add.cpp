#include <windows.h>
#include <map>
#include "main.h"
#include "resource.h"

HWND hMainDlg;
HINSTANCE hInst;
#define DELETE_MSG WM_USER + 1
int counter = 0;
//int size = 0 ;
std::map<HWND, int> basicPointsWindows;

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
			break;
	}
	
	return false;
}

BOOL CALLBACK MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BUTTON:
                    basicPointsWindows.insert(std::pair<HWND, int>(CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)hMainDlg, ChildDlgProc),counter));
                    ++counter;
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_CHILDDLG), hMainDlg, ChildDlgProc);
					break;
			}	// switch(LOWORD(wParam))
			break;
        case DELETE_MSG:
            //basicPointsWindows.size();
            SetDlgItemText(hWnd,IDC_NUMBER,(LPCWSTR)"v");
            break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
	}
	
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	hInst = hInstance;
	
	hMainDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)NULL, MainDlgProc);
	if(!hMainDlg) return false;
	
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	// if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg))
	}	// while (GetMessage(&msg, NULL, NULL, NULL))
	
	return true;
}

