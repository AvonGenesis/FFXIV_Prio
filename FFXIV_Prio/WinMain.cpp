#include <windows.h>
#include "resource.h"
#include <Commdlg.h>

// Forward declare event statements
BOOL WINAPI DlgProc( HWND  hDlg, UINT Msg, WPARAM Param1, LPARAM Param2 );

bool running = true;
int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow ){
	// Create dialog box IDD_DLG_MAIN
	HWND hDialog = CreateDialog(hInstance , MAKEINTRESOURCE(IDD_DLG_MAIN) , NULL , DlgProc );
	ShowWindow( hDialog , nCmdShow );
	// Enter msg loop to track events
	MSG msg;
	while(running){
		// Get FFXIV Handle
		//===========================================================
		const char* strWindowName = "FINAL FANTASY XIV";
		HWND hwndFFXIV = FindWindow(0, strWindowName);
		DWORD hwndFFXIVID;
		GetWindowThreadProcessId(hwndFFXIV, &hwndFFXIVID);
		DWORD PROC_ID = hwndFFXIVID;
		HANDLE PROC_HANDLE;
		PROC_HANDLE = OpenProcess(PROCESS_ALL_ACCESS, false, PROC_ID);
		//===========================================================

		// while(GetMessage(&msg, NULL, 0, 0)){
		while (PeekMessage(&msg, hDialog,  0, 0, PM_REMOVE)){
			if(!IsDialogMessage(hDialog , &msg )){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		
		// If FFXIV in focus
		if (hwndFFXIV == GetForegroundWindow()){
			LPSTR focus, priority;
			GetDlgItemText(hDialog, IDC_FOCUS_STATUS, focus, sizeof(IDC_FOCUS_STATUS));
			GetDlgItemText(hDialog, IDC_PRIORITY_STATUS, priority, sizeof(IDC_PRIORITY_STATUS));
			SetPriorityClass(PROC_HANDLE, ABOVE_NORMAL_PRIORITY_CLASS);
			if ((sizeof(IDC_PRIORITY_STATUS) != sizeof(priority)) && (sizeof(IDC_FOCUS_STATUS) != sizeof(focus))){
				SetDlgItemText(hDialog, IDC_FOCUS_STATUS, "Focused");
				SetDlgItemText(hDialog, IDC_PRIORITY_STATUS, "Above Normal Priority");
			}
		}

		// FFXIV Not in focus
		else {
			LPSTR focus, priority;
			GetDlgItemText(hDialog, IDC_FOCUS_STATUS, focus, sizeof(IDC_FOCUS_STATUS));
			GetDlgItemText(hDialog, IDC_PRIORITY_STATUS, priority, sizeof(IDC_PRIORITY_STATUS));
			SetPriorityClass(PROC_HANDLE, BELOW_NORMAL_PRIORITY_CLASS);
			if ((sizeof(IDC_PRIORITY_STATUS) != sizeof(priority)) && (sizeof(IDC_FOCUS_STATUS) != sizeof(focus))){
				SetDlgItemText(hDialog, IDC_FOCUS_STATUS, "Not Focused");
				SetDlgItemText(hDialog, IDC_PRIORITY_STATUS, "Below Normal Priority");
			}
		}
	}
	return 0;
}

// Events
BOOL WINAPI DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam  ){
    switch(message){

		case WM_CLOSE:
			running = false;
			EndDialog(hDlg, WM_CLOSE);
			PostQuitMessage(WM_CLOSE);
			break;

		/*case WM_COMMAND:
            switch(LOWORD(wParam)){
				case IDC_BUTTON1:
					EndDialog(hDlg, IDC_BUTTON1);
					PostQuitMessage(IDC_BUTTON1);
					break;
			}*/

		default:
			return FALSE;
    }
	return TRUE;
}