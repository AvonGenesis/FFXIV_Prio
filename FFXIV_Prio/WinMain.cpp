#include <windows.h>
#include "resource.h"
#include <Commdlg.h>
#include "constants.h"

BOOL WINAPI DlgProc( HWND  hDlg, UINT Msg, WPARAM Param1, LPARAM Param2 );

bool running = true;

// Keep track of focusing and unfocusing for dialog text change
LPSTR priority = '\0';

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow ){
	// Create dialog box IDD_DLG_MAIN
	HWND hDialog = CreateDialog(hInstance , MAKEINTRESOURCE(IDD_DLG_MAIN) , NULL , DlgProc );
	SetWindowText(hDialog, title);
	ShowWindow( hDialog , nCmdShow );
	MSG msg;
	// Enter program loop
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
			SetPriorityClass(PROC_HANDLE, ABOVE_NORMAL_PRIORITY_CLASS);
			if (priority != "Above Normal Priority"){
				SetDlgItemText(hDialog, IDC_FOCUS_STATUS, "Focused");
				SetDlgItemText(hDialog, IDC_PRIORITY_STATUS, "Above Normal Priority");
				priority = "Above Normal Priority";
			}
		}

		// FFXIV Not in focus
		else {
			SetPriorityClass(PROC_HANDLE, BELOW_NORMAL_PRIORITY_CLASS);
			if (priority != "Below Normal Priority"){
				SetDlgItemText(hDialog, IDC_FOCUS_STATUS, "Not Focused");
				SetDlgItemText(hDialog, IDC_PRIORITY_STATUS, "Below Normal Priority");
				priority = "Below Normal Priority";
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
