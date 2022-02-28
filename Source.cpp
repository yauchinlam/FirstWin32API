#include <windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4 

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void loadImages();
void registerDialogClass(HINSTANCE);
void displayDialog(HWND);



HMENU hMenu;
HWND hName, hAge, hOut, hLogo, hMainWindow;
HBITMAP hLogoImage, hGenerateImage;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	registerDialogClass(hInst);

	hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	//MessageBoxW(NULL, L"HELLO", L"HI", MB_OK);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int val;
	switch (msg)
	{
	case WM_COMMAND:

		switch (wp)
		{
		case FILE_MENU_EXIT:
			val = MessageBoxW(hWnd, L"Are you sure?", L"Wait!", MB_YESNO | MB_ICONEXCLAMATION);
			if (val == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case FILE_MENU_NEW:
			displayDialog(hWnd);
			MessageBeep(MB_ICONINFORMATION);
			break;
		case GENERATE_BUTTON:
			char name[30], age[10], out[50];
			GetWindowTextA(hName, name, 30);
			GetWindowTextA(hAge, age, 10);
			
			if ((strcmp(name," ") == 0) || (strcmp(age," ") == 0))
			{
				val = MessageBoxW(hWnd, L"You did not enter anything!", NULL, MB_ABORTRETRYIGNORE);
				switch (val)
				{
					case IDABORT:
						DestroyWindow(hWnd);
						break;
					case IDRETRY:
						return 0;
					case IDIGNORE:
						break;
				}
			}

			strcpy_s(out, name);
			strcat_s(out, " is ");
			strcat_s(out, age);
			strcat_s(out, " years old.");

			SetWindowTextA(hOut, out);

			break;

		}
		break;
	case WM_CREATE:
		loadImages();
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenuA(hSubMenu, MF_STRING, NULL, "SubMenuItem");

	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenuA(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
	AppendMenuA(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	//Append Menu in Windows default to AppendMenuW so needed to change to AppendMenuA
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenuA(hMenu, MF_STRING, NULL, "Help");
	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	CreateWindowW(L"Static", L"Name: ", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL);
	hName = CreateWindowW(L"Edit", L" ", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Age: ", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, NULL, NULL, NULL);
	hAge = CreateWindowW(L"Edit", L" ", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, hWnd, NULL, NULL, NULL);

	HWND hBut = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 150, 140, 98, 38, hWnd, (HMENU)GENERATE_BUTTON, NULL, NULL);
	SendMessageW(hBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGenerateImage);

	hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 200, 300, 200, hWnd, NULL, NULL, NULL);
	hLogo = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 350, 60, 100, 100, hWnd, NULL, NULL, NULL);
	SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);

}


void loadImages()
{
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"Logo.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	hGenerateImage = (HBITMAP)LoadImageW(NULL, L"Logo.bmp", IMAGE_BITMAP, 98, 38, LR_LOADFROMFILE);
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case 1:
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		}
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void registerDialogClass(HINSTANCE hInst)
{
	WNDCLASSW dialog = { 0 };

	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
	dialog.hInstance = hInst;
	dialog.lpszClassName = L"myDialogClass";
	dialog.lpfnWndProc = DialogProcedure;

	RegisterClassW(&dialog);
}

void displayDialog(HWND hWnd)
{
	HWND hDlg = CreateWindowW(L"myDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400,
		200, 200, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD, 20, 20, 100, 40,
		hDlg, (HMENU)1, NULL, NULL);

	EnableWindow(hWnd, false);
}